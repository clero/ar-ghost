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

#include "core/SerialCommunicator.hpp"

#include <boost/asio/serial_port.hpp>
#include <boost/log/trivial.hpp>
#include <boost/bind.hpp>

namespace ghost
{
namespace core
{
namespace utilities
{

SerialCommunicator::SerialCommunicator(const std::string& serialPort, const uint32_t baudRate)
    : mIoService(), mSerialPort(mIoService, serialPort)
{
    mSerialPort.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
    mSerialPort.set_option(
        boost::asio::serial_port_base::flow_control(
            boost::asio::serial_port_base::flow_control::none));
    mSerialPort.set_option(
        boost::asio::serial_port_base::parity(
            boost::asio::serial_port_base::parity::none));
    mSerialPort.set_option(
        boost::asio::serial_port_base::stop_bits(
            boost::asio::serial_port_base::stop_bits::one));
    mSerialPort.set_option(
        boost::asio::serial_port_base::character_size(
            boost::asio::serial_port_base::character_size(8U)));
}

SerialCommunicator::~SerialCommunicator()
{
}

std::string SerialCommunicator::readLine()
{
    mReceivedMessage = "";

    startReceive();
    BOOST_LOG_TRIVIAL(debug) << "Starting reception";
    // Wait Reception
    mIoService.run();

    return mReceivedMessage;
}

void SerialCommunicator::startReceive()
{
    boost::asio::async_read(
        mSerialPort,
        boost::asio::buffer(&mNextChar, 1),
        boost::bind(
            &SerialCommunicator::handleReceive,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void SerialCommunicator::handleReceive(const boost::system::error_code& errorCode, size_t len)
{
    if (!errorCode) {
        if (mNextChar == '\n') {
            // We read a line
            BOOST_LOG_TRIVIAL(debug) << "Receiving from serial port: " << mReceivedMessage;
        } else {
            // We have just a part of the message
            BOOST_LOG_TRIVIAL(debug)
                << "Receiving message fragment: " << mReceivedMessage
                << ", len: " << len;
            mReceivedMessage += mNextChar;

            // Continue the reading
            startReceive();
        }
    } else {
        startReceive();
    }
}

void SerialCommunicator::send(const std::string& message)
{
    BOOST_LOG_TRIVIAL(debug) << "Sending to serial port: " << message;
    boost::asio::write(mSerialPort, boost::asio::buffer(message));
}

} /* utilities namespace */
} /* core namespace  */
} /* ghost namespace */
