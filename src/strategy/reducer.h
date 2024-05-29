#pragma once

#include <memory>

#include "model/enums/reduction_strategies.h"
#include "model/term.h"
#include "strategy/strategies.h"

namespace strategy {

class Reducer {
private:
    std::unique_ptr<IStrategy> strategy_;

public:
    Reducer(std::shared_ptr<model::term::Term>&& root,
            model::ReductionStrategies strat = model::ReductionStrategies::NO) {
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

    [[nodiscard]] std::pair<bool, std::string> Step() const noexcept;
};

}  // namespace strategy
