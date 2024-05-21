#include <string>

#include <gtest/gtest.h>

#include "model/terms.h"
#include "parser/parser.h"
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

    parsing::Parser parser{input};
    auto actual_term = parser.Parse();

    EXPECT_NE(actual_term, nullptr);

    EXPECT_EQ(*sample_term, *actual_term);
}

// clang-format off

INSTANTIATE_TEST_SUITE_P(
    ParserSimpleTermsTests, TestParser,
    ::testing::Values(
        // Variable with one-character name:
        ParserTestParams("x", std::make_shared<model::term::Variable>("x")),
        // Variable with multi-character name:
        ParserTestParams("abc", std::make_shared<model::term::Variable>("abc")),
// Unicode input isn't currently supported:
#if 0
        // Abstraction (Unicode):
        ParserTestParams("(λx.x)", kSimpleAbstraction),
#endif
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
// clang-format on

}  // namespace tests
