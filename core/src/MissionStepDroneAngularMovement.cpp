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

#include "core/MissionStepDroneAngularMovement.hpp"

#include <boost/log/trivial.hpp>

#include <chrono>
#include <thread>

namespace ghost
{
namespace core
{
namespace mission
{
namespace step
{

MissionStepDroneAngularMovement::MissionStepDroneAngularMovement(
    libDroneMovement::Drone& drone,
    const int angleToMake)
    : mDrone(drone), mAngleToMake(angleToMake)
{
}

MissionStepDroneAngularMovement::~MissionStepDroneAngularMovement()
{
}

int MissionStepDroneAngularMovement::determineAngleCible()
{
    // Drone Angle between 0 and 360
    int currentAngle = (int)mDrone.getPsiAngle() + 180;
    int angleCible = (currentAngle + mAngleToMake) % 360;

    return angleCible;
}

void MissionStepDroneAngularMovement::doAction()
{
    float angleCible = determineAngleCible();
    float yawSpeed = 0.0;

    // Calculate speed
    if (mAngleToMake > 180) {
        // We need to go left
        yawSpeed = -0.7;
    } else {
        // We need to go right
        yawSpeed = 0.7;
    }

    float currentAngle = 0.0;
    do {
        currentAngle = mDrone.getPsiAngle() + 180.0;
        BOOST_LOG_TRIVIAL(debug) << "Desired Angle : " << angleCible
                                 << ", current : " << currentAngle;
        mDrone.movement(0.0, 0.0, 0.0, yawSpeed);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    } while (std::abs(angleCible - (int)currentAngle) > 5);
}

} /* step namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
