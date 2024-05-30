#pragma once

#include <boost/xpressive/xpressive_static.hpp>

namespace config::helper_classes {

/// @brief Helper class for recursive regexes (because they cannot be declared directly)
struct XpressiveTermTypes {
public:
    boost::xpressive::sregex term_, variable_, abstraction_, application_;
    boost::xpressive::mark_tag const lhs_{1};
    boost::xpressive::mark_tag const rhs_{2};

    XpressiveTermTypes() {
        using namespace boost::xpressive;

        term_ = by_ref(variable_) | by_ref(abstraction_) | by_ref(application_);
        variable_ = +alnum;
        abstraction_ = as_xpr('(') >> 'L' >> (lhs_= variable_) >> '.' >> (rhs_= by_ref(term_)) >> ')';
        application_ = '(' >> (lhs_= by_ref(term_)) >> ' ' >> (rhs_= by_ref(term_)) >> ')';
    }
};

}  // namespace config::helper_classes
