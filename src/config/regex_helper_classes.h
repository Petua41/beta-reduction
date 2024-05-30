#pragma once

#include <boost/xpressive/xpressive_static.hpp>

namespace {

using namespace boost::xpressive;

}  // namespace

/// @brief Helper classes for recursive regexes (because they cannot be declared directly)
namespace config::helper_classes {

struct XpressiveTermTypes {
public:
    sregex term_, variable_, abstraction_, application_;
    mark_tag const lhs_{1};
    mark_tag const rhs_{2};

    XpressiveTermTypes() {
        term_ = by_ref(variable_) | by_ref(abstraction_) | by_ref(application_);
        variable_ = +alnum;
        // Currently, Unicode lambda isn't supported:
        abstraction_ = as_xpr('(') >> 'L' >> (lhs_= variable_) >> '.' >> (rhs_= by_ref(term_)) >> ')';
        application_ = '(' >> (lhs_= by_ref(term_)) >> ' ' >> (rhs_= by_ref(term_)) >> ')';
    }
};

}  // namespace config::helper_classes
