#pragma once

#include <cassert>
#include <memory>
#include <sstream>
#include <string>

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

    void AlphaConversionStep(std::string const& new_variable_name) override {
        lhs_->AlphaConversionStep(new_variable_name);
        rhs_->AlphaConversionStep(new_variable_name);
    }

    std::shared_ptr<Term> ReplaceVariableWithTerm(std::string const& variable_name,
                                                  std::shared_ptr<Term> const term) const override {
        std::shared_ptr<Term> new_lhs = lhs_->ReplaceVariableWithTerm(variable_name, term);
        std::shared_ptr<Term> new_rhs = rhs_->ReplaceVariableWithTerm(variable_name, term);
        return std::make_shared<Application>(std::move(new_lhs), std::move(new_rhs));
    }

    std::shared_ptr<Term> Lhs() const {
        return lhs_;
    }

    std::shared_ptr<Term> Rhs() const {
        return rhs_;
    }

    void ReplaceLhs(std::shared_ptr<Term>&& new_lhs) override {
        lhs_ = std::move(new_lhs);
    }

    void ReplaceRhs(std::shared_ptr<Term>&& new_rhs) override {
        rhs_ = std::move(new_rhs);
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
