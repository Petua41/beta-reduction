#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "model/term.h"
#include "model/term_info.h"

namespace util::algorithm {

std::stack<std::vector<model::strategy::TermInfo>> BFSSplitByLevels(
        std::shared_ptr<model::term::Term> root);

std::stack<std::vector<model::strategy::TermInfo>> BFSSplitByLevelsSkipChildren(
        std::shared_ptr<model::term::Term> root,
        std::function<bool(std::shared_ptr<model::term::Term>)> skip_if);

model::strategy::TermInfo BFSFindInnermostLeftmost(
        std::stack<std::vector<model::strategy::TermInfo>> levels,
        std::function<bool(std::shared_ptr<model::term::Term>)> pred);

}  // namespace util::algorithm
