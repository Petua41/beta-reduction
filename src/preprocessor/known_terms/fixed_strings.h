#pragma once

#include "preprocessor/known_terms/fixed_string_known_term.h"

namespace {

constexpr static char kY[] = "Y";
constexpr static char kFixpointComb[] = "(Lf.((Lx.(f(x x)))) (Lx.(f(x x))))))";

constexpr static char kTrue[] = "true";
constexpr static char kTrueTerm[] = "(Lx.(Ly.x))";

constexpr static char kFalse[] = "false";
constexpr static char kFalseTerm[] = "(Lx.(Ly.y))";

}  // namespace

namespace preprocessor::terms {

using FixpointCombinator = FixedStringKnownTerm<kY, kFixpointComb>;

}  // namespace preprocessor::terms
