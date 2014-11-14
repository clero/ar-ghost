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

#include <string>
#include <atomic>
#include <sstream>

namespace ghost
{
namespace libDroneMovement
{

/** Object Factory allowing to create ARDrone AT Commands easily. */
class AtCommandFactory
{
public:

    /** AtCommandFactory Constructor */
    AtCommandFactory();

    /** Generate the AT Command allowing the drone to take off */
    const std::string takeOffCommand();

    /** Generate the AT Command allowing the drone to land */
    const std::string landCommand();

    /** Generate the AT Command setting the drone in emergency mode */
    const std::string emergencyCommand();

    /** Generate the AT Command allowing the drone to stay above the current point on the ground */
    const std::string hoveringCommand();

    /**
     *  Generate the AT Command allowing the drone to move.
     *
     *  @param[in] roll the desired roll coefficient, must be in [-1,1]
     *  @param[in] pitch the desired pitch coefficient, must be in [-1,1]
     *  @param[in] gaz the desired gaz coefficient, must be in [-1,1]
     *  @param[in] yaw the desired yaw coefficient, must be in [-1,1]
     */
    const std::string movementCommand(float roll, float pitch, float gaz, float yaw);

    /**
     *  Generate the AT Command allowing the drone to move.
     *  This function lets the firmware choose the yaw coefficient based on the roll one.
     *
     *  @param[in] roll the desired roll coefficient, must be in [-1,1]
     *  @param[in] pitch the desired pitch coefficient, must be in [-1,1]
     *  @param[in] gaz the desired gaz coefficient, must be in [-1,1]
     */
    const std::string movementCommand(float roll, float pitch, float gaz);

    /**
     *  Generate the AT Command allowing the drone to move.
     *  This function uses magneto meter informations to move.
     *
     *  @param[in] roll the desired roll coefficient, must be in [-1,1]
     *  @param[in] pitch the desired pitch coefficient, must be in [-1,1]
     *  @param[in] gaz the desired gaz coefficient, must be in [-1,1]
     *  @param[in] yaw the desired yaw coefficient, must be in [-1,1]
     *  @param[in] magnetoPsi the magnenometer psi, must be in [-1,1]
     *  @param[in] magnetoPsiAccuracy the magnenometer psi accuracy, must be in [-1,1]
     */
    const std::string movementWithMagnetoCommand(
        float roll,
        float pitch,
        float gaz,
        float yaw,
        float magnetoPsi,
        float magnetoPsiAccuracy);

    /**
     *  Generate the AT Command allowing the drone to move.
     *  This function lets the firmware choose the yaw coefficient based on the roll one.
     *  This function uses magneto meter informations to move.
     *
     *  @param[in] roll the desired roll coefficient, must be in [-1,1]
     *  @param[in] pitch the desired pitch coefficient, must be in [-1,1]
     *  @param[in] gaz the desired gaz coefficient, must be in [-1,1]
     *  @param[in] magnetoPsi the magnenometer psi, must be in [-1,1]
     *  @param[in] magnetoPsiAccuracy the magnenometer psi accuracy, must be in [-1,1]
     */
    const std::string movementWithMagnetoCommand(
        float roll,
        float pitch,
        float gaz,
        float magnetoPsi,
        float magnetoPsiAccuracy);

    /** Generate the AT Command allowing the drone to reset his internal watchdog */
    const std::string resetWatchdogCommand();

    /** Generate the AT Command allowing the drone to reset his internal watchdog */
    const std::string horizontalPlaneCalibrationCommand();

    /** Generate the AT Command allowing to modify current drone's internal configuration */
    const std::string configureCommand(const std::string& argA, const std::string& argB);

    /** Generate the AT Command allowing to modify current drone's internal configuration ids. */
    const std::string setConfigureIdsCommands();

    /** Generate the AT Command allowing the drone to calibrate his magnetometer */
    const std::string magnetoCalibrationCommand();

    /** Generate the AT Command allowing the drone to ack configure command */
    const std::string ackConfigureCommand();

private:

    /**
     * General method which allows to create an AT Command.
     *
     * @tparam T command's arguments type
     * @param[in] atCmdHeader the header of the desired AT Command
     * @param[in] atCmdArgs,... the list of command's arguments
     *
     */
    template <typename ... T>
    const std::string commandFactory(const std::string& atCmdHeader, const T ... atCmdArgs)
    {
        // Command's stream initialisation
        std::ostringstream atCmd;
        atCmd << mAtCommandPrefix << atCmdHeader << mAtCommandAssignement << mSequence;

        // Let's add all arguments
        commandTemplate(atCmd, atCmdArgs ...);

        incrSequenceNumber();

        // Get the final AT command
        return atCmd.str();
    }

    /** General method which allows to create an AT Command without arguments. */
    const std::string commandFactory(const std::string& atCmdHeader);

    /**
     * General method which allows to add an argument to an AT Command
     *
     * @tparam C command's argument type
     * @param[in] atCmdStream the stream containing the AT Command
     */
    template <typename C>
    void commandTemplate(std::ostringstream& atCmdStream, const C atCmdArg)
    {
        // We have the last arguments, we reach the end of the command
        atCmdStream << mAtCommandSeparator << atCmdArg << mAtCommandSuffix;
    }

    /**
     * General method which allows to add an argument to an AT Command
     *
     * @tparam C current command's argument type
     * @tparam T other command's arguments type
     * @param[in] atCmdStream the stream containing the AT Command
     * @param[in] atCmdArg the current argument
     * @param[in] otherArgs,... the list of command's arguments
     */
    template <typename C, typename ... T>
    void commandTemplate(std::ostringstream& atCmdStream, const C atCmdArg, const T ... otherArgs)
    {
        // There still some arguments, let's make a recursive call
        atCmdStream << mAtCommandSeparator << atCmdArg;
        commandTemplate(atCmdStream, otherArgs ...);
    }

    /**
     *  Set the desired bit.
     *
     *  @param[in] bitField the field to modify
     *  @param[in] the bit number to set
     */
    inline void setBit(int32_t& bitField, int bitToSet);

    /**
     *  Clear the desired bit.
     *
     *  @param[in] bitField the field to modify
     *  @param[in] the bit number to clear
     */
    inline void clearBit(int32_t& bitField, int bitToClear);

    /**
     *  Convert the given float to the corresponding encoded int
     *
     *  @param[in] argToConv the float to convert
     *  @return the corresponding int
     */
    inline int floatingPointConversion(float argToConv);

    /** Increment the sequence number to prepare next command */
    inline void incrSequenceNumber();

    /** Prepare flags for commands using REF header */
    int32_t prepareRefHeaderFlag();

    /**
     * Generate the AT Command containing or application configuration ids.
     * This function is called every time the configureCommand function is called, it is
     * necessary to modify the drone internal configuration.
     */
    const std::string configureIdsCommand();


    /**
     *  Current Sequence number
     *  It needs to be atomic as a command can be sent by the
     *  AtCommandSender or by the Drone entity.
     */
    std::atomic_uint_fast32_t mSequence;

    /** At Command parts */
    static const std::string mAtCommandPrefix;
    static const std::string mAtCommandAssignement;
    static const std::string mAtCommandSuffix;
    static const std::string mAtCommandSeparator;

    /** Available At Commands */
    static const std::string mAtCommandRefHeader;
    static const std::string mAtCommandPcmdHeader;
    static const std::string mAtCommandPcmdMagHeader;
    static const std::string mAtCommandFtrimHeader;
    static const std::string mAtCommandConfigHeader;
    static const std::string mAtCommandConfigIdsHeader;
    static const std::string mAtCommandComWdgHeader;
    static const std::string mAtCommandCalibHeader;
    static const std::string mAtCommandControlHeader;

    /** Application Ids */
    static const std::string mSessionId;
    static const std::string mProfileId;
    static const std::string mApplicationId;
};

} /* ghost namespace */
} /* libDroneMovement namespace  */
