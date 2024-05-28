#pragma once

#include <memory>
#include <string>

namespace preprocessor {

class IMacro {
public:
    [[nodiscard]] virtual bool IsPresent(std::string const& str) const noexcept = 0;
    [[nodiscard]] virtual std::string Replace(std::string const& str) const noexcept = 0;

    /// @brief Tries to create inverse macro
    /// @return std::shared_ptr to inverse macro if one exists, nullptr otherwise
    [[nodiscard]] virtual std::shared_ptr<IMacro> Reversed() const noexcept {
        return nullptr;
    }
};

}  // namespace preprocessor
