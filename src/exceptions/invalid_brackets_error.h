#pragma once

#include <sstream>
#include <string>

#include "exceptions/fatal_error.h"
#include "model/enums/brackets_problems.h"

namespace exceptions {

class InvalidBracketsError : public FatalError {
private:
    std::string message_;

public:
    InvalidBracketsError(std::string&& term, size_t problem_bracket_index,
                         model::BracketsProblems problem = model::BracketsProblems::Other) {
        std::stringstream sstream;
        sstream << "Invalid brackets:\n" << std::move(term) << '\n';

        for (size_t i{0}; i < problem_bracket_index; ++i) {
            sstream << ' ';
        }
        sstream << '^';

        switch (problem) {
            case model::BracketsProblems::WasntClosed:
                sstream << " This bracket wasn't closed";
                break;
            case model::BracketsProblems::WasntOpened:
                sstream << " This bracket wasn't opened";
                break;
            case model::BracketsProblems::Other:
                break;
        }
        sstream << '\n';

        message_ = sstream.str();
    }

    InvalidBracketsError(InvalidBracketsError const&) = default;
    InvalidBracketsError(InvalidBracketsError&&) = delete;
    InvalidBracketsError& operator=(InvalidBracketsError const&) = default;
    InvalidBracketsError& operator=(InvalidBracketsError&&) = delete;
    ~InvalidBracketsError() = default;

    virtual char const* what() const noexcept override {
        return message_.c_str();
    }
};

}  // namespace exceptions
