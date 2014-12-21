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

cmake_minimum_required(VERSION 2.8)

project(drone-ghost)

# Unit Test declaration
function (add_unit_test UnitTestName)
    IF(NOT BUILD_DESTINATION STREQUAL "TARGET")
        add_test(${UnitTestName} ${UnitTestName})
        add_custom_command(TARGET ${UnitTestName} POST_BUILD COMMAND ${CMAKE_COMMAND}
            -DTEST=${UnitTestName}
            -DTARGET=$<TARGET_FILE:${UnitTestName}>
            -P ${CMAKE_SOURCE_DIR}/cmake/unit_test_launcher.cmake)
    ENDIF()
endfunction()

# General C and CXX Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined -Wl,-z,noexecstack")

# Project Build Directories
set(HOST_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/host)
set(TARGET_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/target)

# Let's see where we are going to build
IF(BUILD_DESTINATION STREQUAL "TARGET")
    MESSAGE("Target Build requested")

    # File containing configuration to cross-compile
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-gnueabi.toolchain.cmake)

    # File containing every subdirectory to build (target only)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/target_projects.cmake)

ELSEIF(BUILD_DESTINATION STREQUAL "TARGETSOFTFP")
    MESSAGE("SoftFp Target Build requested")

    set(SOFTFP true)

    # File containing configuration to cross-compile
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-gnueabi.toolchain.cmake)

    # File containing every subdirectory to build (target only)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/target_softfp_projects.cmake)

ELSE()
    MESSAGE("Host Build requested")

    set(EXECUTABLE_OUTPUT_PATH ${HOST_BUILD_DIR}/bin CACHE PATH "Build Directory" FORCE)
    set(LIBRARY_OUTPUT_PATH ${HOST_BUILD_DIR}/lib CACHE PATH "Build Directory" FORCE)

    # File containing every subdirectory to build (host only)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/host_projects.cmake)

ENDIF()

IF (NOT SOFTFP)
    # File containing every subdirectory to build (host and hf target)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/multiplatform_projects.cmake)
ENDIF()
