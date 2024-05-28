#pragma once

#include <memory>
#include <vector>

#include "preprocessor/all_macros.h"
#include "preprocessor/i_macro.h"

namespace postprocessor {

class AllMacros {
private:
    std::unique_ptr<std::vector<std::shared_ptr<preprocessor::IMacro>>> all_macros_{nullptr};

    void FillAllMacros() noexcept {
        all_macros_ = std::make_unique<std::vector<std::shared_ptr<preprocessor::IMacro>>>();
        for (auto macro : preprocessor::all_macros) {
            auto rev = macro->Reversed();
            if (rev != nullptr) {
                all_macros_->push_back(std::move(rev));
            }
        }
    }

public:
    std::vector<std::shared_ptr<preprocessor::IMacro>> GetAllMacros() {
        if (all_macros_ == nullptr) {
            FillAllMacros();
        }

        return *all_macros_;
    }
};

}  // namespace postprocessor
