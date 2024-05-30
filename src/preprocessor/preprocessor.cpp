#include "preprocessor/preprocessor.h"

#include "preprocessor/macros/church_numeral.h"
#include "preprocessor/macros/fixed_strings.h"

namespace preprocessor {

using namespace preprocessor::terms;

std::vector<std::shared_ptr<IMacro>> const Preprocessor::all_macros_{
        std::make_shared<FixpointCombinator>(),
        std::make_shared<True>(),
        std::make_shared<False>(),
        std::make_shared<ChuchNumeral>(),
        std::make_shared<Not>(),
        std::make_shared<IsZero>(),
        std::make_shared<Plus>(),
        std::make_shared<Mult>(),
        std::make_shared<And>(),
        std::make_shared<Or>()};

void Preprocessor::ReplaceMacros() noexcept {
    bool found_macros = true;
    do {
        found_macros = false;
        for (auto const& macro : all_macros_) {
            auto found = macro->IsPresent(input_);
            if (found) {
                found_macros = true;
                input_ = macro->Replace(input_);
            }
        }
    } while (found_macros);
}

void Preprocessor::CheckBracketsMatch() {
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
}

void Preprocessor::CheckOutermostBrackets() noexcept {
    if (!input_.starts_with('(') || !input_.ends_with(')')) {
        // Input contains Abstractions or Applications:
        if (input_.find_first_of(" .") != std::string::npos) {
            input_ = '(' + input_ + ')';
        }
    } else {
        // Remove extra outermost brackets:
        while (input_.starts_with("((") && input_.ends_with("))")) {
            input_ = input_.substr(1, input_.size() - 2);
        }
    }
}

void Preprocessor::CheckAbstractionRhs() noexcept {
    size_t last_dot_position = 0;
    while (true) {
        auto current_dot_position = input_.find('.', last_dot_position + 1);
        if (current_dot_position == std::string::npos) {
            // No more dots
            return;
        }
        last_dot_position = current_dot_position;

        if (current_dot_position == input_.size() - 1) {
            // Dot is the last character (strange case, but Parser will handle it)
            return;
        }

        if (input_[current_dot_position + 1] != '(') {
            // No bracket after dot -- our case

            auto after_dot = input_.substr(current_dot_position + 1);

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

            auto pre_rhs = input_.substr(current_dot_position + 1, characters_in_rhs);

            if (pre_rhs.find_first_of(" .") != std::string::npos) {
                // Rhs is Abstraction or Application -- must be enclosed
                auto prefix = input_.substr(0, current_dot_position + 1);
                auto suffix = input_.substr(current_dot_position + 1 + characters_in_rhs);

                input_ = std::move(prefix) + '(' + std::move(pre_rhs) + ')' + std::move(suffix);
            }
        }
    }
}

}  // namespace preprocessor
