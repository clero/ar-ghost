#
# Copyright 2014 Jules Cléro
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

### Core ###

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS log thread system REQUIRED)
find_package (Threads REQUIRED)

include_directories(
    include
    ${PROJECT_SOURCE_DIR}/libDroneMovement/include
    ${PROJECT_SOURCE_DIR}/libDroneVideo/include
    ${PROJECT_SOURCE_DIR}/libDroneVideoSharing/include
    ${PROJECT_SOURCE_DIR}/libLineFollowing/include)

add_executable(
    main
    src/Main.cpp
    src/EntryPoint.cpp
    src/DestinationPointFlyer.cpp
    src/Mission.cpp
    src/MissionStep.cpp
    src/MissionStepSendCarryingSystemCommand.cpp
    src/SerialCommunicator.cpp
    src/MissionParser.cpp
    src/MissionStepFactory.cpp
    src/CarryingSystemAtCommandFactory.cpp)

link_directories(${LIBRARY_OUTPUT_PATH})

target_link_libraries(main DroneMovement DroneVideo LineFollowing ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS main RUNTIME DESTINATION bin)

### Tests ###

add_executable(
    CarryingSystemCommunicationTest
    test/CarryingSystemCommunicationTest.cpp
    src/SerialCommunicator.cpp
    src/CarryingSystemAtCommandFactory.cpp)

target_link_libraries(CarryingSystemCommunicationTest ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS CarryingSystemCommunicationTest RUNTIME DESTINATION bin)
