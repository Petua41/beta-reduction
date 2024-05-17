#include <easylogging++.h>
#include <string>

#include <gtest/gtest.h>

#include "config/names.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
    // TODO(senichenkov): remove hotfix with easylogging++ configuration:
    if (std::string{get_current_dir_name()}.find("target") == std::string::npos) {
        el::Loggers::configureFromGlobal("build/target/logging.conf");
    } else {
        el::Loggers::configureFromGlobal("logging.conf");
    }

    // It's easier to compare strings in ASCII mode in tests:
    config::Names::Instance().SetASCIIMode(true);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
