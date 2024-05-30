#pragma once

#include <memory>
#include <string>

#include "model/term.h"

namespace parsing {

class StringTerm {
private:
    std::string data_;

public:
    StringTerm(std::string&& data) : data_(std::move(data)) {}

    StringTerm(std::string const& data) : data_(data) {}

    /// @throw ParsingError -- if term contains syntax or semantic errors
    [[nodiscard]] std::shared_ptr<model::term::Term> Parse();
};

}  // namespace parsing
