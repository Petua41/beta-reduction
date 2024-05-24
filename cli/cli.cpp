#include "cli/cli.h"

#include <boost/program_options.hpp>
#include <easylogging++.h>
#include <iostream>

#include "config/misc.h"
#include "config/names.h"
#include "option_descriptions.h"
#include "option_names.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    START_EASYLOGGINGPP(argc, argv);

    cli::CLI cli{argc, argv};

    return cli.Run();
}

namespace cli {

namespace po = boost::program_options;
using namespace names;
using namespace descriptions;

void CLI::DeclareOptions() {
    // Declare option groups:
    // clang-format off
    po::options_description preprocessor{"Preprocessor options"};
    preprocessor.add_options()
        (kNoBrackets, kdNoBrackets)
        (kNoMacros, kdNoMacros)
        (kRaw, kdRaw)
    ;

    po::options_description config{"Configuration options"};
    config.add_options()
        (kASCII, kdASCII)
        (kMaxOp, po::value<unsigned>(), kdMaxOp)
        (kStrategy, po::value<StrategyOption>(), kdStrategy)
    ;

    po::options_description generic{"Generic options"};
    generic.add_options()
        (kHelp, kdHelp)
    ;

    po::options_description hidden{"Hidden options"};
    hidden.add_options()
        (kLongTerm, po::value<std::string>(), kdTerm)
    ;
    // clang-format on

    // Add positional options group:
    p_desc_.add(kLongTerm, -1);

    // Add option groups to description objects:
    visible_desc_.add(preprocessor).add(config).add(generic);
    desc_.add(visible_desc_).add(hidden);
}

void CLI::ParseCommandLineArguments(int argc, char* argv[]) {
    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(desc_).positional(p_desc_).run(), vm);
        po::notify(vm);
    } catch (po::invalid_command_line_syntax const& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        exit(EXIT_FAILURE);
    } catch (po::validation_error const& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        exit(EXIT_FAILURE);
    }

    if (vm.contains(kLongHelp)) {
        PrintHelpAndExit();
    }

    if (vm.contains(kLongNoBrackets)) {
        preprocessor_no_brackets_ = true;
    }
    if (vm.contains(kLongNoMacros)) {
        preprocessor_no_macros_ = true;
    }
    if (vm.contains(kLongRaw)) {
        preprocessor_no_brackets_ = true;
        preprocessor_no_macros_ = true;
    }
    if (vm.contains(kLongASCII)) {
        ascii_mode_ = true;
    }
    if (vm.contains(kLongMaxOp)) {
        max_operations_ = vm[kLongMaxOp].as<unsigned>();
    }
    if (vm.contains(kLongStrategy)) {
        auto strat_opt = vm[kLongStrategy].as<StrategyOption>();
        strategy_ = strat_opt.strategy_;
    }
    if (vm.contains(kLongTerm)) {
        term_ = vm[kLongTerm].as<std::string>();
    }
}

void CLI::PrintHelpAndExit() {
    std::cout << config::misc::kGlobalHelpPrefix << visible_desc_ << '\n';
    exit(EXIT_SUCCESS);
}

int CLI::Run() {
    // TODO(senichenkov): implement CLI::Run

    return EXIT_SUCCESS;
}

}  // namespace cli
