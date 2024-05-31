#pragma once

#include <memory>
#include <string>
#include <vector>

#include "preprocessor/i_macro.h"
#include "preprocessor/macros/church_numeral.h"
#include "preprocessor/macros/fixed_strings.h"

namespace preprocessor {

static std::vector<std::shared_ptr<IMacro>> const kAllMacros{
        std::make_shared<terms::FixpointCombinator>(),
        std::make_shared<terms::True>(),
        std::make_shared<terms::False>(),
        std::make_shared<terms::ChuchNumeral>(),
        std::make_shared<terms::Not>(),
        std::make_shared<terms::IsZero>(),
        std::make_shared<terms::Plus>(),
        std::make_shared<terms::Mult>(),
        std::make_shared<terms::And>(),
        std::make_shared<terms::Or>()};

[[nodiscard]] std::string CheckBrackets(std::string input);
[[nodiscard]] std::string ReplaceMacros(std::string input) noexcept;

}  // namespace preprocessor
