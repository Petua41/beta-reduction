#include "preprocessor/preprocessor.h"

#include <memory>
#include <string>

#include "exceptions/invalid_brackets_error.h"

namespace {

using namespace exceptions;

void CheckBracketsMatch(std::string input) {
    short counter{0};
    size_t first_open_bracket{0};
    for (size_t i{0}; i < input.size(); ++i) {
        auto ch = input[i];

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
            throw InvalidBracketsError{std::move(input), i, model::BracketsProblems::WasntOpened};
        }
    }

    if (counter > 0) {
        throw InvalidBracketsError{std::move(input), first_open_bracket,
                                   model::BracketsProblems::WasntClosed};
    } else if (counter < 0) {
        throw InvalidBracketsError{std::move(input), input.size() - 1,
                                   model::BracketsProblems::WasntOpened};
    }
}

[[nodiscard]] std::string CheckOutermostBrackets(std::string input) noexcept {
    if (!input.starts_with('(') || !input.ends_with(')')) {
        // Input contains Abstractions or Applications:
        if (input.find_first_of(" .") != std::string::npos) {
            input = '(' + input + ')';
        }
    }
    return input;
}

[[nodiscard]] std::string CheckAbstractionRhs(std::string input) noexcept {
    size_t last_dot_position = 0;
    while (true) {
        auto current_dot_position = input.find('.', last_dot_position + 1);
        if (current_dot_position == std::string::npos) {
            // No more dots
            return input;
        }
        last_dot_position = current_dot_position;

        if (current_dot_position == input.size() - 1) [[unlikely]] {
            // Dot is the last character (strange case, but Parser will handle it)
            return input;
        }

        if (input[current_dot_position + 1] != '(') {
            // No bracket after dot -- our case

            auto after_dot = input.substr(current_dot_position + 1);

            // Capture all that we can reach:
            int counter{0};
            size_t characters_in_rhs{0};
            for (auto const& ch : after_dot) {
                if (ch == '(') {
                    ++counter;
                } else if (ch == ')') {
                    --counter;
                }

                if (counter < 0) {
                    // We can't reach further
                    break;
                }
                ++characters_in_rhs;
            }

            auto pre_rhs = input.substr(current_dot_position + 1, characters_in_rhs);

            if (pre_rhs.find_first_of(" .") != std::string::npos) {
                // Rhs is Abstraction or Application -- must be enclosed
                auto prefix = input.substr(0, current_dot_position + 1);
                auto suffix = input.substr(current_dot_position + 1 + characters_in_rhs);

                input = std::move(prefix) + '(' + std::move(pre_rhs) + ')' + std::move(suffix);
            }
        }
    }
    return input;
}

}  // namespace

namespace preprocessor {

[[nodiscard]] std::string CheckBrackets(std::string input) {
    CheckBracketsMatch(input);
    input = CheckOutermostBrackets(input);
    input = CheckAbstractionRhs(input);
    return input;
}

[[nodiscard]] std::string ReplaceMacros(std::string input) noexcept {
    bool found_macros = true;
    while (found_macros) {
        found_macros = false;
        for (auto const& macro : kAllMacros) {
            auto found = macro->IsPresent(input);
            if (found) {
                found_macros = true;
                input = macro->Replace(input);
            }
        }
    }
    return input;
}

}  // namespace preprocessor
