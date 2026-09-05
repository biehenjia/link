#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <span>
#include <unordered_map>
#include <utility>

#include "hash.hpp"
#include "../domain/field.hpp"
#include "../ir/arena.hpp"
#include "../ir/make.hpp"
#include "../ir/node.hpp"


namespace cr {

    template <class Hash, class Value, class Verify, class Construct>
    Value cons(std::unordered_multimap<Hash, Value>& table, Hash h, Verify&& verify, Construct&& construct) {
        auto [it, end] = table.equal_range(h);

        for (; it != end; ++it) {
            if (verify(it->second)) return it->second;
        }

        Value v = construct();
        table.emplace(h, v);
        return v;
    } 


    template <class T>
    class Interner {
        public:
            explicit Interner(Arena<T>& arena) : arena_(arena) {}

            uint32_t intern_leaf(uint8_t var, T value) {
                const uint32_t h = Field<T>::hash(value);

                auto verify = [&](uint32_t id) {
                    const Node& n = arena_.node(id);
                    return n.var == var && Field<T>::equal(arena_.value(n.ops), value);
                };
                auto construct = [&] { return make_leaf(arena_, var, std::move(value)); };

                return cons(leaves_, h, verify, construct);
            }


            uint32_t intern_node(Kind kind, uint8_t var, std::span<const uint32_t> operands) {
                assert(!operands.empty());
                const uint64_t h = suffix_hash(kind, var, operands);

                auto verify = [&](uint32_t id) {
                    const Node& n = arena_.node(id);
                    return n.kind == kind && n.var == var
                        && std::ranges::equal(arena_.operands(id), operands);
                };
                auto construct = [&] { return make_node(arena_, kind, var, operands); };

                return cons(nodes_, h, verify, construct);
            }

            uint32_t intern_pair(uint8_t var, uint32_t sin, uint32_t cos) {
                const std::array<uint32_t, 2> ops{sin, cos};
                return intern_node(Kind::Pair, var, ops);
            }

        private:
            static uint64_t suffix_hash(Kind kind, uint8_t var, std::span<const uint32_t> operands) {
                uint64_t h = hfold(0, (uint64_t(kind) << 8) | var);
                for (size_t idx = operands.size(); idx-- > 0; ) {
                    h = hfold(h, operands[idx]);
                }
                return h;
            }

            Arena<T>& arena_;
            std::unordered_multimap<uint64_t, uint32_t> nodes_;
            std::unordered_multimap<uint32_t, uint32_t> leaves_;
    };

}
