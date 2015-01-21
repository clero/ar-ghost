/*
 * Copyright 2014 Jules Cléro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "core/EntryPoint.hpp"

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <stdint.h>
#include <cstdlib>
#include <string>

/** Application configuration */
struct Config {
    std::string ipDrone;
    std::string jsonMissionFile;
    uint32_t flyingStepNumber;
};

void showHelp()
{
    BOOST_LOG_TRIVIAL(info) << "Usage:" << std::endl
                            << "-i <Drone_ip_address> (optional)" << std::endl
                            << "\tSet the drone local internet address. Default value is 127.0.0.1."
                            << std::endl

                            << "-m <Json_mission_file> (optional)" << std::endl
                            << "\tSet the mission file to use to generate the mission." << std::endl

                            << "-s <Step_Number> (optional)" << std::endl
                            << "\tSet the algorithm step number. Default value is 1000."
                            << std::endl;
}

/**
 * Init the sample application configuration from the command line
 * @return true if the supplied parameters are correct
 */
bool initConfigFromCmdLine(int argc, char* argv[], Config& appConfig)
{
    static const char* const optstring = "i:m:s:h";
    int opt;

    while ((opt = getopt(argc, argv, optstring)) != -1) {

        switch (opt) {
        case 'i':
            appConfig.ipDrone = optarg;
            break;
        case 'm':
            appConfig.jsonMissionFile = optarg;
            break;
        case 's':
            try {
                appConfig.flyingStepNumber = std::stoi(optarg);
            }
            catch (std::invalid_argument& e) {
                BOOST_LOG_TRIVIAL(error) << "Invalid value : " << optarg << " for -s option"
                                         << ", the argument should be an integer value."
                                         << std::endl;
                showHelp();
                return false;
            }
            break;
        case 'h':
            showHelp();
            return false;
        default:
            BOOST_LOG_TRIVIAL(error) << "Unsuported option " << (char)opt
                                     << ", the command line is incorrect." << std::endl;
            showHelp();
            return false;
        }
    }

    return true;
}

void initLog()
{
    boost::log::add_file_log("ghost_main.log");
    boost::log::add_console_log(std::cout);
}

int main(int argc, char* argv[])
{
    Config appConfig = {
        "127.0.0.1",
        "",
        1000,
    };

    initLog();

    // Get the config
    if (!initConfigFromCmdLine(argc, argv, appConfig)) {
        return EXIT_FAILURE;
    }

    if (appConfig.jsonMissionFile.compare("") == 0) {
        // No mission file provided
        ghost::core::EntryPoint(appConfig.ipDrone, appConfig.flyingStepNumber).start();
    } else {
        ghost::core::EntryPoint(appConfig.ipDrone,
                                appConfig.flyingStepNumber,
                                appConfig.jsonMissionFile).start();
    }

    return EXIT_SUCCESS;
}
