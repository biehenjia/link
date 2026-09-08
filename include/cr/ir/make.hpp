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

    // A Ref is an atomic handle onto another chain: its target id lives
    // directly in Node::ops (like a Leaf's value slot), so len is 0 and
    // traversals treat it as a terminal. Used as a chain's trailing operand
    // to splice in a shared tail instead of re-listing that tail's operands.
    template <class T> uint32_t make_ref(Arena<T>& arena, uint8_t var, uint32_t target) {
        return arena.append(Node{Kind::Ref, var, 0, target});
    }


    template <class T> uint32_t make_pair(Arena<T>& arena, uint8_t var, uint32_t sin, uint32_t cos) {
        const std::array<uint32_t, 2> ops{sin, cos};
        return make_node(arena, Kind::Pair, var, ops);
    }

}
