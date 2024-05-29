#pragma once

#include <boost/xpressive/xpressive_static.hpp>
#include <memory>
#include <sstream>
#include <string>

#include "config/regular_expressions.h"
#include "postprocessor/macros/reverse_church_numeral.h"
#include "preprocessor/i_macro.h"
#include "preprocessor/macros/regex_with_generation_macro.h"

namespace preprocessor::terms {

class ChuchNumeral : public RegexWithGenerationMacro {
protected:
    virtual boost::xpressive::sregex const& Regex() const override {
        return config::regular_expressions::kChurchNumeralRegex;
    }

    virtual std::string GenerateTerm(std::string&& macro) const override {
        size_t number;
        try {
            number = std::stoul(macro);
        } catch (std::invalid_argument const& e) {
            // Not a number. Don't replace anything:
            return macro;
        }

        std::stringstream sstream;
        sstream << "(Lf.(Lx.";
        for (size_t i{0}; i < number; ++i) {
            sstream << "(f ";
        }
        sstream << "x";
        for (size_t i{0}; i < number; ++i) {
            sstream << ")";
        }
        sstream << "))";
        return sstream.str();
    }

    virtual std::shared_ptr<IMacro> Reversed() const noexcept override {
        return std::make_shared<postprocessor::terms::ReverseChuchNumeral>();
    }
};

}  // namespace preprocessor::terms
