#pragma once

#include <boost/xpressive/xpressive_static.hpp>
#include <string>

#include "preprocessor/i_macro.h"

namespace preprocessor::terms {

class RegexWithGenerationMacro : public IMacro {
protected:
    virtual boost::xpressive::sregex const& Regex() const = 0;
    virtual std::string GenerateTerm(std::string&& macro) const = 0;

public:
    [[nodiscard]] virtual bool IsPresent(std::string const& str) const noexcept override {
        return boost::xpressive::regex_search(str, Regex());
    }

    [[nodiscard]] virtual std::string Replace(std::string const& str) const noexcept override {
        boost::xpressive::smatch match_res;
        if (boost::xpressive::regex_search(str, match_res, Regex())) {
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
