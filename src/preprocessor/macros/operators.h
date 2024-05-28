#pragma once

#include "preprocessor/macros/binary_operator_macro.h"
#include "preprocessor/macros/unary_operator_macro.h"

namespace {

constexpr static char const kNot[] = "NOT";
constexpr static char const kNotTermPrefix[] = "(Lp.((p (Lx.(Ly.y))) (Lx.(Ly.x)))) ";
constexpr static char const kNotTermSuffix[] = "";

constexpr static char const kIsZero[] = "ISZERO";
constexpr static char const kIsZeroTermPrefix[] = "(Ln.((n (Lx.(Lx.(Ly.y)))) (Lx.(Ly.x)))) ";
constexpr static char const kIsZeroTermSuffix[] = "";

constexpr static char const kPlus[] = "+";
constexpr static char const kPlusTermPrefix[] = "((Lm.(Ln.(Lf.(Lx.((m f) ((n f) x)))))) ";
constexpr static char const kPlusTermInter[] = ") ";
constexpr static char const kPlusTermSuffix[] = "";

constexpr static char const kMult[] = "*";
constexpr static char const kMultTermPrefix[] = "((Lm.(Ln.(Lf.(Lx.((m (n f)) x))))) ";
constexpr static char const kMultTermInter[] = ") ";
constexpr static char const kMultTermSuffix[] = "";

constexpr static char const kAnd[] = "&&";
constexpr static char const kAndTermPrefix[] = "((Lp.(Lq.((p q) p))) ";
constexpr static char const kAndTermInter[] = ") ";
constexpr static char const kAndTermSuffix[] = "";

constexpr static char const kOr[] = "||";
constexpr static char const kOrTermPrefix[] = "((Lp.(Lq.((p p) q))) ";
constexpr static char const kOrTermInter[] = ") ";
constexpr static char const kOrTermSuffix[] = "";

}  // namespace

namespace preprocessor::terms {

using Not = UnaryOperatorMacro<kNot, kNotTermPrefix, kNotTermSuffix>;
using IsZero = UnaryOperatorMacro<kIsZero, kIsZeroTermPrefix, kIsZeroTermSuffix>;

using Plus = BinaryOperatorMacro<kPlus, kPlusTermPrefix, kPlusTermInter, kPlusTermSuffix>;
using Mult = BinaryOperatorMacro<kMult, kMultTermPrefix, kMultTermInter, kMultTermSuffix>;
using And = BinaryOperatorMacro<kAnd, kAndTermPrefix, kAndTermInter, kAndTermSuffix>;
using Or = BinaryOperatorMacro<kOr, kOrTermPrefix, kOrTermInter, kOrTermSuffix>;

}  // namespace preprocessor::terms
