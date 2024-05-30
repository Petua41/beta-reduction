#pragma once

#include <boost/algorithm/string/predicate.hpp>
#include <boost/any.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <vector>

#include "model/enums/reduction_strategies.h"

namespace cli {

struct StrategyOption {
public:
    model::ReductionStrategies strategy_;

    StrategyOption(model::ReductionStrategies strategy = model::ReductionStrategies::NO)
        : strategy_(strategy) {}
};

void validate(boost::any& v, std::vector<std::string> const& values,
              // Last two options are used to workaround something:
              StrategyOption* target_type __attribute__((__unused__)), int) {
    using namespace boost::program_options;

    // Make sure no previous assignment to v was made.
    validators::check_first_occurrence(v);
    // Extract the first string from 'values'. If there is more than
    // one string, it's an error, and exception will be thrown.
    auto const& s = validators::get_single_string(values);

    // Try to convert (using case-insensitive comparison):
    if (boost::iequals(s, "NO")) {
        v = boost::any(StrategyOption(model::ReductionStrategies::NO));
    } else if (boost::iequals(s, "APPL")) {
        v = boost::any(StrategyOption(model::ReductionStrategies::APPL));
    } else if (boost::iequals(s, "CBN")) {
        v = boost::any(StrategyOption(model::ReductionStrategies::CBN));
    } else if (boost::iequals(s, "CBV")) {
        v = boost::any(StrategyOption(model::ReductionStrategies::CBV));
    } else {
        throw validation_error(validation_error::invalid_option_value);
    }
}

}  // namespace cli
