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

#include "core/EntryPoint.hpp"
#include <libDroneVideo/LineDetector.hpp>

#include <boost/log/trivial.hpp>

#include <chrono>
#include <thread>

namespace ghost
{
namespace core
{

EntryPoint::EntryPoint(const std::string& droneIpAdress, const unsigned int flyingStepNumber)
    : mDrone(droneIpAdress), mFrameGrabber(), mFlyingStepNumber(flyingStepNumber),
    mCurrentFlyingStep(0), mLineLostCounter(0), mInputAutoPilot(), mOutputAutoPilot()
{
    SystemDrone_init(&mOutputAutoPilot);
}

inline bool EntryPoint::needImageUpdate() const
{
    return (mCurrentFlyingStep % 10) == 0;
}

inline bool EntryPoint::isLineFound() const
{
    return mLineLostCounter < 5;
}

void EntryPoint::updateAutoPilotInputWithFrame()
{
    // Frame Analysis
    try {
        libDroneVideo::LineDetector<libDroneVideo::FrameGrabber::DroneVerticalFrame> lineDetector(
            mFrameGrabber.getNextVerticalFrame(),
            320,
            240);

        mInputAutoPilot.LineAngle = lineDetector.getLineAngle();
        mInputAutoPilot.GoRight = lineDetector.needToGoRight();
        mInputAutoPilot.GoLeft = lineDetector.needToGoLeft();

        // Reset the lost line counter as we found it.
        mLineLostCounter = 0;
    }
    catch (libDroneVideo::LineDetector<
               libDroneVideo::FrameGrabber::DroneVerticalFrame>::LineDetectionError e) {

        BOOST_LOG_TRIVIAL(warning) << "No line detected in frame, skipping";

        // we don't see the line, let's assume we are good for now
        mInputAutoPilot.LineAngle = 0.0;
        mInputAutoPilot.GoLeft = false;
        mInputAutoPilot.GoRight = false;

        /**
         * Increment the lost line counter in order to take action
         * if we have completely lost the line
         */
        mLineLostCounter++;
    }

    mInputAutoPilot.LineDetected = isLineFound();
    mInputAutoPilot.ImageUpdate = true;

    // TODO: use this option
    mInputAutoPilot.RightWay = true;
}

void EntryPoint::doNextAction()
{
    if (needImageUpdate()) {
        updateAutoPilotInputWithFrame();
    } else {
        mInputAutoPilot.ImageUpdate = false;
    }

    // Get current Psi Angle
    mInputAutoPilot.currentAngle = mDrone.getPsiAngle() / 1000.0;

    // Calcualte the needed movement
    SystemDrone(&mInputAutoPilot, &mOutputAutoPilot);

    mDrone.movement(
        mOutputAutoPilot.Roll,
        mOutputAutoPilot.Pitch,
        mOutputAutoPilot.Gaz,
        mOutputAutoPilot.Yaw);

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void EntryPoint::start()
{
    mDrone.takeOff();
    // Drop Frames
    mFrameGrabber.getNextVerticalFrame(),
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    for (mCurrentFlyingStep = 0; mCurrentFlyingStep < mFlyingStepNumber; mCurrentFlyingStep++) {
        doNextAction();
    }
    mDrone.land();
}

} /* core namespace  */
} /* ghost namespace */
