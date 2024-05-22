#include "util/bfs.h"

#include <cassert>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>

#include "model/term_info.h"
#include "model/terms.h"

namespace util::algorithm {

using namespace model::term;
using namespace model::strategy;

std::stack<std::vector<TermInfo>> BFSSplitByLevels(std::shared_ptr<Term>&& root) {
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

        auto abstr = dynamic_cast<Abstraction*>(current.term.get());
        auto appl = dynamic_cast<Application*>(current.term.get());
        if (abstr) {
            to_visit.emplace(TermInfo{abstr->Rhs(), current.term, false}, next_level);
        } else if (appl) {
            to_visit.emplace(TermInfo{appl->Lhs(), current.term, true}, next_level);
            to_visit.emplace(TermInfo{appl->Rhs(), current.term, false}, next_level);
        }

        visited.insert(current);
    }

    return levels;
}

TermInfo BFSFindInnermostLeftmost(std::shared_ptr<Term> root,
                                  std::function<bool(std::shared_ptr<Term>)> pred) {
    auto levels = BFSSplitByLevels(std::move(root));

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
