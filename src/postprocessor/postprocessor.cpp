#include "postprocessor/postprocessor.h"

#include "postprocessor/all_macros.h"

namespace postprocessor {

void Postprocessor::ReplaceMacros() noexcept {
    bool found_macros = true;
    do {
        found_macros = false;
        for (auto const& macro : all_macros_.GetAllMacros()) {
            auto found = macro->IsPresent(input_);
            if (found) {
                found_macros = true;
                input_ = macro->Replace(input_);
            }
        }
    } while (found_macros);
}

}  // namespace postprocessor
