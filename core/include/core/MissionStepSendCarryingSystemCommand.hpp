/*
 * Copyright 2015 Jules Cléro
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

#pragma once

#include "core/MissionStep.hpp"
#include "core/SerialCommunicator.hpp"

#include <string>

namespace ghost
{
namespace core
{
namespace mission
{
namespace step
{

/** Mission step which allows to send a command to the carrying system */
class MissionStepSendCarryingSystemCommand : public MissionStep
{
public:

    /**
     * Class constructor
     *
     * @param[in] droneSerialCommunicator the reference to the serial communicator object used to
     * send commands to the carrying system.
     * @param[in] commandToSend the command to send
     */
    MissionStepSendCarryingSystemCommand(
        utilities::SerialCommunicator& droneSerialCommunicator,
        const std::string& commandToSend);
    ~MissionStepSendCarryingSystemCommand();

    void doAction();

private:

    utilities::SerialCommunicator& mDroneSerialCommunicator;
    const std::string mCommandToSend;

};

} /* step namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
