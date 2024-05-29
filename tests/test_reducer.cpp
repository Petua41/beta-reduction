#include <gtest/gtest.h>

#include "model/terms.h"
#include "sample_terms.h"
#include "strategy/reducer.h"

namespace tests {

using namespace model;
using namespace model::term;
using namespace model::strategy;
using namespace test::sample_terms;

struct ReducerTestParams {
    std::shared_ptr<Term> initial_term;
    std::string sample_result;
    ReductionStrategies strat;

    ReducerTestParams(std::shared_ptr<Term>&& initial_term, std::string&& sample_result,
                      ReductionStrategies strat = ReductionStrategies::NO)
        : initial_term(std::move(initial_term)),
          sample_result(std::move(sample_result)),
          strat(strat) {}

    ReducerTestParams(std::shared_ptr<Term> const& initial_term, std::string const& sample_result,
                      ReductionStrategies strat = ReductionStrategies::NO)
        : initial_term(initial_term), sample_result(sample_result), strat(strat) {}
};

class TestReducer : public ::testing::TestWithParam<ReducerTestParams> {};

TEST_P(TestReducer, DefaultTests) {
    auto const& param = GetParam();
    auto initial_term = param.initial_term;
    auto sample_result = param.sample_result;
    auto strat = param.strat;

    ::strategy::Reducer reducer{std::move(initial_term), strat};
    while (true) {
        auto [normal_form, actual_result] = reducer.Step();
        if (normal_form) {
            EXPECT_EQ(actual_result, sample_result);
            return;
        }
    }
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
    ReducerNormalOrderTests, TestReducer,
    ::testing::Values(
        ReducerTestParams(kSimpleRedex, "y"),
        ReducerTestParams(k2Plus2, "(Lf.(Lx.(f (f (f (f x))))))")
    ));

INSTANTIATE_TEST_SUITE_P(
    ReducerApplicativeOrderTests, TestReducer,
    ::testing::Values(
        ReducerTestParams(kSimpleRedex, "y", ReductionStrategies::APPL)
    ));

INSTANTIATE_TEST_SUITE_P(
    ReducerCBVTests, TestReducer,
    ::testing::Values(
        ReducerTestParams(kSimpleRedex, "y", ReductionStrategies::CBV),
        // CBV shouldn't reduce kWeakNormalForm:
        ReducerTestParams(kWeakNormalForm, "(Lx.((Ly.y) x))", ReductionStrategies::CBV)
    ));

INSTANTIATE_TEST_SUITE_P(
    ReducerCBNTests, TestReducer,
    ::testing::Values(
        ReducerTestParams(kSimpleRedex, "y", ReductionStrategies::CBN),
        // CBN shouldn't reduce kWeakNormalForm:
        ReducerTestParams(kWeakNormalForm, "(Lx.((Ly.y) x))", ReductionStrategies::CBN)
    ));
// clang-format on

}  // namespace tests
