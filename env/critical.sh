#!/bin/sh
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/critical.sh
##
## created       matthieu bucchianeri   [tue dec 13 19:31:03 2005]
## updated       julien quintard   [fri jun 23 16:00:15 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run  in its directory: env/
#
# this script is called critical because it handles the installation
# of critical environment variables used in next steps of kaneton init.
#

#
# ---------- globals ----------------------------------------------------------
#

MACHINE_CONF="machines/${KANETON_MACHINE}"
_ECHO_="echo"

#
# ---------- functions --------------------------------------------------------
#

#
# CHECK
#
# checks whether the user and machine directories are present to avoid
# problems.
#
check()
{
  # tries to find the machine configuration file.
  if [ ! -e ${MACHINE_CONF}/machine.sh ] ; then
    ${_ECHO_} ""
    ${_ECHO_} "[!] machine configuration file for '${KANETON_MACHINE}' "
              "unreachable"
    ${_ECHO_} ""
    ${_ECHO_} "[!] please check your KANETON_MACHINE environment variable"
    ${_ECHO_} ""
    exit -1
  fi
}

#
# ---------- entry point ------------------------------------------------------
#

check

${KANETON_SHELL} ${MACHINE_CONF}/critical.sh
