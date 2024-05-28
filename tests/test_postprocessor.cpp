#include <gtest/gtest.h>
#include <string>

#include "postprocessor/postprocessor.h"

namespace test {

using namespace postprocessor;

struct PostprocessorTestParams {
    std::string input_;
    std::string sample_output_;

    PostprocessorTestParams(std::string&& input, std::string&& sample_output)
        : input_(std::move(input)), sample_output_(std::move(sample_output)) {}
};

class TestPostprocessor : public ::testing::TestWithParam<PostprocessorTestParams> {};

TEST_P(TestPostprocessor, DefaultTests) {
    auto const& p = GetParam();
    auto input = p.input_;
    auto sample_output = p.sample_output_;

    Postprocessor postpr{input, true};
    auto actual_output = postpr.Process();

    EXPECT_EQ(actual_output, sample_output);
}

// clang-format off

INSTANTIATE_TEST_SUITE_P(
    PostprocessorFixedStringTests, TestPostprocessor,
    ::testing::Values(
        // True:
        PostprocessorTestParams("(Lx.(Ly.x))", "true")
    ));

INSTANTIATE_TEST_SUITE_P(
    PostprocessorChurchNumeralTests, TestPostprocessor,
    ::testing::Values(
        // Primitive case:
        PostprocessorTestParams("(Lf.(Lx.(f (f x))))", "2"),
        // Check that no extra brackets are taken:
        PostprocessorTestParams("(A (Lf.(Lx.(f x))))", "(A 1)"),
        // Check that two numerals are converted correctly:
        PostprocessorTestParams("((Lf.(Lx.(f x))) (Lf.(Lx.(f (f x)))))", "(1 2)"),
        // Check that 0 is processed correctly:
        PostprocessorTestParams("(Lf.(Lx.x))", "0")
    ));

// clang-format on

}  // namespace test
