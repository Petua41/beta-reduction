#pragma once

#include <easylogging++.h>
#include <memory>
#include <string>
#include <unordered_set>

#include "model/term.h"
#include "model/variable.h"

namespace model::term {

static size_t var_number{0};

class Abstraction : public Term {
private:
    Variable lhs_;
    std::shared_ptr<Term> rhs_;

public:
    Abstraction(Variable&& lhs, std::shared_ptr<Term>&& rhs)
        : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    Abstraction(Variable const& lhs, std::shared_ptr<Term>&& rhs)
        : lhs_(lhs), rhs_(std::move(rhs)) {}

    std::shared_ptr<Term> ReplaceVariableWithTerm(std::string const& variable_name,
                                                  std::shared_ptr<Term> const term) const override {
        std::shared_ptr<Term> new_rhs = rhs_->ReplaceVariableWithTerm(variable_name, term);
        return std::make_shared<Abstraction>(lhs_, std::move(new_rhs));
    }

    std::shared_ptr<Term> Rhs() const override {
        return rhs_;
    }

    std::unordered_set<std::string> GetFreeVariables() const noexcept {
        // In terms of a single Abstraction, free variables are {rhs} \ {lhs}
        auto rhs_free_vars = rhs_->GetFreeVariables();
        auto it = rhs_free_vars.find(VariableName());

        if (it != rhs_free_vars.end()) {
            rhs_free_vars.erase(it);
        }
        return rhs_free_vars;
    }

    void ReplaceBoundVariables(std::unordered_set<std::string> const& for_replacement) override {
        auto const& lhs_name = VariableName();
        if (for_replacement.find(lhs_name) != for_replacement.end()) {
            // This variable is bound for this Abstraction
            std::string new_name = 'x' + std::to_string(var_number++);
            lhs_ = Variable{new_name};
            rhs_->ReplaceFreeVariable(lhs_name, new_name);
        }
        // Pass this query:
        rhs_->ReplaceBoundVariables(for_replacement);
    }

    void ReplaceFreeVariable(std::string const& for_replacement,
                             std::string const& new_name) override {
        auto const& lhs_name = VariableName();
        if (lhs_name != for_replacement) {
            // This variable is free for this Abstraction
            rhs_->ReplaceFreeVariable(lhs_name, new_name);
        }
    }

    std::string VariableName() const {
        return lhs_.ToString();
    }

    void Rhs(std::shared_ptr<Term>&& new_rhs) override {
        rhs_ = std::move(new_rhs);
    }

    std::string ToString() const override {
        return "(L" + lhs_.ToString() + '.' + rhs_->ToString() + ')';
    }
};

}  // namespace model::term
