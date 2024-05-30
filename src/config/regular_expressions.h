#pragma once

#include <boost/xpressive/xpressive_static.hpp>

#include "config/regex_helper_classes.h"

namespace config::regular_expressions {

// Term types:
helper_classes::XpressiveTermTypes const kTermTypeRegexes{};

// Macros:
boost::xpressive::sregex const kChurchNumeralRegex = +boost::xpressive::digit;

}  // namespace config::regular_expressions
