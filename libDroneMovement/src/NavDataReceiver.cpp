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

#include "libDroneMovement/NavDataReceiver.hpp"

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include <array>
#include <sstream>

namespace ghost
{
namespace libDroneMovement
{
namespace navData
{

const uint16_t NavDataReceiver::mNavDataPort = 5554;
const uint16_t NavDataReceiver::mNatNavDataPort = 15554;

NavDataReceiver::NavDataReceiver(const std::string& droneIpAddress, NavDataParser& navDataParser)
    : mIoService(),
    mDroneEndpoint(boost::asio::ip::address::from_string(droneIpAddress), mNavDataPort),
    mLocalEndpoint(boost::asio::ip::address::from_string(droneIpAddress), mNatNavDataPort),
    mNavDataSocket(mIoService, mLocalEndpoint), mNavDataParser(navDataParser),
    mRawNavDataPacket(), mNavDataReceived(false)
{
    // NavData request
    const int8_t activationData[4] = {
        0x01, 0x00, 0x00, 0x00
    };
    mNavDataSocket.send_to(boost::asio::buffer(activationData), mDroneEndpoint);

    // Set up NAT to be able to receive navdata in localhost
    initNat(droneIpAddress);

    // Begin reception
    startReceive();

    mReceiverThread = boost::thread(receiverThreadCallback, this);
}

void NavDataReceiver::initNat(const std::string& droneIpAddress)
{
    system("iptables -t nat -F");

    std::ostringstream natRule;
    natRule << "iptables -t nat -A POSTROUTING -p UDP --sport "
            << mNatNavDataPort
            << " -j SNAT --to "
            << droneIpAddress
            << ":"
            << mNavDataPort;

    system(natRule.str().c_str());

    natRule.str("");
    natRule.clear();

    natRule << "iptables -t nat -A PREROUTING -p UDP -d "
            << droneIpAddress
            << " --dport "
            << mNavDataPort
            << " -j DNAT --to "
            << droneIpAddress
            << ":" << mNatNavDataPort;

    system(natRule.str().c_str());
}

void NavDataReceiver::startReceive()
{
    mNavDataSocket.async_receive_from(
        boost::asio::buffer(mRawNavDataPacket),
        mDroneEndpoint,
        boost::bind(
            &NavDataReceiver::handleReceive,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void NavDataReceiver::handleReceive(const boost::system::error_code& errorCode, size_t len)
{
    if (!errorCode) {

        // NavData setup is working
        mNavDataReceived = true;

        // Parse
        if (!mNavDataParser.parseNavDataPacket(mRawNavDataPacket)) {
            BOOST_LOG_TRIVIAL(error) << "While parsing NavPacket of size: " << len;
        }
    } else {
        BOOST_LOG_TRIVIAL(error) << "While receiving NavPacket: " << errorCode;
    }

    startReceive();
}

bool NavDataReceiver::isSetUp()
{
    return mNavDataReceived;
}

void NavDataReceiver::stop()
{
    BOOST_LOG_TRIVIAL(info) << "Stopping receiving thread";
    mIoService.stop();
}

void NavDataReceiver::wait()
{
    BOOST_LOG_TRIVIAL(info) << "Waiting receiving thread";
    mReceiverThread.join();
}

void NavDataReceiver::receiverThreadCallback(NavDataReceiver* navDataReceiver)
{
    navDataReceiver->mIoService.run();
}

} /* navData namespace  */
} /* libDroneMovement namespace  */
} /* ghost namespace */
