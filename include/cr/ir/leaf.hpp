#pragma once

#include <cstdint>

#include "../domain/field.hpp"
#include "arena.hpp"
#include "node.hpp"

namespace cr {

    template <class T> bool is_zero_leaf(const Arena<T>& arena, uint32_t id) {
        const Node& n = arena.node(id);
        return n.kind == Kind::Leaf && Field<T>::equal(arena.value(n.ops), Field<T>::zero());
    }

    template <class T> bool is_one_leaf(const Arena<T>& arena, uint32_t id) {
        const Node& n = arena.node(id);
        return n.kind == Kind::Leaf && Field<T>::equal(arena.value(n.ops), Field<T>::one());
    }

}
