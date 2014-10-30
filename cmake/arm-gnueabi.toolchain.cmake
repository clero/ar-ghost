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

set(CMAKE_C_COMPILER arm-linux-gnueabi-gcc-4.9.1)
set(CMAKE_CXX_COMPILER arm-linux-gnueabi-g++)
set(ARM_LINUX_SYSROOT /usr/arm-linux-gnueabi)

set(EXECUTABLE_OUTPUT_PATH ${TARGET_BUILD_DIR}/bin CACHE PATH "Build Directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${TARGET_BUILD_DIR}/lib CACHE PATH "Build Directory" FORCE)
