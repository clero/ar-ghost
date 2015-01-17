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

#include "core/MissionStepFactory.hpp"

#include <string>
#include <map>

namespace ghost
{
namespace core
{
namespace mission
{
namespace factory
{

/** Parser class which allows to retrieve mission steps from a json file */
class MissionParser
{
public:

    /** Class constructor */
    MissionParser();

    /**
     * Retrieve MissionStep child classes to create from a json file
     *
     * @param[in] jsonMissionFile the file to parse
     */
    MissionStepFactory::ParsedMissionSteps parseMissionFile(const std::string& jsonMissionFile);

private:

    /** Static map to link json keys to MissionStepType enumerate */
    static const std::map<const std::string,
                          const MissionStepFactory::MissionStepType> mFileKeywordToCommandMap;

};

} /* factory namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
