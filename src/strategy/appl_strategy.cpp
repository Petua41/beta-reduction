#include "strategy/appl_strategy.h"

#include <cassert>
#include <memory>

#include "model/terms.h"
#include "util/bfs.h"

namespace strategy {

using namespace model::strategy;
using namespace model::term;

TermInfo APPLStrategy::SelectNext() {
    history_.insert(CurrentString());

    auto is_redex = [](std::shared_ptr<Term> term) {
        auto appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    // Split by levels using BFS:
    auto levels = util::algorithm::BFSSplitByLevels(current_term_);
    // Find innermost leftmost:
    auto redex_info = util::algorithm::BFSFindInnermostLeftmost(std::move(levels), is_redex);

    if (redex_info.term == nullptr) {  // didn't found anything
        no_more_redexes_ = true;
    }

    return redex_info;
}

}  // namespace strategy