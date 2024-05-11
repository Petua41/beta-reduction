#include "model/string_term.h"

#include <cassert>
#include <easylogging++.h>
#include <memory>
#include <regex>
#include <string>

#include "config/regular_expressions.h"
#include "model/enums/term_types.h"
#include "model/terms.h"

namespace model::parsing {

void StringTerm::ResolveType() {
    using namespace config::regular_expressions;
    if (std::regex_match(data_, variable_regex)) {
        type_ = TermTypes::Variable;
    } else if (std::regex_match(data_, abstraction_regex)) {
        type_ = TermTypes::Abstraction;
    } else if (std::regex_match(data_, application_regex)) {
        type_ = TermTypes::Application;
    } else {
        type_ = TermTypes::Error;
    }
}

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

            // TODO(senichenkov): I should inform user if I cannot parse Application
            // because of wrong brackets (and check brackets before parsing)
            assert(space_position < data_.size() && data_[space_position] == ' ');

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

[[nodiscard]] std::shared_ptr<term::Term> StringTerm::Parse() {
    LOG(INFO) << "Parsing " << data_;
    Split();

    if (string_lhs_ != nullptr && string_rhs_ != nullptr) {
        LOG(INFO) << "Splitted version: " << string_lhs_->data_ << " AND " << string_rhs_->data_;
    }

    switch (Type()) {
        case TermTypes::Variable:
            return std::make_shared<term::Variable>(data_);
        case TermTypes::Abstraction: {
            auto lhs = string_lhs_->Parse();
            auto rhs = string_rhs_->Parse();
            assert(rhs != nullptr);
            assert(lhs != nullptr);

            auto* lhs_var = dynamic_cast<term::Variable*>(lhs.get());
            assert(lhs_var != nullptr);

            return std::make_shared<term::Abstraction>(std::move(*lhs_var), std::move(rhs));
        }
        case TermTypes::Application: {
            auto lhs = string_lhs_->Parse();
            auto rhs = string_rhs_->Parse();

            assert(lhs != nullptr);
            assert(rhs != nullptr);

            return std::make_shared<term::Application>(std::move(lhs), std::move(rhs));
        }
        default:
            return nullptr;
    }
}

}  // namespace model::parsing
