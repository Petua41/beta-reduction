#pragma once

#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

#include "model/abstraction.h"
#include "model/term.h"

namespace model::term {

class Application : public Term {
private:
    std::shared_ptr<Term> lhs_;
    std::shared_ptr<Term> rhs_;

public:
    Application(std::shared_ptr<Term>&& lhs, std::shared_ptr<Term>&& rhs)
        : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    std::shared_ptr<Term> ReplaceVariableWithTerm(std::string const& variable_name,
                                                  std::shared_ptr<Term> const term) const override {
        std::shared_ptr<Term> new_lhs = lhs_->ReplaceVariableWithTerm(variable_name, term);
        std::shared_ptr<Term> new_rhs = rhs_->ReplaceVariableWithTerm(variable_name, term);
        return std::make_shared<Application>(std::move(new_lhs), std::move(new_rhs));
    }

    std::shared_ptr<Term> Lhs() const override {
        return lhs_;
    }

    std::shared_ptr<Term> Rhs() const override {
        return rhs_;
    }

    void Lhs(std::shared_ptr<Term>&& new_lhs) override {
        lhs_ = std::move(new_lhs);
    }

    void Rhs(std::shared_ptr<Term>&& new_rhs) override {
        rhs_ = std::move(new_rhs);
    }

    std::unordered_set<std::string> GetFreeVariables() const noexcept {
        // In terms of single Application, free variables are {lhs} \cap {rhs}
        auto lhs_free_vars = lhs_->GetFreeVariables();
        auto rhs_free_vars = rhs_->GetFreeVariables();
        lhs_free_vars.merge(std::move(rhs_free_vars));
        return lhs_free_vars;
    }

    void ReplaceBoundVariables(std::unordered_set<std::string> const& for_replacement) override {
        // Application cannot decide if variable is bound, so just pass this query
        lhs_->ReplaceBoundVariables(for_replacement);
        rhs_->ReplaceBoundVariables(for_replacement);
    }

    void ReplaceFreeVariable(std::string const& for_replacement,
                             std::string const& new_name) override {
        // Application cannot decide if variable is free, so just pass this query
        lhs_->ReplaceFreeVariable(for_replacement, new_name);
        rhs_->ReplaceFreeVariable(for_replacement, new_name);
    }

    std::shared_ptr<Term> BetaReductionStep() {
        auto* lhs_abstr = dynamic_cast<Abstraction*>(lhs_.get());
        // Only Application with Abstraction in Lhs can be reduced:
        assert(lhs_abstr != nullptr);
        auto var_name = lhs_abstr->VariableName();

        auto new_lhs = lhs_->ReplaceVariableWithTerm(var_name, rhs_);
        auto* new_lhs_abstr = dynamic_cast<Abstraction*>(new_lhs.get());
        // After variable replacement, Abstraction must stay Abstraction:
        assert(new_lhs_abstr != nullptr);

        return new_lhs_abstr->Rhs();
    }

    bool IsRedex() {
        // If left part of Application is Abstraction, it can be reduced
        return dynamic_cast<Abstraction*>(lhs_.get()) != nullptr;
    }

    std::string ToString() const override {
        std::stringstream sstream;
        sstream << '(' << lhs_->ToString() << ' ' << rhs_->ToString() << ')';
        return sstream.str();
    }
};

}  // namespace model::term
