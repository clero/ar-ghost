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

#include "libDroneVideo/FrameGrabber.hpp"

#include <boost/log/trivial.hpp>

#include <sys/stat.h>
#include <fcntl.h>

#include <chrono>
#include <thread>

namespace ghost
{
namespace libDroneVideo
{

FrameGrabber::FrameGrabber() : mDroneVerticalFrame({ 0 })
{
}

int FrameGrabber::getVerticalFrameWidth()
{
    return libDroneVideoSharing::CameraDefinition::mVerticalImgWidth;
}

int FrameGrabber::getVerticalFrameHeight()
{
    return libDroneVideoSharing::CameraDefinition::mVerticalImgHeight;
}

FrameGrabber::DroneVerticalFrame& FrameGrabber::getNextVerticalFrame()
{

    int query = 0;
    int failThreshold = 5;

    do {
        // Check if the frame is ready
        if (access(CAMV_READY, F_OK) != -1) {
            int fd = open(CAMV_BUFFER, O_RDONLY);

            // Get the frame
            read(fd,
                 &mDroneVerticalFrame[0],
                 libDroneVideoSharing::CameraDefinition::mVerticalImgSize);
            close(fd);
            remove(CAMV_READY);

            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    } while (++query != failThreshold);

    if (query == failThreshold) {
        BOOST_LOG_TRIVIAL(warning) << "Vertical camera buffer unaivalable";
    }

    return std::ref(mDroneVerticalFrame);
}

} /* libDroneVideo namespace */
} /* ghost namespace  */
