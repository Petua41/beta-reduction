#pragma once

#include "preprocessor/macros/unary_operator_macro.h"

namespace {

constexpr static char const kNot[] = "NOT";
constexpr static char const kNotTermPrefix[] = "(Lp.((p (Lx.(Ly.y))) (Lx.(Ly.x)))) ";
constexpr static char const kNotTermSuffix[] = "";

}  // namespace

namespace preprocessor::terms {

using Not = UnaryOperatorMacro<kNot, kNotTermPrefix, kNotTermSuffix>;

}  // namespace preprocessor::terms
