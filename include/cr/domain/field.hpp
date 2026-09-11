#pragma once
#include <cstdint>
namespace cr {
template <class T>
struct Field;

template <class T>
T field_from_uint(std::uint64_t n) {
    T result = Field<T>::zero();
    T base = Field<T>::one();
    while (n) {
        if (n & 1)
            result = Field<T>::add(result, base);
        n >>= 1;
        if (n)
            base = Field<T>::add(base, base);
    }
    return result;
}
} // namespace cr
