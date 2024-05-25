#pragma once

#include <string>

namespace preprocessor {

class IMacro {
public:
    [[nodiscard]] virtual bool IsPresent(std::string const& str) const noexcept = 0;
    [[nodiscard]] virtual std::string Replace(std::string const& str) const noexcept = 0;
};

}  // namespace preprocessor
