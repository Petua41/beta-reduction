#pragma once

#include <memory>
#include <string>

#include "preprocessor/i_macro.h"

namespace preprocessor::terms {

template <char const* Macro, char const* Term, bool has_reversed = true>
class FixedStringMacro : public IMacro {
private:
    std::string name_;
    std::string term_;

public:
    FixedStringMacro() : name_(Macro), term_(Term) {}

    [[nodiscard]] bool IsPresent(std::string const& str) const noexcept override {
        return str.find(name_) != std::string::npos;
    }

    [[nodiscard]] std::string Replace(std::string const& str) const noexcept override {
        auto pos = str.find(name_);
        auto& str_copy = const_cast<std::string&>(str);
        return str_copy.replace(pos, name_.size(), term_);
    }

    [[nodiscard]] std::shared_ptr<IMacro> Reversed() const noexcept {
        if (has_reversed) {
            return std::make_shared<FixedStringMacro<Term, Macro>>();
        }

        return nullptr;
    }
};

}  // namespace preprocessor::terms
