#pragma once

#include <easylogging++.h>

namespace config {

// It's not a global variable, because easylogging++ should be configured first
[[nodiscard]] el::Configurations GetConfigurations() {
    el::Configurations conf;

    conf.setToDefault();
    conf.setGlobally(el::ConfigurationType::ToFile, "true");
    conf.setGlobally(el::ConfigurationType::Format, "%msg");
    conf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
    conf.setGlobally(el::ConfigurationType::Filename, "latest.log");

    return conf;
}

}  // namespace config
