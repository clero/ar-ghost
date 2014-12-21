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

#include <string>

namespace ghost
{
namespace core
{
namespace carryingSystem
{

/** Factory of At Command used to communicate with the Arduino */
class CarryingSystemAtCommandFactory
{

public:

    /** Ackable Commands */
    enum AtCommandReceivable {
        LIFTED = 666,
        DROPED,
        ACK
    };

    /** Generate the AT Command allowing the carrying system to lift the packet */
    static const std::string liftCommand();

    /** Generate the AT Command allowing the carrying system to drop the packet */
    static const std::string dropCommand();

    /** Test if a received AT Commands try to ack an other one */
    static bool isAckCommandOf(const std::string& atCmd, const std::string& commandType);

private:

    /** At Command parts */
    static const std::string mAtCommandPrefix;
    static const std::string mAtCommandAssignement;
    static const std::string mAtCommandSuffix;

    /** Available At Commands */
    static const std::string mAtCommandDropHeader;
    static const std::string mAtCommandLiftHeader;
    static const std::string mAtCommandAckHeader;

    /** General method which allows to create an AT Command without arguments. */
    static const std::string commandFactory(const std::string& atCmdHeader);

    /** General method which allows to create an AT Command with one argument. */
    // const std::string commandFactory(const std::string& atCmdHeader, const std::string& atCmdParameter);

    static const std::string determineParameter(const std::string& atCmd);
    static const std::string determineHeader(const std::string& atCmd);

};

} /* carryingSystem namespace */
} /* core namespace  */
} /* ghost namespace */
