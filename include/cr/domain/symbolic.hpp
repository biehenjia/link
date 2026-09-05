#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

#include <symengine/add.h>
#include <symengine/basic.h>
#include <symengine/functions.h>
#include <symengine/integer.h>
#include <symengine/mul.h>
#include <symengine/pow.h>

#include "field.hpp"

namespace cr {
    
    using Symbolic = SymEngine::RCP<const SymEngine::Basic>;

    template <>
    struct Field<Symbolic> {
        static uint32_t hash(const Symbolic& s) {
            const std::size_t h = s->hash();
            return uint32_t(h) ^ uint32_t(h >> 32);
        }

        static bool equal(const Symbolic& a, const Symbolic& b) { return SymEngine::eq(*a, *b); }

        static Symbolic zero() { return SymEngine::integer(0); }
        static Symbolic one() { return SymEngine::integer(1); }
        static Symbolic neg_one() { return SymEngine::integer(-1); }

        static std::optional<long> whole(const Symbolic& v) {
            if (!SymEngine::is_a<SymEngine::Integer>(*v)) return std::nullopt;
            const SymEngine::Integer& i = SymEngine::down_cast<const SymEngine::Integer&>(*v);
            if (i.is_negative()) return std::nullopt;
            return long(i.as_int());
        }

        static Symbolic add(const Symbolic& a, const Symbolic& b) { return SymEngine::add(a, b); }
        static Symbolic sub(const Symbolic& a, const Symbolic& b) { return SymEngine::sub(a, b); }
        static Symbolic mul(const Symbolic& a, const Symbolic& b) { return SymEngine::mul(a, b); }
        static Symbolic div(const Symbolic& a, const Symbolic& b) { return SymEngine::div(a, b); }
        static Symbolic pow(const Symbolic& a, const Symbolic& b) { return SymEngine::pow(a, b); }
        static Symbolic log(const Symbolic& a) { return SymEngine::log(a); }
        static Symbolic sin(const Symbolic& a) { return SymEngine::sin(a); }
        static Symbolic cos(const Symbolic& a) { return SymEngine::cos(a); }
        static Symbolic tan(const Symbolic& a) { return SymEngine::tan(a); }
        static Symbolic cot(const Symbolic& a) { return SymEngine::cot(a); }
    };

}
