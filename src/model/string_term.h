#pragma once

#include <memory>
#include <string>

#include "model/enums/term_types.h"
#include "model/terms.h"

namespace model::parsing {

class StringTerm {
private:
    std::string data_;

    // Cached data:
    TermTypes type_{TermTypes::Unknown};

    std::unique_ptr<StringTerm> string_lhs_{nullptr};
    std::unique_ptr<StringTerm> string_rhs_{nullptr};

    void ResolveType();

    void Split();

public:
    StringTerm(std::string&& data) : data_(std::move(data)) {}

    StringTerm(std::string const& data) : data_(data) {}

    TermTypes const& Type() {
        if (type_ == TermTypes::Unknown) {
            ResolveType();
        }

        return type_;
    }

    [[nodiscard]] std::shared_ptr<term::Term> Parse();
};

}  // namespace model::parsing
