/*
 * Copyright 2014 Jules Cléro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
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

#define CAMV_DEVICE "/dev/video2"
#define CAMV_BUFFER "/tmp/camv_buffer"
#define CAMV_READY "/tmp/camv_ready"

#define CAMH_DEVICE "/dev/video1"
#define CAMH_BUFFER "/tmp/camh_buffer"
#define CAMH_READY "/tmp/camh_ready"

namespace ghost
{
namespace libDroneVideoSharing
{

class CameraDefinition
{
public:

    /** Yuv frame channel number */
    static constexpr int mImgChannelNumber = 2;

    /** Vertical Camera information */
    static constexpr int mVerticalImgWidth = 320;
    static constexpr int mVerticalImgHeight = 240;
    static constexpr int mVerticalImgSize = mVerticalImgWidth * mVerticalImgHeight *
                                            mImgChannelNumber;

    /** Horizontal Camera information */
    static constexpr int mHorizontalImgWidth = 1280;
    static constexpr int mHorizontalImgHeight = 720;
    static constexpr int mHorizontalImgSize = mHorizontalImgWidth * mHorizontalImgHeight *
                                              mImgChannelNumber;

private:

    CameraDefinition();
    ~CameraDefinition();

};


} /* libDroneVideoSharing namespace */
} /* ghost namespace  */
