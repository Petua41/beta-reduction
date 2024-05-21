#include "cli/cli.h"

#include <easylogging++.h>
#include <iostream>

#include "config/misc.h"
#include "config/names.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    START_EASYLOGGINGPP(argc, argv);

    cli::CLI cli{argc, argv};

    config::Names::Instance().SetASCIIMode(cli.ASCIIMode());
    config::misc::max_operations = cli.MaxOperations();
    config::misc::use_preprocessor = cli.UsePreprocessor();

    std::cout << "Executing..." << std::endl;
    auto error_code = cli.Run();

    return error_code;
}

namespace cli {

void CLI::ParseCommandLineArguments(int argc, char* argv[]) {
    bool read_max_op = false;
    for (int i{1}; i < argc; ++i) {  // skip executable name
        std::string arg{argv[i]};
        if (read_max_op) {
            try {
                max_operations_ = std::stoul(arg);
            } catch (std::invalid_argument const& e) {
                std::cout << "Max operations number must be a positive integer";
            }
        } else if (arg == "-h" || arg == "--help") {
            PrintHelpAndExit();
        } else if (arg == "-a" || arg == "--ascii_mode") {
            ascii_mode_ = true;
        } else if (arg == "-m" || arg == "--max_operations") {
            read_max_op = true;  // Next option whould be N
        } else if (arg.starts_with("-m")) {
            try {
                max_operations_ = std::stoul(arg.substr(2));
            } catch (std::invalid_argument const& e) {
                std::cout << "Max operations number must be a positive integer";
            }
        } else if (arg.starts_with("--max_operations")) {
            try {
                max_operations_ = std::stoul(arg.substr(16));
            } catch (std::invalid_argument const& e) {
                std::cout << "Max operations number must be a positive integer";
            }
        } else if (arg == "-r" || arg == "--raw") {
            use_preprocessor_ = false;
        } else {
            std::cout << "Unknown argument: " << arg << std::endl;
        }
    }
}

void CLI::PrintHelpAndExit() const {
    std::cout << config::misc::kGlobalHelp;
    exit(0);
}

}  // namespace cli
