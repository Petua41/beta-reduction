#include "reducer/reducer.h"

#include <easylogging++.h>
#include <memory>

#include "model/application.h"

namespace reducer {

using namespace model::term;

[[nodiscard]] std::pair<bool, std::string> Reducer::Step() noexcept {
    auto next_redex_info = select_next_(current_term_);

    auto pre_redex = next_redex_info.term;
    if (pre_redex == nullptr) {
        // No more redexes
        return std::make_pair(true, current_term_->ToString());
    }

    auto redex = dynamic_cast<Application*>(pre_redex.get());
    assert(redex != nullptr);  // strategy must select only Applications as redexes

    auto reduced_redex = redex->BetaReductionStep();

    auto parent = next_redex_info.parent;
    if (parent == nullptr) {
        // Redex was the outermost term
        current_term_ = std::move(reduced_redex);
    } else {
        if (next_redex_info.in_lhs) {
            parent->Lhs(std::move(reduced_redex));
        } else {
            parent->Rhs(std::move(reduced_redex));
        }
    }

    return std::make_pair(false, current_term_->ToString());
}

}  // namespace reducer
