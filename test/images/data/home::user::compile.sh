#! /bin/bash

USER="user"

error()
{
  echo "[error] ${1}" >> /home/${USER}/.error

  exit 1
}

if [ "${PWD}" != "/home/${USER}" ] ; then
  error "the compile script should be run from /home/${USER}/"
fi

rm -Rf kaneton/ || error "unable to remove the kaneton/ directory"

touch .error
touch .log

source environment.sh || error "unable to source the environment file"

tar xjf kaneton.tar.bz2 >>/home/${USER}/.log 2>&1 || error "unable to extract the kaneton snapshot"

mkdir -p kaneton/test/store/bundle/${KANETON_PLATFORM}.${KANETON_ARCHITECTURE}/ >>/home/${USER}/.log 2>&1 || error "unable to create the bundles directory"
cp kaneton.lo kaneton/test/store/bundle/${KANETON_PLATFORM}.${KANETON_ARCHITECTURE}/ >>/home/${USER}/.log 2>&1 || error "unable to move the kaneton tests bundle"

cd kaneton >>/home/${USER}/.log 2>&1 || error "unable to enter the kaneton/ root directory"

make initialize >>/home/${USER}/.log 2>&1 || error "unable to initialize the environment"

make >>/home/${USER}/.log 2>&1 || error "unable to compile the kaneton kernel"
make build >>/home/${USER}/.log 2>&1 || error "unable to build the kaneton image"
make install >>/home/${USER}/.log 2>&1 || error "unable to install the kernel on the image"
