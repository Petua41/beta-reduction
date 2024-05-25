#pragma once

#include <memory>
#include <string>

#include "model/enums/term_types.h"
#include "model/string_term.h"
#include "model/term.h"

namespace parsing {

class Parser {
private:
    std::string input_;

public:
    Parser(std::string const& input) : input_(input) {}

    /// @throw ParsingError -- if term contains syntax or semantic errors
    [[nodiscard]] std::shared_ptr<model::term::Term> Parse() {
        model::parsing::StringTerm string_root{input_};
        return string_root.Parse();
    }
};

}  // namespace parsing
