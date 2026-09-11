#pragma once
#include "../mix.hpp"
#include "field.hpp"
#include <bit>
#include <cstdint>
#include <mpfr.h>
#include <optional>
#include <utility>
namespace cr {
class MpReal {
public:
    MpReal() {
        mpfr_init2(v_, precision_);
        mpfr_set_zero(v_, 1);
    }
    explicit MpReal(double d) {
        mpfr_init2(v_, precision_);
        mpfr_set_d(v_, d, MPFR_RNDN);
    }
    explicit MpReal(long l) {
        mpfr_init2(v_, precision_);
        mpfr_set_si(v_, l, MPFR_RNDN);
    }

    MpReal(const MpReal& o) {
        mpfr_init2(v_, precision_);
        mpfr_set(v_, o.v_, MPFR_RNDN);
    }
    MpReal(MpReal&& o) noexcept {
        mpfr_init2(v_, precision_);
        mpfr_swap(v_, o.v_);
    }

    MpReal& operator=(const MpReal& o) {
        mpfr_set(v_, o.v_, MPFR_RNDN);
        return *this;
    }
    MpReal& operator=(MpReal&& o) noexcept {
        mpfr_swap(v_, o.v_);
        return *this;
    }

    ~MpReal() { mpfr_clear(v_); }
    static void set_precision(mpfr_prec_t bits) { precision_ = bits; }

    mpfr_srcptr raw() const { return v_; }
    mpfr_ptr raw() { return v_; }

private:
    static inline mpfr_prec_t precision_ = 256;
    mpfr_t v_;
};
template <>
struct Field<MpReal> {
    static uint32_t hash(const MpReal& v) {
        const uint64_t bits = std::bit_cast<uint64_t>(mpfr_get_d(v.raw(), MPFR_RNDN));
        return hmix(uint32_t(bits), uint32_t(bits >> 32));
    }
    static bool equal(const MpReal& a, const MpReal& b) { return mpfr_equal_p(a.raw(), b.raw()) != 0; }
    static MpReal zero() { return MpReal(0.0); }
    static MpReal one() { return MpReal(1.0); }
    static MpReal neg_one() { return MpReal(-1.0); }

    static std::optional<long> whole(const MpReal& v) {
        if (mpfr_sgn(v.raw()) < 0 || !mpfr_integer_p(v.raw()) || !mpfr_fits_slong_p(v.raw(), MPFR_RNDN))
            return std::nullopt;
        return mpfr_get_si(v.raw(), MPFR_RNDN);
    }
    static MpReal add(const MpReal& a, const MpReal& b) { return binop(mpfr_add, a, b); }
    static MpReal sub(const MpReal& a, const MpReal& b) { return binop(mpfr_sub, a, b); }
    static MpReal mul(const MpReal& a, const MpReal& b) { return binop(mpfr_mul, a, b); }
    static MpReal div(const MpReal& a, const MpReal& b) { return binop(mpfr_div, a, b); }
    static MpReal pow(const MpReal& a, const MpReal& b) { return binop(mpfr_pow, a, b); }
    static MpReal log(const MpReal& a) { return unop(mpfr_log, a); }
    static MpReal sin(const MpReal& a) { return unop(mpfr_sin, a); }
    static MpReal cos(const MpReal& a) { return unop(mpfr_cos, a); }
    static MpReal tan(const MpReal& a) { return unop(mpfr_tan, a); }
    static MpReal cot(const MpReal& a) { return unop(mpfr_cot, a); }

private:
    using BinOp = int (*)(mpfr_t, mpfr_srcptr, mpfr_srcptr, mpfr_rnd_t);
    using UnOp = int (*)(mpfr_t, mpfr_srcptr, mpfr_rnd_t);
    static MpReal binop(BinOp op, const MpReal& a, const MpReal& b) {
        MpReal r;
        op(r.raw(), a.raw(), b.raw(), MPFR_RNDN);
        return r;
    }
    static MpReal unop(UnOp op, const MpReal& a) {
        MpReal r;
        op(r.raw(), a.raw(), MPFR_RNDN);
        return r;
    }
};
} // namespace cr
