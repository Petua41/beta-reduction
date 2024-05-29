#pragma once

#include <boost/xpressive/xpressive_static.hpp>
#include <memory>
#include <sstream>
#include <string>

#include "config/regular_expressions.h"
#include "preprocessor/i_macro.h"

namespace preprocessor::terms {

class ChuchNumeral : public IMacro {
private:
    boost::xpressive::sregex const& regex_ = config::regular_expressions::kChurchNumeralRegex;

    std::string GenerateTerm(std::string&& macro) const {
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

public:
    [[nodiscard]] virtual bool IsPresent(std::string const& str) const noexcept override {
        return boost::xpressive::regex_search(str, regex_);
    }

    [[nodiscard]] virtual std::string Replace(std::string const& str) const noexcept override {
        boost::xpressive::smatch match_res;
        if (boost::xpressive::regex_search(str, match_res, regex_)) {
            if (match_res.size() >= 1) {
                // Replace only first match:
                auto matched_string = match_res.str(0);
                auto start = match_res.position(0);
                auto length = match_res.length(0);

                auto term = GenerateTerm(std::move(matched_string));

                std::string result{str};
                result.replace(start, length, term);
                return result;
            }
        }

        // No match:
        return str;
    }
};

}  // namespace preprocessor::terms
