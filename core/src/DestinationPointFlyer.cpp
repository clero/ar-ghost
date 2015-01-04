/*
 * Copyright 2015 Jules Cléro
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

#include "core/DestinationPointFlyer.hpp"
#include <libDroneVideo/CircleDetector.hpp>

#include <boost/log/trivial.hpp>

#include <chrono>
#include <thread>
#include <cmath>

namespace ghost
{
namespace core
{
namespace mission
{

DestinationPointFlyer::DestinationPointFlyer(
    libDroneMovement::Drone& drone,
    libDroneVideo::FrameGrabber& frameGrabber,
    int xCoordinate,
    int yCoordinate)
    : mDrone(drone), mFrameGrabber(frameGrabber),
    mXCoordinate(xCoordinate), mYCoordinate(yCoordinate)
{
    shiftCartesianOriginSystem();
}

void DestinationPointFlyer::start()
{
    const int coordinateThreshold = 20;
    bool circleFound = true;
    BOOST_LOG_TRIVIAL(info) << "Going to circle";
    while (std::abs(mXCoordinate) > coordinateThreshold
           && std::abs(mYCoordinate) > coordinateThreshold
           && circleFound) {

        BOOST_LOG_TRIVIAL(info) << "Move to Circle";
        moveToXCoordinate();
        moveToYCoordinate();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        circleFound = getCircleNewCoordinates();
    }
    BOOST_LOG_TRIVIAL(info) << "Above circle";
}

inline void DestinationPointFlyer::shiftCartesianOriginSystem()
{
    mXCoordinate = 120 - mXCoordinate;
    mYCoordinate = 160 - mYCoordinate;
}

bool DestinationPointFlyer::getCircleNewCoordinates()
{

    bool circleFound = false;
    int threshold = 0;

    do {
        BOOST_LOG_TRIVIAL(info) << "Search circle";
        libDroneVideo::CircleDetector<libDroneVideo::FrameGrabber::DroneVerticalFrame>
        circleDetector(
            mFrameGrabber.getNextVerticalFrame(),
            320,
            240);

        if (circleDetector.isCircleInFrame()) {
            mXCoordinate = circleDetector.getCenterXCoordinate();
            mYCoordinate = circleDetector.getCenterYCoordinate();

            shiftCartesianOriginSystem();

            circleFound = true;
        } else {
            threshold++;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } while (!circleFound && threshold < 15);

    if (!circleFound) {
        BOOST_LOG_TRIVIAL(error) << "Cannot find circle any more";
        // LOg and throw smg
    }

    BOOST_LOG_TRIVIAL(info) << "Circle Found";
    return circleFound;
}

void DestinationPointFlyer::moveToXCoordinate()
{
    if (mXCoordinate > 20) {
        // Go right
        mDrone.movement(0.1, 0.0, 0.0, 0.0);
    } else if (mXCoordinate < -20) {
        // Go Left
        mDrone.movement(-0.1, 0.0, 0.0, 0.0);
    }
}

void DestinationPointFlyer::moveToYCoordinate()
{
    if (mYCoordinate > 20) {
        // Go backward
        mDrone.movement(0.0, -0.1, 0.0, 0.0);
    } else if (mYCoordinate < -20) {
        // Go forward
        mDrone.movement(0.0, 0.1, 0.0, 0.0);
    }
}

} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
