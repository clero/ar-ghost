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

#pragma once

#include <libDroneMovement/Drone.hpp>
#include <libDroneVideo/FrameGrabber.hpp>

namespace ghost
{
namespace core
{
namespace mission
{

/**
 * Move the drone until the point given in parameter become the center
 * of the image.
 * The point must also be represented by a circle on the ground.
 */
class DestinationPointFlyer
{
public:

    /**
     * Class Constructor
     *
     * @param[in] drone the reference on the drone object to control
     * @param[in] frameGrabber the reference on the main frameGrabber object
     * @param[in] xCoordinate the destination abscissa coordinate (with top left corner as origin)
     * @param[in] yCoordinate the destination ordinate coordinate (with top left corner as origin)
     */
    DestinationPointFlyer(
        libDroneMovement::Drone& drone,
        libDroneVideo::FrameGrabber& frameGrabber,
        int xCoordinate,
        int yCoordinate);

    /** Launch the movement */
    void start();

private:

    /** Analyse a new frame in order to evaluate the new position of the circle */
    bool getCircleNewCoordinates();

    /** Move towards the point trough absissa axis */
    void moveToXCoordinate();

    /** Move towards the point trough ordinate axis */
    void moveToYCoordinate();

    /** Shift the cartesian coordinates system to use the image center as origin */
    inline void shiftCartesianOriginSystem();

    libDroneMovement::Drone& mDrone;
    libDroneVideo::FrameGrabber& mFrameGrabber;

    /** Coordinates with the image center as coordinate */
    int mXCoordinate;
    int mYCoordinate;
};

} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
