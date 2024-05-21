#include "preprocessor/preprocessor.h"

#include "preprocessor/all_known_terms.h"

namespace preprocessor {

void Preprocessor::ReplaceMacros() {
    bool found_macros = true;
    do {
        found_macros = false;
        for (auto const& macro : all_known_terms) {
            auto found = macro->IsPresent(input_);
            if (found) {
                found_macros = true;
                input_ = macro->Replace(input_);
            }
        }
    } while (found_macros);
}

}  // namespace preprocessor
