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

    [[nodiscard]] bool IsPresent(std::string const& str) const noexcept override {
        return str.find(name_) != std::string::npos;
    }

    [[nodiscard]] std::string Replace(std::string const& str) const noexcept override {
        auto pos = str.find(name_);
        auto& str_copy = const_cast<std::string&>(str);
        return str_copy.replace(pos, name_.size(), term_);
    }
};

}  // namespace preprocessor::terms
