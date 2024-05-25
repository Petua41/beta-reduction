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
    std::unordered_set<std::string> history_{};
    bool no_more_redexes_{false};

protected:
    [[nodiscard]] std::unordered_set<std::string> const& History() const noexcept override {
        return history_;
    }

    [[nodiscard]] bool CannotFindRedexes() const noexcept override {
        return no_more_redexes_;
    }

    [[nodiscard]] std::shared_ptr<model::term::Term> CurrentTerm() const noexcept override {
        return current_term_;
    }

public:
    [[nodiscard]] CBVStrategy(std::shared_ptr<model::term::Term>&& root)
        : current_term_(std::move(root)) {}

    [[nodiscard]] model::strategy::TermInfo SelectNext() noexcept override;

    void SetCurrent(std::shared_ptr<model::term::Term>&& term) noexcept override {
        current_term_ = std::move(term);
    }
};

}  // namespace strategy
