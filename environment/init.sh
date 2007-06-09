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

  display " generating kaneton prototypes" "+"

  # XXX needless=$(launch "Makefile" "${_SRC_DIR_}" "proto" "")
}



#
# DEP
#
# this function generates the dependencies.
#
dep()
{
  local needless

  display " generating kaneton dependencies" "+"

  needless=$(launch "Makefile" "${_SRC_DIR_}" "dep" "")
}


#
# RUNTIME
#
# this function generates the kaneton runtime configuration file.
#
runtime()
{
  local kaneton_conf

  display " generating the kaneton runtime configuration file" "+"

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
# ENVIRONMENT
#
# this function installs everything necessary for the kaneton
# development environment.
#
environment()
{
  # creates the links for architecture dependencies.
  display " installing links to machine-dependent directories" "+"

  remove "${_MACHDEP_BOOTSTRAP_DIR_}" ""
  link "${_MACHDEP_BOOTSTRAP_DIR_}" "${_ARCHITECTURE_}" ""

  remove "${_MACHDEP_BOOTLOADER_DIR_}" ""
  link "${_MACHDEP_BOOTLOADER_DIR_}" "${_ARCHITECTURE_}" ""

  remove "${_MACHDEP_CORE_DIR_}" ""
  link "${_MACHDEP_CORE_DIR_}" "${_ARCHITECTURE_}" ""

  remove "${_MACHDEP_LINK_DIR_}" ""
  link "${_MACHDEP_LINK_DIR_}" "${_ARCHITECTURE_}" ""
}



#
# BOOT
#
# this function installs everything necessary for the boot system.
#
boot()
{
  display " installing stuff about the boot system" "+"
}



#
# CORE
#
# this function configures the kaneton core based on the user configuration.
#
core()
{
  display " configuring the kaneton core" "+"
}



#
# USER
#
# this function generates odd things based on the user configuration file.
#
user()
{
  environment
  boot
  core

# XXX
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
# on the machine profile and on the user profile.
#
init()
{
  if [ ! -d ${_MACHINE_DIR_} ] ; then
    display " unknown machine profile: '${_MACHINE_}'" "!"
    display "" ""
    display " please check your KANETON_MACHINE environment variable" "!"
    display "" ""
    machines
    display "" ""
    exit -1
  elif [ ! -e ${_MACHINE_DIR_}/init.sh ] ; then
    display " '${_MACHINE_}' machine-specific init script not present" "!"
    display "" ""
    display " please check your KANETON_MACHINE environment variable" "!"
    display "" ""
    machines
    display "" ""
    exit -1
  else
    display " calling the machine-dependent init script" "+"

    launch "${_MACHINE_DIR_}/init.sh" "" ""
  fi

  if [ -e ${_USER_DIR_}/init.sh ] ; then
    display " calling the user-dependent init script" "+"

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

# generates the user configuration.
user

# calls the init function which will install machine-specific stuff.
init

# generates the prototypes.
proto

# generates the dependencies.
dep

# generates the runtime kaneton configuration file.
runtime

# end.
display " environment installed successfully" "+"

# displays some stuff.
display "" ""
