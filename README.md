Ar-ghost Project
================

The Ar-Ghost project has been realized during the 5th year of five student from the Insa Toulouse.
This project aims to make an Ar.Drone 2.0 fly autonomously with a parcel which
can be delivered.
To do that, we had designed an electronic system which can handle the parcel and
some embedded libraries which allows to interact with the drone. The Drone should be able to load
the parcel to follow a line on the ground autonomously and to drop the packet.

For now, there is three main components:
    * libDroneMovement: allows to interact with parrot internal program through socket
    * libDroneVideo: allows to analyse frame from the drone internal camera
    * libDroneVideoShare: allows to share the vertical camera between parrot's program.elf and our embedded program

Dependency
----------

To be able to build our project you will need:

    * arm-linux-gnueabi-gcc compiler
    * arm-linux-gnueabihf-gcc compiler
    * Cmake
    * [Boost](https://github.com/boostorg/boost "Boost official github") (with arm version under /usr/arm-linux-gnueabihf)
    * [OpenCv](https://github.com/Itseez/opencv/ "OpenCv official github") (with arm version under /usr/arm-linux-gnueabihf)

It can seems awkward that this project needs an hard and a soft gcc version. The problem is that Parrot, which produce
the ArDrone 2.0 has decided to not use the internal FPU of the ArmV7 processor of the drone. Thus, the library which
allows our project to share the camera with the drone has to be compiled for SoftFp architecture.
The Hard floating point compiler is for the rest of our project. Indeed, the ArchLinux system we set up to embed libraries
easily allow us to use hard floating point (it is better for image processing). Moreover, it allow us to use a recent
libc with complete C++11 support.

HOW-TO
------

### Cross-compile and install OpenCv ###

```bash
git clone https://github.com/Itseez/opencv.git
cd opencv/platforms/linux/
git checkout 2.4.9.1

mkdir build
cd build

cmake -DCMAKE_TOOLCHAIN_FILE=../arm-gnueabi.toolchain.cmake ../../..

make && make install

cd install

mv include/* /usr/arm-linux-gnueabihf/include
mv lib/* /usr/arm-linux-gnueabihf/lib
mv share/* /usr/arm-linux-gnueabihf/share
```
You may have to edit your `arm-gnueabi.toolchain.cmake` file in order to allow cmake to find your cross-compiler.
In general, just removing the version in the compiler name is enough.

### Cross-compile and install Boost ###

```bash
mkdir boost
cd boost
mkdir build

wget http://sourceforge.net/projects/boost/files/boost/1.57.0/boost_1_57_0.tar.gz/download

tar -xvf boost_1_57_0.tar.gz

cd boost_1_57_0

./bootstrap.sh --prefix=../build
```
Edit file project-config.jam by adding `using gcc : arm : arm-linux-gnueabihf-g++ ;` after imports

```bash
b2 toolset=gcc-arm
bjam install

cd ../build

mv include/boost /usr/arm-linux-gnueabihf/include/
mv lib/* /usr/arm-linux-gnueabihf/lib/
```

### Cross Compile and install Gtest ###

```bash
wget https://googletest.googlecode.com/files/gtest-1.7.0.zip
unzip gtest-1.7.0.zip

mkdir build
cd build

CC=arm-linux-gnueabihf-gcc CXX=arm-linux-gnueabihf-g++ cmake ../gtest-1.7.0

mkdir /usr/arm-linux-gnueabihf/gtest-1.7/
mv libgtest.a libgtest_main.a /usr/arm-linux-gnueabihf/gtest-1.7/
```

### Project Compilation ###

```bash
. tools/scripts/envsetup.sh

mma -j
```
