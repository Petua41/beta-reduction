#pragma once

#include <boost/xpressive/xpressive_static.hpp>
#include <easylogging++.h>
#include <sstream>
#include <string>

#include "config/regular_expressions.h"
#include "preprocessor/macros/regex_with_generation_macro.h"

namespace postprocessor::terms {

class ReverseChuchNumeral : public preprocessor::terms::RegexWithGenerationMacro {
protected:
    virtual boost::xpressive::sregex const& Regex() const {
        return config::regular_expressions::kReverseChurchNumeralRegex.ReverseChurchNumeral();
    }

    virtual std::string GenerateTerm(std::string&& macro) const {
        LOG(INFO) << "Generating Church numeral for '" << macro << '\'';
        // Here, macro is actually a term
        // Regex is well-formed, so we just need to count closing brackets:
        // Two last brackets enclose abstractions:
        int counter = -2;
        for (auto ptr{macro.rbegin()}; ptr != macro.rend(); ++ptr) {
            if (*ptr == ')') {
                ++counter;
            } else {
                break;
            }
        }
        return std::to_string(counter);
    }
};

}  // namespace postprocessor::terms
