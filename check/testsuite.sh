#!/bin/sh
if [ $# -ne 4 ]; then
    echo "usage $0 <test_suite> <tarball> <stage> <group>"
    exit 0
fi

echo "Configuring for $3 / $4"
old=`pwd`
mkdir /tmp/moulette
tar -C /tmp/moulette -xzf $2
tar -C /tmp/moulette/kaneton -xzf $1
cd /tmp/moulette/kaneton
sed -i 's/^_IMAGE_.*:=.*/_IMAGE_ := \/tmp\/moulette\/kaneton\/kaneton.img/g' env/users/moulette/user.conf
echo "Ready. Ctrl-C to exit, Enter to proceed."
read
export KANETON_USER=moulette
make init
make install
if [ "$?" -ne 0 ]; then
    exit 1
fi
mcopy menu.lst a:/boot/GRUB
qemu -fda kaneton.img -serial pty &
pr=$!
sleep 10s

echo
echo
echo -n "Enter pty device: "
read DEV
if [ "$DEV" = "" ]; then
    DEV=/dev/ttyS0
    echo "using default $DEV"
fi
cd check
./check.py $DEV $3 $4 > ${4}_${3}.html
cp /tmp/moulette/kaneton/check/${4}_${3}.html $old
kill -9 $pr
echo "Cleaning"
rm -Rf /tmp/moulette
