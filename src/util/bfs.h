#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "model/term.h"
#include "strategy/term_info.h"

namespace util::algorithm {

std::stack<std::vector<strategy::TermInfo>> BFSSplitByLevels(
        std::shared_ptr<model::term::Term> root);

std::stack<std::vector<strategy::TermInfo>> BFSSplitByLevelsSkipChildren(
        std::shared_ptr<model::term::Term> root,
        std::function<bool(std::shared_ptr<model::term::Term>)> skip_if);

strategy::TermInfo BFSFindInnermostLeftmost(
        std::stack<std::vector<strategy::TermInfo>> levels,
        std::function<bool(std::shared_ptr<model::term::Term>)> pred);

}  // namespace util::algorithm
