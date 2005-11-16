#! /bin/bash

## copyright quintard julien
## 
## kaneton
## 
## init.sh
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Wed Nov 16 22:41:39 2005   mycure
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: src/env/
#
# this script is called critical because it does not depend of the
# operating system. this is the script used to install the environment
# so it cannot use the environment variables, binaries, functions etc..
#
# for the moment this script uses UNIX tools: sed, cat, rm, make etc..
#

#
# ---------- globals ----------------------------------------------------------
#

MACHINE_CONF=""
USER_CONF="users/${KANETON_USER}/user.conf"

MACHINE_MK=""
USER_MK=""

MACHINE_SH=""
USER_SH=""

ENV_CONF=".env.conf"
ENV_MK=".env.mk"
ENV_SH=".env.sh"

SRC_DIR=""

#
# ---------- functions --------------------------------------------------------
#

#
# USER_CONF
#
# this function reads the user configuration file to get the machine
# variable.
#
user_conf()
{
  local machine

  # gets the _MACHINE_ variable.
  machine=$(sed -E -n							\
            "s/^_MACHINE_[[:space:]]+(=|:=)[[:space:]]+(.*)$/\2/p"	\
            ${USER_CONF})

  # sets the configuration variables.
  MACHINE_CONF="machines/${machine}/machine.conf"

  MACHINE_MK="machines/${machine}/machine.mk"
  USER_MK="users/${KANETON_USER}/user.mk"

  MACHINE_SH="machines/${machine}/machine.sh"
  USER_SH="users/${KANETON_USER}/user.sh"
}



#
# ENV_MK
#
# this function generates the env.mk makefile dependency.
#
env_mk()
{
  # removes the previous env.mk version.
  rm -f ${ENV_MK}

  # sets the source directory path.
  echo "_SRC_DIR_		:=		${SRC_DIR}" >> ${ENV_MK}

  # copy the env.conf contents into the env.mk makefile dependency.
  cat ${ENV_CONF} >> ${ENV_MK}

  # copy the machine.conf contents into the env.mk makefile dependency.
  cat ${MACHINE_CONF} >> ${ENV_MK}

  # copy the user.conf contents into the env.mk makefile dependency.
  cat ${USER_CONF} >> ${ENV_MK}

  # copy the machine.mk contents into the env.mk makefile dependency.
  cat ${MACHINE_MK} >> ${ENV_MK}

  # copy the user.mk contents into the env.mk makefile dependency.
  cat ${USER_MK} >> ${ENV_MK}
}



#
# ENV_SH
#
# this function generates the env.sh file.
#
env_sh()
{
  local makefile
  local regexp
  local replacement

  # removes the previous env.conf version.
  rm -f ${ENV_SH}

  # creates an empty temporary file.
  makefile=$(mktemp)

  # generates the temporary makefile.
  echo "_SRC_DIR_		:=		${SRC_DIR}" >> ${makefile}
  echo "include			${ENV_CONF}" >> ${makefile}
  echo "include			${MACHINE_CONF}" >> ${makefile}
  echo "include			${USER_CONF}" >> ${makefile}
  echo "all:" >> ${makefile}

  # generates the env.sh shell script dependency.
  regexp="^(_[[:alpha:]_]+_)[[:space:]]+(=|:=)[[:space:]]+(.*)$"
  replacement="\1=\"\3\""

  make -p -f ${makefile} |						\
      sed -E -n "s/${regexp}/${replacement}/p" > ${ENV_SH}

  # appends the env.sh file the machine shell script specific code.
  cat ${MACHINE_SH} >> ${ENV_SH}

  # appends the env.sh file the user shell script specific code.
  cat ${USER_SH} >> ${ENV_SH}
}



#
# CHECK
#
# checks whether the user and machine directories are present to avoid
# problems.
#
check()
{
  # tries to find the user configuration file located in the user directory.
  if [ ! -e ${USER_CONF} ] ; then
    echo ""
    echo "[!] user configuration file '${USER_CONF}' unreachable"
    echo ""
    echo "[!] please check your KANETON_USER environment variable"
    echo ""
    exit -1
  fi

  # read user configuration file.
  user_conf

  # tries to find the machine configuration file.
  if [ ! -e ${MACHINE_CONF} ] ; then
    echo ""
    echo "[!] machine configuration file '${MACHINE_CONF}' unreachable"
    echo ""
    echo "[!] please check your _MACHINE_ variable into the '${USER_CONF}'"
    echo ""
    exit -1
  fi
}

#
# ---------- entry point ------------------------------------------------------
#

# checks the user and machine directory.
check

# gets the source directory path.
cd ..
SRC_DIR=$(pwd)
cd env

# generates the env.sh file.
env_sh

# generates the env.mk file.
env_mk
