#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "model/enums/strategy_states.h"
#include "model/term_info.h"
#include "model/terms.h"
#include "util/dfs.h"

namespace strategy {

class IStrategy {
public:
    [[nodiscard]] virtual model::strategy::TermInfo SelectNext() noexcept = 0;

    [[nodiscard]] virtual std::string CurrentString() const noexcept = 0;

    virtual void SetCurrent(std::shared_ptr<model::term::Term>&& term) noexcept = 0;

    [[nodiscard]] virtual bool IsInNormalForm() const noexcept = 0;
};

}  // namespace strategy
