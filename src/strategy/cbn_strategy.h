#pragma once

#include <memory>
#include <unordered_set>

#include "model/term_info.h"
#include "model/terms.h"
#include "strategy/i_strategy.h"

namespace strategy {

/// @brief Call-by-value strategy
/// (always reduce leftmost redex, but don't reduce inside Abstractions)
class CBNStrategy : public IStrategy {
private:
    std::shared_ptr<model::term::Term> current_term_;
    std::unordered_set<std::string> history_{};
    bool no_more_redexes_{false};

protected:
    std::unordered_set<std::string> const& History() const override {
        return history_;
    }

    bool CannotFindRedexes() const override {
        return no_more_redexes_;
    }

    std::shared_ptr<model::term::Term> CurrentTerm() const override {
        return current_term_;
    }

public:
    CBNStrategy(std::shared_ptr<model::term::Term>&& root) : current_term_(std::move(root)) {}

    model::strategy::TermInfo SelectNext() override;

    void SetCurrent(std::shared_ptr<model::term::Term>&& term) {
        current_term_ = std::move(term);
    }
};

}  // namespace strategy
