#pragma once

#include <cstdint>
#include <span>
#include <utility>
#include <vector>

#include "node.hpp"

namespace cr {

    template <class T> class Arena {
        public:
            uint32_t append(Node n) {
                nodes_.push_back(n);
                return uint32_t(nodes_.size()) - 1;
            }

            uint32_t append_operands(std::span<const uint32_t> ops) {
                const uint32_t at = uint32_t(operands_.size());
                operands_.insert(operands_.end(), ops.begin(), ops.end());
                return at;
            }

            uint32_t append_value(T value) {
                values_.push_back(std::move(value));
                return uint32_t(values_.size()) - 1;
            }

            const Node& node(uint32_t id) const { return nodes_[id]; }
            const T& value(uint32_t slot) const { return values_[slot]; }

            std::span<const uint32_t> operands(uint32_t id) const {
                const Node& n = nodes_[id];
                return {operands_.data() + n.ops, n.len};
            }

            uint32_t size() const { return uint32_t(nodes_.size()); }
            uint32_t num_operands() const { return uint32_t(operands_.size()); }

            std::vector<uint32_t> postorder(uint32_t root) const {
                std::vector<uint32_t> order;
                std::vector<bool> seen(nodes_.size(), false);

                auto visit = [&](auto&& self, uint32_t id) -> void {
                    if (seen[id]) return;
                    seen[id] = true;
                    for (const uint32_t child : operands(id)) self(self, child);
                    order.push_back(id);
                };
                visit(visit, root);

                return order;
            }

        private:
            std::vector<Node> nodes_;
            std::vector<uint32_t> operands_;
            std::vector<T> values_;
    };

}
