#pragma once

#include "preprocessor/macros/fixed_string_macro.h"

constexpr static char kY[] = "Y";
constexpr static char kFixpointComb[] = "(Lf.((Lx.(f (x x))) (Lx.(f (x x)))))";

constexpr static char kTrue[] = "true";
constexpr static char kTrueTerm[] = "(Lx.(Ly.x))";

constexpr static char kFalse[] = "false";
constexpr static char kFalseTerm[] = "(Lx.(Ly.y))";

constexpr char kNot[] = "NOT";
constexpr char kNotTerm[] = "(Lp.((p (Lx.(Ly.y))) (Lx.(Ly.x))))";

constexpr char kIsZero[] = "ISZERO";
constexpr char kIsZeroTerm[] = "(Ln.((n (Lx.(Lx.(Ly.y)))) (Lx.(Ly.x))))";

constexpr char kPlus[] = "+";
constexpr char kPlusTerm[] = "(Lm.(Ln.(Lf.(Lx.((m f) ((n f) x))))))";

constexpr char kMult[] = "*";
constexpr char kMultTerm[] = "(Lm.(Ln.(Lf.(Lx.((m (n f)) x)))))";

constexpr char kAnd[] = "&&";
constexpr char kAndTerm[] = "(Lp.(Lq.((p q) p)))";

constexpr char kOr[] = "||";
constexpr char kOrTerm[] = "(Lp.(Lq.((p p) q)))";

namespace preprocessor::terms {

using FixpointCombinator = FixedStringMacro<kY, kFixpointComb>;

using True = FixedStringMacro<kTrue, kTrueTerm>;
using False = FixedStringMacro<kFalse, kFalseTerm>;

using Not = FixedStringMacro<kNot, kNotTerm>;
using IsZero = FixedStringMacro<kIsZero, kIsZeroTerm>;

using Plus = FixedStringMacro<kPlus, kPlusTerm>;
using Mult = FixedStringMacro<kMult, kMultTerm>;

using And = FixedStringMacro<kAnd, kAndTerm>;
using Or = FixedStringMacro<kOr, kOrTerm>;

}  // namespace preprocessor::terms
