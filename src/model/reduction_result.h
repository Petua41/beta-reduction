#pragma once

#include <ostream>
#include <string>

#include "model/enums/reduction_exit_status.h"

namespace model::strategy {

struct ReductionResult {
public:
    std::string const result_string;
    ReductionExitStatus exit_status;

    ReductionResult(std::string const& result_string, ReductionExitStatus exit_status)
        : result_string(result_string), exit_status(exit_status) {}

    bool operator==(ReductionResult const& other) const {
        return result_string == other.result_string && exit_status == other.exit_status;
    }

    bool operator!=(ReductionResult const& other) const {
        return !(*this == other);
    }
};

// GTest uses this operator to report assertion failures:
inline std::ostream& operator<<(std::ostream& os, ReductionResult const& rr) {
    os << '{' << rr.result_string << ", " << rr.exit_status << '}';
    return os;
}

}  // namespace model::strategy
