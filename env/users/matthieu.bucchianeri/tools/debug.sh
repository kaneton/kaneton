#!/bin/sh
## copyright     bucchianeri matthieu
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/debug.sh
##
## created       buckman   [mon nov 28 14:17:53 2005]
## updated       buckman   [mon nov 28 14:19:56 2005]
##

qemu -fda kaneton.img -boot a -s &
sleep 2
gdb core/kaneton/kaneton -x gdb.qemu
