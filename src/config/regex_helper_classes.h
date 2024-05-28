#pragma once

#include <boost/xpressive/xpressive_static.hpp>

namespace {

using namespace boost::xpressive;

}  // namespace

/// @brief Helper classes for recursive regexes (because they cannot be declared directly)
namespace config::helper_classes {

class XpressiveTermTypes {
private:
    sregex term_, variable_, abstraction_, application_;

public:
    XpressiveTermTypes() {
        term_ = by_ref(variable_) | by_ref(abstraction_) | by_ref(application_);
        variable_ = +alnum;
        // Currently, Unicode lambda isn't supported:
        abstraction_ = as_xpr('(') >> 'L' >> variable_ >> '.' >> by_ref(term_) >> ')';
        application_ = '(' >> by_ref(term_) >> ' ' >> by_ref(term_) >> ')';
    }

    [[nodiscard]] sregex const& Variable() const noexcept {
        return variable_;
    }

    [[nodiscard]] sregex const& Abstraction() const noexcept {
        return abstraction_;
    }

    [[nodiscard]] sregex const& Application() const noexcept {
        return application_;
    }
};

class XpressiveReverseChurchNumeral {
private:
    sregex reverse_church_numeral_, inner_;

public:
    XpressiveReverseChurchNumeral() {
        // Unicode lambda character isn't supported yet:
        reverse_church_numeral_ = as_xpr('(') >> 'L' >> +alnum >> '.' >> '(' >> 'L' >> +alnum >>
                                  '.' >> by_ref(inner_) >> ')' >> ')';
        inner_ = +alnum | ('(' >> +alnum >> ' ' >> by_ref(inner_) >> ')');
    }

    [[nodiscard]] sregex const& ReverseChurchNumeral() const noexcept {
        return reverse_church_numeral_;
    }
};

}  // namespace config::helper_classes
