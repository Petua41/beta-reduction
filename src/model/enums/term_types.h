#pragma once

#include <ostream>

namespace model {

enum class TermTypes { Variable, Abstraction, Application, Unknown };

inline std::ostream const& operator<<(std::ostream& os, TermTypes const& tt) {
    switch (tt) {
        case TermTypes::Variable:
            os << "Variable";
            break;
        case TermTypes::Abstraction:
            os << "Abstraction";
            break;
        case TermTypes::Application:
            os << "Application";
            break;
        case TermTypes::Unknown:
            os << "Unknown";
            break;
    }
    return os;
}

}  // namespace model
