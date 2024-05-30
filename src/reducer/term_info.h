#pragma once

#include <memory>
#include <ostream>

#include "model/term.h"

namespace reducer {

struct TermInfo {
public:
    std::shared_ptr<model::term::Term> term;
    std::shared_ptr<model::term::Term> parent;
    bool in_lhs;  // otherwise, in rhs

    TermInfo(std::shared_ptr<model::term::Term> term,
              std::shared_ptr<model::term::Term> parent, bool in_lhs)
        : term(std::move(term)), parent(std::move(parent)), in_lhs(in_lhs) {}

    bool operator==(TermInfo const& other) const {
        return term == other.term && parent == other.parent && in_lhs == other.in_lhs;
    }

    bool operator!=(TermInfo const& other) const {
        return !(*this == other);
    }
};

// This operator is very useful in logging:
inline std::ostream& operator<<(std::ostream& os, TermInfo const& ti) {
    os << '[';

    if (ti.term == nullptr) {
        os << "Empty RedexInfo";
    } else {
        os << ti.term->ToString();

        if (ti.parent == nullptr) {
            os << " without parent";
        } else {
            os << " in " << (ti.in_lhs ? "lhs " : "rhs ") << "of " << ti.parent;
        }
    }

    os << ']';
    return os;
}

}  // namespace reducer

template <>
struct std::hash<reducer::TermInfo> {
    std::size_t operator()(reducer::TermInfo const& ri) const noexcept {
        auto h1 = std::hash<std::shared_ptr<model::term::Term>>{}(ri.term);
        auto h2 = std::hash<std::shared_ptr<model::term::Term>>{}(ri.parent);
        auto h3 = std::hash<bool>{}(ri.in_lhs);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
