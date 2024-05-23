#pragma once

#include <string>

namespace preprocessor {

class Preprocessor {
private:
    std::string input_;

    // Actions to perform:
    bool const check_brackets_;
    bool const replace_macros_;

    /// @brief Checks and adds missing brackets
    /// @return true if brackets are (now) correct, false otherwise
    bool CheckBrackets() {
        // TODO(senichenkov): implement Preprocessor::CheckBrackets

        // FIXME(senichenkov): outermost brackets check should be smarter
        // Check outermost brackets:
        if (!input_.starts_with('(') || !input_.ends_with(')')) {
            // Input contains Abstractions and Applications:
            if (input_.find(' ') != std::string::npos || input_.find('.') != std::string::npos) {
                input_ = '(' + input_ + ')';
            }
        }

        // Check if brackets match:
        short counter = 0;
        for (auto ch : input_) {
            if (ch == '(') {
                ++counter;
            } else if (ch == ')') {
                --counter;
            }

            if (counter < 0) {
                return false;
            }
        }
        if (counter != 0) {
            return false;
        }

        return true;
    }

    void ReplaceMacros();

public:
    Preprocessor(std::string const& input, bool check_brackets = true, bool replace_macros = true)
        : input_(input), check_brackets_(check_brackets), replace_macros_(replace_macros) {}

    std::pair<bool, std::string> Preprocess() {
        bool brackets_are_correct = true;

        if (check_brackets_) {
            brackets_are_correct = CheckBrackets();
        }
        if (replace_macros_) {
            ReplaceMacros();
        }

        return std::make_pair(brackets_are_correct, input_);
    }
};

}  // namespace preprocessor
