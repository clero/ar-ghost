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
#include <libDroneMovement/Drone.hpp>

#include <queue>
#include <vector>
#include <string>
#include <memory>

namespace ghost
{
namespace core
{
namespace mission
{
namespace factory
{

/** Factory object of Mission Step */
class MissionStepFactory
{
public:

    /** Available Mission Steps */
    enum MissionStepType {
        LiftParcel = 777,
        DropParcel,
        DroneAngularMovement,
        DroneLand,
        DoNothing
    };

    /** Mission Step container type */
    typedef std::queue<std::shared_ptr<step::MissionStep> > MissionSteps;

    /** Parser result type */
    typedef std::vector<MissionStepType> ParsedMissionSteps;

    /** Class constructor */
    MissionStepFactory(
        utilities::SerialCommunicator& droneSerialCommunicator,
        libDroneMovement::Drone& drone);

    /**
     * Create a new MissionStepContainer from a given json file
     *
     * @param[in] jsonMissionFile the mission json file.
     *
     * @return a queue of shared pointer on generated mission steps
     */
    MissionSteps generateMissionStepsFromFile(const std::string& jsonMissionFile);

private:

    /**
     * Create a new Mission Step
     *
     * @param[in] missionStepType the type of desired MissionStep child.
     *
     * @return a shared pointer to the generated MissionStep object
     */
    std::shared_ptr<step::MissionStep> makeMissionStep(MissionStepType missionStepType);

    utilities::SerialCommunicator& mDroneSerialCommunicator;
    libDroneMovement::Drone& mDrone;

};

} /* factory namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
