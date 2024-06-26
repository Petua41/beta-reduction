#include <string>

#include <gtest/gtest.h>

#include "exceptions/invalid_brackets_error.h"
#include "preprocessor/preprocessor.h"

namespace test {

struct PreprocessorTestParams {
    std::string input_;
    std::string sample_result_;
    bool check_brackets_;
    bool replace_macros_;

    PreprocessorTestParams(std::string&& input, std::string&& sample_result,
                           bool check_brackets = true, bool replace_macros = true)
        : input_(std::move(input)),
          sample_result_(std::move(sample_result)),
          check_brackets_(check_brackets),
          replace_macros_(replace_macros) {}
};

class TestPreprocessor : public ::testing::TestWithParam<PreprocessorTestParams> {};

TEST_P(TestPreprocessor, DefaultTests) {
    auto const& p = GetParam();
    auto const& input = p.input_;
    auto const& sample_result = p.sample_result_;
    auto const check_brackets = p.check_brackets_;
    auto const replace_macros = p.replace_macros_;

    std::string actual_result{input};
    if (check_brackets) {
        actual_result = preprocessor::CheckBrackets(actual_result);
    }
    if (replace_macros) {
        actual_result = preprocessor::ReplaceMacros(actual_result);
    }

    EXPECT_EQ(actual_result, sample_result);
}

class TestPreprocessorException : public ::testing::TestWithParam<PreprocessorTestParams> {};

TEST_P(TestPreprocessorException, ExceptionTests) {
    auto const& p = GetParam();
    auto input = p.input_;

    EXPECT_THROW(auto discard = preprocessor::CheckBrackets(input),
                 exceptions::InvalidBracketsError);
}

// clang-format off

INSTANTIATE_TEST_SUITE_P(
    PreprocessorBracketsTests, TestPreprocessor,
    ::testing::Values(
        // Missing outermost brackets:
        PreprocessorTestParams("Lx.(x x)", "(Lx.(x x))", true, false),
        // Abstraction is missing brackets in it's rhs:
        PreprocessorTestParams("(Lx.x y)", "(Lx.(x y))", true, false),
        // OK (Abstraction's rhs is a single Variable):
        PreprocessorTestParams("(Lx.x)", "(Lx.x)", true, false),
        // A bit more complex case:
        PreprocessorTestParams("(Lx.A Ly.B C)", "(Lx.(A Ly.(B C)))", true, false)
    ));

INSTANTIATE_TEST_SUITE_P(
    PreprocessorFixedStringMacrosTests, TestPreprocessor,
    ::testing::Values(
        // Fixpoint combinator:
        PreprocessorTestParams("(Y R)", "((Lf.((Lx.(f (x x))) (Lx.(f (x x))))) R)", false),
        // True:
        PreprocessorTestParams("(Lz.true)", "(Lz.(Lx.(Ly.x)))", false),
        // False:
        PreprocessorTestParams("(false A)", "((Lx.(Ly.y)) A)", false)
    ));

INSTANTIATE_TEST_SUITE_P(
    PreprocessorChurchNumeralsTests, TestPreprocessor,
    ::testing::Values(
        // Check that zero is replaced correctly:
        PreprocessorTestParams("(0 x)", "((Lf.(Lx.x)) x)", false),
        // Check basic case -- one numeral from [1, 9]:
        PreprocessorTestParams("(Lx.2)", "(Lx.(Lf.(Lx.(f (f x)))))", false),
        // Check multiple different numerals in single term:
        PreprocessorTestParams("(3 1)", "((Lf.(Lx.(f (f (f x))))) (Lf.(Lx.(f x))))", false),
        // Check multiple similar numerals in single term:
        PreprocessorTestParams("(Lz.(4 4))", "(Lz.((Lf.(Lx.(f (f (f (f x)))))) (Lf.(Lx.(f (f (f (f x))))))))", false),
        // Check multi-digit numeral:
        PreprocessorTestParams("10", "(Lf.(Lx.(f (f (f (f (f (f (f (f (f (f x))))))))))))", false)
    ));

INSTANTIATE_TEST_SUITE_P(
    PreprocessorUnaryOperatorsTests, TestPreprocessor,
    ::testing::Values(
        // NOT operator:
        PreprocessorTestParams("(NOT false)", "((Lp.((p (Lx.(Ly.y))) (Lx.(Ly.x)))) (Lx.(Ly.y)))", false),
        // NOT operator (with not-empty suffix):
        PreprocessorTestParams("(Lz.(NOT z) A)", "(Lz.((Lp.((p (Lx.(Ly.y))) (Lx.(Ly.x)))) z) A)", false),
        // ISZERO operator:
        PreprocessorTestParams("(ISZERO 0)", "((Ln.((n (Lx.(Lx.(Ly.y)))) (Lx.(Ly.x)))) (Lf.(Lx.x)))", false)
    ));

INSTANTIATE_TEST_SUITE_P(
    PreprocessorBinaryOperatorsTests, TestPreprocessor,
    ::testing::Values(
        // + operator:
        PreprocessorTestParams("((+ 2) 2)", "(((Lm.(Ln.(Lf.(Lx.((m f) ((n f) x)))))) (Lf.(Lx.(f (f x))))) (Lf.(Lx.(f (f x)))))", false),
        // * operator:
        PreprocessorTestParams("((* 1) 2)", "(((Lm.(Ln.(Lf.(Lx.((m (n f)) x))))) (Lf.(Lx.(f x)))) (Lf.(Lx.(f (f x)))))", false),
        // AND operator:
        PreprocessorTestParams("((&& true) false)", "(((Lp.(Lq.((p q) p))) (Lx.(Ly.x))) (Lx.(Ly.y)))", false),
        // OR operator:
        PreprocessorTestParams("((|| true) false)", "(((Lp.(Lq.((p p) q))) (Lx.(Ly.x))) (Lx.(Ly.y)))", false)
    ));

INSTANTIATE_TEST_SUITE_P(
    PreprocessorExceptionTests, TestPreprocessorException,
    ::testing::Values(
        // Opens more than closes:
        PreprocessorTestParams("(A(B C C)", ""),
        // Closes more than opens:
        PreprocessorTestParams("(A)B)C", "")
    ));

// clang-format on

}  // namespace test
