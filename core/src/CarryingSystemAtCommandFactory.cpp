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

#include "core/CarryingSystemAtCommandFactory.hpp"

#include <sstream>

namespace ghost
{
namespace core
{
namespace carryingSystem
{

const std::string CarryingSystemAtCommandFactory::mAtCommandPrefix = "AT*";
const std::string CarryingSystemAtCommandFactory::mAtCommandAssignement = "=";
const std::string CarryingSystemAtCommandFactory::mAtCommandSuffix = "\n";

const std::string CarryingSystemAtCommandFactory::mAtCommandLiftHeader = "LIFT";
const std::string CarryingSystemAtCommandFactory::mAtCommandDropHeader = "DROP";
const std::string CarryingSystemAtCommandFactory::mAtCommandAckHeader = "ACK";

const std::string CarryingSystemAtCommandFactory::commandFactory(const std::string& atCmdHeader)
{
    // Command's stream initialisation
    std::ostringstream atCmd;
    atCmd << mAtCommandPrefix << atCmdHeader << mAtCommandSuffix;

    // Get the final AT command
    return atCmd.str();
}

const std::string CarryingSystemAtCommandFactory::liftCommand()
{
    return commandFactory(mAtCommandLiftHeader);
}

const std::string CarryingSystemAtCommandFactory::dropCommand()
{
    return commandFactory(mAtCommandDropHeader);
}

const std::string CarryingSystemAtCommandFactory::determineHeader(const std::string& atCmd)
{
    size_t headerDelimiterPos = atCmd.find(mAtCommandPrefix);
    if (headerDelimiterPos == std::string::npos) {
        // throw smtg
        return "";
    }

    size_t parameterDelimiterPos = atCmd.find(mAtCommandAssignement);
    if (parameterDelimiterPos == std::string::npos) {
        // throw smtg
        return "";
    }

    return atCmd.substr(
        headerDelimiterPos + mAtCommandPrefix.size(),
        parameterDelimiterPos);
}

const std::string CarryingSystemAtCommandFactory::determineParameter(const std::string& atCmd)
{
    size_t parameterDelimiterPos = atCmd.find(mAtCommandAssignement);
    if (parameterDelimiterPos == std::string::npos) {
        // TODO : throw smtg
        return "";
    }

    size_t endDelimiterPos = atCmd.find(mAtCommandSuffix);
    if (endDelimiterPos == std::string::npos) {
        // TODO : throw smtg
        return "";
    }

    return atCmd.substr(
        parameterDelimiterPos + mAtCommandAssignement.size(),
        endDelimiterPos);
}

bool CarryingSystemAtCommandFactory::isAckCommandOf(
    const std::string& atCmd,
    const std::string& commandType)
{
    std::string atCommandHeader = determineHeader(atCmd);

    if (atCommandHeader.compare(mAtCommandAckHeader) != 0) {
        // This is not an Ack Command, so we Ack Nothing
        return false;
    }

    std::string atCmdParameter = determineParameter(atCmd);

    return atCommandHeader.compare(commandType) == 0;
}

} /* carryingSystem namespace */
} /* core namespace  */
} /* ghost namespace */
