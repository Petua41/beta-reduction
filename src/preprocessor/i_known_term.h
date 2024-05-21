#pragma once

#include <string>

namespace preprocessor {

class IKnownTerm {
public:
    virtual bool IsPresent(std::string const& str) const = 0;
    virtual std::string Replace(std::string const& str) const = 0;
};

}  // namespace preprocessor
