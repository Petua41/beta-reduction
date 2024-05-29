#pragma once

#include <memory>
#include <unordered_set>

#include "model/term_info.h"
#include "model/terms.h"
#include "strategy/i_strategy.h"

namespace strategy {

/// @brief Call-by-value strategy
/// (always reduce innermost leftmost redex, but don't reduce inside Abstractions)
class CBVStrategy : public IStrategy {
private:
    std::shared_ptr<model::term::Term> current_term_;
    bool no_more_redexes_{false};

public:
    CBVStrategy(std::shared_ptr<model::term::Term>&& root) : current_term_(std::move(root)) {}

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
