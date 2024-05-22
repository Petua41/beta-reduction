#include <gtest/gtest.h>

#include "model/enums/reduction_exit_status.h"
#include "model/enums/reduction_strategies.h"
#include "model/reduction_result.h"
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
    ReductionResult sample_result;
    ReductionStrategies strat;

    ReducerTestParams(std::shared_ptr<Term>&& initial_term, ReductionResult&& sample_result,
                      ReductionStrategies strat = ReductionStrategies::NO)
        : initial_term(std::move(initial_term)),
          sample_result(std::move(sample_result)),
          strat(strat) {}

    ReducerTestParams(std::shared_ptr<Term> const& initial_term,
                      ReductionResult const& sample_result,
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
    auto actual_result = reducer.MainLoop();

    EXPECT_EQ(actual_result, sample_result);
}

ReductionResult const simple_redex_result{"y", ReductionExitStatus::NormalForm};
ReductionResult const addition_result{"(Lf.(Lx.(f (f (f (f x))))))",
                                      ReductionExitStatus::NormalForm};

// clang-format off
INSTANTIATE_TEST_SUITE_P(
    ReducerNormalOrderTests, TestReducer,
    ::testing::Values(
        ReducerTestParams(kSimpleRedex, simple_redex_result),
        ReducerTestParams(k2Plus2, addition_result)
    ));
// clang-format on

}  // namespace tests