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
    [[nodiscard]] virtual std::unordered_set<std::string> const& History() const noexcept = 0;
    [[nodiscard]] virtual bool CannotFindRedexes() const noexcept = 0;
    [[nodiscard]] virtual std::shared_ptr<model::term::Term> CurrentTerm() const noexcept = 0;

    [[nodiscard]] virtual bool IsInLoop() noexcept {
        auto history = History();
        auto current_str = CurrentString();

        return history.find(current_str) != history.end();
    }

public:
    [[nodiscard]] virtual model::strategy::TermInfo SelectNext() noexcept = 0;

    [[nodiscard]] virtual model::StrategyStates State() noexcept {
        if (CannotFindRedexes()) {
            return model::StrategyStates::NormalForm;
        }
        if (IsInLoop()) {
            return model::StrategyStates::Loop;
        }
        return model::StrategyStates::Ready;
    }

    [[nodiscard]] virtual std::string CurrentString() const noexcept {
        return CurrentTerm()->ToString();
    }

    virtual void SetCurrent(std::shared_ptr<model::term::Term>&& term) noexcept = 0;
};

}  // namespace strategy
