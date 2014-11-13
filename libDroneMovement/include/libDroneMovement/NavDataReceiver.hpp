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

#include "libDroneMovement/NavDataParser.hpp"

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <cstdint>
#include <string>

namespace ghost
{
namespace libDroneMovement
{
namespace navData
{

/**
 * Active Class which receive NavData packets from the drone.
 */
class NavDataReceiver
{

public:

    /**
     * Class Constructor.
     *
     * @param[in] droneIpAddress the drone ip address
     * @param[in] navDataParser object able to decode raw navdata packet
     */
    NavDataReceiver(const std::string& droneIpAddress, NavDataParser& navDataParser);

    /** Allows to see if the NavData thread is running correctly */
    bool isSetUp();

    void stop();
    void wait();

private:

    /**
     * Thread callback which call receivingProcess function
     *
     * @param[in] the pointer to the current NavDataReceiver object.
     */
    static void receiverThreadCallback(NavDataReceiver* navDataReceiver);

    /** Start the asynchronous reception */
    void startReceive();

    /** Callback function to handle a new packet reception */
    void handleReceive(const boost::system::error_code& errorCode, size_t len);

    /**
     * Set up a Port Nat using iptables
     *
     * Parrot firmware is using the same port for emission and reception for NavData
     * To perform localhost reception we have to use a Nat.
     *
     * @param[in] droneIpAddress the drone ip address
     */
    void initNat(const std::string& droneIpAddress);

    /** NavData ports */
    static const uint16_t mNavDataPort;
    static const uint16_t mNatNavDataPort;

    /** Internal thread */
    boost::thread mReceiverThread;

    /** Network variables */
    boost::asio::io_service mIoService;
    boost::asio::ip::udp::endpoint mDroneEndpoint;
    boost::asio::ip::udp::endpoint mLocalEndpoint;
    boost::asio::ip::udp::socket mNavDataSocket;

    /** Ref to the main NavDataParser object */
    NavDataParser& mNavDataParser;

    /** Holds a packet NavDataPacket */
    NavDataParser::RawNavDataPacket mRawNavDataPacket;

    /** NavDataReceiver State */
    bool mNavDataReceived;
};

} /* navData namespace  */
} /* libDroneMovement namespace  */
} /* ghost namespace */
