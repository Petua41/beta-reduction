#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "model/term_info.h"
#include "model/terms.h"
#include "strategy/i_strategy.h"

namespace strategy {

/// @brief Normal-order strategy (always reduce leftmost redex)
class NOStrategy : public IStrategy {
private:
    std::shared_ptr<model::term::Term> current_term_;
    bool no_more_redexes_{false};

public:
    NOStrategy(std::shared_ptr<model::term::Term>&& root) : current_term_(std::move(root)) {}

    [[nodiscard]] model::strategy::TermInfo SelectNext() noexcept override;

    [[nodiscard]] virtual std::string CurrentString() const noexcept override {
        return current_term_->ToString();
    }

    void SetCurrent(std::shared_ptr<model::term::Term>&& term) noexcept override {
        current_term_ = std::move(term);
    }

    [[nodiscard]] virtual bool IsInNormalForm() const noexcept override {
        return no_more_redexes_;
    }
};

}  // namespace strategy
