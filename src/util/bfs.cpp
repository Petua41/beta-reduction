#include "util/bfs.h"

#include <cassert>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>

#include "model/terms.h"
#include "strategy/term_info.h"

namespace util::algorithm {

using namespace model::term;
using namespace strategy;

std::stack<std::vector<TermInfo>> BFSSplitByLevels(std::shared_ptr<Term> root) {
    return BFSSplitByLevelsSkipChildren(
            std::move(root), [](auto param __attribute__((__unused__))) { return false; });
}

std::stack<std::vector<TermInfo>> BFSSplitByLevelsSkipChildren(
        std::shared_ptr<Term> root, std::function<bool(std::shared_ptr<Term>)> skip_if) {
    TermInfo root_term_info{std::move(root), nullptr, true};

    std::stack<std::vector<TermInfo>> levels;

    std::queue<std::pair<TermInfo, u_short>> to_visit;
    to_visit.emplace(root_term_info, 0);
    std::unordered_set<TermInfo> visited;

    u_short last_level{0};
    while (!to_visit.empty()) {
        auto [current, current_level] = to_visit.front();
        to_visit.pop();

        if (visited.find(current) != visited.end()) {
            continue;
        }

        // Skip current's children if predicate is true:
        if (skip_if(current.term)) {
            continue;
        }

        assert(current_level >= last_level);
        if (current_level > last_level) {
            std::vector<TermInfo> vec{current};
            levels.push(std::move(vec));
            last_level = current_level;
        } else {
            if (levels.size() == (size_t)(current_level + 1)) {
                levels.top().push_back(current);
            } else {
                std::vector<TermInfo> vec{current};
                levels.push(std::move(vec));
            }
        }

        auto next_level{current_level + 1};

        auto lhs_ptr = current.term->Lhs();
        auto rhs_ptr = current.term->Rhs();
        if (lhs_ptr != nullptr) {
            to_visit.emplace(TermInfo{lhs_ptr, current.term, true}, next_level);
        }
        if (rhs_ptr != nullptr) {
            to_visit.emplace(TermInfo{rhs_ptr, current.term, false}, next_level);
        }

        visited.insert(current);
    }

    return levels;
}

TermInfo BFSFindInnermostLeftmost(std::stack<std::vector<TermInfo>> levels,
                                  std::function<bool(std::shared_ptr<Term>)> pred) {
    // Go from bottom to top:
    while (!levels.empty()) {
        auto current_level = levels.top();
        levels.pop();

        // Go from left to right:
        for (auto const& term_info : current_level) {
            if (pred(term_info.term)) {
                return term_info;
            }
        }
    }

    return TermInfo{nullptr, nullptr, true};
}

}  // namespace util::algorithm
