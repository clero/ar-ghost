#!/bin/sh

#
# Our project need C++11 to work. As the default libc embedded in the drone
# does not owns all C++11 capacity, we need to set up a chroot environment
# on a usb key.
# This is also allowing us to use the FPU which is ignored by the default
# embedded kernel. The FPU makes OpenCV library run fine on the Ar Drone.
#

mount /dev/sda1 /mnt
mount -t proc proc /mnt/proc
mount -t sysfs sys /mnt/sys
mount -o bind /dev /mnt/dev

# Directory containing sent data
mount /data/video /mnt/drone

# Needed to use VideoSharing library
mount /tmp /mnt/tmp

# Launch the Chroot
chroot /mnt /bin/bash
