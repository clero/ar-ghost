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

namespace ghost
{
namespace core
{
namespace mission
{
namespace step
{

/** Abstract class which describe a mission step implementation */
class MissionStep
{
public:

    /** Class constructor */
    MissionStep();
    virtual ~MissionStep();

    /** Launch the action realized by the step */
    virtual void doAction() = 0;
};

} /* step namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
