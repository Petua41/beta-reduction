#pragma once

#include "model/term.h"
#include "model/term_info.h"

namespace strategy {

[[nodiscard]] model::strategy::TermInfo NOSelectNext(
        std::shared_ptr<model::term::Term> term) noexcept;

[[nodiscard]] model::strategy::TermInfo APPLSelectNext(
        std::shared_ptr<model::term::Term> term) noexcept;

[[nodiscard]] model::strategy::TermInfo CBNSelectNext(
        std::shared_ptr<model::term::Term> term) noexcept;

[[nodiscard]] model::strategy::TermInfo CBVSelectNext(
        std::shared_ptr<model::term::Term> term) noexcept;

}  // namespace strategy
