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

#include <cstdint>
#include <vector>

namespace ghost
{
namespace libDroneVideo
{

class LineDetection
{

public:

    /** TODO: DOXYGEN
     * It takes an input image in IplImage format,
     * It returns the processed image in IplImage format
     * It prints the angle, the coordinates of the detected lines,
     * the information about the drone's direction,
     * and if the drone must go right or left whether the line is
     * to the right or to the left
     *
     * It also takes a pointer to an angle as an input !
     * The angle's value is modified during the process
     * so as it can be used in other applications
     */
    static double detectLines(uint8_t* rawYUImg, const int width, const int height);

private:

    LineDetection();
    ~LineDetection();

    /** TODO: rename coordinates of lines */
    struct coord_s {
        int minAbscisse;
        int maxAbscisse;
        int middleAbscisse;
        int minOrdonnee;
        int maxOrdonnee;
        int middleOrdonnee;
    };

    static double proccessAxis(char axis, coord_s& P1, coord_s& P2, std::vector<cv::Vec2f>& lines);
    static double processLeftRight(coord_s& P1, coord_s& P2);
    static void processMiddle(char axis,
                              int min,
                              int max,
                              std::vector<cv::Vec2f> lines,
                              coord_s& P1,
                              coord_s& P2);
    static void computeRelativePosition(int x1, int x2, char pos);
    static void computeDirection(int min, int max, int middle);
};

} /* libDroneVideo namespace */
} /* ghost namespace  */
