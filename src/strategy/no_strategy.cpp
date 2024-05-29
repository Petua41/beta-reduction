#include "strategy/no_strategy.h"

#include <memory>

#include "model/terms.h"
#include "util/dfs.h"

namespace strategy {

using namespace model::strategy;
using namespace model::term;

[[nodiscard]] TermInfo NOStrategy::SelectNext() noexcept {
    auto is_redex = [](std::shared_ptr<Term> term) {
        auto appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    // DFS will firstly find leftmost redex
    auto redex_info = util::algorithm::DFSFindTermInfo(current_term_, is_redex);

    return redex_info;
}

}  // namespace strategy
