#include "parser/string_term.h"

#include <boost/xpressive/regex_algorithms.hpp>
#include <easylogging++.h>
#include <memory>

#include "config/regular_expressions.h"
#include "exceptions/parsing_error.h"
#include "model/terms.h"

namespace parsing {

using namespace boost::xpressive;
using namespace config::regular_expressions;
using namespace model::term;

[[nodiscard]] std::shared_ptr<Term> StringTerm::Parse() {
    smatch what;
    if (regex_match(data_, what, kTermTypeRegexes.variable_)) {
        return std::make_shared<Variable>(data_);
    } else if (regex_match(data_, what, kTermTypeRegexes.abstraction_)) {
        auto sm_lhs = what[kTermTypeRegexes.lhs_];
        auto sm_rhs = what[kTermTypeRegexes.rhs_];

        StringTerm string_lhs_{std::move(sm_lhs.str())};
        StringTerm string_rhs_{std::move(sm_rhs.str())};

        auto lhs = string_lhs_.Parse();
        auto rhs = string_rhs_.Parse();

        auto* lhs_var = dynamic_cast<Variable*>(lhs.get());

        if (lhs_var == nullptr) {
            throw exceptions::ParsingError{"lhs of Abstraction must be Variable",
                                            std::move(data_)};
        }

        return std::make_shared<Abstraction>(std::move(*lhs_var), std::move(rhs));
    } else if (regex_match(data_, what, kTermTypeRegexes.application_)) {
        auto sm_lhs = what[kTermTypeRegexes.lhs_];
        auto sm_rhs = what[kTermTypeRegexes.rhs_];

        StringTerm string_lhs_{std::move(sm_lhs.str())};
        StringTerm string_rhs_{std::move(sm_rhs.str())};

        auto lhs = string_lhs_.Parse();
        auto rhs = string_rhs_.Parse();

        return std::make_shared<Application>(std::move(lhs), std::move(rhs));
    } else {
        throw exceptions::ParsingError{"cannot resolve type of term", std::move(data_)};
    }
}

}  // namespace parsing
