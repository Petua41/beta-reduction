#include <string>

#include <gtest/gtest.h>

#include "exceptions/parsing_error.h"
#include "model/term.h"
#include "model/variable.h"
#include "parser/string_term.h"
#include "sample_terms.h"

namespace tests {

using namespace model::term;
using namespace test::sample_terms;

struct ParserTestParams {
    std::string input_;
    std::shared_ptr<Term> sample_term_;

    ParserTestParams(std::string const& input, std::shared_ptr<Term>&& sample_term)
        : input_(input), sample_term_(std::move(sample_term)) {}

    ParserTestParams(std::string const& input, std::shared_ptr<Term> const& sample_term)
        : input_(input), sample_term_(sample_term) {}
};

class TestParser : public ::testing::TestWithParam<ParserTestParams> {};

TEST_P(TestParser, DefaultTests) {
    auto const& p = GetParam();
    auto& input = p.input_;
    auto& sample_term = p.sample_term_;

    parsing::StringTerm string_root{input};
    auto actual_term = string_root.Parse();

    EXPECT_EQ(*sample_term, *actual_term);
}

class TestParserException : public ::testing::TestWithParam<ParserTestParams> {};

TEST_P(TestParserException, ExceptionTests) {
    auto const& p = GetParam();
    auto input = p.input_;

    parsing::StringTerm string_root{input};
    EXPECT_THROW(auto discard = string_root.Parse(), exceptions::ParsingError);
}

// clang-format off

INSTANTIATE_TEST_SUITE_P(
    ParserSimpleTermsTests, TestParser,
    ::testing::Values(
        // Variable with one-character name:
        ParserTestParams("x", std::make_shared<Variable>("x")),
        // Variable with multi-character name:
        ParserTestParams("abc", std::make_shared<Variable>("abc")),
        // Abstraction (ASCII):
        ParserTestParams("(Lx.x)", kSimpleAbstraction),
        // Application:
        ParserTestParams("(x x)", kSimpleApplication)
    ));

INSTANTIATE_TEST_SUITE_P(
	ParserComplexTermsTests, TestParser,
	::testing::Values(
		ParserTestParams("(Lf.(Lx.(f (f x))))", kChurch2),
		ParserTestParams("(Lf.((Lx.(f (x x))) (Lx.(f (x x)))))", kFixpointCombinator)
	));

INSTANTIATE_TEST_SUITE_P(
    ParserExceptionTests, TestParserException,
    ::testing::Values(
        // Unknown type of term:
        ParserTestParams("(L).x", nullptr),
        // Invalid syntax in Application:
        ParserTestParams("(A(X Y))", nullptr),
        // Lhs of Abstraction is not Variable:
        ParserTestParams("(L(A B).(X Y))", nullptr)
    ));
// clang-format on

}  // namespace tests
