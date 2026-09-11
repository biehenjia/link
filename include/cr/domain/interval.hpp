#pragma once
#include "field.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <optional>
namespace cr {
template <class T>
struct Interval {
    T lo, hi;
};
template <class T>
struct Field<Interval<T>> {
    using I = Interval<T>;
    static uint32_t hash(const I& v) { return Field<T>::hash(v.lo) ^ (Field<T>::hash(v.hi) * 0x9E3779B1u); }
    static bool equal(const I& a, const I& b) { return Field<T>::equal(a.lo, b.lo) && Field<T>::equal(a.hi, b.hi); }
    static I zero() { return {Field<T>::zero(), Field<T>::zero()}; }
    static I one() { return {Field<T>::one(), Field<T>::one()}; }
    static I neg_one() { return {Field<T>::neg_one(), Field<T>::neg_one()}; }
    static std::optional<long> whole(const I& v) {
        if (!Field<T>::equal(v.lo, v.hi))
            return std::nullopt;
        return Field<T>::whole(v.lo);
    }
    static I add(const I& a, const I& b) { return {Field<T>::add(a.lo, b.lo), Field<T>::add(a.hi, b.hi)}; }
    static I sub(const I& a, const I& b) { return {Field<T>::sub(a.lo, b.hi), Field<T>::sub(a.hi, b.lo)}; }
    static I mul(const I& a, const I& b) {
        const T p1 = Field<T>::mul(a.lo, b.lo);
        const T p2 = Field<T>::mul(a.lo, b.hi);
        const T p3 = Field<T>::mul(a.hi, b.lo);
        const T p4 = Field<T>::mul(a.hi, b.hi);
        return {std::min({p1, p2, p3, p4}), std::max({p1, p2, p3, p4})};
    }
    static I div(const I& a, const I& b) {
        assert(!contains_zero(b));
        const T r1 = Field<T>::div(a.lo, b.lo);
        const T r2 = Field<T>::div(a.lo, b.hi);
        const T r3 = Field<T>::div(a.hi, b.lo);
        const T r4 = Field<T>::div(a.hi, b.hi);
        return {std::min({r1, r2, r3, r4}), std::max({r1, r2, r3, r4})};
    }
    static I pow(const I& a, const I& b) {
        assert(!(a.lo < Field<T>::zero()));
        assert(Field<T>::equal(b.lo, b.hi));
        return {Field<T>::pow(a.lo, b.lo), Field<T>::pow(a.hi, b.hi)};
    }
    static I log(const I& a) {
        assert(Field<T>::zero() < a.lo);
        return {Field<T>::log(a.lo), Field<T>::log(a.hi)};
    }
    static I sin(const I& a) { return {Field<T>::sin(a.lo), Field<T>::sin(a.hi)}; }
    static I cos(const I& a) { return {Field<T>::cos(a.hi), Field<T>::cos(a.lo)}; }
    static I tan(const I& a) { return {Field<T>::tan(a.lo), Field<T>::tan(a.hi)}; }
    static I cot(const I& a) { return {Field<T>::cot(a.hi), Field<T>::cot(a.lo)}; }

private:
    static bool contains_zero(const I& v) { return !(v.hi < Field<T>::zero()) && !(Field<T>::zero() < v.lo); }
};
} // namespace cr
