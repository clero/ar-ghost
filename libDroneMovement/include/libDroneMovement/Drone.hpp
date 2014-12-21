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

#pragma once

#include "libDroneMovement/AtCommandFactory.hpp"
#include "libDroneMovement/AtCommandSender.hpp"
#include "libDroneMovement/NavDataReceiver.hpp"
#include "libDroneMovement/NavDataParser.hpp"

#include <string>

namespace ghost
{
namespace libDroneMovement
{

class Drone
{
public:

    /**
     * Class Constructor
     *
     * @param[in] droneIpAddress the ip address of the drone
     */
    Drone(const std::string& droneIpAddress);

    /** Class Destructor */
    ~Drone();

    void takeOff();
    void land();
    void hover();
    void movement(float roll, float pitch, float gaz, float yaw);
    void movement(float roll, float pitch, float gaz);

    /**
     * Edit drone current configuration
     *
     * @param[in] argA parrot configuration parameter
     * @param[in] argB parrot configuration parameter
     */
    void setConfiguration(const std::string& argA, const std::string& argB);
    void setEmergencyMode();
    void resetWatchdog();

    int getAltitude();
    float getPhiAngle();
    float getPsiAngle();
    float getThetaAngle();

private:

    /** AtCommand Factory shared with mAtCommandSender member */
    atCommand::AtCommandFactory mAtCommandFactory;

    /** Active object which send At Commands to drone internal program */
    atCommand::AtCommandSender mAtCommandSender;

    /** NavData parser shared with the mNavDataReceiver member */
    navData::NavDataParser mNavDataParser;

    /** Active object which receive NavData from the drone internal program */
    navData::NavDataReceiver mNavDataReceiver;

};

} /* libDroneMovement namespace  */
} /* ghost namespace */
