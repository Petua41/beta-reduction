#pragma once

#include <easylogging++.h>
#include <memory>
#include <string>
#include <unordered_set>

namespace model::term {

class Term {
public:
    virtual std::shared_ptr<Term> ReplaceVariableWithTerm(
            std::string const& variable_name, std::shared_ptr<Term> const term) const = 0;

    /// @brief Accesses left-hand side (read-only) if one exists, else nullptr
    virtual std::shared_ptr<Term> Lhs() const {
        return nullptr;
    }

    /// @brief Accesses right-hand side (read-only) if one exists, else nullptr
    virtual std::shared_ptr<Term> Rhs() const {
        return nullptr;
    }

    /// @brief Replace left-hand side with new_lhs if this operation is supported
    virtual void Lhs([[maybe_unused]] std::shared_ptr<Term>&& new_lhs) {
        LOG(WARNING) << "Trying to replace lhs of '" << ToString() << "'";
    }

    /// @brief Replace right-hand side with new_lhs if this operation is supported
    virtual void Rhs([[maybe_unused]] std::shared_ptr<Term>&& new_rhs) {
        LOG(WARNING) << "Trying to replace rhs of '" << ToString() << "'";
    }

    virtual std::unordered_set<std::string> GetFreeVariables() const noexcept = 0;
    virtual void ReplaceBoundVariables(std::unordered_set<std::string> const& for_replacement) = 0;
    virtual void ReplaceFreeVariable(std::string const& for_replacement,
                                     std::string const& new_name) = 0;
    virtual std::string ToString() const = 0;

    virtual bool operator==(Term const& other) const {
        return ToString() == other.ToString();
    }

    virtual bool operator!=(Term const& other) const {
        return !(*this == other);
    }
};

}  // namespace model::term
