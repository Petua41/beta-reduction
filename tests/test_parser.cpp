#include <string>

#include <gtest/gtest.h>

#include "model/terms.h"
#include "parser/parser.h"

namespace tests {

using namespace model::term;

struct ParserTestParams {
    std::string input_;
    std::shared_ptr<Term> sample_term_;

    ParserTestParams(std::string const& input, std::shared_ptr<Term>&& sample_term)
        : input_(input), sample_term_(std::move(sample_term)) {}
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
auto const simple_abstraction = std::make_shared<Abstraction>(
        Variable("x"),
		std::make_shared<Variable>("x"));

auto const simple_application = std::make_shared<Application>(
        std::make_shared<Variable>("x"),
		std::make_shared<Variable>("x"));

auto const church_2 = std::make_shared<Abstraction>(
	Variable("f"),
	std::make_shared<Abstraction>(
		Variable("x"),
		std::make_shared<Application>(
			std::make_shared<Variable>("f"),
			std::make_shared<Application>(
				std::make_shared<Variable>("f"),
				std::make_shared<Variable>("x")))));

auto const fixpoint_comb = std::make_shared<Abstraction>(
	Variable("f"),
	std::make_shared<Application>(
		std::make_shared<Abstraction>(
			Variable("x"),
			std::make_shared<Application>(
				std::make_shared<Variable>("f"),
				std::make_shared<Application>(
					std::make_shared<Variable>("x"),
					std::make_shared<Variable>("x")))),
		std::make_shared<Abstraction>(
			Variable("x"),
			std::make_shared<Application>(
				std::make_shared<Variable>("f"),
				std::make_shared<Application>(
					std::make_shared<Variable>("x"),
					std::make_shared<Variable>("x"))))));

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
        ParserTestParams("(λx.x)", simple_abstraction),
#endif
        // Abstraction (ASCII):
        ParserTestParams("(Lx.x)", simple_abstraction),
        // Application:
        ParserTestParams("(x x)", simple_application)
    ));

INSTANTIATE_TEST_SUITE_P(
	ParserComplexTermsTests, TestParser,
	::testing::Values(
		ParserTestParams("(Lf.(Lx.(f (f x))))", church_2),
		ParserTestParams("(Lf.((Lx.(f (x x))) (Lx.(f (x x)))))", fixpoint_comb)
	));
// clang-format on

}  // namespace tests
