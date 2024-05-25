#pragma once

#include <easylogging++.h>

#include "preprocessor/macros/fixed_search_term_capturing_macro.h"

namespace preprocessor::terms {

template <char const macro[], char const term_prefix[], char const term_suffix[],
          bool omit_spaces = true>
class UnaryOperatorMacro : public FixedSearchTermCapturingMacro<macro> {
public:
    [[nodiscard]] virtual std::string Replace(std::string const& str) const noexcept override {
        LOG(INFO) << "Replacing unary operator macro...";
        LOG(INFO) << "\tMacro is '" << macro << "', term prefix is '" << term_prefix
                  << "', term suffix is '" << term_suffix << '\'';
        LOG(INFO) << "\tInitial string is '" << str << '\'';

        auto [remainder_prefix, real_macro, argument, remainder_suffix] =
                this->template SplitByMacro<omit_spaces>(str, macro);
        return remainder_prefix + term_prefix + argument + term_suffix + remainder_suffix;
    }
};

}  // namespace preprocessor::terms
