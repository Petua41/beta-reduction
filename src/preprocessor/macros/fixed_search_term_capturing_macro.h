#pragma once

#include <string>

#include "exceptions/macro_parsing_error.h"
#include "preprocessor/i_macro.h"

namespace preprocessor::terms {

template <char const macro[]>
class FixedSearchTermCapturingMacro : public IMacro {
protected:
    /// @return Captured term and index of it's last character
    virtual std::pair<std::string, size_t> CaptureTerm(std::string&& str) const {
        std::stringstream result;
        int counter{0};
        bool was_bracket{false};

        for (size_t i{0}; i < str.size(); ++i) {
            auto ch = str[i];

            if (ch == '(') {
                was_bracket = true;
                ++counter;
            } else if (ch == ')') {
                was_bracket = true;
                --counter;
            }

            if (counter < 0) {
                return std::make_pair(result.str(), i - 1);
            }

            result << ch;

            if (was_bracket && counter == 0) {
                return std::make_pair(result.str(), i);
            }
        }

        if (counter == 0) {
            // All str is term:
            return std::make_pair(str, str.size());
        } else {
            // Error in brackets
            if (counter > 0) {
                throw exceptions::MacroParsingError{
                        "cannot capture term: some bracket wasn't closed"};
            } else {
                throw exceptions::MacroParsingError{
                        "cannot capture term: some bracket wasn't opened"};
            }
        }
    }

    /// @return Captured term and index of it's first character
    virtual std::pair<std::string, size_t> ReverseCaptureTerm(std::string&& str) const {
        std::stringstream result;
        int counter{0};
        bool was_bracket{false};

        for (size_t i{0}; i < str.size(); ++i) {
            auto ch = str[str.size() - i - 1];

            if (ch == ')') {
                was_bracket = true;
                ++counter;
            } else if (ch == '(') {
                was_bracket = true;
                --counter;
            }

            if (counter < 0) {
                return std::make_pair(result.str(), str.size() - i);
            }

            result << ch;

            if (was_bracket && counter == 0) {
                return std::make_pair(result.str(), str.size() - i - 1);
            }
        }

        if (counter == 0) {
            // All str is term:
            return std::make_pair(str, 0);
        } else {
            // Error in brackets
            if (counter > 0) {
                throw exceptions::MacroParsingError{
                        "cannot capture term (reversed): some bracket wasn't opened"};
            } else {
                throw exceptions::MacroParsingError{
                        "cannot capture term (reversed): some bracket wasn't closed"};
            }
        }
    }

    template <bool omit_spaces = true>
    std::tuple<std::string, std::string, std::string, std::string> SplitByMacro(
            std::string const& str, char const macro_to_split[]) const {
        auto position = str.find(macro_to_split);
        // There's no such macro:
        if (position == std::string::npos) {
            throw exceptions::MacroParsingError{"cannot split by macro: no macro"};
        }

        std::string real_macro{macro_to_split};

        // Check space before macro:
        if (omit_spaces && position > 0 && str[position - 1] == ' ') {
            --position;
            real_macro = ' ' + real_macro;
        }

        auto remainder_prefix = str.substr(0, position);
        auto macro_and_suffix = str.substr(position);

        auto after_end = position + real_macro.size();
        if (after_end >= str.size()) {
            return std::make_tuple(remainder_prefix, real_macro, "", "");
        }

        // Check space after macro:
        if (omit_spaces && str[after_end] == ' ') {
            ++after_end;
            real_macro += ' ';
            if (after_end >= str.size()) {
                return std::make_tuple(remainder_prefix, real_macro, "", "");
            }
        }

        auto suffix = str.substr(after_end);
        auto [captured_term, length_of_captured_term] = CaptureTerm(std::move(suffix));

        size_t after_end_of_all{after_end + length_of_captured_term + 1};
        auto remainder_suffix = str.substr(after_end_of_all);

        return std::make_tuple(remainder_prefix, real_macro, captured_term, remainder_suffix);
    }

public:
    [[nodiscard]] virtual bool IsPresent(std::string const& str) const noexcept override {
        return str.find(macro) != std::string::npos;
    }
};

}  // namespace preprocessor::terms
