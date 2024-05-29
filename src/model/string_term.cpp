#include "model/string_term.h"

#include <boost/xpressive/regex_algorithms.hpp>
#include <easylogging++.h>
#include <memory>
#include <string>

#include "config/regular_expressions.h"
#include "exceptions/parsing_error.h"
#include "model/enums/term_types.h"
#include "model/terms.h"

namespace model::parsing {

using enum TermTypes;

void StringTerm::ResolveType() {
    using namespace boost::xpressive;
    using namespace config::regular_expressions;

    if (regex_match(data_.begin(), data_.end(), kTermTypeRegexes.Variable())) {
        type_ = Variable;
    } else if (regex_match(data_.begin(), data_.end(), kTermTypeRegexes.Abstraction())) {
        type_ = Abstraction;
    } else if (regex_match(data_.begin(), data_.end(), kTermTypeRegexes.Application())) {
        type_ = Application;
    } else {
        throw exceptions::ParsingError{"cannot resolve type of term", std::move(data_)};
    }
}

/// @throw ParsingError -- if term contains syntax errors
void StringTerm::Split() {
    switch (Type()) {
        case Variable:
            break;
        case Abstraction: {
            auto first_dot_position = data_.find('.');
            auto const& lhs = data_.substr(2, first_dot_position - 2);
            auto const& rhs =
                    data_.substr(first_dot_position + 1, data_.size() - first_dot_position - 2);
            string_lhs_ = std::make_unique<StringTerm>(std::move(lhs));
            string_rhs_ = std::make_unique<StringTerm>(std::move(rhs));
            break;
        }
        case Application: {
            size_t brackets_count{0};
            size_t space_position{0};
            for (size_t i{1}; i < data_.size() - 1; ++i) {
                if (data_[i] == '(') {
                    ++brackets_count;
                } else if (data_[i] == ')') {
                    --brackets_count;
                }

                if (brackets_count == 0) {
                    space_position = i + 1;
                    break;
                }
            }

            if (space_position >= data_.size() || data_[space_position] != ' ') {
                throw exceptions::ParsingError{"invalid brackets or whitespace in abstraction",
                                               std::move(data_)};
            }

            auto const& lhs = data_.substr(1, space_position - 1);
            auto const& rhs = data_.substr(space_position + 1, data_.size() - space_position - 2);
            string_lhs_ = std::make_unique<StringTerm>(std::move(lhs));
            string_rhs_ = std::make_unique<StringTerm>(std::move(rhs));
            break;
        }
        default:
            break;
    }
}

/// @throw ParsingError -- if term contains syntax or semantic errors
[[nodiscard]] std::shared_ptr<term::Term> StringTerm::Parse() {
    LOG(INFO) << "Parsing '" << data_ << "'...";
    Split();

    if (string_lhs_ != nullptr && string_rhs_ != nullptr) {
        LOG(INFO) << "\tSplitted version: '" << string_lhs_->data_ << "' and '"
                  << string_rhs_->data_ << '\'';
    }

    switch (Type()) {
        case Variable:
            return std::make_shared<term::Variable>(data_);
        case Abstraction: {
            auto lhs = string_lhs_->Parse();
            auto rhs = string_rhs_->Parse();

            auto* lhs_var = dynamic_cast<term::Variable*>(lhs.get());

            if (lhs_var == nullptr) {
                throw exceptions::ParsingError{"lhs of Abstraction must be Variable",
                                               std::move(data_)};
            }

            return std::make_shared<term::Abstraction>(std::move(*lhs_var), std::move(rhs));
        }
        case Application: {
            auto lhs = string_lhs_->Parse();
            auto rhs = string_rhs_->Parse();

            return std::make_shared<term::Application>(std::move(lhs), std::move(rhs));
        }
            [[unlikely]] default :
                // This case couldn't happen, because ResolveType() never sets type to Unknown
                return nullptr;
    }
}

}  // namespace model::parsing
