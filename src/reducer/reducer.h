#pragma once

#include <functional>
#include <memory>

#include "model/enums/reduction_strategies.h"
#include "model/term.h"
#include "reducer/dfs.h"
#include "reducer/term_info.h"

namespace reducer {

class Reducer {
private:
    std::shared_ptr<model::term::Term> current_term_;
    std::function<TermInfo(std::shared_ptr<model::term::Term>)> select_next_;

public:
    Reducer(std::shared_ptr<model::term::Term>&& root, model::ReductionStrategies strat)
        : current_term_(std::move(root)) {
        switch (strat) {
            case model::ReductionStrategies::NO:
                select_next_ = std::bind(&algorithm::DFSFindRedex, std::placeholders::_1, false);
                break;
            case model::ReductionStrategies::APPL:
                select_next_ = std::bind(&algorithm::DFSFindLeftmostInerrmostRedex,
                                         std::placeholders::_1, false);
                break;
            case model::ReductionStrategies::CBN:
                select_next_ = std::bind(&algorithm::DFSFindRedex, std::placeholders::_1, true);
                break;
            case model::ReductionStrategies::CBV:
                select_next_ = std::bind(&algorithm::DFSFindLeftmostInerrmostRedex,
                                         std::placeholders::_1, true);
                break;
        }
    }

    [[nodiscard]] std::pair<bool, std::string> Step() noexcept;
};

}  // namespace reducer
