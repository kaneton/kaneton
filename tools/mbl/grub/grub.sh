#!/bin/bash
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/tools/mbl/grub/grub.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       matthieu bucchianeri   [wed dec 14 12:46:13 2005]
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
# GRUB BUILD
#
# initialise the grub boot device.
#
# ${1}:		bootmode
# ${2}:		kaneton image
# ${3}:		udevice
# ${4}:		image
#
grub-build()
{
  bootmode="${1}"
  kaneton_image="${2}"
  udevice="${3}"
  image="${4}"

  case "${bootmode}" in
    "floppy"|"tftp")
      contents ${kaneton_image} > ${udevice}
      ;;

    "floppy-image")
      copy ${kaneton_image} ${image}
      ;;

    *)
      return -1
      ;;
  esac

  return 0
}



#
# GRUB MENU
#
# this function creates the grub menu file.
#
# ${1}:		menu output file
# ${2}:		bootmode
# ${3}:		address
# ${4}:		tftp_address
# ${5}:		modules
#
grub-menu()
{
  local menu
  local bootmode
  local address
  local tftp_address
  local modules

  local array
  local m
  local i

  menu="${1}"
  bootmode="${2}"
  address="${3}"
  tftp_address="${4}"
  modules="${5}"

  # cleans the menu file contents.
  print "" "" "--no-newline" > ${menu}

  # creates new version.
  print "" "title kaneton" "" >> ${menu}
  print "" "" "" >> ${menu}

  # inserts each module in the menu file.
  case "${bootmode}" in
    "floppy"|"floppy-image")
      print "" "root (fd0)" "" >> ${menu}
      print "" "" "" >> ${menu}
      ;;

    "tftp")
      print "" "ifconfig --address=${address} --server=${tftp_address}" "" \
	  >> ${menu}
      print "" "" "" >> ${menu}
      print "" "root (nd)" "" >> ${menu}
      print "" "" "" >> ${menu}
      ;;

    *)
      return -1
      ;;
  esac

  # fills in an array from the modules list
  array=()
  i=0

  for m in ${modules} ; do
    array[${i}]=${m}

    let "i += 1"
  done

  # sets the kernel into the grub menu file.
  print "" "${array[0]}" "" |						\
    substitute "^.*\/(.*)$" "kernel \/modules\/\1" "all" >> ${menu}

  print "" "" "" >> ${menu}

  i=1
  while [ ${i} -lt ${#array[*]} ] ; do
    print "" "${array[${i}]}" "" |					\
      substitute "^.*\/(.*)$" "module \/modules\/\1" "all" >> ${menu}

    let "i += 1"
  done

  return 0
}



#
# GRUB INSTALL
#
# installs the distribution on the grub boot device.
#
# ${1}:		bootmode
# ${2}:		menu
# ${3}:		mdevice
# ${4}:		tftp directory
# ${5}:		image
# ${6}:		modules
#
grub-install()
{
  local bootmode
  local menu
  local mdevice
  local tftp_directory
  local image
  local modules

  local m

  local menu_lst

  bootmode="${1}"
  menu="${2}"
  mdevice="${3}"
  tftp_directory="${4}"
  image="${5}"
  modules="${6}"

  menu_lst="menu.lst"

  case "${bootmode}" in
    "floppy")
      if [ ! -e ${menu} ] ; then
        display " ${menu_lst}" "!"
      else
	device-copy ${menu} ${mdevice} /boot/grub/${menu_lst} ""
        display " ${menu_lst}" "+"
      fi

      for m in ${modules} ; do
        if [ ! -e ${_SRC_DIR_}/${m} ] ; then
          display " ${m}" "!"
        else
          device-copy ${_SRC_DIR_}/${m} ${mdevice} /modules/ ""
          display " ${m}" "+"
        fi
      done

      ;;

    "tftp")
      if [ ! -e ${menu} ] ; then
        display " ${menu_lst}" "!"
      else
        device-copy ${menu} ${mdevice} /boot/grub/${menu_lst} ""
        display " ${menu_lst}" "+"
      fi

      for m in ${modules} ; do
        if [ ! -e ${_SRC_DIR_}/${m} ] ; then
          display " ${m}" "!"
        else
          copy ${_SRC_DIR_}/${m} ${tftp_directory}
          display " ${m}" "+"
        fi
      done

      ;;

    "floppy-image")
      if [ ! -e ${menu} ] ; then
        display " ${menu_lst}" "!"
      else
	device-copy ${menu} ${image} /boot/grub/${menu_lst} "--image"
        display " ${menu_lst}" "+"
      fi

      for m in ${modules} ; do
        if [ ! -e ${_SRC_DIR_}/${m} ] ; then
          display " ${m}" "!"
        else
          device-copy ${_SRC_DIR_}/${m} ${image} /modules/ "--image"
          display " ${m}" "+"
        fi
      done

      ;;
  esac
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
  display "   address:                  ${_ADDRESS_}" "+"
  display "   tftp address:             ${_TFTP_ADDRESS_}" "+"
  display "   tftp directory:           ${_TFTP_DIRECTORY_}" "+"
  display "   bootmode:                 ${_BOOTMODE_}" "+"
  display "   udevice:                  ${_UDEVICE_}" "+"
  display "   mdevice:                  ${_MDEVICE_}" "+"
  display "   image:                    ${_IMAGE_}" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display ""

# checks the number of arguments.
if [ ${#} -lt 1 ] ; then
  usage
  exit -1
fi

# gets the argument.
ACTION="${1}"

# choose what to do.
case ${ACTION} in
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
    MENU=$(tempfile)

    # calls the menu function to generate the grub menu file.
    menu

    # calls the install function
    install

    # display some stuff.
    display ""

    ;;

  *)
    display " unknown action \"${ACTION}\"" "!"
    display ""
    usage
    exit -1
    ;;
esac

# boot system initialised.
display " boot system initialised successfully" "+"

# displays some stuff.
display ""
