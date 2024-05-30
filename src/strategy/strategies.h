#pragma once

#include "model/term.h"
#include "strategy/term_info.h"

namespace strategy {

[[nodiscard]] TermInfo NOSelectNext(std::shared_ptr<model::term::Term> term) noexcept;
[[nodiscard]] TermInfo APPLSelectNext(std::shared_ptr<model::term::Term> term) noexcept;
[[nodiscard]] TermInfo CBNSelectNext(std::shared_ptr<model::term::Term> term) noexcept;
[[nodiscard]] TermInfo CBVSelectNext(std::shared_ptr<model::term::Term> term) noexcept;

}  // namespace strategy
