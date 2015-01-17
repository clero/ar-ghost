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

#include "core/Mission.hpp"
#include "core/SerialCommunicator.hpp"

extern "C"
{
/** LibLineFollowing */
#include "SystemDrone.h"
}

#include <libDroneMovement/Drone.hpp>
#include <libDroneVideo/FrameGrabber.hpp>

#include <string>

namespace ghost
{
namespace core
{

/** Entry Point object launched by the main program */
class EntryPoint
{
public:

    /**
     * Class constructor
     *
     * @param[in] droneIpAdress the drone ip address
     * @param[in] flyingStepNumber the number of step the algorithm should realize.
     *            Each step are approximatively 30ms long
     */
    EntryPoint(const std::string& droneIpAdress, const unsigned int flyingStepNumber);
    EntryPoint(
        const std::string& droneIpAdress,
        const unsigned int flyingStepNumber,
        const std::string& jsonMissionFile);

    /** Launch the algorithm */
    void start();

private:

    /** Tty to interact with the embeded carrying system */
    static const std::string mCarryingSystemTty;

    /** Baudrate of the serial connection of the carrying system */
    static const uint32_t mCarryingSystemTtyBaudRate;

    /** Check if we need a new image */
    inline bool needImageUpdate() const;

    /** Check if the line is lost or not */
    inline bool isLineFound() const;

    /** Run a new step of the line following algorithm */
    void updateAutoPilotInputWithFrame();

    /** Do the calculated movement */
    void doNextAction();

    libDroneMovement::Drone mDrone;
    libDroneVideo::FrameGrabber mFrameGrabber;
    utilities::SerialCommunicator mCarryingSystemSerialCommunicator;
    mission::Mission mMission;

    unsigned int mFlyingStepNumber;
    unsigned int mCurrentFlyingStep;
    unsigned int mLineLostCounter;

    /** Used in order to avoid double mission trigger detection */
    unsigned int mForgetMissionCounter;

    /** Scade Auto Pilot state Machine variables */
    inC_SystemDrone mInputAutoPilot;
    outC_SystemDrone mOutputAutoPilot;
};

} /* core namespace  */
} /* ghost namespace */
