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

    drone.takeOff();

    for (size_t i = 0; i < 100; i++) {
        LineDetector<FrameGrabber::DroneVerticalFrame> lineDetector(
            frameGrabber.getNextVerticalFrame(),
            320,
            240);

        double lineAngle = lineDetector.getLineAngle();

        float currentAngle = drone.getPsiAngle() / 1000.0;
        std::cout << "Angle Ligne: " << lineAngle << std::endl;
        std::cout << "Angle Drone: " << currentAngle << std::endl;

        if (std::isnan(lineAngle)) {
            // we don't see the line, let's assume we are good for now
            lineAngle = 0.0;
        }

        float yawSpeed = 0.0;
        float angleCible = currentAngle;

        if (lineAngle > 10.0) {
            // We need to go left
            yawSpeed = -0.2;
            angleCible -= lineAngle;
        }
        if (lineAngle < -10.0) {
            yawSpeed = 0.2;
            angleCible += lineAngle;
        }

        int counter = 0;
        int threshold = 20;

        std::cout << "Yawwwww Speeeed: " << yawSpeed << std::endl;
        while (counter <= threshold) {
            float currentAngle = drone.getPsiAngle() / 1000.0;
            if (std::abs(angleCible - currentAngle) <= 10.0) {
                break;
            }
            drone.movement(0.0, 0.0, 0.0, yawSpeed);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            counter++;
        }

        int left = (threshold - counter) * 20;

        if (left > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(left));
        }
    }
    drone.land();

    return 0;
}
