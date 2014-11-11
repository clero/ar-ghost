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

#include <cstdint>
#include <string>
#include <array>
#include <iostream>
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace ghost
{
namespace libDroneMovement
{
namespace navData
{

/**
 * Class allowing to parse a raw NavData Packet
 * The internal state is set after each parsing to let other components to get usefull information.
 * Parsed information can be read by several threads at the same timei. On the contrary, update requests
 * has to be made only by one thread.
 */
class NavDataParser
{

public:

    /** Packet containing essentials drone's information */
    struct NavDataDemoPacket {

        // Same State as the one in the basic Packet
        uint32_t ctrl_state;
        uint32_t vbat_flying_percentage;

        // Angles in milli-degrees
        float theta;
        float phi;
        float psi;

        // Altitude in cm
        int32_t altitude;

        float velocityX;
        float velocityY;
        float velocityZ;

        /*
         * Not used
         * uint32_t num_frames;
         */

        /*
         * Deprecated by Parrot - Ignore
         *
         * Camera parameters compute by detection
         * matrix33_t detection_camera_rot;
         * vector31_t detection_camera_trans;
         * uint32_t detection_tag_index;
         * uint32_t detection_camera_type;
         *
         * Camera parameters compute by drone
         * matrix33_t drone_camera_rot;
         * vector31_t drone_camera_trans;
         */
    } __attribute__((packed));

    /**
     * NavDataPacket as received through the network
     *
     * We are using a size of 1kB as requested by the
     * Parrot Navdata Server.
     */
    typedef std::array<uint8_t, 1024> RawNavDataPacket;

    /** Class Constructor */
    NavDataParser();

    /**
     * Launch Packet Parsing
     * This function has to be called by a unique thread.
     *
     * @param[in] rawNavDataPacket the packet to parse.
     *
     * @return true if the parse has succeed, false otherwise
     */
    bool parseNavDataPacket(const RawNavDataPacket& rawNavDataPacket);

    bool isDroneFlying();
    void waitControlCommandAck();
    void waitNotControlCommandAck();
    bool isControlCommandAck();

    /**
     * Get last updated NavData Demo Packet
     *
     * @return NavDataDemoPacket object
     */
    NavDataDemoPacket getCurrentNavData();

private:

    /** Structure which describe Parrot NavData Packet Header */
    struct NavDataPacketHeader {
        uint32_t header;
        uint32_t state;
        uint32_t sequence;
        uint32_t visionDefined;

        // Common Option Part
        uint16_t tag;
        uint16_t size;
    } __attribute__((packed));


    /** Drone states flags signification as defined by parrots */
    enum DroneState {
        /*!< FLY MASK : (0) mykonos is landed, (1) mykonos is flying */
        Fly = 1 << 0,
        /*!< VIDEO MASK : (0) video disable, (1) video enable */
        Video = 1 << 1,
        /*!< VISION MASK : (0) vision disable, (1) vision enable */
        Vision = 1 << 2,
        /*!< CONTROL ALGO : (0) euler angles control, (1) angular speed control */
        Control = 1 << 3,
        /*!< ALTITUDE CONTROL ALGO : (0) altitude control inactive (1) altitude control active */
        Altitude = 1 << 4,
        /*!< USER feedback : Start button state */
        UserFeedbackStart = 1 << 5,
        /*!< Control command ACK : (0) None, (1) one received */
        ControlCommandAck = 1 << 6,
        /*!< Trim command ACK : (0) None, (1) one received */
        TrimCommandAck = 1 << 7,
        /*!< Trim running : (0) none, (1) running */
        TrimRunning = 1 << 8,
        /*!< Trim result : (0) failed, (1) succeeded */
        TrimResult = 1 << 9,
        /*!< Navdata demo : (0) All navdata, (1) only navdata demo */
        NavDataDemo = 1 << 10,
        /*!< Navdata bootstrap : (0) options sent in all or demo mode, (1) no navdata options sent */
        NavDataBootstrap = 1 << 11,
        /*!< Motors brushed : (0) brushless, (1) brushed */
        MotorsBrushed = 1 << 12,
        /*!< Communication Lost : (1) com problem, (0) Com is ok */
        ComLost = 1 << 13,
        /*!< Bit means that there's an hardware problem with gyrometers */
        GyrosError = 1 << 14,
        /*!< VBat low : (1) too low, (0) Ok */
        VbatLow = 1 << 15,
        /*!< VBat high (US mad) : (1) too high, (0) Ok */
        VbatHigh = 1 << 16,
        /*!< Timer elapsed : (1) elapsed, (0) not elapsed */
        TimerElapsed = 1 << 17,
        /*!< Power : (0) Ok, (1) not enough to fly */
        NotEnoughPower = 1 << 18,
        /*!< Angles : (0) Ok, (1) out of range */
        AnglesOutOfRange = 1 << 19,
        /*!< Wind : (0) Ok, (1) too much to fly */
        Wind = 1 << 20,
        /*!< Ultrasonic sensor : (0) Ok, (1) deaf */
        Ultrasound = 1 << 21,
        /*!< Cutout system detection : (0) Not detected, (1) detected */
        Cutout = 1 << 22,
        /*!< PIC Version number OK : (0) a bad version number, (1) version number is OK */
        PicVersion = 1 << 23,
        /*!< ATCodec thread ON : (0) thread OFF (1) thread ON */
        AtCodecOn = 1 << 24,
        /*!< Navdata thread ON : (0) thread OFF (1) thread ON */
        NavDataOn = 1 << 25,
        /*!< Video thread ON : (0) thread OFF (1) thread ON */
        VideoOn = 1 << 26,
        /*!< Acquisition thread ON : (0) thread OFF (1) thread ON */
        AcqOn = 1 << 27,
        /*!< CTRL watchdog : (1) delay in control execution (> 5ms), (0) control is well scheduled */
        CtrlWatchdog = 1 << 28,
        /*!< ADC Watchdog : (1) delay in uart2 dsr (> 5ms), (0) uart2 is good */
        AdcWatchdog = 1 << 29,
        /*!< Communication Watchdog : (1) com problem, (0) Com is ok */
        ComWatchdog = 1 << 30,
        /*!< Emergency landing : (0) no emergency, (1) emergency */
        Emergency = 1 << 31
    };

    enum NavDataOptionTag {
        DemoTag = 0,
        CksTag = 0xFFFF
                 /*
                  * Currently we are supporting only Demo options
                  * Feel free to add the support of a new option if you
                  * need it
                  *
                  * TimeTag,
                  * RawMeasuresTag,
                  * PhysMeasuresTag,
                  * GyrosOffsetTag,
                  * EulerAnglesTag,
                  * ReferenceTag,
                  * TrimsTag,
                  * RcReferencesTag,
                  * PwmTag,
                  * AltitudeTag,
                  * VisionRawTag,
                  * VisionOfTag,
                  * VisionTag,
                  * VisionPerfTag,
                  * TrackersSendTag,
                  * VisionDetectTag,
                  * WatchdogTag,
                  * AdcDataFrameTag,
                  * VideoStreamTag,
                  * CksTag = 0xFFFF
                  */
    };

    /** NavData Packet header */
    static const uint32_t mNavDataHeader;

    /**
     * Update internal state
     *
     * @param[in] state the current state
     */
    void setNewState(uint32_t state);

    /**
     * Parse the state field to say if the desired field is set or not.
     *
     * @param[in] state the current state
     * @param[in] fieldToCheck the field to check
     *
     * @return true if the field is set, false otherwise
     */
    inline bool checkState(uint32_t state, DroneState fieldToCheck);

    /** Current NavData sequence number */
    uint32_t mSequenceNumber;

    /** Drone current state */
    std::atomic_bool mIsFlying;
    std::atomic_bool mIsControlCommandAck;

    /** Hold updated essaentials NavData*/
    NavDataDemoPacket mCurrentNavDataDemoPacket;

    /** mCurrentNavDataDemoPacket mutex */
    std::mutex mCurrentNavDataDemoMutex;

    /** Drone condition_variable to synchronize other threads based on drone's states */
    std::condition_variable mIsControlCommandAckCheck;
    std::mutex mControlCommandAckMutex;
};

} /* navData namespace  */
} /* libDroneMovement namespace  */
} /* ghost namespace */
