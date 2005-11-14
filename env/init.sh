#! /bin/bash

## copyright quintard julien
## 
## kaneton
## 
## init.sh
## 
## path          /home/mycure/kaneton/tools/mbl/grub
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Sun Nov 13 00:56:34 2005   mycure
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: src/env/
#

#
# ---------- globals ----------------------------------------------------------
#

CRITICAL_SH="critical.sh"

#
# ---------- functions --------------------------------------------------------
#

#
# PROTO
#
# this function generates the prototypes.
#
proto()
{
  directory "${_SRC_DIR_}"
  makefile "proto" 2>/dev/null >/dev/null
  directory "${_ENV_DIR_}"
}



#
# DEP
#
# this function generates the dependencies.
#
dep()
{
  directory "${_SRC_DIR_}"
  makefile "dep" 2>/dev/null 1>/dev/null
  directory "${_ENV_DIR_}"
}



#
# CONF
#
# this function generates the runtime kaneton configuration file from
# the user one.
#
conf()
{
  display " generating kaneton runtime configuration file" "+"

  runtime-configuration "${_USER_KANETON_CONF_}" "${_KANETON_CONF_}"
}



#
# MACHINES
#
# this function displays the supported machines.
#
machines()
{
  local l
  local m

  l=""

  machines=$(list "${_MACHINES_DIR_}")

  for m in ${machines} ; do
    if [ -d ${_MACHINES_DIR_}/${m} ] ; then
      l="${l} ${m}"
    fi
  done

  display " supported machines are:${l}" "!"
}



#
# LINKS
#
# this function links the kernel configuration files.
#
links()
{
  display " linking kernel configuration files" "+"

  remove "${_CORE_CONF_DIR_}/conf.c"
  link "${_CORE_CONF_DIR_}/conf.c" "${_USER_DIR_}/conf.c"

  remove "${_CORE_INCLUDE_DIR_}/kaneton/conf.h"
  link "${_CORE_INCLUDE_DIR_}/kaneton/conf.h" "${_USER_DIR_}/conf.h"
}



#
# INIT
#
# this function installs the environment, calling the script depending
# of your operating system.
#
init()
{
  if [ ! -d ${_MACHINE_DIR_} ] ; then
    display " unknown system: '${_MACHINE_}'" "!"
    display ""
    display " please check your _MACHINE_ variable into '${_USER_CONF_}'" "!"
    display ""
    machines
    display ""
    usage
    display ""
    exit
  fi

  if [ ! -e ${_MACHINE_DIR_}/init.sh ] ; then
    display " '${_MACHINE_}' machine-specific init script not present" "!"
    display ""
    display " please check your _MACHINE_ variable into '${_USER_CONF_}'" "!"
    display ""
    machines
    display ""
    usage
    display ""
    exit
  else
    launch "${_MACHINE_DIR_}/init.sh"
  fi

  if [ -e ${_USER_DIR_}/init.sh ] ; then
    launch "${_USER_DIR_}/init.sh"
  fi
}



#
# WARNING
#
# this function alerts the user, displaying information and asking to continue.
#
warning()
{
  # displays information and asks the user to continue or cancel.
  display " your current configuration:" "+"
  display "   user:                     ${_USER_}" "+"
  display "   machine:                  ${_MACHINE_}" "+"
  display "   architecture:             ${_ARCHITECTURE_}" "+"
  display "   multi-bootloader:         ${_MBL_}" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  waitkey
}

#
# ---------- entry point ------------------------------------------------------
#

# runs the critical shell script.
${SHELL} "${CRITICAL_SH}"

# from here include the env.sh to get access to every kaneton shell script
# functionalities.
source .env.sh

# displays some stuff.
display ""

display " environment files generated successfully" "+"
display ""

# asks the user to continue.
warning

# installs some links.
links

# calls the init function which will install machine-specific stuff.
init

# generates the prototypes.
proto

# generates the dependencies.
dep

# generates the runtime kaneton configuration file.
conf

# end.
display " environment installed successfully" "+"

# displays some stuff.
display ""
