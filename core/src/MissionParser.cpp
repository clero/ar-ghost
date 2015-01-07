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

#include "core/MissionParser.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>
#include <boost/foreach.hpp>

namespace ghost
{
namespace core
{
namespace mission
{
namespace factory
{

const std::map<const std::string,
               const MissionStepFactory::MissionStepType> MissionParser::mFileKeywordToCommandMap =
{ { "LiftParcel", MissionStepFactory::LiftParcel },
  { "DropParcel", MissionStepFactory::DropParcel },
  { "DroneAngularMovement", MissionStepFactory::DroneAngularMovement } };

MissionParser::MissionParser()
{
}

MissionStepFactory::ParsedMissionSteps MissionParser::parseMissionFile(
    const std::string& jsonMissionFile)
{
    boost::property_tree::ptree missionTree;
    boost::property_tree::read_json(jsonMissionFile, missionTree);
    MissionStepFactory::ParsedMissionSteps parsedMissionSteps(20);

    BOOST_FOREACH(boost::property_tree::ptree::value_type & missionStepType, missionTree)
    {
        BOOST_LOG_TRIVIAL(debug) << "Parsing mission step : "
                                 << missionStepType.first;

        try {
            parsedMissionSteps.push_back(mFileKeywordToCommandMap.at(missionStepType.first));
        }
        catch (std::out_of_range& e) {
            BOOST_LOG_TRIVIAL(error) << "While parsing mission : Undefined mission step : "
                                     << missionStepType.first;
            throw e;
        }
    }

    return parsedMissionSteps;
}

} /* factory namespace  */
} /* mission namespace  */
} /* core namespace  */
} /* ghost namespace */
