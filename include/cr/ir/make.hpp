#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <utility>

#include "arena.hpp"
#include "node.hpp"



namespace cr {

    template <class T> uint32_t make_leaf(Arena<T>& arena, uint8_t var, T value) {
        const uint32_t slot = arena.append_value(std::move(value));
        return arena.append(Node{Kind::Leaf, var, 0, slot});
    }

    template <class T> uint32_t make_node(Arena<T>& arena, Kind kind, uint8_t var, std::span<const uint32_t> operands) {
        const uint32_t ops = arena.append_operands(operands);
        return arena.append(Node{kind, var, uint16_t(operands.size()), ops});
    }

    
    template <class T> uint32_t make_pair(Arena<T>& arena, uint8_t var, uint32_t sin, uint32_t cos) {
        const std::array<uint32_t, 2> ops{sin, cos};
        return make_node(arena, Kind::Pair, var, ops);
    }

}
