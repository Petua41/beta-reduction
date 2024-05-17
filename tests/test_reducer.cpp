#include <gtest/gtest.h>

#include "model/enums/reduction_exit_status.h"
#include "model/enums/reduction_strategies.h"
#include "model/reduction_result.h"
#include "model/terms.h"
#include "strategy/reducer.h"

namespace tests {

using namespace model;
using namespace model::term;
using namespace model::strategy;

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

// clang-format off
auto const simple_application = std::make_shared<Application>(
    std::make_shared<Abstraction>(
        Variable{"x"},
        std::make_shared<Variable>("x")),
    std::make_shared<Variable>("y"));
ReductionResult const simple_application_result{"y", ReductionExitStatus::NormalForm};

INSTANTIATE_TEST_SUITE_P(
    ReducerNormalOrderTests, TestReducer,
    ::testing::Values(
        ReducerTestParams(simple_application, simple_application_result)
    ));
// clang-format on

}  // namespace tests
