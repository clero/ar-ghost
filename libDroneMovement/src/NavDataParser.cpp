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

#include "libDroneMovement/NavDataParser.hpp"

#include <boost/log/trivial.hpp>

#include <iostream>
#include <cstring>

namespace ghost
{
namespace libDroneMovement
{
namespace navData
{

const uint32_t NavDataParser::mNavDataHeader = 0x55667788;

NavDataParser::NavDataParser()
    : mSequenceNumber(0), mIsFlying(false), mIsControlCommandAck(false),
    mCurrentNavDataDemoPacket(),
    mCurrentNavDataDemoMutex(), mIsControlCommandAckCheck(), mControlCommandAckMutex()
{
}

bool NavDataParser::parseNavDataPacket(const RawNavDataPacket& rawNavDataPacket)
{
    NavDataPacketHeader navDataPacketHeader = {
        0, 0, 0, 0, 0, 0
    };

    // Parse the packet
    std::memcpy(&navDataPacketHeader, rawNavDataPacket.data(), sizeof(NavDataPacketHeader));

    // Header check
    if (navDataPacketHeader.header != mNavDataHeader) {
        BOOST_LOG_TRIVIAL(error) << "Bad NavData header";
        return false;
    }

    if (mSequenceNumber >= navDataPacketHeader.sequence) {
        BOOST_LOG_TRIVIAL(warning) << "Discarding old NavData packet";
        return false;
    }

    // Update internal NavData packet
    mSequenceNumber = navDataPacketHeader.sequence;

    // Update internal state
    setNewState(navDataPacketHeader.state);

    // Check Option
    switch (navDataPacketHeader.tag) {
    case NavDataOptionTag::DemoTag: {
        std::unique_lock<std::mutex> locker(mCurrentNavDataDemoMutex);
        std::memcpy(
            &mCurrentNavDataDemoPacket,
            rawNavDataPacket.data() + sizeof(NavDataPacketHeader),
            sizeof(NavDataDemoPacket));
        return true;
    }
    case NavDataOptionTag::CksTag:
        // We are working on localhost, no need to check Checksum
        return true;
    }

    BOOST_LOG_TRIVIAL(error) << "Unknown NavData option";
    return false;
}

NavDataParser::NavDataDemoPacket NavDataParser::getCurrentNavData()
{
    std::unique_lock<std::mutex> locker(mCurrentNavDataDemoMutex);
    return mCurrentNavDataDemoPacket;
}

inline bool NavDataParser::checkState(uint32_t state, DroneState fieldToCheck)
{
    return (state & fieldToCheck) != 0;
}

void NavDataParser::setNewState(uint32_t state)
{
    mIsFlying = checkState(state, DroneState::Fly);

    mIsControlCommandAck = checkState(state, DroneState::ControlCommandAck);
    mIsControlCommandAckCheck.notify_all();
}

bool NavDataParser::isDroneFlying()
{
    return mIsFlying;
}

bool NavDataParser::isControlCommandAck()
{
    return mIsControlCommandAck;
}

void NavDataParser::waitControlCommandAck()
{
    std::unique_lock<std::mutex> locker(mControlCommandAckMutex);

    // Block until Command ack is received
    BOOST_LOG_TRIVIAL(debug) << "Waiting Control Command";
    mIsControlCommandAckCheck.wait(locker, [this]() {
                    return this->isControlCommandAck();
                });
}

void NavDataParser::waitNotControlCommandAck()
{
    std::unique_lock<std::mutex> locker(mControlCommandAckMutex);

    // Block until Command ack is unset
    BOOST_LOG_TRIVIAL(debug) << "Waiting Control Command reset";
    mIsControlCommandAckCheck.wait(locker, [this]() {
                    return !this->isControlCommandAck();
                });
}

} /* navData namespace  */
} /* libDroneMovement namespace  */
} /* ghost namespace */
