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

#include "core/CarryingSystemAtCommandFactory.hpp"
#include "core/SerialCommunicator.hpp"

#include <chrono>
#include <thread>
#include <iostream>
#include <string>

/**
 * Basic functional test which verify the effective
 * communication between the drone and the carrying
 * system through the debug serial interface.
 */
int main(void)
{
    using namespace ghost::core::carryingSystem;
    using namespace ghost::core::utilities;

    std::string port = "/dev/ttyO3";
    uint32_t baudRate = 115200;

    SerialCommunicator sComm(port, baudRate);

    // Wait the Arduino to reboot
    std::this_thread::sleep_for(std::chrono::seconds(2));

    sComm.send(CarryingSystemAtCommandFactory::liftCommand());
    std::cout << "Sent: " << CarryingSystemAtCommandFactory::liftCommand() << std::endl;

    std::string ack = sComm.readLine();
    std::cout << "Received: " << ack << std::endl;

    if (CarryingSystemAtCommandFactory::isAckCommandOf(ack, "LIFT")) {
        std::cout << "Valid ACK Received" << std::endl;
    }
}
