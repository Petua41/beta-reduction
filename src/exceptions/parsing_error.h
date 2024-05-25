#pragma once

#include <string>

#include "exceptions/fatal_error.h"

namespace exceptions {

class ParsingError : public FatalError {
private:
    std::string message_;

public:
    ParsingError(std::string&& problem, std::string&& term) {
        message_ = "Parsing error: " + problem + ":\n" + term + '\n';
    }

    ParsingError(ParsingError const&) = default;
    ParsingError(ParsingError&&) = delete;
    ParsingError& operator=(ParsingError const&) = default;
    ParsingError& operator=(ParsingError&&) = delete;
    ~ParsingError() = default;

    virtual char const* what() const noexcept override {
        return message_.c_str();
    }
};

}  // namespace exceptions
