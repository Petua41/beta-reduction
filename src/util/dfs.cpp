#include "util/dfs.h"

#include <memory>
#include <stack>
#include <unordered_set>

#include "model/term_info.h"
#include "model/terms.h"

namespace util::algorithm {

using namespace model::term;
using namespace model::strategy;

TermInfo DFSFindTermInfo(std::shared_ptr<Term> root,
                         std::function<bool(std::shared_ptr<Term>)> pred) {
    return DFSFindTermInfoSkipChildren(
            std::move(root), std::move(pred),
            [](auto param __attribute__((__unused__))) { return false; });
}

model::strategy::TermInfo DFSFindTermInfoSkipChildren(
        std::shared_ptr<model::term::Term> root,
        std::function<bool(std::shared_ptr<model::term::Term>)> pred,
        std::function<bool(std::shared_ptr<model::term::Term>)> skip_if) {
    TermInfo root_term_info{root, nullptr, true};

    if (pred(std::move(root))) {
        return root_term_info;
    }

    std::stack<TermInfo> to_visit;
    to_visit.push(root_term_info);
    std::unordered_set<TermInfo> visited;

    while (!to_visit.empty()) {
        auto current = to_visit.top();
        to_visit.pop();

        if (visited.find(current) != visited.end()) {
            continue;
        }

        if (pred(current.term)) {
            return current;
        }

        // Skip current's children if predicate is true:
        if (skip_if(current.term)) {
            continue;
        }

        auto lhs_ptr = current.term->Lhs();
        auto rhs_ptr = current.term->Rhs();
        if (lhs_ptr != nullptr) {
            to_visit.emplace(lhs_ptr, current.term, true);
        }
        if (rhs_ptr != nullptr) {
            to_visit.emplace(rhs_ptr, current.term, false);
        }

        visited.insert(current);
    }
    return TermInfo{nullptr, nullptr, true};
}

}  // namespace util::algorithm
