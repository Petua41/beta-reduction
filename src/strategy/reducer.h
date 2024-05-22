#pragma once

#include <easylogging++.h>
#include <memory>
#include <string>

#include "config/misc.h"
#include "config/names.h"
#include "model/enums/reduction_exit_status.h"
#include "model/enums/reduction_strategies.h"
#include "model/enums/strategy_states.h"
#include "model/reduction_result.h"
#include "model/term.h"
#include "strategy/strategies.h"

namespace strategy {

class Reducer {
private:
    std::unique_ptr<IStrategy> strategy_;

public:
    Reducer(std::shared_ptr<model::term::Term>&& root,
            model::ReductionStrategies strat = model::ReductionStrategies::NO) {
        switch (strat) {
            case model::ReductionStrategies::NO:
                strategy_ = std::make_unique<NOStrategy>(std::move(root));
                break;
            case model::ReductionStrategies::APPL:
                strategy_ = std::make_unique<APPLStrategy>(std::move(root));
            default:
                break;
        }
    }

    model::strategy::ReductionResult MainLoop() {
        using namespace model;
        using namespace model::strategy;

        unsigned operations{0};

        while (true) {
            ++operations;
            if (operations > config::misc::max_operations && config::misc::max_operations > 0) {
                LOG(WARNING) << "Too many reduction steps!";
                return ReductionResult{strategy_->CurrentString(),
                                       ReductionExitStatus::TooManyOperations};
            }

            auto state = strategy_->State();
            switch (state) {
                case StrategyStates::NormalForm:
                    return ReductionResult{strategy_->CurrentString(),
                                           ReductionExitStatus::NormalForm};
                case StrategyStates::Loop:
                    LOG(INFO) << "Entered loop";
                    return ReductionResult{strategy_->CurrentString(), ReductionExitStatus::Loop};
                default:
                    break;
            }

            auto next_redex_info = strategy_->SelectNext();

            auto pre_redex = next_redex_info.term;
            if (pre_redex == nullptr) {  // no more redexes, strategy will handle it
                LOG(INFO) << "Haven't found any redexes";
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
};

}  // namespace strategy
