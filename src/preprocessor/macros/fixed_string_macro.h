#pragma once

#include <string>

#include "preprocessor/i_macro.h"

namespace preprocessor::terms {

template <char const* Name, char const* Term>
class FixedStringMacro : public IMacro {
private:
    std::string name_;
    std::string term_;

public:
    FixedStringMacro() : name_(Name), term_(Term) {}

    bool IsPresent(std::string const& str) const override {
        return str.find(name_) != std::string::npos;
    }

    std::string Replace(std::string const& str) const override {
        auto pos = str.find(name_);
        auto& str_copy = const_cast<std::string&>(str);
        return str_copy.replace(pos, name_.size(), term_);
    }
};

}  // namespace preprocessor::terms
