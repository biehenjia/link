#pragma once
#include "../mix.hpp"
#include "field.hpp"
#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <optional>
#include <type_traits>
namespace cr {
template <std::floating_point T>
struct Field<T> {
    using bits_t = std::conditional_t<sizeof(T) == 4, uint32_t, uint64_t>;
    static uint32_t hash(T v) {
        if (v == T(0))
            v = T(0);
        const uint64_t bits = uint64_t(std::bit_cast<bits_t>(v));
        return hmix(uint32_t(bits), uint32_t(bits >> 32));
    }
    static bool equal(T a, T b) { return a == b; }
    static T zero() { return T(0); }
    static T one() { return T(1); }
    static T neg_one() { return T(-1); }
    static std::optional<long> whole(T v) {
        if (v < 0 || v != std::floor(v))
            return std::nullopt;
        return long(v);
    }
    static T add(T a, T b) { return a + b; }
    static T sub(T a, T b) { return a - b; }
    static T mul(T a, T b) { return a * b; }
    static T div(T a, T b) { return a / b; }
    static T pow(T a, T b) { return std::pow(a, b); }
    static T log(T a) { return std::log(a); }
    static T sin(T a) { return std::sin(a); }
    static T cos(T a) { return std::cos(a); }
    static T tan(T a) { return std::tan(a); }
    static T cot(T a) { return T(1) / std::tan(a); }
};
} // namespace cr
