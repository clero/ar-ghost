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
#include <boost/log/trivial.hpp>

#include <cmath>
#include <vector>

namespace ghost
{
namespace libDroneVideo
{

/**
 * Analyse a given frame in order to detect lines in it
 * The lines detected are designed to let the drone be able
 * to follow them.
 * A normal line is composed of two lines of different colors.
 * One must be thinner than the other in order to let the drone
 * be able to detect is way.
 *
 * @tparam FrameType the type of frame to analyse
 */
template <class FrameType>
class LineDetector
{

public:

    LineDetector(const FrameType frame, const int imgWidth, const int mImgHeight);
    ~LineDetector();

    /**
     * Exception raised when the detection algorithm detect something odd.
     * Need to be catched by the user of this class in order to retry the
     * detection on another frame.
     */
    class LineDetectionError : public std::runtime_error
    {
    public:
        LineDetectionError(std::string const& msg) : std::runtime_error(msg)
        {
        }
    };

    double getLineAngle();
    bool isInRightWay();
    bool needToGoLeft();
    bool needToGoRight();

private:

    typedef std::vector<cv::Vec2f> FrameLines;

    /** First Element: X, Second: Y **/
    struct Line {
        cv::Point top;
        cv::Point bottom;
    };

    double mLineAngle;
    bool mIsInRightWay;
    bool mNeedToGoLeft;
    bool mNeedToGoRight;

    FrameType mFrame;

    const int mImgWidth;
    const int mImgHeight;
    static constexpr int mImgChannelNumber = 2;

    FrameLines mLines;

    Line mRightLine;
    Line mLeftLine;
    Line mMiddleLine;

    /**
     * Analyse HoughLine transform data to determine line position
     */
    void proccessAxis(bool processAxisX);
    void processMiddleAxis(bool proccessXAxis, int minAxisValue, int maxAxisValue);

    void computeLineAngle();
    void computeRelativePosition(bool computeLeftAxis);
    void computeDirection(int leftAxis, int rightAxis, int middleAxis);

    inline double determineAngleFromLine(const Line& line);
};

template <class FrameType>
LineDetector<FrameType>::LineDetector(const FrameType frame,
                                      const int imgWidth,
                                      const int imgHeight) :
    mLineAngle(0.0), mIsInRightWay(true), mFrame(frame), mImgWidth(imgWidth), mImgHeight(imgHeight),
    mLines()
{
    // Creating openCv img from raw YUV Image
    cv::Mat droneImg(cv::Size(mImgWidth, mImgHeight), CV_8UC2, mFrame.data());

    // Converting yuv format to grayscale
    cv::cvtColor(droneImg, droneImg, CV_YUV2GRAY_Y422);

    // Detecting edges
    cv::Canny(droneImg, droneImg, 50, 200, 3);

    // Apply HoughLine Transform to detect lines in the image
    cv::HoughLines(droneImg, mLines, 1, CV_PI / 180, 100, 0, 0);

    proccessAxis(true);

    // Let's see if we have detected the line correctly
    if (std::isnan(mLineAngle)) {
        // Let's try through the other axis
        proccessAxis(false);
    }

    // If the line remains not found, throw an exception
    if (std::isnan(mLineAngle)) {
        throw LineDetectionError("Line Angle is NaN");
    }
}

template <class FrameType>
LineDetector<FrameType>::~LineDetector()
{
}

template <class FrameType>
double LineDetector<FrameType>::getLineAngle()
{
    return mLineAngle;
}

template <class FrameType>
bool LineDetector<FrameType>::isInRightWay()
{
    return mIsInRightWay;
}

template <class FrameType>
bool LineDetector<FrameType>::needToGoLeft()
{
    return mNeedToGoLeft;
}

template <class FrameType>
bool LineDetector<FrameType>::needToGoRight()
{
    return mNeedToGoRight;
}

template <class FrameType>
void LineDetector<FrameType>::proccessAxis(bool processXAxis)
{
    // Variable used to detect our two lines
    int minAxisValue = 2000;
    int maxAxisValue = -2000;
    int currentAxisValue = 0;

    // Left, right and center lines research
    size_t lineNumberThreshold = 25;
    for (size_t i = 0; i < std::min(mLines.size(), lineNumberThreshold); i++) {

        cv::Point pt1, pt2;
        // Get Line informations
        float rho = mLines[i][0];
        float theta = mLines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));

        if (processXAxis) {
            currentAxisValue = pt1.x;
        } else {
            currentAxisValue = pt1.y;
        }

        // Check if we found the lines we search
        if (currentAxisValue < minAxisValue) {
            minAxisValue = currentAxisValue;
            mLeftLine.top = pt1;
            mLeftLine.bottom = pt2;
        }

        if (currentAxisValue > maxAxisValue) {
            maxAxisValue = currentAxisValue;
            mRightLine.top = pt1;
            mRightLine.bottom = pt2;
        }
    }

    if ((minAxisValue > -1000) && (maxAxisValue < 1000)) {

        // Update mLineAngle
        computeLineAngle();

        // We have our Right and Left Lines, check for the middle one
        processMiddleAxis(processXAxis, minAxisValue, maxAxisValue);

        if (processXAxis) {
            computeDirection(mLeftLine.top.x, mRightLine.top.x, mMiddleLine.top.x);
        } else {
            computeDirection(mLeftLine.top.y, mRightLine.top.y, mMiddleLine.top.y);
        }
    }
}

template <class FrameType>
void LineDetector<FrameType>::processMiddleAxis(bool processXAxis,
                                                int minAxisValue,
                                                int maxAxisValue)
{
    cv::Point pt1, pt2;
    int currentAxisValue = 0;

    // Avoid noise detection
    size_t lineNumberThreshold = 25;

    // We know were are left and right line, let's search the middle one
    // TODO: Avoid Code duplication
    for (size_t i = 0; i < std::min(mLines.size(), lineNumberThreshold); i++) {

        float rho = mLines[i][0];
        float theta = mLines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;

        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));

        if (processXAxis) {
            currentAxisValue = pt1.x;
        } else {
            currentAxisValue = pt1.y;
        }

        int thresholdDetection = 80;
        if ((currentAxisValue > minAxisValue + thresholdDetection) &&
            (currentAxisValue < maxAxisValue - thresholdDetection)) {
            mMiddleLine.top = pt1;
            mMiddleLine.bottom = pt2;

            BOOST_LOG_TRIVIAL(debug) << "Central line : A (" << pt1.x << ";" << pt1.y
                                     << ") et B(" << pt2.x << ";" << pt2.y << ")";
        }
    }
}

template <class FrameType>
double LineDetector<FrameType>::determineAngleFromLine(const Line& line)
{
    return (180 / CV_PI) *
           atan((double)(line.bottom.x - line.top.x) / (double)(line.bottom.y - line.top.y));
}


template <class FrameType>
void LineDetector<FrameType>::computeLineAngle()
{
    // Left line processing
    double angleL = determineAngleFromLine(mLeftLine);

    BOOST_LOG_TRIVIAL(debug) << "Left line : A (" << mLeftLine.top.x << ";" << mLeftLine.top.y
                             << ") et B(" << mLeftLine.bottom.x << ";" << mLeftLine.bottom.y
                             << ")  =>  Angle : " << angleL;


    if ((angleL > -25) && (angleL < 25)) {
        computeRelativePosition(true);
    }

    // Right line processing
    double angleR = determineAngleFromLine(mRightLine);

    BOOST_LOG_TRIVIAL(debug) << "Right line : A (" << mRightLine.top.x << ";" << mRightLine.top.y
                             << ") et B(" << mRightLine.bottom.x << ";" << mRightLine.bottom.y
                             << ")  =>  Angle : " << angleR;

    if ((angleR > -25) && (angleR < 25)) {
        computeRelativePosition(false);
    }

    mLineAngle = (angleR + angleL) / 2.0;
}

template <class FrameType>
void LineDetector<FrameType>::computeRelativePosition(bool computeLeftAxis)
{
    // width /2
    int pixelCenter = mImgWidth / 2;

    if (computeLeftAxis && (mLeftLine.top.x > pixelCenter) && (mLeftLine.bottom.x > pixelCenter)) {
        BOOST_LOG_TRIVIAL(debug) << "Go RIGHT !";
        mNeedToGoRight = true;
    } else {
        mNeedToGoRight = false;
    }

    if (!computeLeftAxis && (mRightLine.top.x < pixelCenter) &&
        (mRightLine.bottom.x < pixelCenter)) {
        BOOST_LOG_TRIVIAL(debug) << "Go LEFT !";
        mNeedToGoLeft = true;
    } else {
        mNeedToGoLeft = false;
    }

    // Check that results are meaningfull
    if (mNeedToGoLeft && mNeedToGoRight) {
        // We need to go left and right, something is wrong
        throw LineDetectionError(
                  "when analysing lines: we need to go left and right at the same time");
    }
}

template <class FrameType>
void LineDetector<FrameType>::computeDirection(int leftAxis, int rightAxis, int middleAxis)
{
    int distance = rightAxis - leftAxis;
    int interDistance = middleAxis - leftAxis;
    BOOST_LOG_TRIVIAL(debug) << "Distance between left and right lines: " << distance;
    BOOST_LOG_TRIVIAL(debug) << "Distance between left and middle lines: " << interDistance;

    if (interDistance < 0.5 * distance) {
        BOOST_LOG_TRIVIAL(debug) << "Good Way !";
        mIsInRightWay = true;
    } else {
        BOOST_LOG_TRIVIAL(debug) << "Bad Way !";
        mIsInRightWay = false;
    }
}

} /* libDroneVideo namespace */
} /* ghost namespace  */
