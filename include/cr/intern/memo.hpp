#pragma once
#include "../domain/field.hpp"
#include "intern.hpp"
#include <cstdint>
#include <unordered_map>
#include <utility>
namespace cr {
template <class Key, class Value>
class Memo {
public:
    template <class Compute>
    Value memoize(const Key& key, Compute&& compute) {
        return cons(
                   entries_, Field<Key>::hash(key),
                   [&](const std::pair<Key, Value>& entry) { return Field<Key>::equal(entry.first, key); },
                   [&] { return std::pair<Key, Value>{key, compute()}; })
            .second;
    }

private:
    std::unordered_multimap<uint32_t, std::pair<Key, Value>> entries_;
};
} // namespace cr
