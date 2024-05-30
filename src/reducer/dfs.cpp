#include "reducer/dfs.h"

#include <memory>
#include <stack>
#include <unordered_set>

#include "model/terms.h"
#include "reducer/term_info.h"

namespace reducer::algorithm {

using namespace model::term;

TermInfo DFSFindRedex(std::shared_ptr<Term> root, bool weak) {
    auto is_redex = [](std::shared_ptr<Term>& term) {
        auto* appl = dynamic_cast<Application*>(term.get());
        return appl != nullptr && appl->IsRedex();
    };

    TermInfo root_term_info{root, nullptr, true};

    if (is_redex(root)) {
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

        if (is_redex(current.term)) {
            return current;
        }

        if (weak && dynamic_cast<Abstraction*>(current.term.get()) != nullptr) {
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

TermInfo DFSFindLeftmostInerrmostRedex(std::shared_ptr<Term> root, bool weak) {
    std::unordered_set<TermInfo> visited{};
    std::unordered_map<size_t, std::vector<TermInfo>> levels{};

    TermInfo root_term_info{std::move(root), nullptr, true};

    auto root_level = weak ? WeakRecursiveDFS(std::move(root_term_info), visited, levels)
                           : RecursiveDFS(std::move(root_term_info), visited, levels);

    std::vector<std::vector<TermInfo>> reverse_levels{root_level + 1};
    for (auto& [level_num, level] : levels) {
        reverse_levels[level_num] = level;
    }

    // Go from bottom to top:
    for (auto const& level : reverse_levels) {
        // Go from left to right:
        for (auto const& elem : level) {
            auto* appl = dynamic_cast<Application*>(elem.term.get());
            if (appl != nullptr && appl->IsRedex()) {
                return elem;
            }
        }
    }

    // Haven't found anything:
    return TermInfo{nullptr, nullptr, true};
}

size_t RecursiveDFS(TermInfo&& parent, std::unordered_set<TermInfo>& visited,
                    std::unordered_map<size_t, std::vector<TermInfo>>& levels) {
    int lhs_level = -1;
    int rhs_level = -1;

    auto lhs_ptr = parent.term->Lhs();
    auto rhs_ptr = parent.term->Rhs();
    if (lhs_ptr != nullptr) {
        TermInfo lhs_term_info{std::move(lhs_ptr), parent.term, true};
        if (visited.find(lhs_term_info) == visited.end()) {
            lhs_level = RecursiveDFS(std::move(lhs_term_info), visited, levels);
        }
    }
    if (rhs_ptr != nullptr) {
        TermInfo rhs_term_info{std::move(rhs_ptr), parent.term, false};
        if (visited.find(rhs_term_info) == visited.end()) {
            rhs_level = RecursiveDFS(std::move(rhs_term_info), visited, levels);
        }
    }

    visited.insert(parent);

    auto level = std::max(rhs_level, rhs_level) + 1;

    if (levels.find(level) == levels.end()) {
        levels.emplace(level, std::vector<TermInfo>{});
    }
    levels[level].push_back(std::move(parent));

    return level;
}

size_t WeakRecursiveDFS(TermInfo&& parent, std::unordered_set<TermInfo>& visited,
                        std::unordered_map<size_t, std::vector<TermInfo>>& levels, bool skip) {
    int lhs_level = -1;
    int rhs_level = -1;

    // We don't need to add Abstractions' children to levels,
    // but we still need to calculate their level
    skip = skip || dynamic_cast<Abstraction*>(parent.term.get()) != nullptr;

    auto lhs_ptr = parent.term->Lhs();
    auto rhs_ptr = parent.term->Rhs();
    if (lhs_ptr != nullptr) {
        TermInfo lhs_term_info{std::move(lhs_ptr), parent.term, true};
        if (visited.find(lhs_term_info) == visited.end()) {
            lhs_level = WeakRecursiveDFS(std::move(lhs_term_info), visited, levels, skip);
        }
    }
    if (rhs_ptr != nullptr) {
        TermInfo rhs_term_info{std::move(rhs_ptr), parent.term, false};
        if (visited.find(rhs_term_info) == visited.end()) {
            rhs_level = WeakRecursiveDFS(std::move(rhs_term_info), visited, levels, skip);
        }
    }

    visited.insert(parent);

    auto level = std::max(rhs_level, rhs_level) + 1;

    if (!skip) {
        if (levels.find(level) == levels.end()) {
            levels.emplace(level, std::vector<TermInfo>{});
        }
        levels[level].push_back(std::move(parent));
    }

    return level;
}

}  // namespace reducer::algorithm
