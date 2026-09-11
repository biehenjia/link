#pragma once
#include "../domain/field.hpp"
#include "../ir/arena.hpp"
#include "../ir/make.hpp"
#include "../ir/node.hpp"
#include "hash.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <span>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cr {
template <class Hash, class Value, class Verify, class Construct>
Value cons(std::unordered_multimap<Hash, Value>& table, Hash h, Verify&& verify, Construct&& construct) {
    auto [it, end] = table.equal_range(h);
    for (; it != end; ++it) {
        if (verify(it->second))
            return it->second;
    }
    Value v = construct();
    table.emplace(h, v);
    return v;
}
template <class T>
class Interner {
public:
    explicit Interner(Arena<T>& a) : arena(a) {}
    Arena<T>& arena;
    const Node& node(uint32_t id) const { return arena.node(id); }
    std::span<const uint32_t> operands(uint32_t id) const { return arena.operands(id); }
    const T& value(uint32_t slot) const { return arena.value(slot); }
    uint32_t intern_leaf(uint8_t var, T value) {
        const uint32_t h = Field<T>::hash(value);
        auto verify = [&](uint32_t id) {
            const Node& n = arena.node(id);
            return n.var == var && Field<T>::equal(arena.value(n.ops), value);
        };
        auto construct = [&] { return make_leaf(arena, var, std::move(value)); };
        return cons(leaves_, h, verify, construct);
    }

    uint32_t intern_node(Kind kind, uint8_t var, std::span<const uint32_t> operands) {
        assert(!operands.empty());
        std::vector<uint64_t> sfx = suffix_hashes(kind, var, operands);
        const uint64_t h = sfx[0]; // whole-list hash
        auto verify = [&](uint32_t id) {
            const Node& n = arena.node(id);
            return n.kind == kind && n.var == var && std::ranges::equal(arena.operands(id), operands);
        };
        auto construct = [&] {
            const uint32_t id = make_node(arena, kind, var, operands);
            suffixes_.emplace(id, std::move(sfx));
            return id;
        };
        return cons(nodes_, h, verify, construct);
    }
    uint32_t intern_pair(uint8_t var, uint32_t sin, uint32_t cos) {
        const std::array<uint32_t, 2> ops{sin, cos};
        return intern_node(Kind::Pair, var, ops);
    }
    static constexpr uint32_t npos = ~uint32_t(0);

    std::span<const uint64_t> suffixes(uint32_t id) const {
        const auto it = suffixes_.find(id);
        if (it == suffixes_.end())
            return {};
        return it->second;
    }

    uint32_t find_node(Kind kind, uint8_t var, std::span<const uint32_t> operands) const {
        const uint64_t h = suffix_hashes(kind, var, operands)[0];
        auto [it, end] = nodes_.equal_range(h);
        for (; it != end; ++it) {
            const Node& n = arena.node(it->second);
            if (n.kind == kind && n.var == var && std::ranges::equal(arena.operands(it->second), operands))
                return it->second;
        }
        return npos;
    }

private:
    static std::vector<uint64_t> suffix_hashes(Kind kind, uint8_t var, std::span<const uint32_t> operands) {
        std::vector<uint64_t> out(operands.size());
        uint64_t h = hfold(0, (uint64_t(kind) << 8) | var);
        for (size_t idx = operands.size(); idx-- > 0;) {
            h = hfold(h, operands[idx]);
            out[idx] = h;
        }
        return out;
    }
    std::unordered_multimap<uint64_t, uint32_t> nodes_;
    std::unordered_multimap<uint32_t, uint32_t> leaves_;
    std::unordered_map<uint32_t, std::vector<uint64_t>> suffixes_;
};
} // namespace cr
