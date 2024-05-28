#pragma once

#include <boost/xpressive/xpressive_static.hpp>
#include <regex>

#include "config/regex_helper_classes.h"

namespace {

using namespace boost::xpressive;

}  // namespace

namespace config::regular_expressions {

// Term types:
helper_classes::XpressiveTermTypes const kTermTypeRegexes{};

// Macros:
sregex const church_numeral_regex = +digit;

// auto const church_numeral_regex = boost::xpressive::sregex::compile(R"([0-9]+)");

}  // namespace config::regular_expressions
