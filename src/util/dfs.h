#pragma once

#include <memory>

#include "model/terms.h"
#include "strategy/term_info.h"

namespace util::algorithm {

strategy::TermInfo DFSFindTermInfo(std::shared_ptr<model::term::Term> root,
                                   std::function<bool(std::shared_ptr<model::term::Term>)> pred);
strategy::TermInfo DFSFindTermInfoSkipChildren(
        std::shared_ptr<model::term::Term> root,
        std::function<bool(std::shared_ptr<model::term::Term>)> pred,
        std::function<bool(std::shared_ptr<model::term::Term>)> skip_if);

}  // namespace util::algorithm
