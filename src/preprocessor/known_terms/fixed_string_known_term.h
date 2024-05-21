#pragma once

#include <string>

#include "preprocessor/i_known_term.h"

namespace preprocessor::terms {

template <char const* Name, char const* Term>
class FixedStringKnownTerm : public IKnownTerm {
private:
    std::string name_;
    std::string term_;

public:
    FixedStringKnownTerm() : name_(Name), term_(Term) {}

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
