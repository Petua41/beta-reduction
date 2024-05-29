#include <easylogging++.h>
#include <string>

#include <gtest/gtest.h>

#include "config/easylogging++_config.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
    el::Loggers::reconfigureAllLoggers(config::GetConfigurations());

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
