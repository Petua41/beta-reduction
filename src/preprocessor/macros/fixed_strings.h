#pragma once

#include "preprocessor/macros/fixed_string_macro.h"

namespace {

constexpr static char kY[] = "Y";
constexpr static char kFixpointComb[] = "(Lf.((Lx.(f(x x)))) (Lx.(f(x x))))))";

constexpr static char kTrue[] = "true";
constexpr static char kTrueTerm[] = "(Lx.(Ly.x))";

constexpr static char kFalse[] = "false";
constexpr static char kFalseTerm[] = "(Lx.(Ly.y))";

}  // namespace

namespace preprocessor::terms {

using FixpointCombinator = FixedStringMacro<kY, kFixpointComb>;
using True = FixedStringMacro<kTrue, kTrueTerm>;
// False doesn't have reversed macro, because it's term is the same as the 0's one
using False = FixedStringMacro<kFalse, kFalseTerm, false>;

}  // namespace preprocessor::terms
