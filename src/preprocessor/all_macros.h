#pragma once

#include <memory>
#include <vector>

#include "preprocessor/i_macro.h"
#include "preprocessor/macros/church_numeral.h"
#include "preprocessor/macros/fixed_strings.h"
#include "preprocessor/macros/simple_boolean_operations.h"

namespace {

using namespace preprocessor::terms;

}  // namespace

namespace preprocessor {

std::vector<std::shared_ptr<IMacro>> const all_macros{
        std::make_shared<FixpointCombinator>(), std::make_shared<True>(), std::make_shared<False>(),
        std::make_shared<ChuchNumeral>(),       std::make_shared<Not>(),  std::make_shared<Plus>()};

}  // namespace preprocessor
