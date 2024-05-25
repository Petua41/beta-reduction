#include "strategy/reducer.h"

#include <easylogging++.h>
#include <memory>

#include "model/application.h"
#include "model/enums/reduction_exit_status.h"
#include "model/enums/strategy_states.h"
#include "model/reduction_result.h"
#include "strategy/i_strategy.h"

namespace strategy {

using namespace model;
using namespace model::term;
using namespace model::strategy;

[[nodiscard]] ReductionResult Reducer::MainLoop() noexcept {
    unsigned operations{0};

    while (true) {
        ++operations;
        if (operations > max_operations_ && max_operations_ > 0) {
            LOG(WARNING) << "Too many reduction steps!";
            return ReductionResult{strategy_->CurrentString(),
                                   ReductionExitStatus::TooManyOperations};
        }

        auto state = strategy_->State();
        switch (state) {
            case StrategyStates::NormalForm:
                return ReductionResult{strategy_->CurrentString(), ReductionExitStatus::NormalForm};
            case StrategyStates::Loop:
                return ReductionResult{strategy_->CurrentString(), ReductionExitStatus::Loop};
            default:
                break;
        }

        auto next_redex_info = strategy_->SelectNext();

        auto pre_redex = next_redex_info.term;
        if (pre_redex == nullptr) {  // no more redexes, strategy will handle it
            continue;
        }

        auto redex = dynamic_cast<Application*>(pre_redex.get());
        assert(redex != nullptr);  // strategy must return only Applications as redexes

        auto reduced_redex = redex->BetaReductionStep();

        LOG(INFO) << redex->ToString() << " -" << config::Names::Instance().Beta() << "-> "
                  << reduced_redex->ToString();

        auto parent = next_redex_info.parent;
        if (parent == nullptr) {  // redex was the outermost term
            strategy_->SetCurrent(std::move(reduced_redex));
        } else {
            if (next_redex_info.in_lhs) {
                parent->ReplaceLhs(std::move(reduced_redex));
            } else {
                parent->ReplaceRhs(std::move(reduced_redex));
            }
        }
    }
}

}  // namespace strategy
