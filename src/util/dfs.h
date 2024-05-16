#pragma once

#include <memory>

#include "model/term_info.h"
#include "model/terms.h"

namespace util::algorithm {

std::shared_ptr<model::term::Term> DFSFind(
        std::shared_ptr<model::term::Term> root,
        std::function<bool(std::shared_ptr<model::term::Term>)> pred);
model::strategy::TermInfo DFSFindTermInfo(
        std::shared_ptr<model::term::Term> root,
        std::function<bool(std::shared_ptr<model::term::Term>)> pred);

}  // namespace util::algorithm