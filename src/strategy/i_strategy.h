#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "model/enums/strategy_states.h"
#include "model/term_info.h"
#include "model/terms.h"
#include "util/dfs.h"

namespace strategy {

class IStrategy {
protected:
    virtual std::unordered_set<std::string> const& History() const = 0;
    virtual bool CannotFindRedexes() const = 0;
    virtual std::shared_ptr<model::term::Term> CurrentTerm() const = 0;

    virtual bool IsInLoop() {
        auto history = History();
        auto current_str = CurrentString();

        return history.find(current_str) != history.end();
    }

public:
    virtual model::strategy::TermInfo SelectNext() = 0;

    virtual model::StrategyStates State() {
        if (CannotFindRedexes()) {
            return model::StrategyStates::NormalForm;
        }
        if (IsInLoop()) {
            return model::StrategyStates::Loop;
        }
        return model::StrategyStates::Ready;
    }

    virtual std::string CurrentString() const {
        return CurrentTerm()->ToString();
    }

    virtual void SetCurrent(std::shared_ptr<model::term::Term>&& term) = 0;
};

}  // namespace strategy
