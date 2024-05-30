#pragma once

#include <functional>
#include <memory>

#include "model/enums/reduction_strategies.h"
#include "model/term.h"
#include "strategy/strategies.h"
#include "strategy/term_info.h"

namespace strategy {

class Reducer {
private:
    std::shared_ptr<model::term::Term> current_term_;
    std::function<TermInfo(std::shared_ptr<model::term::Term>)> select_next_;

public:
    Reducer(std::shared_ptr<model::term::Term>&& root,
            model::ReductionStrategies strat = model::ReductionStrategies::NO)
        : current_term_(std::move(root)) {
        using enum model::ReductionStrategies;

        switch (strat) {
            case NO:
                select_next_ = &NOSelectNext;
                break;
            case APPL:
                select_next_ = &APPLSelectNext;
                break;
            case CBN:
                select_next_ = &CBNSelectNext;
                break;
            case CBV:
                select_next_ = &CBVSelectNext;
                break;
        }
    }

    [[nodiscard]] std::pair<bool, std::string> Step() noexcept;
};

}  // namespace strategy
