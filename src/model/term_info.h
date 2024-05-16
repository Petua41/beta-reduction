#pragma once

#include <memory>

#include "model/application.h"
#include "model/term.h"

namespace model::strategy {

using namespace model::term;

struct TermInfo {
public:
    std::shared_ptr<Term> term;
    std::shared_ptr<Term> parent;
    bool in_lhs;  // otherwise, in rhs

    TermInfo(std::shared_ptr<Term> term, std::shared_ptr<Term> parent, bool in_lhs)
        : term(std::move(term)), parent(std::move(parent)), in_lhs(in_lhs) {}

    bool operator==(TermInfo const& other) const {
        return term == other.term && parent == other.parent && in_lhs == other.in_lhs;
    }

    bool operator!=(TermInfo const& other) const {
        return !(*this == other);
    }
};

}  // namespace model::strategy

template <>
struct std::hash<model::strategy::TermInfo> {
    std::size_t operator()(model::strategy::TermInfo const& ri) const noexcept {
        auto h1 = std::hash<std::shared_ptr<model::term::Term>>{}(ri.term);
        auto h2 = std::hash<std::shared_ptr<model::term::Term>>{}(ri.parent);
        auto h3 = std::hash<bool>{}(ri.in_lhs);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
