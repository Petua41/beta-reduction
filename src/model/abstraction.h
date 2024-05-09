#pragma once

#include <easylogging++.h>
#include <memory>
#include <sstream>
#include <string>

#include "config/names.h"
#include "model/term.h"
#include "model/variable.h"

namespace model::term {

class Abstraction : public Term {
private:
    Variable lhs_;
    std::shared_ptr<Term> rhs_;

public:
    Abstraction(Variable&& lhs, std::shared_ptr<Term>&& rhs)
        : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    Abstraction(Variable const& lhs, std::shared_ptr<Term>&& rhs)
        : lhs_(lhs), rhs_(std::move(rhs)) {}

    void AlphaConversionStep(std::string const& new_variable_name) override {
        lhs_.AlphaConversionStep(new_variable_name);
        rhs_->AlphaConversionStep(new_variable_name);
    }

    std::shared_ptr<Term> ReplaceVariableWithTerm(std::string const& variable_name,
                                                  std::shared_ptr<Term> const term) const override {
        std::shared_ptr<Term> new_rhs = rhs_->ReplaceVariableWithTerm(variable_name, term);
        return std::make_shared<Abstraction>(lhs_, std::move(new_rhs));
    }

    std::shared_ptr<Variable> Lhs() const {
        return std::make_shared<Variable>(lhs_);
    }

    std::shared_ptr<Term> Rhs() const {
        return rhs_;
    }

    std::string VariableName() const {
        return lhs_.ToString();
    }

    void ReplaceLhs(std::shared_ptr<Term>&& new_lhs) override {
        LOG(WARNING) << "Trying to replace Variable's Lhs with " << new_lhs->ToString();
    }

    void ReplaceRhs(std::shared_ptr<Term>&& new_rhs) override {
        rhs_ = std::move(new_rhs);
    }

    std::string ToString() const override {
        std::stringstream sstream;
        sstream << '(' << config::Names::Instance().Lambda() << lhs_.ToString() << '.'
                << rhs_->ToString() << ')';
        return sstream.str();
    }
};

}  // namespace model::term
