#pragma once

#include <memory>
#include <string>

namespace model::term {

class Term {
public:
    virtual void AlphaConversionStep(std::string const& new_variable_name) = 0;
    virtual std::shared_ptr<Term> ReplaceVariableWithTerm(
            std::string const& variable_name, std::shared_ptr<Term> const term) const = 0;
    virtual void ReplaceLhs(std::shared_ptr<Term>&& new_lhs) = 0;
    virtual void ReplaceRhs(std::shared_ptr<Term>&& new_rhs) = 0;

    virtual std::string ToString() const {
        return "<unknown term>";
    }

    virtual bool operator==(Term const& other) const {
        return ToString() == other.ToString();
    }

    virtual bool operator!=(Term const& other) const {
        return !(*this == other);
    }
};

}  // namespace model::term
