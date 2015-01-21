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

#include <boost/asio.hpp>

#include <string>
#include <cstdint>

namespace ghost
{
namespace core
{
namespace utilities
{

/** Basic Serial communication object */
class SerialCommunicator
{
public:

    /**
     * Class constructor
     *
     * @param[in] serialPort the tty to open
     * @param[in] baudRate the baudrate to use
     */
    SerialCommunicator(const std::string& serialPort, const uint32_t baudRate);
    ~SerialCommunicator();

    /**
     * Read next line
     *
     * @return the readed line
     */
    std::string readLine();

    /**
     * Send a string through the opened serial port
     *
     * @param[in] message the message to send
     */
    void send(const std::string& message);

private:

    void startReceive();
    void handleReceive(const boost::system::error_code& errorCode, size_t len);

    boost::asio::io_service mIoService;
    boost::asio::serial_port mSerialPort;

    char mNextChar;
    std::string mReceivedMessage;

};

} /* utilities namespace */
} /* core namespace  */
} /* ghost namespace */
