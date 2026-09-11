#pragma once
#include <cstdint>
namespace cr {
enum class Kind : uint8_t {
    Leaf = 0,
    Ref,
    Pair, // atomic type node
    Sum,
    Prod,
    Rot, // chain type node
    Sin,
    Cos,
    Tan,
    Cot, // trig wrapper node
    EAdd,
    ESub,
    EMul,
    EDiv,
    EPow,
    ELog, // binary algebraic combinator
    ESin,
    ECos,
    ETan,
    ECot, // unary algebraic combinator
};
struct Node {
    Kind kind;
    uint8_t var;
    uint16_t len;
    uint32_t ops;
};
} // namespace cr
