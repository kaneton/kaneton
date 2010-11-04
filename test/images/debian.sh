#! /bin/bash
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/test/images/debian.sh
#
# created       julien quintard   [fri oct 29 13:12:53 2010]
# updated       julien quintard   [thu nov  4 00:34:27 2010]
#

#
# ---------- information ------------------------------------------------------
#
# this script generates a Debian Live system that can be used to generate
# an image from a kaneton snapshot in a confined and immutable environment.
#

#
# ---------- constants --------------------------------------------------------
#

ROOT=$(pwd)

DATA="${ROOT}/data"
DIRECTORY="${ROOT}/.debian"
IMAGE="${ROOT}/debian.iso"

PACKAGES="bzip2 patch make gcc mtools python"
USER="user"

#
# ---------- script -----------------------------------------------------------
#

# remove the temporary directory, just in case.
rm -Rf ${DIRECTORY}

# create and move to the temporary directory.
mkdir ${DIRECTORY}
cd ${DIRECTORY}

# configure the environment.
lh_config --binary-images iso --architecture i386 --syslinux-timeout 1 --syslinux-menu disabled --username ${USER} --packages "${PACKAGES}"

# create the directories which will contain the files to replace in the live system.
mkdir -p ${DIRECTORY}/config/chroot_local-includes/etc/init.d/
mkdir -p ${DIRECTORY}/config/chroot_local-includes/home/${USER}/

# copy the kaneton script which will be triggered at boot time and will
# execute another script, referred to as 'kaneton.script' below.
cp ${DATA}/etc::init.d::kaneton.sh ${DIRECTORY}/config/chroot_local-includes/etc/init.d/kaneton.sh

# and give it the execution permission.
chmod +x ${DIRECTORY}/config/chroot_local-includes/etc/init.d/kaneton.sh

# copy the script which will compile a kaneton snapshot into an image.
cp ${DATA}/home::user::compile.sh ${DIRECTORY}/config/chroot_local-includes/home/${USER}/compile.sh

# copy the patch for the 'live-initramfs' init script so that the system does not hang
# when shutting down.
# the patch has been produced by: diff -Naur /etc/init.d/live-initramfs /etc/init.d/live-initramfs.new
cp ${DATA}/etc::init.d::live-initramfs.patch ${DIRECTORY}/config/chroot_local-includes/etc/init.d/live-initramfs.patch

# generate a hook in order to specify the live system to take the
# 'kaneton.sh' init script into account.
cat <<END >${DIRECTORY}/config/chroot_local-hooks/99-kaneton
#! /bin/sh

# add the kaneton.sh init script
update-rc.d kaneton.sh defaults

# patch the live-initramfs init script
patch -u /etc/init.d/live-initramfs /etc/init.d/live-initramfs.patch

# create the mount point.
mkdir /mnt/snapshot
END

# give the execution permission to this script so that it gets executed.
chmod +x ${DIRECTORY}/config/chroot_local-hooks/99-kaneton

# generate a book in order to replace the isolinux.cfg file.
cat <<EOF >${DIRECTORY}/config/binary_local-hooks/00-kaneton
#! /bin/sh

cat <<FOE >binary/isolinux/isolinux.cfg
serial 0 9600
timeout 0
default live

LABEL live
  MENU LABEL Start Debian Live
  kernel /live/vmlinuz1
  append initrd=/live/initrd1.img boot=live live-getty union=aufs console=tty0 console=ttyS0,9600n8
FOE
EOF

# give the execution permission to this script so that it gets executed.
chmod +x ${DIRECTORY}/config/binary_local-hooks/00-kaneton

# finally, launch the build process.
lh_build

# move the generated image.
mv ${DIRECTORY}/binary.iso ${IMAGE}

# come back to the root directory.
cd ${ROOT}

# remove the temporary directory.
rm -Rf ${DIRECTORY}
