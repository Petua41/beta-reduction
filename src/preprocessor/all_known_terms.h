#pragma once

#include <memory>
#include <vector>

#include "preprocessor/i_known_term.h"
#include "preprocessor/known_terms/fixed_strings.h"

namespace preprocessor {

std::vector<std::shared_ptr<IKnownTerm>> const all_known_terms{
        std::make_shared<terms::FixpointCombinator>()};

}  // namespace preprocessor
