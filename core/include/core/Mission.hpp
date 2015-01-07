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
#include "core/MissionStepFactory.hpp"
#include "core/SerialCommunicator.hpp"

#include <string>

namespace ghost
{
namespace core
{
namespace mission
{

/**
 * Container class of MissionStep.
 * This class allows to store and to run some mission steps.
 */
class Mission
{
public:

    /**
     * Class constructor.
     *
     * Create an empty mission.
     */
    Mission();
    Mission(const std::string& jsonMissionFile,
            utilities::SerialCommunicator& carryingSystemSerialCommunicator);
    Mission(const std::string& jsonMissionFile);

    /** Check if we have other mission step to do */
    bool isDone() const;

    /** Retrieve the next Action to do */
    void doNextStep();

private:

    /** MissionStep container */
    factory::MissionStepFactory::MissionSteps mMissionSteps;

};

} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
