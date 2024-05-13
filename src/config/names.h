#pragma once

#include <memory>
#include <string>

namespace config {

class Names {
private:
    bool ascii_mode_;

    Names() {}

public:
    static Names& Instance() {
        static Names instance;
        return instance;
    }

    void SetASCIIMode(bool ascii_mode) {
        ascii_mode_ = ascii_mode;
    }

    std::string Alpha() {
        if (ascii_mode_) {
            return "Alpha";
        } else {
            return "α";
        }
    }

    std::string Beta() {
        if (ascii_mode_) {
            return "Beta";
        } else {
            return "β";
        }
    }

    std::string Lambda() {
        if (ascii_mode_) {
            return "L";
        } else {
            return "λ";
        }
    }
};

}  // namespace config
