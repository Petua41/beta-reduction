#pragma once

#include <memory>
#include <vector>

#include "preprocessor/i_known_term.h"
#include "preprocessor/known_terms/fixed_strings.h"

namespace {

using namespace preprocessor::terms;

}  // namespace

namespace preprocessor {

std::vector<std::shared_ptr<IKnownTerm>> const all_known_terms{
        std::make_shared<FixpointCombinator>(), std::make_shared<True>(),
        std::make_shared<False>()};

}  // namespace preprocessor
