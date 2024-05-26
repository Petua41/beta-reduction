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

    /// @brief Checks and adds missing brackets
    /// @throw InvalidBracketsError -- if there's some problem with brackets, that cannot be solved
    void CheckBrackets() {
        // TODO(senichenkov): implement Preprocessor::CheckBrackets
        // Check if brackets match:
        short counter{0};
        size_t first_open_bracket{0};
        for (size_t i{0}; i < input_.size(); ++i) {
            auto ch = input_[i];

            if (ch == '(') {
                if (first_open_bracket == 0) {
                    first_open_bracket = i;
                }
                ++counter;
            } else if (ch == ')') {
                --counter;
            }

            if (counter == 0) {
                // Reset this counter:
                first_open_bracket = 0;
            } else if (counter < 0) {
                throw exceptions::InvalidBracketsError{std::move(input_), i,
                                                       model::BracketsProblems::WasntOpened};
            }
        }

        if (counter > 0) {
            throw exceptions::InvalidBracketsError{std::move(input_), first_open_bracket,
                                                   model::BracketsProblems::WasntClosed};
        } else if (counter < 0) {
            throw exceptions::InvalidBracketsError{std::move(input_), input_.size() - 1,
                                                   model::BracketsProblems::WasntOpened};
        }

        // FIXME(senichenkov): outermost brackets check should be smarter
        // Check outermost brackets:
        if (!input_.starts_with('(') || !input_.ends_with(')')) {
            // Input contains Abstractions and Applications:
            if (input_.find(' ') != std::string::npos || input_.find('.') != std::string::npos) {
                input_ = '(' + input_ + ')';
            }
        }
    }

    void ReplaceMacros() noexcept;

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
