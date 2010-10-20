
#! /bin/bash

SCRIPT=${$}

error()
{
  echo "[error] ${1}" > /mnt/snapshot/error

  halt
}

if [ "${1}" != "start" ] ; then
  exit 0
fi

mount /dev/sdb1 /mnt/snapshot || error "unable to mount the snapshot image"

rm -Rf /home/compile/test || error "unable to remove the test/ directory"

mkdir /home/compile/test || error "unable to create a test/ directory"

cp /mnt/snapshot/kaneton.tar.bz2 /home/compile/test || error "unable to copy the snapshot into the test/ directory"

chown -R compile:compile /home/compile/test || error "unable to change the ownership of the snapshot file"

# XXX[ARCHITECTURE='ia32']

commands=$(cat <<EOF
cd /home/compile/test/ &&
tar xjf kaneton.tar.bz2 &&
cd kaneton &&
export KANETON_USER='test' &&
export KANETON_HOST='linux/ia32' &&
export KANETON_PYTHON='/usr/bin/python' &&
export KANETON_PLATFORM='ibm-pc' &&
export KANETON_ARCHITECTURE='ia32/educational' &&
make initialize &&
make &&
make build &&
make install
EOF
)

su compile -c "${commands}" > /mnt/snapshot/log 2>&1 || error "unable to compile the snapshot"

cp /home/compile/test/kaneton/environment/profile/user/test/test.img /mnt/snapshot/kaneton.img || error "unable to copy the snapshot image"

rm -Rf /home/compile/test || error "unable to remove the test/ directory"

umount /mnt/snapshot || error "unable to unmount the snapshot"

halt
