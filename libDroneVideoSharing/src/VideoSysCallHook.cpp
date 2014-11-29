/*
 * Copyright 2014 Jules Cléro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * Override system calls to share the camera with parrot internal program.
 * This library has to be preloaded with program.elf launch.
 */

#define _GNU_SOURCE 1

#include "libDroneVideoSharing/FunctionHooker.hpp"
#include "libDroneVideoSharing/CameraDefinition.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/videodev2.h>

// We're overriding open, so we have to define it before fcntl include
#define open HOOKED_OPEN
#include <fcntl.h>
#undef open

#include <cstdint>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <iostream>

static const std::string gLibC = "/lib/libc.so.6";

static int gCamHFd = -1;
static int gCamVFd = -1;

static off_t gBufferOffsetsCamV[] = {
    0, 0, 0, 0, 0, 0, 0, 0
};
static void* gBuffersCamV[] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

extern "C"
int open(const char* file, int oflag, ...)
{
    using namespace ghost::libDroneVideoSharing;

    static int (* libc_open) (const char* file, int oflag, ...) =
        FunctionHooker::hookSetup<int (*)(const char*, int, ...)>(gLibC, "open");
    int open_handle;

    open_handle = libc_open(file, oflag);

    // Check if program.elf try to open cameras
    if (strcmp(file, CAMV_DEVICE) == 0) {
        gCamVFd = open_handle;
    } else if (strcmp(file, CAMH_DEVICE) == 0) {
        gCamHFd = open_handle;
    }

    return open_handle;
}

extern "C"
void* mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    using namespace ghost::libDroneVideoSharing;

    static void* (* libc_mmap) (void* addr, size_t len, int prot, int flags, int fd, off_t offset) =
        FunctionHooker::hookSetup<void* (*)(void*, size_t, int, int, int, off_t)>(gLibC, "mmap");
    void* mmap_buffer;

    if (gCamHFd != -1 && fd == gCamHFd) {
        // To save memory, we don't allow program.elf to mmap the horizontal camera
        return NULL;
    }

    // Normal Call
    mmap_buffer = libc_mmap(addr, len, prot, flags, fd, offset);

    if (gCamVFd != -1 && fd == gCamVFd) {
        mmap_buffer = libc_mmap(addr, len, prot, flags, fd, offset);
        int index = 0;
        bool found = false;

        // Let's find the current buffer index
        while (index++ < 8 && !found) {
            if (offset == gBufferOffsetsCamV[index]) {
                found = true;
            }
        }

        if (found) {
            gBuffersCamV[index] = mmap_buffer;
        }
    }

    return mmap_buffer;
}

extern "C"
int ioctl(int fd, int request, ...)
{
    using namespace ghost::libDroneVideoSharing;

    static int (* libc_ioctl) (int fd, int request, ...) =
        FunctionHooker::hookSetup<int (*)(int, int, ...)>(gLibC, "ioctl");

    if (gCamHFd != -1 && fd == gCamHFd) {
        // To save memory, we don't allow program.elf to ioctl the horizontal camera
        return -1;
    }

    // Get variadic arguments
    va_list arguments;
    va_start(arguments, request);

    // Gather variadic arguments fot libc_ioctl call
    void* arg = va_arg(arguments, void*);
    int ioctl_val = libc_ioctl(fd, request, arg);

    switch (request) {
    case VIDIOC_QUERYBUF: {
        if (gCamVFd != -1 && fd == gCamVFd) {
            // Get the index of the buffer
            int index = static_cast<v4l2_buffer*>(arg)->index;

            // Check bounds
            if (index >= 0 && index < 8) {
                // Save offset for matching with mmap
                gBufferOffsetsCamV[index] = static_cast<v4l2_buffer*>(arg)->m.offset;
            }
        }
    }
    break;
    case VIDIOC_DQBUF: {
        if (gCamVFd != -1 && fd == gCamVFd) {
            int index = static_cast<v4l2_buffer*>(arg)->index;

            // If Ready Flag is not present, or if buffer is not present
            if (!(access(CAMV_READY, F_OK) != -1) ||
                !(access(CAMV_BUFFER, F_OK) != -1)) {

                int buffer_file = open(CAMV_BUFFER,
                                       O_WRONLY | O_CREAT | O_TRUNC, 0666);

                write(buffer_file, gBuffersCamV[index], CameraDefinition::mVerticalImgSize);
                close(buffer_file);

                int readyFile =
                    open(CAMV_READY, O_WRONLY | O_CREAT, 0666);
                close(readyFile);
            }
        }
    }
    break;
    }

    return ioctl_val;
}
