#pragma once

#include <memory>
#include <string>

#include "model/term.h"

namespace model::term {

class Variable : public Term {
private:
    std::string name_;

public:
    Variable(std::string const& name) : name_(name) {}

    void AlphaConversionStep(std::string const& new_variable_name) override {
        name_ = new_variable_name;
    }

    std::shared_ptr<Term> ReplaceVariableWithTerm(std::string const& variable_name,
                                                  std::shared_ptr<Term> const term) const override {
        if (variable_name == name_) {
            return term;
        } else {
            return std::make_shared<Variable>(*this);
        }
    }

    void ReplaceLhs(std::shared_ptr<Term>&& new_lhs) override {
        LOG(WARNING) << "Trying to replace Variable's Lhs with " << new_lhs->ToString();
    }

    void ReplaceRhs(std::shared_ptr<Term>&& new_rhs) override {
        LOG(WARNING) << "Trying to replace Variable's Rhs with " << new_rhs->ToString();
    }

    std::string ToString() const override {
        return name_;
    }
};

}  // namespace model::term
