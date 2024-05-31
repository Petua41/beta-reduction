#include "cli.h"

#include <boost/program_options.hpp>
#include <easylogging++.h>
#include <iostream>
#include <sstream>

#include "config/easylogging++_config.h"
#include "exceptions/fatal_error.h"
#include "names.h"
#include "parser/string_term.h"
#include "preprocessor/preprocessor.h"
#include "reducer/reducer.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    el::Loggers::reconfigureAllLoggers(config::GetConfigurations());

    START_EASYLOGGINGPP(argc, argv);

    cli::CLI cli{argc, argv};

    return cli.Run();
}

namespace cli {

namespace po = boost::program_options;

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
    if (vm.contains(kLongStrategy)) {
        auto strat_opt = vm[kLongStrategy].as<StrategyOption>();
        strategy_ = strat_opt.strategy_;
    }
    if (vm.contains(kLongTerm)) {
        term_ = vm[kLongTerm].as<std::string>();
    }
}

void CLI::PrintHelpAndExit() {
    std::cout << kGlobalHelpPrefix << visible_desc_ << std::endl;
    exit(EXIT_SUCCESS);
}

int CLI::Run() {
    if (term_ == "") {
        std::cout << "Usage: beta-red [options] term " << std::endl
                  << "See beta-red --help" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        std::string preprocessed{term_};
        if (preprocessor_brackets_) {
            preprocessed = preprocessor::CheckBrackets(preprocessed);
            if (preprocessed != term_) [[likely]] {
                std::cout << "Checked brackets: " << std::endl << '\t' << preprocessed << std::endl;
            }
        }
        if (preprocessor_macros_) {
            preprocessed = preprocessor::ReplaceMacros(preprocessed);
            if (preprocessed != term_) [[likely]] {
                std::cout << "Replaced macros: " << std::endl << '\t' << preprocessed << std::endl;
            }
        }

        parsing::StringTerm string_term_root{std::move(preprocessed)};
        auto root_term = string_term_root.Parse();

        auto free_variables = root_term->GetFreeVariables();
        if (free_variables.size() > 0) {
            std::stringstream sstream;
            sstream << "Free variables: ";
            for (auto const& free_var : free_variables) {
                sstream << free_var << ' ';
            }
            LOG(INFO) << sstream.str();

            auto old_string = root_term->ToString();
            root_term->ReplaceBoundVariables(free_variables);
            auto new_string = root_term->ToString();

            if (new_string != old_string) {
                std::cout << "-Alpha-> " << new_string << std::endl;
            }
        }

        reducer::Reducer reducer{std::move(root_term), strategy_};
        bool normal_form = false;
        while (!normal_form) {
            auto step_result = reducer.Step();
            normal_form = step_result.first;
            if (normal_form) {
                std::cout << "Reached normal form:" << std::endl << step_result.second << std::endl;
            } else {
                std::cout << "-Beta-> " << step_result.second << std::endl;
            }
        }
    } catch (exceptions::FatalError const& e) {
        std::cerr << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

}  // namespace cli
