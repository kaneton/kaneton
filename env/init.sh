## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/init.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       julien quintard   [fri jul 14 13:51:20 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: env/
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
  local needless

  needless=$(launch "Makefile" "${_SRC_DIR_}" "proto" "")
}



#
# DEP
#
# this function generates the dependencies.
#
dep()
{
  local needless

  needless=$(launch "Makefile" "${_SRC_DIR_}" "dep" "")
}


#
# RUNTIME CONFIGURATION
#
# this function generates the kaneton runtime configuration file.
#
runtime-configuration()
{
  local kaneton_conf

  # changes the directory.
  change-directory "${_SRC_DIR_}" ""

  # makes a temporary file.
  kaneton_conf=$(temporary "--file")

  # removes the previous version.
  remove ${_KANETON_CONF_}

  # creates the temporary file.
  print "" "! kaneton.conf" "" >> ${kaneton_conf}
  contents "${_USER_KANETON_CONF_}" "" >> ${kaneton_conf}
  print "" "! /kaneton.conf" "" >> ${kaneton_conf}

  # preprocess the temporary file to generate a correct runtime
  # configuration file.
  preprocess "${kaneton_conf}" "kaneton.h" "--no-markers" |		\
    cut "^! kaneton\.conf$" "^! \/kaneton\.conf$" "--print" |		\
    substitute "^! .?kaneton\.conf$" "" "" > ${_KANETON_CONF_}

  # returns in the env directory.
  change-directory "${_ENV_DIR_}" ""
}


#
# MACHINES
#
# this function displays the supported machines.
#
machines()
{
  local machines
  local l
  local m

  machines=$(list "${_MACHINES_DIR_}" "--directory")

  display " supported machines are:" "!"

  for m in ${machines} ; do
    display "   ${m}" "!"
  done
}



#
# LINKS
#
# this function links the kernel configuration files.
#
links()
{
  display " linking kernel configuration files" "+"

  remove "${_CORE_INCLUDE_DIR_}/conf.h" ""
  link "${_CORE_INCLUDE_DIR_}/conf.h" "${_USER_DIR_}/conf.h" ""

  remove "${_CORE_CONF_DIR_}/conf.c" ""
  link "${_CORE_CONF_DIR_}/conf.c" "${_USER_DIR_}/conf.c" ""
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
    display "" ""
    display " please check your KANETON_MACHINE environment variable" "!"
    display "" ""
    machines
    display "" ""
    exit -1
  fi

  if [ ! -e ${_MACHINE_DIR_}/init.sh ] ; then
    display " '${_MACHINE_}' machine-specific init script not present" "!"
    display "" ""
    display " please check your KANETON_MACHINE environment variable" "!"
    display "" ""
    machines
    display "" ""
    exit -1
  fi

  launch "${_MACHINE_DIR_}/init.sh" "" ""

  if [ -e ${_USER_DIR_}/init.sh ] ; then
    launch "${_USER_DIR_}/init.sh" "" ""
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
  display "" ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key ""
}

#
# ---------- entry point ------------------------------------------------------
#

# runs the critical shell script.
${KANETON_SHELL} "${CRITICAL_SH}"

if [ ${?} -ne 0 ] ; then
  exit -1
fi

# from here include the env.sh to get access to every kaneton shell script
# functionalities.
source .env.sh

# displays some stuff.
display "" ""

display " environment files generated successfully" "+"
display "" ""

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
runtime-configuration

# end.
display " environment installed successfully" "+"

# displays some stuff.
display "" ""
