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

#include "libDroneMovement/NavDataParser.hpp"
#include <gtest/gtest.h>

#include <cstdint>
#include <array>

namespace ghost
{
namespace libDroneMovement
{
namespace navData
{

class NavDataParserTest : public testing::Test
{
protected:
    /** Parser used during tests */
    NavDataParser parser;
};

TEST_F(NavDataParserTest, basic)
{

    NavDataParser::RawNavDataPacket test = {
        0x88, 0x77, 0x66, 0x55,
        0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00
    };

    EXPECT_TRUE(parser.parseNavDataPacket(test));
    EXPECT_TRUE(parser.isDroneFlying());
}

TEST_F(NavDataParserTest, NavDataDemo)
{

    NavDataParser::RawNavDataPacket test = {
        0x88, 0x77, 0x66, 0x55,
        0x94, 0x04, 0x80, 0x0f,
        0x39, 0x26, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x94, 0x00,

        0x00, 0x00, 0x02, 0x00,
        0x32, 0x00, 0x00, 0x00,

        0x00, 0x80, 0xa2, 0x44,
        0x00, 0xc0, 0x5b, 0xc5,
        0x00, 0xf6, 0x0d, 0x48,

        0xa5, 0x03, 0x00, 0x00,

        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    EXPECT_TRUE(parser.parseNavDataPacket(test));

    EXPECT_EQ(parser.getCurrentNavData().altitude, 933);
    EXPECT_EQ(parser.getCurrentNavData().theta, 1300);
}

} /* navData namespace */
} /* libDroneMovement namespace  */
} /* ghost namespace */
