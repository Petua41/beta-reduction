#include "strategy/appl_strategy.h"

#include <memory>

#include "model/terms.h"
#include "util/bfs.h"

namespace strategy {

using namespace model::strategy;
using namespace model::term;

[[nodiscard]] TermInfo APPLStrategy::SelectNext() noexcept {
    auto is_redex = [](std::shared_ptr<Term> term) {
        auto appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    // Split by levels using BFS:
    auto levels = util::algorithm::BFSSplitByLevels(current_term_);
    // Find innermost leftmost:
    auto redex_info = util::algorithm::BFSFindInnermostLeftmost(std::move(levels), is_redex);

    return redex_info;
}

}  // namespace strategy
