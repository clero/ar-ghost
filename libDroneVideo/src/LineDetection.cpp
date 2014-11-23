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

#include "libDroneVideo/LineDetection.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <boost/log/trivial.hpp>

#include <stdexcept>
#include <cmath>

namespace ghost
{
namespace libDroneVideo
{

double LineDetection::detectLines(uint8_t* rawYUImg, const int width, const int height)
{
    // Defining the line
    LineDetection::coord_s P1 = {
        2000, -2000, 0, 2000, -2000, 0
    };
    LineDetection::coord_s P2 = {
        2000, -2000, 0, 2000, -2000, 0
    };

    if (!rawYUImg) {
        throw std::runtime_error("No video stream available");
    }

    // Creating openCv img from rawYUImg
    cv::Mat droneImg(cv::Size(width, height), CV_8UC2, rawYUImg);

    // Converting yuv format to grayscale
    cv::cvtColor(droneImg, droneImg, CV_YUV2GRAY_Y422);

    // Detecting edges
    cv::Canny(droneImg, droneImg, 50, 200, 3);

    // Apply HoughLine Transform to detect lines in the image
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(droneImg, lines, 1, CV_PI / 180, 100, 0, 0);


    double angle = proccessAxis('X', P1, P2, lines);
    if (std::isnan(angle)) {
        proccessAxis('Y', P1, P2, lines);
    }

    return angle;
}



double LineDetection::proccessAxis(char axis,
                                   LineDetection::coord_s& P1,
                                   LineDetection::coord_s& P2,
                                   std::vector<cv::Vec2f>& lines)
{
    unsigned int i = 0;
    int min = 2000;
    int max = -2000;
    int currentPoint = 0;
    cv::Point pt1, pt2;

    // Left, right and center lines research
    size_t lineNumberThreshold = 25;
    for (i = 0; i < std::min(lines.size(), lineNumberThreshold); i++) {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        if (axis == 'X') {
            currentPoint = pt1.x;
        } else if (axis == 'Y') {
            currentPoint = pt1.y;
        }
        if (currentPoint < min) {
            min = currentPoint;
            P1.minAbscisse = pt1.x;
            P1.minOrdonnee = pt1.y;
            P2.minAbscisse = pt2.x;
            P2.minOrdonnee = pt2.y;
        }
        if (currentPoint > max) {
            max = currentPoint;
            P1.maxAbscisse = pt1.x;
            P1.maxOrdonnee = pt1.y;
            P2.maxAbscisse = pt2.x;
            P2.maxOrdonnee = pt2.y;
        }
    }

    /*********
     *  We have points, we can release lines from here
     **********/

    double angle = 0.0;
    if ((min > -1000) && (max < 1000)) {
        angle = processLeftRight(P1, P2);
        processMiddle(axis, min, max, lines, P1, P2);
        if (axis == 'X') {
            computeDirection(P1.minAbscisse, P1.maxAbscisse, P1.middleAbscisse);
        } else if (axis == 'Y') {
            computeDirection(P1.minOrdonnee, P1.maxOrdonnee, P1.middleOrdonnee);
        }
    }

    return angle;
}

double LineDetection::processLeftRight(LineDetection::coord_s& P1, LineDetection::coord_s& P2)
{
    cv::Point pt1, pt2;
    double angleL = 0.0;

    // Left line processing
    pt1.x = P1.minAbscisse;
    pt1.y = P1.minOrdonnee;
    pt2.x = P2.minAbscisse;
    pt2.y = P2.minOrdonnee;

    angleL = (180 / CV_PI) * atan((double)(pt2.x - pt1.x) / (double)(pt2.y - pt1.y));

    BOOST_LOG_TRIVIAL(debug) << "Left line : A (" << pt1.x << ";" << pt1.y
                             << ") et B(" << pt2.x << ";" << pt2.y
                             << ")  =>  Angle : " << angleL;

    if ((angleL > -25) && (angleL < 25)) {
        computeRelativePosition(pt1.x, pt2.x, 'L');
    }

    double angleR = 0.0;
    // Right line processing
    pt1.x = P1.maxAbscisse;
    pt1.y = P1.maxOrdonnee;
    pt2.x = P2.maxAbscisse;
    pt2.y = P2.maxOrdonnee;
    angleR = (180 / CV_PI) * atan((double)(pt2.x - pt1.x) / (double)(pt2.y - pt1.y));

    BOOST_LOG_TRIVIAL(debug) << "Right line : A (" << pt1.x << ";" << pt1.y
                             << ") et B(" << pt2.x << ";" << pt2.y
                             << ")  =>  Angle : " << angleR;

    if ((angleR > -25) && (angleR < 25)) {
        computeRelativePosition(pt1.x, pt2.x, 'R');
    }

    return (angleR + angleL) / 2.0;
}

void LineDetection::processMiddle(char axis,
                                  int min,
                                  int max,
                                  std::vector<cv::Vec2f> lines,
                                  LineDetection::coord_s& P1,
                                  LineDetection::coord_s& P2)
{
    cv::Point pt1, pt2;
    unsigned int i = 0;
    int currentPoint = 0;
    int thresholdDetection = 80;
    double angle = 0.0;

    // Avoid noise detection
    size_t lineNumberThreshold = 25;
    for (i = 0; i < std::min(lines.size(), lineNumberThreshold); i++) {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        if (axis == 'X') {
            currentPoint = pt1.x;
        } else if (axis == 'Y') {
            currentPoint = pt1.y;
        }
        if ((currentPoint > min + thresholdDetection) &&
            (currentPoint < max - thresholdDetection)) {
            P1.middleAbscisse = pt1.x;
            P1.middleOrdonnee = pt1.y;
            P2.middleAbscisse = pt2.x;
            P2.middleOrdonnee = pt2.y;

            angle = (180 / CV_PI) * atan((double)(pt2.x - pt1.x) / (double)(pt2.y - pt1.y));

            BOOST_LOG_TRIVIAL(debug) << "Central line : A (" << pt1.x << ";" << pt1.y
                                     << ") et B(" << pt2.x << ";" << pt2.y
                                     << ")  =>  Angle : " << angle;
        }
    }
}

void LineDetection::computeRelativePosition(int x1, int x2, char pos)
{
    // width /2
    int pixelCenter = 160;
    if ((x1 > pixelCenter) && (x2 > pixelCenter) && (pos == 'L')) {
        BOOST_LOG_TRIVIAL(debug) << "Go LEFT !";
    }
    if ((x1 < pixelCenter) && (x2 < pixelCenter) && (pos == 'R')) {
        BOOST_LOG_TRIVIAL(debug) << "Go RIGHT !";
    }
}

void LineDetection::computeDirection(int min, int max, int middle)
{
    int distance = max - min;
    int interDistance = middle - min;
    BOOST_LOG_TRIVIAL(debug) << "Distance between left and right lines: " << distance;
    BOOST_LOG_TRIVIAL(debug) << "Distance between left and middle lines: " << interDistance;
    if (interDistance < 0.5 * distance) {
        BOOST_LOG_TRIVIAL(debug) << "Good Way !";
    } else {
        BOOST_LOG_TRIVIAL(debug) << "Bad Way !";
    }
}

} /* libDroneVideo namespace */
} /* ghost namespace  */
