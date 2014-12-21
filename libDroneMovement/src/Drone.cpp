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

#include "libDroneMovement/Drone.hpp"

#include <cstdint>
#include <chrono>
#include <thread>
#include <exception>

namespace ghost
{
namespace libDroneMovement
{

Drone::Drone(const std::string& droneIpAddress)
    : mAtCommandFactory(), mAtCommandSender(droneIpAddress, mAtCommandFactory),
    mNavDataParser(), mNavDataReceiver(droneIpAddress, mNavDataParser)
{

    /**
     * TODO: Synchronise NavData Thread
     */

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (mNavDataReceiver.isSetUp()) {
        mAtCommandSender.start();

        // Initialize horizontal plane
        mAtCommandSender.addCommandToSend(mAtCommandFactory.horizontalPlaneCalibrationCommand());
        std::this_thread::sleep_for(std::chrono::seconds(3));


        if (mNavDataParser.isControlCommandAck()) {

            mAtCommandSender.addCommandToSend(mAtCommandFactory.ackConfigureCommand());

            mNavDataParser.waitNotControlCommandAck();
        }

        // Set Drone Max altitude
        setConfiguration("control:altitude_max", "2000");

        // Initialize NavData Sending
        setConfiguration("general:navdata_demo", "TRUE");
    } else {
        throw std::runtime_error("NavData service unavailable");
    }
}

Drone::~Drone()
{
    mNavDataReceiver.stop();
    mNavDataReceiver.wait();
    mAtCommandSender.stop();
    mAtCommandSender.wait();
}

void Drone::takeOff()
{
    mAtCommandSender.addCommandToSend(mAtCommandFactory.takeOffCommand());

    /**
     * Wait that the drone take off.
     * We can't use NavData to detect if the take off has ended because
     * the fly bit state is set as soon as the drone receive the take-off
     * AT command.
     */
    std::this_thread::sleep_for(std::chrono::seconds(4));
}

void Drone::land()
{
    mAtCommandSender.addCommandToSend(mAtCommandFactory.landCommand());

    /**
     * Wait that the drone land.
     * We can't use NavData to detect if the land has ended because
     * the fly bit state is unset as soon as the drone receive the land
     * AT command.
     */
    std::this_thread::sleep_for(std::chrono::seconds(4));
}

void Drone::hover()
{
    mAtCommandSender.addCommandToSend(mAtCommandFactory.hoveringCommand());
}

void Drone::movement(float roll, float pitch, float gaz, float yaw)
{
    mAtCommandSender.addCommandToSend(mAtCommandFactory.movementCommand(roll, pitch, gaz, yaw));
}

void Drone::movement(float roll, float pitch, float gaz)
{
    mAtCommandSender.addCommandToSend(mAtCommandFactory.movementCommand(roll, pitch, gaz));
}

void Drone::setConfiguration(const std::string& argA, const std::string& argB)
{
    // Send command
    mAtCommandSender.addCommandToSend(mAtCommandFactory.configureCommand(argA, argB));

    // Wait Drone's ACK
    mNavDataParser.waitControlCommandAck();

    // Reset Drone's ACK Bit
    mAtCommandSender.addCommandToSend(mAtCommandFactory.ackConfigureCommand());

    // Wait Reset
    mNavDataParser.waitNotControlCommandAck();
}

void Drone::setEmergencyMode()
{
    mAtCommandSender.addCommandToSend(mAtCommandFactory.emergencyCommand());
}

int Drone::getAltitude()
{
    return mNavDataParser.getCurrentNavData().altitude;
}

float Drone::getPhiAngle()
{
    return mNavDataParser.getCurrentNavData().phi;
}

float Drone::getThetaAngle()
{
    return mNavDataParser.getCurrentNavData().theta;
}

float Drone::getPsiAngle()
{
    return mNavDataParser.getCurrentNavData().psi;
}

} /* libDroneMovement namespace  */
} /* ghost namespace */
