#pragma once

namespace cli::descriptions {

constexpr auto kdNoBrackets = "don't add missing brackets";
constexpr auto kdNoMacros = "don't replace macros with corresponding terms";
constexpr auto kdRaw = "don't preprocess input. Combination of -b and -M";
constexpr auto kdASCII = "don't use greek letters in output (doesn't affect input)";
constexpr auto kdMaxOp = "maximum reduction operations before exit. 0 is infinity. Default is 0";
constexpr auto kdStrategy = "reduction strategy to use. Default is NO";
constexpr auto kdHelp = "print this help and exit";
constexpr auto kdTerm = "term to reduce";

}  // namespace cli::descriptions
