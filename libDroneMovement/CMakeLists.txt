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

### libDroneMovement ###

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS log thread system REQUIRED)
find_package (Threads REQUIRED)

include_directories(
    include/)

add_library(
    DroneMovement
    src/AtCommandFactory.cpp
    src/AtCommandSender.cpp)


### Unit Test ###

enable_testing()
find_package(GTest REQUIRED)
find_package (Threads REQUIRED)

include_directories(
    include/)

add_executable(
    AtCommandFactoryUnitTest
    test/AtCommandFactoryUnitTest.cpp
    src/AtCommandFactory.cpp)

target_link_libraries(
    AtCommandFactoryUnitTest ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS AtCommandFactoryUnitTest RUNTIME DESTINATION test)

add_unit_test(AtCommandFactoryUnitTest)
