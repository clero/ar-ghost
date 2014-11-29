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

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 2.6)
set(CMAKE_SYSTEM_PROCESSOR armv7)

set(FLOAT_ABI_SUFFIX "")

if (NOT SOFTFP)
  set(FLOAT_ABI_SUFFIX "hf")
endif()

set(CMAKE_C_COMPILER    arm-linux-gnueabi${FLOAT_ABI_SUFFIX}-gcc)
set(CMAKE_CXX_COMPILER  arm-linux-gnueabi${FLOAT_ABI_SUFFIX}-g++)
set(ARM_LINUX_SYSROOT /usr/arm-linux-gnueabi${FLOAT_ABI_SUFFIX} CACHE PATH "ARM cross compilation system root")

set(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabihf/)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(EXECUTABLE_OUTPUT_PATH ${TARGET_BUILD_DIR}${FLOAT_ABI_SUFFIX}/bin CACHE PATH "Build Directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${TARGET_BUILD_DIR}${FLOAT_ABI_SUFFIX}/lib CACHE PATH "Build Directory" FORCE)
