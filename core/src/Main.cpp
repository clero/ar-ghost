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

extern "C"
{
/** LibLineFollowing */
#include "SystemDrone.h"
}

#include <libDroneMovement/Drone.hpp>
#include <libDroneVideo/FrameGrabber.hpp>
#include <libDroneVideo/LineDetector.hpp>
#include <chrono>
#include <thread>

int main(void)
{
    using namespace ghost::libDroneMovement;
    using namespace ghost::libDroneVideo;

    Drone drone("127.0.0.1");
    FrameGrabber frameGrabber = FrameGrabber();

    // Auto Pilot initialization
    inC_SystemDrone inputAutoPilot;
    outC_SystemDrone outputAutoPilot;

    SystemDrone_init(&outputAutoPilot);

    drone.takeOff();

    // Drop Frames
    frameGrabber.getNextVerticalFrame(),
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    double lineAngle = 0.0;
    bool needToGoLeft = false;
    bool needToGoRight = false;
    bool needImageUpdate = true;
    bool lineDetected = true;
    int count = 0;

    for (size_t i = 0; i < 1000; i++) {

        if (i % 10 == 0) {
            needImageUpdate = true;
        }

        if (needImageUpdate) {
            // Frame Analysis
            std::cout << "Update Image: " << std::endl;
            LineDetector<FrameGrabber::DroneVerticalFrame> lineDetector(
                frameGrabber.getNextVerticalFrame(),
                320,
                240);

            lineDetected = true;
            lineAngle = lineDetector.getLineAngle();
            needToGoRight = lineDetector.needToGoRight();
            needToGoLeft = lineDetector.needToGoLeft();

            if (std::isnan(lineAngle)) {
                // we don't see the line, let's assume we are good for now
                lineAngle = 0.0;
                count++;
            } else {
                lineDetected = true;
                count = 0;
            }

            if (count >= 5) {
                lineDetected = false;
            }

            inputAutoPilot.LineAngle = lineAngle;
            inputAutoPilot.RightWay = true;
            inputAutoPilot.GoLeft = needToGoLeft;
            inputAutoPilot.GoRight = needToGoRight;

            inputAutoPilot.ImageUpdate = needImageUpdate;
            inputAutoPilot.LineDetected = lineDetected;

            needImageUpdate = false;
        } else {
            inputAutoPilot.ImageUpdate = needImageUpdate;
        }

        float currentAngle = drone.getPsiAngle() / 1000.0;

        inputAutoPilot.currentAngle = currentAngle;

        std::cout << "Angle Ligne: " << lineAngle << std::endl;
        std::cout << "Angle Drone: " << currentAngle << std::endl;

        std::cout << "Yaw speed Drone: " << outputAutoPilot.Yaw << std::endl;

        SystemDrone(&inputAutoPilot, &outputAutoPilot);

        drone.movement(
            outputAutoPilot.Roll,
            outputAutoPilot.Pitch,
            outputAutoPilot.Gaz,
            outputAutoPilot.Yaw);

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    drone.land();

    return 0;
}
