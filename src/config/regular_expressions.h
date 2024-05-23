#pragma once

#include <regex>

namespace config::regular_expressions {

// Term types:
std::regex const variable_regex{R"([a-zA-Z]+)"};
std::regex const abstraction_regex{R"(\([L|λ][a-zA-Z]+\..*\))"};
std::regex const application_regex{R"(\(.* .*\))"};

// Macros:
std::regex const church_numeral_regex{R"([0-9]+)"};

}  // namespace config::regular_expressions
