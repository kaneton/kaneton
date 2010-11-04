#! /bin/bash

DEVICE="/dev/hdb"
USER="user"

error()
{
  # retrieve the user's error and log files, if present.
  if [ -f /home/${USER}/.log ] ; then
    cat /home/${USER}/.log >>/mnt/snapshot/.log
  fi

  if [ -f /home/${USER}/.error ] ; then
    cat /home/${USER}/.error >>/mnt/snapshot/.error
  fi

  # display the error in the error file.
  echo "[error] ${1}" >>/mnt/snapshot/.error

  # umount the snapshot.
  umount /mnt/snapshot

  # halt
  halt

  exit 1
}

if [ "${1}" != "start" ] ; then
  exit 0
fi

#
# if the snapshot device exists.
#
if [ -e "${DEVICE}" ] ; then
  # mount the snapshot disk.
  mount ${DEVICE} /mnt/snapshot

  # exit silently if in debug mode.
  if [ -e "/mnt/snapshot/.debug" ] ; then
    rm -f "/mnt/snapshot/.debug"

    umount /mnt/snapshot

    exit 0
  fi

  # remove the error and log files.
  rm -f /mnt/snapshot/.error
  rm -f /mnt/snapshot/.log

  # copy the files necessary for the test user to compile kaneton into a bootable image:
  # the kaneton snapshot, the test environment variables, the compile script and the
  # test bundle containing the already compiled tests for this machine.
  cp /mnt/snapshot/kaneton.tar.bz2 /home/${USER}/ >>/mnt/snapshot/.log 2>&1 || error "unable to copy the kaneton snapshot to the user's home directory"
  cp /mnt/snapshot/environment.sh /home/${USER}/ >>/mnt/snapshot/.log 2>&1 || error "unable to copy the environment file to the user's home directory"
  cp /mnt/snapshot/kaneton.lo /home/${USER}/ >>/mnt/snapshot/.log 2>&1 || error "unable to copy the kaneton tests bundle to the user's home directory"

  # give ownership to the test user.
  chown -R ${USER}:users /home/${USER} >>/mnt/snapshot/.log 2>&1 || error "unable to grant ownership on the user's home directory files to the user"

  # trigger the compile script under the user's identity.
  su -l -c "/bin/bash /home/${USER}/compile.sh" ${USER} >>/mnt/snapshot/.log 2>&1 || error "unable to run the compile script under the user's identity"

  # retrieve the generated bootable image.
  cp /home/${USER}/kaneton/environment/profile/user/test/test.img /mnt/snapshot/kaneton.img >>/mnt/snapshot/.log 2>&1 || error "unable to copy the kaneton image"

  # retrieve the compilation error and log files.
  cat /home/${USER}/.error >>/mnt/snapshot/.error 2>&1 || error "unable to retrieve the compilation process' error file"
  cat /home/${USER}/.log >>/mnt/snapshot/.log 2>&1 || error "unable to retrieve the compilation process' log file"

  # remove the error and log files.
  rm -f /home/${USER}/.error
  rm -f /home/${USER}/.log

  # remove the error file if empty so that the process is not considered as a failure.
  if [ -f /mnt/snapshot/.error ] ; then
    if [ $(stat --format="%s" /mnt/snapshot/.error) -eq 0 ] ; then
      rm /mnt/snapshot/.error
    fi
  fi

  # umount the kaneton snapshot
  umount /mnt/snapshot || error "unable to unmount the snapshot"

  # stop the virtual machine.
  halt
else
  echo "[error] the snapshot image does not seem to be present through ${DEVICE}"
fi
