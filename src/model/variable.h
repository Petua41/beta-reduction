#pragma once

#include <memory>
#include <string>
#include <unordered_set>

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

    std::unordered_set<std::string> GetFreeVariables() const noexcept {
        // A single Variable is always free
        return std::unordered_set<std::string>{name_};
    }

    void ReplaceBoundVariables(std::unordered_set<std::string> const& for_replacement) override {
        // A single Variable is never bound
    }

    void ReplaceFreeVariable(std::string const& for_replacement,
                             std::string const& new_name) override {
        // A single Variable is always free
        if (name_ == for_replacement) {
            name_ = new_name;
        }
    }

    std::string ToString() const override {
        return name_;
    }
};

}  // namespace model::term
