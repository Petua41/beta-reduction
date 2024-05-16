#include "util/dfs.h"

#include <memory>
#include <stack>
#include <unordered_set>

#include "model/term_info.h"
#include "model/terms.h"

namespace util::algorithm {

using namespace model::term;
using namespace model::strategy;

std::shared_ptr<Term> DFSFind(std::shared_ptr<Term> root,
                              std::function<bool(std::shared_ptr<Term>)> pred) {
    if (pred(root)) {
        return root;
    }

    std::stack<std::shared_ptr<Term>> to_visit;
    to_visit.push(root);
    std::unordered_set<std::shared_ptr<Term>> visited;

    while (to_visit.size()) {
        auto current = to_visit.top();
        to_visit.pop();

        if (visited.find(current) != visited.end()) {
            continue;
        }

        if (pred(current)) {
            return current;
        }

        auto abstr = dynamic_cast<Abstraction*>(current.get());
        auto appl = dynamic_cast<Application*>(current.get());
        if (abstr) {
            to_visit.push(abstr->Lhs());
            to_visit.push(abstr->Rhs());
        } else if (appl) {
            to_visit.push(appl->Lhs());
            to_visit.push(appl->Rhs());
        }

        visited.insert(current);
    }
    return nullptr;
}

TermInfo DFSFindTermInfo(std::shared_ptr<Term> root,
                         std::function<bool(std::shared_ptr<Term>)> pred) {
    TermInfo root_term_info{std::move(root), nullptr, true};

    if (pred(root)) {
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

        auto abstr = dynamic_cast<Abstraction*>(current.term.get());
        auto appl = dynamic_cast<Application*>(current.term.get());
        if (abstr) {
            to_visit.emplace(abstr->Rhs(), current.term, false);
        } else if (appl) {
            to_visit.emplace(appl->Lhs(), current.term, true);
            to_visit.emplace(appl->Rhs(), current.term, false);
        }

        visited.insert(current);
    }
    return TermInfo{nullptr, nullptr, true};
}

}  // namespace util::algorithm
