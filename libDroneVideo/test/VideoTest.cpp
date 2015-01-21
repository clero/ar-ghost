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

#include <libDroneVideo/FrameGrabber.hpp>
#include <libDroneVideo/LineDetector.hpp>
#include <libDroneVideo/CircleDetector.hpp>

#include <iostream>
#include <thread>
#include <chrono>

/**
 * Simple test program which allow to test embedded lines and circles
 * detection.
 */
int main(void)
{
    using namespace ghost::libDroneVideo;

    FrameGrabber frameGrabber = FrameGrabber();
    double lineAngle = 0.0;

    for (size_t i = 0; i < 1000; i++) {

        try {
            // Frame Analysis
            LineDetector<FrameGrabber::DroneVerticalFrame> lineDetector(
                frameGrabber.getNextVerticalFrame(),
                320,
                240);

            lineAngle = lineDetector.getLineAngle();
        }
        catch (LineDetector<FrameGrabber::DroneVerticalFrame>::LineDetectionError e) {
            std::cout << "No line found" << std::endl;
        }

        // Frame Analysis
        CircleDetector<FrameGrabber::DroneVerticalFrame> circleDetector(
            frameGrabber.getNextVerticalFrame(),
            320,
            240);

        if (std::isnan(lineAngle)) {
            // we don't see the line, let's assume we are good for now
            lineAngle = 0.0;
        }

        std::cout << "Angle Ligne: " << lineAngle << std::endl;
        std::cout << "Rayon Cercle: " << circleDetector.getCircleRadius() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    return 0;
}
