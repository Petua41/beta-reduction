#include "strategy/reducer.h"

#include <easylogging++.h>
#include <memory>

#include "model/application.h"
#include "strategy/i_strategy.h"

namespace strategy {

using namespace model;
using namespace model::term;
using namespace model::strategy;

[[nodiscard]] std::pair<bool, std::string> Reducer::Step() const noexcept {
    auto next_redex_info = strategy_->SelectNext();

    auto pre_redex = next_redex_info.term;
    if (pre_redex == nullptr) {
        // No more redexes
        return std::make_pair(true, strategy_->CurrentString());
    }

    auto redex = dynamic_cast<Application*>(pre_redex.get());
    assert(redex != nullptr);  // strategy must return only Applications as redexes

    auto reduced_redex = redex->BetaReductionStep();

    auto old_term = strategy_->CurrentString();

    auto parent = next_redex_info.parent;
    if (parent == nullptr) {
        // Redex was the outermost term
        strategy_->SetCurrent(std::move(reduced_redex));
    } else {
        if (next_redex_info.in_lhs) {
            parent->Lhs(std::move(reduced_redex));
        } else {
            parent->Rhs(std::move(reduced_redex));
        }
    }

    return std::make_pair(false, strategy_->CurrentString());
}

}  // namespace strategy
