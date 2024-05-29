#include "strategy/cbn_strategy.h"

#include <memory>

#include "model/terms.h"
#include "util/dfs.h"

namespace strategy {

using namespace model::strategy;
using namespace model::term;

[[nodiscard]] TermInfo CBNStrategy::SelectNext() noexcept {
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

    return redex_info;
}

}  // namespace strategy
