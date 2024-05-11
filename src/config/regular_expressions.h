#pragma once

#include <regex>

namespace config::regular_expressions {

std::regex const variable_regex{R"([a-zA-Z]+)"};
std::regex const abstraction_regex{R"(\([L|λ][a-zA-Z]+\..*\))"};
std::regex const application_regex{R"(\(.* .*\))"};

}  // namespace config::regular_expressions
