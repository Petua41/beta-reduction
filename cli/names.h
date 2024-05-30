#pragma once

namespace cli {

// Option names:
constexpr auto kNoBrackets = "no-brackets,b";
constexpr auto kNoMacros = "no-macros,M";
constexpr auto kRaw = "raw,r";
constexpr auto kStrategy = "strategy,s";
constexpr auto kHelp = "help,h";

// Options long names:
constexpr auto kLongNoBrackets = "no-brackets";
constexpr auto kLongNoMacros = "no-macros";
constexpr auto kLongRaw = "raw";
constexpr auto kLongStrategy = "strategy";
constexpr auto kLongHelp = "help";
constexpr auto kLongTerm = "term";

// Option descriptions:
constexpr auto kDNoBrackets = "don't add missing brackets";
constexpr auto kDNoMacros = "don't replace macros with corresponding terms";
constexpr auto kDRaw = "don't preprocess input. Combination of -b and -M";
constexpr auto kDStrategy = "reduction strategy to use. Possible values: NO, APPL, CBN, CBV";
constexpr auto kDHelp = "print this help and exit";
constexpr auto kDTerm = "term to reduce";

// Misc:
constexpr auto kGlobalHelpPrefix =
        "Perform beta-reduction on lambda terms.\n"
        "Usage: ./beta-red [options] term\n\n";

}  // namespace cli
