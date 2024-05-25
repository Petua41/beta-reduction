#pragma once

#include <string>

#include "exceptions/fatal_error.h"

namespace exceptions {

class MacroParsingError : public FatalError {
private:
    std::string message_;

public:
    MacroParsingError(std::string&& problem) {
        message_ = "Macro parsing error: " + std::move(problem);
    }

    MacroParsingError(MacroParsingError const&) = default;
    MacroParsingError(MacroParsingError&&) = delete;
    MacroParsingError& operator=(MacroParsingError const&) = default;
    MacroParsingError& operator=(MacroParsingError&&) = delete;
    ~MacroParsingError() = default;

    virtual char const* what() const noexcept override {
        return message_.c_str();
    }
};

}  // namespace exceptions
