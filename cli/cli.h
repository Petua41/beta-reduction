#pragma once

#include <boost/optional.hpp>
#include <boost/program_options.hpp>
#include <string>

#include "model/enums/reduction_strategies.h"
#include "strategy_option.h"

namespace cli {

class CLI {
private:
    // boost::program_options objects:
    // Description to print help:
    boost::program_options::options_description visible_desc_{"Possible options"};
    // Description for positional arguments:
    boost::program_options::positional_options_description p_desc_;
    // Description to parse:
    boost::program_options::options_description desc_{"All options"};

    // Paramemeters (with default values):
    bool preprocessor_no_brackets_{false};
    bool preprocessor_no_macros_{false};
    bool ascii_mode_{false};
    unsigned max_operations_{0};
    model::ReductionStrategies strategy_{model::ReductionStrategies::NO};
    std::string term_{""};

    void ParseCommandLineArguments(int argc, char* argv[]);
    void DeclareOptions();
    void PrintHelpAndExit();

public:
    CLI(int argc, char* argv[]) {
        DeclareOptions();
        ParseCommandLineArguments(argc, argv);
    }

    int Run();
};

}  // namespace cli
