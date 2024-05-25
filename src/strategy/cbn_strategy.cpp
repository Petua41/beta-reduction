#include "strategy/cbn_strategy.h"

#include <cassert>
#include <memory>

#include "model/terms.h"
#include "util/dfs.h"

namespace strategy {

using namespace model::strategy;
using namespace model::term;

[[nodiscard]] TermInfo CBNStrategy::SelectNext() noexcept {
    history_.insert(CurrentString());

    auto is_redex = [](std::shared_ptr<Term> term) {
        auto appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    auto is_abstr = [](std::shared_ptr<Term> term) {
        return dynamic_cast<Abstraction*>(term.get()) != nullptr;
    };

    // Find leftmost using DFS, but skip Abstractions' children:
    auto redex_info =
            util::algorithm::DFSFindTermInfoSkipChildren(current_term_, is_redex, is_abstr);

    if (redex_info.term == nullptr) {  // haven't found anything
        no_more_redexes_ = true;
    }

    return redex_info;
}

}  // namespace strategy
