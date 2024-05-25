#include "model/string_term.h"

#include <easylogging++.h>
#include <memory>
#include <regex>
#include <string>

#include "config/regular_expressions.h"
#include "exceptions/parsing_error.h"
#include "model/enums/term_types.h"
#include "model/terms.h"

namespace model::parsing {

/// @throw ParsingError -- if term type couldn't be resolved (because of syntax errors)
void StringTerm::ResolveType() {
    using namespace config::regular_expressions;
    if (std::regex_match(data_, variable_regex)) {
        type_ = TermTypes::Variable;
    } else if (std::regex_match(data_, abstraction_regex)) {
        type_ = TermTypes::Abstraction;
    } else if (std::regex_match(data_, application_regex)) {
        type_ = TermTypes::Application;
    } else {
        throw exceptions::ParsingError{"cannot resolve type of term", std::move(data_)};
    }
}

/// @throw ParsingError -- if term contains syntax errors
void StringTerm::Split() {
    switch (Type()) {
        case TermTypes::Variable:
            break;
        case TermTypes::Abstraction: {
            auto first_dot_position = data_.find('.');
            auto const& lhs = data_.substr(2, first_dot_position - 2);
            auto const& rhs =
                    data_.substr(first_dot_position + 1, data_.size() - first_dot_position - 2);
            string_lhs_ = std::make_unique<StringTerm>(std::move(lhs));
            string_rhs_ = std::make_unique<StringTerm>(std::move(rhs));
            break;
        }
        case TermTypes::Application: {
            size_t brackets_count{0};
            size_t space_position;
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
        case TermTypes::Variable:
            return std::make_shared<term::Variable>(data_);
        case TermTypes::Abstraction: {
            auto lhs = string_lhs_->Parse();
            auto rhs = string_rhs_->Parse();

            auto* lhs_var = dynamic_cast<term::Variable*>(lhs.get());

            if (lhs_var == nullptr) {
                throw exceptions::ParsingError{"lhs of Abstraction must be Variable",
                                               std::move(data_)};
            }

            return std::make_shared<term::Abstraction>(std::move(*lhs_var), std::move(rhs));
        }
        case TermTypes::Application: {
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
