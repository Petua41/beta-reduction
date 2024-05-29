#include "strategy/cbv_strategy.h"

#include <memory>

#include "model/terms.h"
#include "util/bfs.h"

namespace strategy {

using namespace model::strategy;
using namespace model::term;

[[nodiscard]] TermInfo CBVStrategy::SelectNext() noexcept {
    auto is_redex = [](std::shared_ptr<Term> term) {
        auto appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    // Split by levels using BFS, but skip children of Abstractions:
    auto levels = util::algorithm::BFSSplitByLevelsSkipChildren(
            current_term_, [](std::shared_ptr<Term> term) {
                return dynamic_cast<Abstraction*>(term.get()) != nullptr;
            });
    // Find innermost leftmost:
    auto redex_info = util::algorithm::BFSFindInnermostLeftmost(std::move(levels), is_redex);

    if (redex_info.term == nullptr) {  // haven't found anything
        no_more_redexes_ = true;
    }

    return redex_info;
}

}  // namespace strategy
