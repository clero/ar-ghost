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

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

namespace ghost
{
namespace libDroneVideo
{

/**
 * Analyse a given frame in order to detect a circle
 *
 * @tparam FrameType the type of frame to analyse
 */
template <class FrameType>
class CircleDetector
{

public:

    CircleDetector(const FrameType frame, const int imgWidth, const int mImgHeight);
    ~CircleDetector();

    bool IsCircleInFrame();
    int getCenterXCoordinate();
    int getCenterYCoordinate();
    int getCircleRadius();

private:

    typedef std::vector<cv::Vec3f> FrameCircles;

    FrameType mFrame;

    const int mImgWidth;
    const int mImgHeight;
    static constexpr int mImgChannelNumber = 2;

    bool mIsCircleInFrame;

    int mCenterXCoordinate;
    int mCenterYCoordinate;
    int mCircleRadius;

};

template <class FrameType>
CircleDetector<FrameType>::CircleDetector(const FrameType frame,
                                          const int imgWidth,
                                          const int imgHeight) :
    mFrame(frame), mImgWidth(imgWidth), mImgHeight(imgHeight),
    mIsCircleInFrame(false), mCenterXCoordinate(0.0),
    mCenterYCoordinate(0.0), mCircleRadius(0.0)
{
    /** FIXME:
     *  The conversion is done for line detection also, we need to do it only once
     */

    // Creating openCv img from raw YUV Image
    cv::Mat droneImg(cv::Size(mImgWidth, mImgHeight), CV_8UC2, mFrame.data());

    // Converting yuv format to grayscale
    cv::cvtColor(droneImg, droneImg, CV_YUV2GRAY_Y422);

    // Reduce the noise so we avoid false circle detection
    cv::GaussianBlur(droneImg, droneImg, cv::Size(9, 9), 2, 2);

    FrameCircles frameCircles;
    // Circle detection
    cv::HoughCircles(
        droneImg,
        frameCircles,
        CV_HOUGH_GRADIENT,
        1,
        droneImg.rows / 16,
        50,
        100,
        0,
        0);

    if (frameCircles.size() >= 1) {

        mIsCircleInFrame = true;

        // For now, we consider only the first detected circle
        mCenterXCoordinate = cvRound(frameCircles[0][0]);
        mCenterYCoordinate = cvRound(frameCircles[0][1]);
        mCircleRadius = cvRound(frameCircles[0][2]);
    }
}

template <class FrameType>
CircleDetector<FrameType>::~CircleDetector()
{
}

template <class FrameType>
bool CircleDetector<FrameType>::IsCircleInFrame()
{
    return mIsCircleInFrame;
}

template <class FrameType>
int CircleDetector<FrameType>::getCenterXCoordinate()
{
    return mCenterXCoordinate;
}

template <class FrameType>
int CircleDetector<FrameType>::getCenterYCoordinate()
{
    return mCenterYCoordinate;
}

template <class FrameType>
int CircleDetector<FrameType>::getCircleRadius()
{
    return mCircleRadius;
}

} /* libDroneVideo namespace */
} /* ghost namespace  */
