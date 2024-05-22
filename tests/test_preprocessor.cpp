#include <string>

#include <gtest/gtest.h>

#include "preprocessor/preprocessor.h"

namespace test {

using namespace preprocessor;

struct PreprocessorTestParams {
    std::string input_;
    std::string sample_result_;
    bool check_brackets_;
    bool replace_macros_;
    bool sample_brackets_are_correct_;

    PreprocessorTestParams(std::string&& input, std::string&& sample_result,
                           bool check_brackets = true, bool replace_macros = true,
                           bool sample_brackets_are_correct = true)
        : input_(std::move(input)),
          sample_result_(std::move(sample_result)),
          check_brackets_(check_brackets),
          replace_macros_(replace_macros),
          sample_brackets_are_correct_(sample_brackets_are_correct) {}
};

class TestPreprocessor : public ::testing::TestWithParam<PreprocessorTestParams> {};

TEST_P(TestPreprocessor, DefaultTests) {
    auto const& p = GetParam();
    auto const& input_ = p.input_;
    auto const& sample_result = p.sample_result_;
    auto const check_brackets = p.check_brackets_;
    auto const replace_macros = p.replace_macros_;
    auto const sample_brackets_are_correct = p.sample_brackets_are_correct_;

    Preprocessor prep{input_, check_brackets, replace_macros};
    auto [actual_brackets_are_correct, actual_result] = prep.Preprocess();

    EXPECT_EQ(actual_brackets_are_correct, sample_brackets_are_correct);
    EXPECT_EQ(actual_result, sample_result);
}

// clang-format off

INSTANTIATE_TEST_SUITE_P(
    PreprocessorBracketsTests, TestPreprocessor,
    ::testing::Values(
        // Missing outermost brackets:
        PreprocessorTestParams("Lx.(x x)", "(Lx.(x x))", true, false)
    ));

INSTANTIATE_TEST_SUITE_P(
    PreprocessorFixedStringMacrosTests, TestPreprocessor,
    ::testing::Values(
        // Fixpoint combinator:
        PreprocessorTestParams("(Y R)", "((Lf.((Lx.(f(x x)))) (Lx.(f(x x)))))) R)", false),
        // True:
        PreprocessorTestParams("(Lz.true)", "(Lz.(Lx.(Ly.x)))", false),
        // False:
        PreprocessorTestParams("(false A)", "((Lx.(Ly.y)) A)", false)
    ));

// clang-format on

}  // namespace test
