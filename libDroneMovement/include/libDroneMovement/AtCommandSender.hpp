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

#include <boost/lockfree/spsc_queue.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <cstdint>
#include <string>
#include <atomic>

namespace ghost
{
namespace libDroneMovement
{
namespace atCommand
{

/**
 *  Active Class which send AtCommands to the drone.
 *  It keeps the communication alive.
 */
class AtCommandSender
{
public:

    /**
     * Class Constructor.
     *
     * @param[in] droneIpAddress the ip address of the drone
     * @param[in] atCommandFactory the factory shared with the main component.
     */
    AtCommandSender(const std::string& droneIpAddress, AtCommandFactory& atCommandFactory);

    /**
     * Add a command to be sent.
     *
     * @param[in] atCommand the command to send
     */
    void addCommandToSend(const std::string atCommand);

    /**
     * TODO:
     * Stop normal sending process to send an urgent command.
     *
     * @param[in] urgentCommand the urgent command to send
     */
    void sendUrgentCommand(const std::string& urgentCommand);

    void start();

    void stop();

    void wait();

private:

    /** Thread callback which call sendingProcess function */
    static void senderThreadCallback(AtCommandSender* atCommandSender);

    /** Function which contains sending methodology */
    void sendingProcess();

    /**
     * Send a message to the At Command server
     *
     * @param[in] atCommand the command to send
     */
    inline void sendToDrone(const std::string& atCommand);


    /** At command server ports */
    static const uint16_t mAtCommandPort;

    /** Internal thread */
    boost::thread mSenderThread;

    /** Network variables */
    boost::asio::io_service mIoService;
    boost::asio::ip::udp::socket mAtCommandSocket;
    boost::asio::ip::udp::endpoint mDroneEndpoint;

    /** Thread safe queue for One producer One consumer */
    boost::lockfree::spsc_queue<std::string> mAtCommandQueue;

    /** Reference to the main AtCommandFactory Object */
    AtCommandFactory& mAtCommandFactory;

    /** Indicate that the thread should quit */
    std::atomic_bool mStopCommandSenderThread;

};

} /* atCommand namespace */
} /* libDroneMovement namespace  */
} /* ghost namespace */
