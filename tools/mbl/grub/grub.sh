#! /bin/bash

## copyright quintard julien
## 
## kaneton
## 
## grub.sh
## 
## path          /home/mycure/kaneton/env
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Sun Nov 13 13:18:48 2005   mycure
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: tools/mbl/grub/
#

#
# ---------- dependencies -----------------------------------------------------
#

source			../../../env/.env.sh

#
# ---------- globals ----------------------------------------------------------
#

KANETON_IMAGE="data/kaneton.img"

MENU=""
ACTION=""

#
# ---------- functions --------------------------------------------------------
#

#
# USAGE
#
# this function displays the usage but does not exit.
#
usage()
{
  display " usage: grub.sh [build|install]" "!"
}



#
# BUILD
#
# this function builds the grub boot device.
#
build()
{
  # builds the grub boot device.
  grub-build "${_BOOTMODE_}" "${KANETON_IMAGE}" "${_UDEVICE_}" "${_IMAGE_}"

  if [ ${?} -ne 0 ] ; then
    display " unknown mode \"${_BOOTMODE_}\"" "!"
    display ""
    display " please check your BOOTMODE variable in ${_USER_CONF_}" "!"
    display ""
    usage
    display " current supported boot modes for grub are:" "!" 
    display "      floppy-image" "!"
    display "      floppy" "!"
    display "      tftp" "!"
    exit
  fi
}



#
# MENU
#
# this function generates the grub menu file.
#
menu()
{
  # creates the grub menu file.
  grub-menu "${MENU}" "${_BOOTMODE_}" "${_ADDRESS_}"			\
            "${_TFTP_ADDRESS_}" "${_MODULES_}"

  if [ $? -ne 0 ] ; then
    display " unknown mode \"${_BOOTMODE_}\"" "!"
    display ""
    display " please check your BOOTMODE variable in ${_USER_CONF_}" "!"
    display ""
    usage
    display " supported boot modes for grub are:" "!"
    display "      floppy-image" "!"
    display "      floppy" "!"
    display "      tftp" "!"
    display ""
    exit -1
  fi
}



#
# INSTALL
#
# this function installs the kaneton binaries on the boot device.
#
install()
{
  # installs the grub modules on the boot device.
  grub-install "${_BOOTMODE_}" "${MENU}" "${_MDEVICE_}"			\
               "${_TFTP_DIRECTORY_}" "${_IMAGE_}" "${_MODULES_}"

  if [ $? -ne 0 ] ; then
    display " unknown mode \"${_BOOTMODE_}\"" "!"
    display ""
    display " please check your BOOTMODE variable in ${_USER_CONF_}" "!"
    display ""
    usage
    display " supported boot modes for grub are:" "!"
    display "      floppy-image" "!"
    display "      floppy" "!"
    display "      tftp" "!"
    display ""
    exit -1
  fi
}



#
# WARNING
#
# this function alerts the user, displaying information and asking to continue.
#
warning()
{
  # display information and ask the user to continue or cancel
  display " your current configuration" "+"
  display "   address:                  $_ADDRESS_" "+"
  display "   tftp address:             $_TFTP_ADDRESS_" "+"
  display "   tftp directory:           $_TFTP_DIRECTORY_" "+"
  display "   bootmode:                 $_BOOTMODE_" "+"
  display "   udevice:                  $_UDEVICE_" "+"
  display "   mdevice:                  $_MDEVICE_" "+"
  display "   image:                    $_IMAGE_" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  waitkey
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display ""

# checks the number of arguments.
if [ $# -lt 1 ] ; then
  usage
  exit -1
fi

# gets the argument.
ACTION=$1

# choose what to do.
case $ACTION in
  "build")
    # initialising the boot system.
    display " initialising the boot system" "+"
    display ""

    # calls the warning function.
    warning

    # calls the build function.
    build

    ;;

  "install")
    # initialising the boot system.
    display " initialising the boot system" "+"
    display ""

    # calls the warning function.
    warning

    # generates a temporary file for the grub menu file.
    MENU=$(mktemp)

    # calls the menu function to generate the grub menu file.
    menu

    # calls the install function
    install

    # display some stuff.
    display ""

    ;;

  *)
    display " unknown action \"$action\"" "!"
    display ""
    usage
    exit -1
    ;;
esac

# boot system initialised.
display " boot system initialised successfully" "+"

# displays some stuff.
display ""
