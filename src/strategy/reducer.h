#pragma once

#include <memory>

#include "model/enums/reduction_strategies.h"
#include "model/reduction_result.h"
#include "model/term.h"
#include "strategy/strategies.h"

namespace strategy {

class Reducer {
private:
    std::unique_ptr<IStrategy> strategy_;
    unsigned max_operations_;

public:
    Reducer(std::shared_ptr<model::term::Term>&& root,
            model::ReductionStrategies strat = model::ReductionStrategies::NO,
            unsigned max_operations = 0)
        : max_operations_(max_operations) {
        using enum model::ReductionStrategies;

        switch (strat) {
            case NO:
                strategy_ = std::make_unique<NOStrategy>(std::move(root));
                break;
            case APPL:
                strategy_ = std::make_unique<APPLStrategy>(std::move(root));
                break;
            case CBV:
                strategy_ = std::make_unique<CBVStrategy>(std::move(root));
                break;
            case CBN:
                strategy_ = std::make_unique<CBNStrategy>(std::move(root));
                break;
        }
    }

    [[nodiscard]] model::strategy::ReductionResult MainLoop() noexcept;
};

}  // namespace strategy
