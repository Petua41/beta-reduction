#include "cli.h"

#include <boost/program_options.hpp>
#include <easylogging++.h>
#include <iostream>

#include "config/easylogging++_config.h"
#include "config/misc.h"
#include "config/names.h"
#include "exceptions/fatal_error.h"
#include "option_descriptions.h"
#include "option_names.h"
#include "parser/parser.h"
#include "preprocessor/preprocessor.h"
#include "strategy/reducer.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    el::Loggers::reconfigureAllLoggers(config::GetConfigurations());

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
        (kNoBrackets, kDNoBrackets)
        (kNoMacros, kDNoMacros)
        (kRaw, kDRaw)
    ;

    po::options_description config{"Configuration options"};
    config.add_options()
        (kASCII, kDASCII)
        (kMaxOp, po::value<unsigned>(), kDMaxOp)
        (kStrategy, po::value<StrategyOption>(), kDStrategy)
    ;

    po::options_description generic{"Generic options"};
    generic.add_options()
        (kHelp, kDHelp)
    ;

    po::options_description hidden{"Hidden options"};
    hidden.add_options()
        (kLongTerm, po::value<std::string>(), kDTerm)
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
    } catch (po::error const& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (vm.contains(kLongHelp)) {
        PrintHelpAndExit();
    }

    if (vm.contains(kLongNoBrackets)) {
        preprocessor_brackets_ = false;
    }
    if (vm.contains(kLongNoMacros)) {
        preprocessor_macros_ = false;
    }
    if (vm.contains(kLongRaw)) {
        preprocessor_brackets_ = false;
        preprocessor_macros_ = false;
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
    std::cout << config::misc::kGlobalHelpPrefix << visible_desc_ << std::endl;
    exit(EXIT_SUCCESS);
}

int CLI::Run() {
    if (term_ == "") {
        std::cout << "Usage: beta-red [options] term " << std::endl
                  << "See beta-red --help" << std::endl;
        return EXIT_FAILURE;
    }

    config::Names::Instance().SetASCIIMode(ascii_mode_);

    try {
        preprocessor::Preprocessor prep{term_, preprocessor_brackets_, preprocessor_macros_};
        auto preprocessed_input = prep.Preprocess();

        if (preprocessed_input != term_) {
            std::cout << "Checked brackets and replaced macros: " << std::endl
                      << '\t' << preprocessed_input << std::endl;
        }

        parsing::Parser parser{preprocessed_input};
        auto root_term = parser.Parse();

        strategy::Reducer reducer{std::move(root_term), strategy_, max_operations_};
        auto [result_string, exit_status] = reducer.MainLoop();
        switch (exit_status) {
            case model::ReductionExitStatus::NormalForm:
                std::cout << "Reached normal form:" << std::endl
                          << '\t' << result_string << std::endl;
                break;
            case model::ReductionExitStatus::Loop:
                std::cout << "Entered loop. First term in loop is:" << std::endl
                          << '\t' << result_string << std::endl;
                break;
            case model::ReductionExitStatus::TooManyOperations:
                std::cout << "Too much operations (>= " << max_operations_
                          << "). The last term was:" << std::endl
                          << '\t' << result_string << std::endl;
                break;
        }
    } catch (exceptions::FatalError const& e) {
        std::cerr << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

}  // namespace cli
