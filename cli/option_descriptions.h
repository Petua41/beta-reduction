#pragma once

namespace cli::descriptions {

constexpr auto kDNoBrackets = "don't add missing brackets";
constexpr auto kDNoMacros = "don't replace macros with corresponding terms";
constexpr auto kDRaw = "don't preprocess input. Combination of -b and -M";
constexpr auto kDStrategy = "reduction strategy to use. Possible values: NO, APPL, CBN, CBV";
constexpr auto kDHelp = "print this help and exit";
constexpr auto kDTerm = "term to reduce";

}  // namespace cli::descriptions
