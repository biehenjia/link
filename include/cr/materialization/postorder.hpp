#pragma once

#include <cstdint>
#include <vector>

#include "../ir/arena.hpp"
#include "../ir/node.hpp"

namespace cr {

    template <class T>
    std::vector<uint32_t> postorder(const Arena<T>& arena, uint32_t root) {
        std::vector<bool> seen(arena.size(), false);
        std::vector<uint32_t> order;

        auto visit = [&](auto&& self, uint32_t id) -> void {
            if (seen[id]) return;
            seen[id] = true;

            const Kind kind = arena.node(id).kind;
            if (kind != Kind::Leaf && kind != Kind::Ref) {
                for (const uint32_t child : arena.operands(id)) self(self, child);
            }

            order.push_back(id);
        };

        visit(visit, root);
        return order;
    }

}
