#!/bin/sh
## copyright     bucchianeri matthieu
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/debug.sh
##
## created       buckman   [mon nov 28 14:17:53 2005]
## updated       matthieu bucchianeri   [tue jan 17 23:42:10 2006]
##

qemu -fda kaneton.img -boot a -s -dummy-net &
sleep 2
if [ "$1" = "boot" ]; then
    echo "debugging bootloader"
    gdb core/bootloader/bootloader -x gdb.qemu
else
    echo "debugging kernel"
    gdb core/kaneton/kaneton -x gdb.qemu
fi
