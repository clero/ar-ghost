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

#include "core/MissionStepFactory.hpp"
#include "core/MissionParser.hpp"
#include "core/MissionStepSendCarryingSystemCommand.hpp"
#include "core/CarryingSystemAtCommandFactory.hpp"

#include <queue>
#include <stdexcept>

namespace ghost
{
namespace core
{
namespace mission
{
namespace factory
{

MissionStepFactory::MissionStepFactory(
    utilities::SerialCommunicator& droneSerialCommunicator)
    : mDroneSerialCommunicator(droneSerialCommunicator)
{
}

std::shared_ptr<step::MissionStep> MissionStepFactory::makeMissionStep(
    MissionStepFactory::MissionStepType missionStepType)
{
    switch (missionStepType) {
    case MissionStepType::LiftParcel:
        return std::shared_ptr<step::MissionStep>(new step::MissionStepSendCarryingSystemCommand(
                                                      mDroneSerialCommunicator,
                                                      carryingSystem::CarryingSystemAtCommandFactory
                                                      ::liftCommand()));
        break;
    case MissionStepType::DropParcel:
        return std::shared_ptr<step::MissionStep>(new step::MissionStepSendCarryingSystemCommand(
                                                      mDroneSerialCommunicator,
                                                      carryingSystem::CarryingSystemAtCommandFactory
                                                      ::dropCommand()));
        break;
    case MissionStepType::DroneAngularMovement:
        // TODO
        return std::shared_ptr<step::MissionStep>(new step::MissionStepSendCarryingSystemCommand(
                                                      mDroneSerialCommunicator,
                                                      carryingSystem::CarryingSystemAtCommandFactory
                                                      ::liftCommand()));
        break;
    }

    // Unreachable, prevent gcc to complain
    throw new std::runtime_error("Unreachable code");
}

MissionStepFactory::MissionSteps MissionStepFactory::generateMissionStepsFromFile(
    const std::string& jsonMissionFile)
{
    // Parse mission file
    ParsedMissionSteps parsedMissionSteps = MissionParser().parseMissionFile(jsonMissionFile);

    // Create steps container
    MissionSteps missionSteps;

    for (MissionStepType type : parsedMissionSteps) {
        // Parser sometimes 0 value to type, we need to avoid that
        if ((int)type != 0) {
            missionSteps.push(makeMissionStep(type));
        }
    }

    return missionSteps;
}

} /* factory namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
