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

    std::shared_ptr<model::term::Term> Parse() {
        model::parsing::StringTerm string_root{input_};

        if (string_root.Type() == model::TermTypes::Error) {
            // TODO(senichenkov): implement behaviour if root term is of Error type
            return nullptr;
        }

        return string_root.Parse();
    }
};

}  // namespace parsing
