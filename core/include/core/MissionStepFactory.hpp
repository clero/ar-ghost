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

class MissionStepFactory
{
public:

    enum MissionStepType {
        LiftParcel = 777,
        DropParcel,
        DroneAngularMovement
    };

    typedef std::queue<std::shared_ptr<step::MissionStep> > MissionSteps;
    typedef std::vector<MissionStepType> ParsedMissionSteps;

    /** Class constructor
     */
    MissionStepFactory(
        utilities::SerialCommunicator& droneSerialCommunicator);

    MissionSteps generateMissionStepsFromFile(const std::string& jsonMissionFile);
    std::shared_ptr<step::MissionStep> makeMissionStep(MissionStepType missionStepType);

private:

    utilities::SerialCommunicator& mDroneSerialCommunicator;

};

} /* factory namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
