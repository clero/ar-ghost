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

ROOTDIR=$(echo $PWD | sed 's/\(.*ar-ghost\)\(.*$\)/\1/')

# Allows to go to the project root directory
function croot {
    cd $ROOTDIR
}

# Generate CMake and make Current Directory host and target
function mm {
    # Target Build
    croot
    rm CMakeCache.txt
    cmake . -DBUILD_DESTINATION=TARGET
    cd - > /dev/null
    make $@

    # Target SoftFp Build
    croot
    rm CMakeCache.txt
    cmake . -DBUILD_DESTINATION=TARGETSOFTFP
    cd - > /dev/null
    make $@

    # Host Build
    croot
    rm CMakeCache.txt
    cmake .
    cd - > /dev/null
    make $@
}

# Generate CMake and make all project for host and target
function mma {
    # Target Build
    croot
    rm CMakeCache.txt
    cmake . -DBUILD_DESTINATION=TARGET
    make $@

    # Target SoftFp Build
    croot
    rm CMakeCache.txt
    cmake . -DBUILD_DESTINATION=TARGETSOFTFP
    make $@

    # Host Build
    rm CMakeCache.txt
    cmake .
    make $@
    cd - > /dev/null
}

# Clean all CMake and Build tmp files
function mmc {
    croot
    find . -name $(cat .gitignore 2> /dev/null | sed -e 's/^/-o -name /' -e '0,/^/ s/^-o -name//' \
        | tr "\n" " ") 2> /dev/null | xargs rm -r
    cd - > /dev/null
}

# PATH Update
PATH=$PATH:$ROOTDIR/build/host/bin:$ROOTDIR/tools/scripts
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTDIR/build/host/lib

# Display Information

echo "#################################################"
echo "#                Ghost Envsetup                 #"
echo "#################################################"
echo "Available commands:"
echo "    mm: Build current directory"
echo "    mma: Build the entire project"
echo "    mmc: Clean the project"
