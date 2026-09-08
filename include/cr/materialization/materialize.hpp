#pragma once

#include <cstdint>
#include <span>
#include <vector>

#include "../intern/intern.hpp"
#include "../ir/arena.hpp"
#include "../ir/make.hpp"
#include "../ir/node.hpp"


#include "postorder.hpp"

namespace cr {
    template <class T>
    Arena<T> materialize(const Arena<T>& src, uint32_t root, const Interner<T>* prior = nullptr) {
        const std::vector<uint32_t> order = postorder(src, root);

        Arena<T> out;
        std::vector<uint32_t> remap(src.size(), Interner<T>::npos);

        for (const uint32_t old : order) {
            const Node& n = src.node(old);

            uint32_t fresh;
            if (n.kind == Kind::Leaf) {
                fresh = make_leaf(out, n.var, src.value(n.ops));
            } else {
                const std::span<const uint32_t> src_ops = src.operands(old);

                uint32_t tail = Interner<T>::npos;
                uint32_t cut = n.len; 

                if (prior && n.len >= 3) {
                    for (uint32_t start = 1; start + 2 <= n.len; ++start) {
                        const uint32_t hit = prior->find_node(n.kind, n.var, src_ops.subspan(start));
                        if (hit != Interner<T>::npos && hit != old && remap[hit] != Interner<T>::npos) {
                            tail = hit;
                            cut = start;
                            break;
                        }
                    }
                }

                std::vector<uint32_t> ops;
                ops.reserve(cut + (tail != Interner<T>::npos));
                for (uint32_t i = 0; i < cut; ++i) ops.push_back(remap[src_ops[i]]);
                if (tail != Interner<T>::npos) ops.push_back(make_ref(out, n.var, remap[tail]));

                fresh = make_node(out, n.kind, n.var, ops);
            }

            remap[old] = fresh;
        }

        return out;
    }

}
