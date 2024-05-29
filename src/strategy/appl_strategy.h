#pragma once

#include <memory>
#include <unordered_set>

#include "model/term_info.h"
#include "model/terms.h"
#include "strategy/i_strategy.h"

namespace strategy {

/// @brief Applicative-order strategy (always reduce innermost leftmost redex)
class APPLStrategy : public IStrategy {
private:
    std::shared_ptr<model::term::Term> current_term_;

public:
    APPLStrategy(std::shared_ptr<model::term::Term>&& root) : current_term_(std::move(root)) {}

    [[nodiscard]] model::strategy::TermInfo SelectNext() noexcept override;

    [[nodiscard]] virtual std::string CurrentString() const noexcept override {
        return current_term_->ToString();
    }

    void SetCurrent(std::shared_ptr<model::term::Term>&& term) noexcept override {
        current_term_ = std::move(term);
    }
};

}  // namespace strategy
