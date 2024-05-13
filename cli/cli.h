#pragma once

namespace cli {

class CLI {
private:
    // Arguments:
    bool ascii_mode_ = false;
    unsigned max_operations_ = 100;
    bool use_preprocessor_ = true;

    void ParseCommandLineArguments(int argc, char* argv[]);

public:
    CLI(int argc, char* argv[]) {
        ParseCommandLineArguments(argc, argv);
    }

    int Run() {
        // TODO(senichenkov): implement CLI::Run
        return 0;
    }

    bool ASCIIMode() const {
        return ascii_mode_;
    }

    unsigned MaxOperations() const {
        return max_operations_;
    }

    bool UsePreprocessor() const {
        return use_preprocessor_;
    }

    void PrintHelpAndExit() const;
};

}  // namespace cli
