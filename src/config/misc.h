#pragma once

#include <string>

namespace config::misc {

unsigned max_operations = 100;

bool use_preprocessor = true;

constexpr auto kGlobalHelp = "Perform beta-reduction on lambda terms.\n"
    "Usage: Beta-reduction_cli [options] term\n"
    "Options:\n"
    "\t-a --ascii_mode \t\t Don't use characters for alpha, beta and lambda\n"
    "\t-m --max_operations N \t\t Set maximum operations before exit to N. 0 is infinity\n"
    "\t-r --raw \t\t Don't replace known named terms with corresponding real terms\n"
    "\t-h --help \t\t Show this help and exit";

}  // namespace config::misc
