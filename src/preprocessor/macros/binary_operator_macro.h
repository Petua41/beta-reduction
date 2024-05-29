#pragma once

#include <easylogging++.h>
#include <string>

#include "exceptions/macro_parsing_error.h"
#include "preprocessor/macros/fixed_search_term_capturing_macro.h"

namespace preprocessor::terms {

template <char const Macro[], char const TermPrefix[], char const TermInter[],
          char const TermSuffix[], bool OmitSpaces = true>
class BinaryOperatorMacro : public FixedSearchTermCapturingMacro<Macro> {
public:
    [[nodiscard]] virtual std::string Replace(std::string const& str) const noexcept override {
        LOG(INFO) << "Replacing binary operator macro...";
        LOG(INFO) << "\tInitial string is '" << str << "'";
        try {
            auto [prefix, real_macro, sescond_argument, remainder_suffix] =
                    this->template SplitByMacro<OmitSpaces>(str, Macro);

            auto [remainder_prefix, first_argument, real_macro2, suffix] =
                    this->template ReverseSplitByMacro<OmitSpaces>(prefix + real_macro, Macro);

            LOG(INFO) << "\tResult is: '" << remainder_prefix << "', '" << TermPrefix << "', '"
                      << first_argument << "', '" << TermInter << "', '" << sescond_argument
                      << "', '" << TermSuffix << "', '" << remainder_suffix << '\'';

            return remainder_prefix + TermPrefix + first_argument + TermInter + sescond_argument +
                   TermSuffix + remainder_suffix;
        } catch (exceptions::MacroParsingError const& e) {
            // It's not a problem if we cannot parse a macro. Just don't replace it
            return str;
        }
    }
};

}  // namespace preprocessor::terms
