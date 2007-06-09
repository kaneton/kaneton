#!/bin/bash
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/machines/netbsd-epita/critical.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       julien quintard   [wed dec 14 12:36:18 2005]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: src/env/
#
# for the moment this script uses UNIX tools: sed, cat, rm, make etc.
#

#
# ---------- globals ----------------------------------------------------------
#

_MAKE_="gmake"
_SED_="/u/a1/bucchi_m/pub/kaneton/sed -r"
_MKTEMP_="/u/a1/bucchi_m/pub/kaneton/mktemp"
_RM_="rm -f"
_CD_="cd"
_PWD_="pwd"
_CAT_="cat"
_ECHO_="echo"

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
  machine=${KANETON_MACHINE}

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
  ${_RM_} ${ENV_MK}

  # sets the source directory path.
  ${_ECHO_} "_SRC_DIR_		:=		${SRC_DIR}" >> ${ENV_MK}

  # copy the env.conf contents into the env.mk makefile dependency.
  ${_CAT_} ${ENV_CONF} >> ${ENV_MK}

  # copy the machine.conf contents into the env.mk makefile dependency.
  ${_CAT_} ${MACHINE_CONF} >> ${ENV_MK}

  # copy the user.conf contents into the env.mk makefile dependency.
  ${_CAT_} ${USER_CONF} >> ${ENV_MK}

  # copy the machine.mk contents into the env.mk makefile dependency.
  ${_CAT_} ${MACHINE_MK} >> ${ENV_MK}

  # copy the user.mk contents into the env.mk makefile dependency.
  ${_CAT_} ${USER_MK} >> ${ENV_MK}
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
  ${_RM_} ${ENV_SH}

  # creates an empty temporary file.
  makefile=$(${_MKTEMP_})

  # generates the temporary makefile.
  ${_ECHO_} "_SRC_DIR_		:=		${SRC_DIR}" >> ${makefile}
  ${_ECHO_} "include			${ENV_CONF}" >> ${makefile}
  ${_ECHO_} "include			${MACHINE_CONF}" >> ${makefile}
  ${_ECHO_} "include			${USER_CONF}" >> ${makefile}
  ${_ECHO_} "all:" >> ${makefile}

  # generates the env.sh shell script dependency.
  regexp="^(_[[:alpha:]_]+_)[[:space:]]+(=|:=)[[:space:]]+(.*)$"
  replacement="\1=\"\3\""

  ${_MAKE_} -p -f ${makefile} |						\
      ${_SED_} -n "s/${regexp}/${replacement}/p" > ${ENV_SH}

  # appends the env.sh file the machine shell script specific code.
  ${_CAT_} ${MACHINE_SH} >> ${ENV_SH}

  # appends the env.sh file the user shell script specific code.
  ${_CAT_} ${USER_SH} >> ${ENV_SH}
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
    ${_ECHO_} ""
    ${_ECHO_} "[!] user configuration file '${USER_CONF}' unreachable"
    ${_ECHO_} ""
    ${_ECHO_} "[!] please check your KANETON_USER environment variable"
    ${_ECHO_} ""
    exit -1
  fi

  # read user configuration file.
  user_conf
}

#
# ---------- entry point ------------------------------------------------------
#

# checks the user and machine directory.
check

# gets the source directory path.
${_CD_} ..
SRC_DIR=$(${_PWD_})
${_CD_} env

# generates the env.sh file.
env_sh

# generates the env.mk file.
env_mk
