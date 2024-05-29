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

    std::shared_ptr<Term> ReplaceVariableWithTerm(std::string const& variable_name,
                                                  std::shared_ptr<Term> const term) const override {
        if (variable_name == name_) {
            return term;
        } else {
            return std::make_shared<Variable>(*this);
        }
    }

    std::string ToString() const override {
        return name_;
    }
};

}  // namespace model::term
