#pragma once

#include <easylogging++.h>

#include "preprocessor/macros/fixed_search_term_capturing_macro.h"

namespace preprocessor::terms {

template <char const Macro[], char const TermPrefix[], char const TermSuffix[],
          bool OmitSpaces = true>
class UnaryOperatorMacro : public FixedSearchTermCapturingMacro<Macro> {
public:
    [[nodiscard]] virtual std::string Replace(std::string const& str) const noexcept override {
        LOG(INFO) << "Replacing unary operator macro...";
        LOG(INFO) << "\tMacro is '" << Macro << "', term prefix is '" << TermPrefix
                  << "', term suffix is '" << TermSuffix << '\'';
        LOG(INFO) << "\tInitial string is '" << str << '\'';

        auto [remainder_prefix, real_macro, argument, remainder_suffix] =
                this->template SplitByMacro<OmitSpaces>(str, Macro);
        return remainder_prefix + TermPrefix + argument + TermSuffix + remainder_suffix;
    }
};

}  // namespace preprocessor::terms
