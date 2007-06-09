#!/bin/sh
qemu -fda kaneton.img -serial pty $* &
PID=$!
sleep 1
for i in /dev/pts/*; do
    i=`echo $i | sed 's/\/dev\///1'`
    who |grep $i > /dev/null
    if [ -w /dev/$i -a $? -ne 0 ]; then
	echo "On /dev/$i"
	cat /dev/$i
	kill -9 $PID
	exit 0;
    fi
done
echo "cannot find pts !"
exit 1
