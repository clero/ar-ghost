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
#include "core/MissionStepDroneAngularMovement.hpp"
#include "core/MissionStepDroneLand.hpp"
#include "core/MissionStepDoNothing.hpp"
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
    utilities::SerialCommunicator& droneSerialCommunicator,
    libDroneMovement::Drone& drone)
    : mDroneSerialCommunicator(droneSerialCommunicator), mDrone(drone)
{
}

std::shared_ptr<step::MissionStep> MissionStepFactory::makeMissionStep(
    MissionStepFactory::MissionStepType& missionStepType,
    MissionStepFactory::MissionStepTypeParameters& missionStepTypeParameters)
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
    case MissionStepType::DroneAngularMovement: {
        uint32_t angle = missionStepTypeParameters.front();
        missionStepTypeParameters.erase(missionStepTypeParameters.begin());
        return std::shared_ptr<step::MissionStep>(new step::MissionStepDroneAngularMovement(
                                                      mDrone,
                                                      angle));
    }
    break;
    case MissionStepType::DroneLand:
        return std::shared_ptr<step::MissionStep>(new step::MissionStepDroneLand(mDrone));
        break;
    case MissionStepType::DoNothing:
        return std::shared_ptr<step::MissionStep>(new step::MissionStepDoNothing());
        break;
    }

    // Unreachable, prevent gcc to complain
    throw new std::runtime_error("Unreachable code");
}

MissionStepFactory::MissionSteps MissionStepFactory::generateMissionStepsFromFile(
    const std::string& jsonMissionFile)
{
    // Parse mission file
    MissionParser missionParser = MissionParser();
    ParsedMissionSteps parsedMissionSteps = missionParser.parseMissionFile(jsonMissionFile);
    MissionStepTypeParameters missionStepTypeParameters = missionParser.getParameters();

    // Create steps container
    MissionSteps missionSteps;

    for (MissionStepType type : parsedMissionSteps) {
        // Parser sometimes 0 value to type, we need to avoid that
        if ((int)type != 0) {
            missionSteps.push(makeMissionStep(type, missionStepTypeParameters));
        }
    }

    return missionSteps;
}

} /* factory namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
