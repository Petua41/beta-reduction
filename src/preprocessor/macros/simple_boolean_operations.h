#pragma once

#include "preprocessor/macros/binary_operator_macro.h"
#include "preprocessor/macros/unary_operator_macro.h"

namespace {

constexpr static char const kNot[] = "NOT";
constexpr static char const kNotTermPrefix[] = "(Lp.((p (Lx.(Ly.y))) (Lx.(Ly.x)))) ";
constexpr static char const kNotTermSuffix[] = "";

constexpr static char const kPlus[] = "+";
constexpr static char const kPlusTermPrefix[] = "(((Lm.(Ln.(Lf.(Lx.((m f) ((n f) x)))))) ";
constexpr static char const kPlusTermInter[] = ") ";
constexpr static char const kPlusTermSuffix[] = ")";

}  // namespace

namespace preprocessor::terms {

using Not = UnaryOperatorMacro<kNot, kNotTermPrefix, kNotTermSuffix>;
using Plus = BinaryOperatorMacro<kPlus, kPlusTermPrefix, kPlusTermInter, kPlusTermSuffix>;

}  // namespace preprocessor::terms
