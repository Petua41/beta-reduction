#pragma once

namespace cli::descriptions {

constexpr auto kDNoBrackets = "don't add missing brackets";
constexpr auto kDNoMacros = "don't replace macros with corresponding terms";
constexpr auto kDRaw = "don't preprocess input. Combination of -b and -M";
constexpr auto kDMaxOp = "maximum reduction operations before exit. 0 is infinity. Default is 0";
constexpr auto kDStrategy = "reduction strategy to use. Default is NO";
constexpr auto kDHelp = "print this help and exit";
constexpr auto kDTerm = "term to reduce";

}  // namespace cli::descriptions
