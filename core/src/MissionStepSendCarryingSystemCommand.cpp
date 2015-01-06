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

#include "core/MissionStepSendCarryingSystemCommand.hpp"

namespace ghost
{
namespace core
{
namespace mission
{
namespace step
{

MissionStepSendCarryingSystemCommand::MissionStepSendCarryingSystemCommand(
    utilities::SerialCommunicator& droneSerialCommunicator,
    const std::string& commandToSend)
    : mDroneSerialCommunicator(droneSerialCommunicator), mCommandToSend(commandToSend)
{
}

MissionStepSendCarryingSystemCommand::~MissionStepSendCarryingSystemCommand()
{
}

void MissionStepSendCarryingSystemCommand::doAction()
{
    mDroneSerialCommunicator.send(mCommandToSend);

    /** Wait smt, ack end of lift etc */
}

} /* step namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
