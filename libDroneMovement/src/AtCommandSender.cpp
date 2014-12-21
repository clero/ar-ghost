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

#include "libDroneMovement/AtCommandSender.hpp"

#include <boost/log/trivial.hpp>

#include <chrono>
#include <thread>

namespace ghost
{
namespace libDroneMovement
{
namespace atCommand
{

const uint16_t AtCommandSender::mAtCommandPort = 5556;

AtCommandSender::AtCommandSender(const std::string& droneIpAddress,
                                 AtCommandFactory& atCommandFactory)
    : mIoService(), mAtCommandSocket(mIoService, boost::asio::ip::udp::v4()),
    mDroneEndpoint(boost::asio::ip::address::from_string(droneIpAddress), mAtCommandPort),
    mAtCommandQueue(1024), mAtCommandFactory(atCommandFactory), mStopCommandSenderThread(false)
{
}

void AtCommandSender::addCommandToSend(const std::string atCommand)
{
    mAtCommandQueue.push(atCommand);
}

void AtCommandSender::start()
{
    // Internal Thread Initialisation
    mSenderThread = boost::thread(senderThreadCallback, this);
}

void AtCommandSender::stop()
{
    BOOST_LOG_TRIVIAL(info) << "Stopping sending thread";
    mStopCommandSenderThread = true;
}

void AtCommandSender::wait()
{
    BOOST_LOG_TRIVIAL(info) << "Joining sending thread";
    mSenderThread.join();
}

inline void AtCommandSender::sendToDrone(const std::string& atCommand)
{
    mAtCommandSocket.send_to(boost::asio::buffer(atCommand), mDroneEndpoint);
}

void AtCommandSender::senderThreadCallback(AtCommandSender* atCommandSender)
{
    atCommandSender->sendingProcess();
}

void AtCommandSender::sendingProcess()
{
    bool mIsSendingQueueEmpty = true;
    while (!mStopCommandSenderThread || !mIsSendingQueueEmpty) {
        std::string nextAtCommand;

        if (mAtCommandQueue.pop(nextAtCommand)) {
            mIsSendingQueueEmpty = false;
            sendToDrone(nextAtCommand);
            BOOST_LOG_TRIVIAL(info) << "Sending: " << nextAtCommand;
        } else {
            mIsSendingQueueEmpty = true;
            sendToDrone(mAtCommandFactory.hoveringCommand());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    /**
     * FIXME:
     *  Check that the drone is landed.
     */
    BOOST_LOG_TRIVIAL(info) << "Exiting sending thread";
}

} /* atCommand namespace */
} /* libDroneMovement namespace  */
} /* ghost namespace */
