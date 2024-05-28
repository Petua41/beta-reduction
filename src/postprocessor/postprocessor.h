#pragma once

#include <string>

#include "postprocessor/all_macros.h"

namespace postprocessor {

class Postprocessor {
private:
    // Parameters:
    std::string input_;

    // Actions to perform:
    bool replace_macros_;

    AllMacros all_macros_{};

    void ReplaceMacros() noexcept;

public:
    Postprocessor(std::string const& input, bool replace_macros = true)
        : input_(input), replace_macros_(replace_macros) {}

    [[nodiscard]] std::string Process() noexcept {
        if (replace_macros_) {
            ReplaceMacros();
        }

        return input_;
    }
};

}  // namespace postprocessor
