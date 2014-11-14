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

#include "libDroneMovement/AtCommandFactory.hpp"

#include <stdexcept>

namespace ghost
{
namespace libDroneMovement
{

AtCommandFactory::AtCommandFactory() : mSequence(1)
{
}

const std::string AtCommandFactory::mSessionId = "00000000";
const std::string AtCommandFactory::mProfileId = "00000000";
const std::string AtCommandFactory::mApplicationId = "00000000";

const std::string AtCommandFactory::mAtCommandPrefix = "AT*";
const std::string AtCommandFactory::mAtCommandAssignement = "=";
const std::string AtCommandFactory::mAtCommandSuffix = "\r";
const std::string AtCommandFactory::mAtCommandSeparator = ",";

const std::string AtCommandFactory::mAtCommandRefHeader = "REF";
const std::string AtCommandFactory::mAtCommandPcmdHeader = "PCMD";
const std::string AtCommandFactory::mAtCommandPcmdMagHeader = "PCMD_MAG";
const std::string AtCommandFactory::mAtCommandFtrimHeader = "FTRIM";
const std::string AtCommandFactory::mAtCommandConfigHeader = "CONFIG";
const std::string AtCommandFactory::mAtCommandConfigIdsHeader = "CONFIG_IDS";
const std::string AtCommandFactory::mAtCommandComWdgHeader = "COMWDG";
const std::string AtCommandFactory::mAtCommandCalibHeader = "CALIB";
const std::string AtCommandFactory::mAtCommandControlHeader = "CTRL";

const std::string AtCommandFactory::commandFactory(const std::string& atCmdHeader)
{
    // Command's stream initialisation
    std::ostringstream atCmd;
    atCmd << mAtCommandPrefix << atCmdHeader << mAtCommandAssignement << mSequence
          << mAtCommandSuffix;

    incrSequenceNumber();

    // Get the final AT command
    return atCmd.str();
}

void AtCommandFactory::setBit(int32_t& bitField, int bitToSet)
{
    bitField |= 1 << bitToSet;
}

void AtCommandFactory::clearBit(int32_t& bitField, int bitToClear)
{
    bitField &= ~(1 << bitToClear);
}

int AtCommandFactory::floatingPointConversion(float argToConv)
{
    if (argToConv < -1.0 || argToConv > 1.0) {
        throw std::range_error("AtCommand floating point parameter out of range");
    }
    return *(reinterpret_cast<int*>(&argToConv));
}

void AtCommandFactory::incrSequenceNumber()
{
    // if the sequence number had overflowed, we reset it to one
    mSequence == 0 ? 1 : mSequence++;
}

int32_t AtCommandFactory::prepareRefHeaderFlag()
{
    int32_t flag = 0;

    // When sending AT*REF command, bits 18, 20, 22, 24 and 28 needs to be set
    setBit(flag, 18);
    setBit(flag, 20);
    setBit(flag, 22);
    setBit(flag, 24);
    setBit(flag, 28);

    return flag;
}

const std::string AtCommandFactory::takeOffCommand()
{
    int32_t cmd = prepareRefHeaderFlag();
    setBit(cmd, 9);

    return commandFactory<int>(mAtCommandRefHeader, cmd);
}

const std::string AtCommandFactory::landCommand()
{
    int32_t cmd = prepareRefHeaderFlag();

    return commandFactory<int>(mAtCommandRefHeader, cmd);
}

const std::string AtCommandFactory::emergencyCommand()
{
    int32_t cmd = prepareRefHeaderFlag();
    setBit(cmd, 8);

    return commandFactory<int>(mAtCommandRefHeader, cmd);
}

const std::string AtCommandFactory::hoveringCommand()
{
    /* Hovering mode is activated if the bit 0 of the flag arguments from
     * a PCMD atCommand is set to 0
     */
    int32_t flag = 0;

    return commandFactory<int>(mAtCommandPcmdHeader, flag, 0, 0, 0, 0);
}

const std::string AtCommandFactory::movementCommand(float roll, float pitch, float gaz, float yaw)
{
    int32_t flag = 0;

    // Setting flag bit 0 to avoid entering in hovering mode
    setBit(flag, 0);

    return commandFactory<int>(
        mAtCommandPcmdHeader,
        flag,
        floatingPointConversion(roll),
        floatingPointConversion(pitch),
        floatingPointConversion(gaz),
        floatingPointConversion(yaw));
}

const std::string AtCommandFactory::movementCommand(float roll, float pitch, float gaz)
{
    int32_t flag = 0;

    // Setting  bit 0 to avoid entering in hovering mode
    setBit(flag, 0);
    // Setting flag bit 1 to let the firmware choose yaw parameter in function of the roll one
    setBit(flag, 1);

    return commandFactory<int>(
        mAtCommandPcmdHeader,
        flag,
        floatingPointConversion(roll),
        floatingPointConversion(pitch),
        floatingPointConversion(gaz),
        0);
}

const std::string AtCommandFactory::movementWithMagnetoCommand(
    float roll,
    float pitch,
    float gaz,
    float yaw,
    float magnetoPsi,
    float magnetoPsiAccuracy)
{
    int32_t flag = 0;

    // Setting flag bit 0 to avoid entering in hovering mode
    setBit(flag, 0);

    return commandFactory<int>(
        mAtCommandPcmdMagHeader,
        flag,
        floatingPointConversion(roll),
        floatingPointConversion(pitch),
        floatingPointConversion(gaz),
        floatingPointConversion(yaw),
        floatingPointConversion(magnetoPsi),
        floatingPointConversion(magnetoPsiAccuracy));
}

const std::string AtCommandFactory::movementWithMagnetoCommand(
    float roll,
    float pitch,
    float gaz,
    float magnetoPsi,
    float magnetoPsiAccuracy)
{
    int32_t flag = 0;

    // Setting flag bit 0 to avoid entering in hovering mode
    setBit(flag, 0);

    // Setting flag bit 1 to let the firmware choose yaw parameter in function of the roll one
    setBit(flag, 1);

    return commandFactory<int>(
        mAtCommandPcmdMagHeader,
        flag,
        floatingPointConversion(roll),
        floatingPointConversion(pitch),
        floatingPointConversion(gaz),
        0,
        floatingPointConversion(magnetoPsi),
        floatingPointConversion(magnetoPsiAccuracy));
}

const std::string AtCommandFactory::resetWatchdogCommand()
{
    return commandFactory(mAtCommandComWdgHeader);
}

const std::string AtCommandFactory::horizontalPlaneCalibrationCommand()
{
    return commandFactory(mAtCommandFtrimHeader);
}

const std::string AtCommandFactory::setConfigureIdsCommands()
{
    std::ostringstream atCmd;
    atCmd << configureCommand("custom:session_id", mSessionId);
    atCmd << configureCommand("custom:profile_id", mProfileId);
    atCmd << configureCommand("custom:application_id", mApplicationId);

    return atCmd.str();
}

const std::string AtCommandFactory::configureIdsCommand()
{
    return commandFactory<std::string>(
        mAtCommandConfigIdsHeader,
        "\"" + mSessionId + "\"",
        "\"" + mProfileId + "\"",
        "\"" + mApplicationId + "\"");
}

const std::string AtCommandFactory::configureCommand(const std::string& argA,
                                                     const std::string& argB)
{
    std::ostringstream atCmd;
    atCmd << configureIdsCommand();
    atCmd << commandFactory<std::string>(
        mAtCommandConfigHeader,
        "\"" + argA + "\"",
        "\"" + argB + "\"");

    return atCmd.str();
}

const std::string AtCommandFactory::magnetoCalibrationCommand()
{
    // The magnetometer is the only device which can be calibrated
    int deviceToCalibrate = 0;

    return commandFactory<int>(mAtCommandCalibHeader, deviceToCalibrate);
}

const std::string AtCommandFactory::ackConfigureCommand()
{
    // Value from the SDK which reset the Ack field from AtCommands
    int ackControlMode = 5;
    int ackControlArg2 = 0;

    return commandFactory<int>(mAtCommandControlHeader, ackControlMode, ackControlArg2);
}

} /* ghost namespace */
} /* libDroneMovement namespace  */
