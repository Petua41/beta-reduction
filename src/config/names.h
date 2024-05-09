#pragma once

#include <string>
#include <memory>

namespace config
{

class Names {
private:
    bool static ascii_mode_;

public:
    static void Init(bool ascii_mode) {
        ascii_mode_ = ascii_mode;
    }

    static std::string Alpha() {
        if (ascii_mode_) {
            return "Alpha";
        } else {
            return "α";
        }
    }

    static std::string Beta() {
        if (ascii_mode_) {
            return "Beta";
        } else {
            return "β";
        }
    }

    static std::string Lambda() {
        if (ascii_mode_) {
            return "L";
        } else {
            return "λ";
        }
    }
};

}  // namespace config
