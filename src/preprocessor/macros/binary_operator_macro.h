#pragma once

#include <easylogging++.h>
#include <string>

#include "exceptions/macro_parsing_error.h"
#include "preprocessor/macros/fixed_search_term_capturing_macro.h"

namespace preprocessor::terms {

template <char const macro[], char const term_prefix[], char const term_inter[],
          char const term_suffix[], bool omit_spaces = true>
class BinaryOperatorMacro : public FixedSearchTermCapturingMacro<macro> {
public:
    [[nodiscard]] virtual std::string Replace(std::string const& str) const noexcept override {
        LOG(INFO) << "Replacing binary operator macro...";
        LOG(INFO) << "\tInitial string is '" << str << "'";
        try {
            auto [prefix, real_macro, sescond_argument, remainder_suffix] =
                    this->template SplitByMacro<omit_spaces>(str, macro);

            auto [remainder_prefix, first_argument, real_macro2, suffix] =
                    this->template ReverseSplitByMacro<omit_spaces>(prefix + real_macro, macro);

            LOG(INFO) << "\tResult is: '" << remainder_prefix << "', '" << term_prefix << "', '"
                      << first_argument << "', '" << term_inter << "', '" << sescond_argument
                      << "', '" << term_suffix << "', '" << remainder_suffix << '\'';

            return remainder_prefix + term_prefix + first_argument + term_inter + sescond_argument +
                   term_suffix + remainder_suffix;
        } catch (exceptions::MacroParsingError const& e) {
            // It's not a problem if we cannot parse a macro. Just don't replace it
            return str;
        }
    }
};

}  // namespace preprocessor::terms
