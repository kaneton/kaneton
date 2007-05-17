## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/clean.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       julien quintard   [fri jul 14 13:52:31 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: env/
#

#
# ---------- dependencies -----------------------------------------------------
#

source			.env.sh

#
# ---------- functions --------------------------------------------------------
#

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
  display " unlinking kernel configuration files" "+"

  remove "${_CORE_CONF_DIR_}/conf.c" ""
  remove "${_CORE_INCLUDE_DIR_}/conf.h" ""
}



#
# DEP
#
# this function cleans dependency files.
#
dep()
{
  local makefiles
  local m

  display " cleaning makefile dependencies" "+"

  makefiles=$(search "${_SRC_DIR_}/" ".makefile.mk" "--file")

  for m in "${makefiles}" ; do
    remove "${m}" ""
  done
}



#
# CLEAN
#
# this function cleans the environment, calling the script depending
# of your operating system.
#
clean()
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

  if [ ! -e ${_MACHINE_DIR_}/clean.sh ] ; then
    display " '${_MACHINE_}' machine-specific clean script not present" "!"
    display "" ""
    display " please check your KANETON_MACHINE environment variable" "!"
    display "" ""
    machines
    display "" ""
    exit -1
  fi

  launch "${_MACHINE_DIR_}/clean.sh" "" ""

  if [ -e ${_USER_DIR_}/clean.sh ] ; then
    launch "${_USER_DIR_}/clean.sh" "" ""
  fi

  # finally removes the env.mk and env.sh files.
  remove "${_ENV_MK_}" ""
  remove "${_ENV_SH_}" ""

  # also removes the kaneton runtime configuration file
  remove "${_KANETON_CONF_}" ""
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display "" ""

display " cleaning the environment" "+"
display "" ""

# removes some links.
links

# cleans the dependency files
dep

# calls the clean function which will perform machine-specific stuff.
clean

# end.
display " environment installed successfully" "+"

# displays some stuff.
display ""
