#include <easylogging++.h>
#include <string>

#include <gtest/gtest.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
    // TODO(senichenkov): remove hotfix with easylogging++ configuration:
    if (std::string{get_current_dir_name()}.find("target") == std::string::npos) {
        el::Loggers::configureFromGlobal("build/target/logging.conf");
    } else {
        el::Loggers::configureFromGlobal("logging.conf");
    }

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
