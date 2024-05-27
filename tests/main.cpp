#include <easylogging++.h>
#include <string>

#include <gtest/gtest.h>

#include "config/easylogging++_config.h"
#include "config/names.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
    el::Loggers::reconfigureAllLoggers(config::GetConfigurations());

    // It's easier to compare strings in ASCII mode in tests:
    config::Names::Instance().SetASCIIMode(true);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
