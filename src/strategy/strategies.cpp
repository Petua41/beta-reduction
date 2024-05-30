#include "strategies.h"

#include <memory>

#include "model/abstraction.h"
#include "model/application.h"
#include "model/term.h"
#include "strategy/term_info.h"
#include "util/bfs.h"
#include "util/dfs.h"

namespace strategy {

using namespace model::term;
using namespace util::algorithm;

[[nodiscard]] TermInfo NOSelectNext(std::shared_ptr<Term> term) noexcept {
    auto is_redex = [](std::shared_ptr<Term> term) {
        auto appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    // DFS will firstly find leftmost redex
    auto redex_info = DFSFindTermInfo(term, is_redex);

    return redex_info;
}

[[nodiscard]] TermInfo APPLSelectNext(std::shared_ptr<Term> term) noexcept {
    auto is_redex = [](std::shared_ptr<Term> term) {
        auto appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    // Split by levels using BFS:
    auto levels = BFSSplitByLevels(term);
    // Find innermost leftmost:
    auto redex_info = BFSFindInnermostLeftmost(std::move(levels), is_redex);

    return redex_info;
}

[[nodiscard]] TermInfo CBNSelectNext(std::shared_ptr<Term> term) noexcept {
    auto is_redex = [](std::shared_ptr<Term> term) {
        auto appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    auto is_abstr = [](std::shared_ptr<Term> term) {
        return dynamic_cast<Abstraction*>(term.get()) != nullptr;
    };

    // Find leftmost using DFS, but skip Abstractions' children:
    auto redex_info = DFSFindTermInfoSkipChildren(term, is_redex, is_abstr);

    return redex_info;
}

[[nodiscard]] TermInfo CBVSelectNext(std::shared_ptr<Term> term) noexcept {
    auto is_redex = [](std::shared_ptr<Term> term) {
        auto appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    // Split by levels using BFS, but skip children of Abstractions:
    auto levels = BFSSplitByLevelsSkipChildren(term, [](std::shared_ptr<Term> term) {
        return dynamic_cast<Abstraction*>(term.get()) != nullptr;
    });
    // Find innermost leftmost:
    auto redex_info = BFSFindInnermostLeftmost(std::move(levels), is_redex);

    return redex_info;
}

}  // namespace strategy
