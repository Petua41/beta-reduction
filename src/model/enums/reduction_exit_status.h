#pragma once

#include <ostream>

namespace model {

enum class ReductionExitStatus { NormalForm, Loop, TooManyOperations };

// This operator is used in ReductionResult:
inline std::ostream& operator<<(std::ostream& os, ReductionExitStatus const& res) {
    switch (res) {
        case ReductionExitStatus::NormalForm:
            os << "Normal form";
            break;
        case ReductionExitStatus::Loop:
            os << "Loop";
            break;
        case ReductionExitStatus::TooManyOperations:
            os << "Too many operations";
            break;
    }
    return os;
}

}  // namespace model
