#pragma once

#include <algorithm>
#include <sstream>
#include <string>

#include "exceptions/macro_parsing_error.h"
#include "preprocessor/i_macro.h"

namespace preprocessor::terms {

template <char const Macro[]>
class FixedSearchTermCapturingMacro : public IMacro {
protected:
    virtual std::string CaptureTerm(std::string&& str) const {
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
                return result.str();
            }

            result << ch;

            if (was_bracket && counter == 0) {
                return result.str();
            }
        }

        if (counter == 0) {
            // The whole str is term:
            return str;
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

    virtual std::string ReverseCaptureTerm(std::string&& str) const {
        std::stringstream result;
        int counter{0};
        bool was_bracket{false};

        auto reverse = [](std::stringstream const& sstream) {
            auto str = sstream.str();
            std::reverse(str.begin(), str.end());
            return str;
        };

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
                return reverse(result);
            }

            result << ch;

            if (was_bracket && counter == 0) {
                return reverse(result);
            }
        }

        if (counter == 0) {
            // All str is term:
            return str;
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

    /// @return [remainder_prefix, real_macro, argument, remainder_suffix]
    template <bool OmitSpaces = true>
    std::tuple<std::string, std::string, std::string, std::string> SplitByMacro(
            std::string const& str, char const macro_to_split[]) const {
        auto position = str.find(macro_to_split);
        // There's no such macro:
        if (position == std::string::npos) {
            throw exceptions::MacroParsingError{"cannot split by macro: no macro"};
        }

        std::string real_macro{macro_to_split};

        // Check space before macro:
        if (OmitSpaces && position > 0 && str[position - 1] == ' ') {
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
        if (OmitSpaces && str[after_end] == ' ') {
            ++after_end;
            real_macro += ' ';
            if (after_end >= str.size()) {
                return std::make_tuple(remainder_prefix, real_macro, "", "");
            }
        }

        auto suffix = str.substr(after_end);
        auto captured_term = CaptureTerm(std::move(suffix));

        size_t after_end_of_all{after_end + captured_term.size()};
        std::string remainder_suffix{""};
        if (after_end_of_all < str.size()) {
            remainder_suffix = str.substr(after_end_of_all);
        }

        return std::make_tuple(remainder_prefix, real_macro, captured_term, remainder_suffix);
    }

    /// @return [remainder_prefix, argument, real_macro, remainder_suffix]
    template <bool OmitSpaces = true>
    std::tuple<std::string, std::string, std::string, std::string> ReverseSplitByMacro(
            std::string const& str, char const macro_to_split[]) const {
        auto position = str.find(macro_to_split);
        // There's no such macro:
        if (position == std::string::npos) {
            throw exceptions::MacroParsingError{"cannot split by macro: no macro"};
        }

        std::string real_macro{macro_to_split};

        // Check space before macro:
        if (OmitSpaces && position > 0 && str[position - 1] == ' ') {
            --position;
            real_macro = ' ' + real_macro;
        }

        auto prefix = str.substr(0, position);
        auto macro_and_suffix = str.substr(position);

        auto after_end = position + real_macro.size();

        // Check space after macro:
        if (after_end < str.size() && OmitSpaces && str[after_end] == ' ') {
            ++after_end;
            real_macro += ' ';
        }

        auto remainder_suffix = str.substr(after_end);

        auto prefix_len = prefix.size();
        auto captured_term = ReverseCaptureTerm(std::move(prefix));
        auto remainder_prefix = str.substr(0, prefix_len - captured_term.size());

        return std::make_tuple(remainder_prefix, captured_term, real_macro, remainder_suffix);
    }

public:
    [[nodiscard]] virtual bool IsPresent(std::string const& str) const noexcept override {
        return str.find(Macro) != std::string::npos;
    }
};

}  // namespace preprocessor::terms
