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

#include "libDroneVideoSharing/CameraDefinition.hpp"

#include <string>
#include <array>

namespace ghost
{
namespace libDroneVideo
{

/**
 * Grab a new frame from the buffer shared with Parrot
 * internal application.
 */
class FrameGrabber
{

public:

    FrameGrabber();

    /** Container for vertical camera frame */
    typedef std::array<uint8_t,
                       libDroneVideoSharing::CameraDefinition::mVerticalImgSize> DroneVerticalFrame;

    /**
     * Get a new frame from the vertical Camera
     *
     * @return DroneFrame container of the new frame
     */
    DroneVerticalFrame& getNextVerticalFrame();

    int getVerticalFrameWidth();
    int getVerticalFrameHeight();

private:

    /** Internal vertical frame container */
    DroneVerticalFrame mDroneVerticalFrame;

};

} /* libDroneVideo namespace */
} /* ghost namespace  */
