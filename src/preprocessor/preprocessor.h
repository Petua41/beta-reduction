#pragma once

#include <string>

#include "exceptions/invalid_brackets_error.h"
#include "model/enums/brackets_problems.h"

namespace preprocessor {

class Preprocessor {
private:
    std::string input_;

    // Actions to perform:
    bool const check_brackets_;
    bool const replace_macros_;

    /// @brief Checks brackets and adds missing
    /// @throw InvalidBracketsError -- if there's some problem with brackets, that cannot be solved
    void CheckBrackets() {
        CheckBracketsMatch();
        CheckOutermostBrackets();
        CheckAbstractionRhs();
    }

    /// @brief Checks that brackets match (every opening bracket has corresponding closing bracket
    /// and vice versa)
    /// @throw InvalidBracketsError -- if brackets don't match
    void CheckBracketsMatch();

    /// @brief Replaces marcos with corresponding terms
    void ReplaceMacros() noexcept;

    /// @brief Checks that the whole term is enclosed with brackets
    /// (if it's not a single Variable or macro)
    void CheckOutermostBrackets() noexcept;

    /// @brief Checks that every Abstraction's rhs is enclosed with brackets
    /// (if it's not a single Variable or macro).
    /// Abstraction is greedy, i. e. it "takes all that it can reach"
    void CheckAbstractionRhs() noexcept;

public:
    Preprocessor(std::string const& input, bool check_brackets = true, bool replace_macros = true)
        : input_(input), check_brackets_(check_brackets), replace_macros_(replace_macros) {}

    /// @throw InvalidBracketsError -- if there's some problem with brackets, that cannot be solved
    [[nodiscard]] std::string Preprocess() {
        if (check_brackets_) {
            CheckBrackets();
        }
        if (replace_macros_) {
            ReplaceMacros();
        }

        return input_;
    }
};

}  // namespace preprocessor
