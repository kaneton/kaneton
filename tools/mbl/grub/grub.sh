## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/tools/mbl/grub/grub.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       julien quintard   [sat jul  8 03:12:23 2006]
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

MENU_LST="menu.lst"

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
# initialise the grub boot device.
#
build()
{
  case "${_BOOTMODE_}" in
    "floppy"|"tftp")
      contents "${KANETON_IMAGE}" > ${_UDEVICE_}
      ;;

    "floppy-image")
      copy "${KANETON_IMAGE}" "${_IMAGE_}"
      ;;

    *)
      ;;
  esac
}



#
# MENU
#
# this function creates the grub menu file.
#
menu()
{
  local array
  local m
  local i

  # cleans the menu file contents.
  print "" "" "--no-newline" > ${MENU}

  # creates new version.
  print "" "title kaneton" "" >> ${MENU}
  print "" "" "" >> ${MENU}

  # inserts each module in the menu file.
  case "${_BOOTMODE_}" in
    "floppy"|"floppy-image")
      print "" "root (fd0)" "" >> ${MENU}
      print "" "" "" >> ${MENU}
      ;;

    "tftp")
      print "" "ifconfig --address=${_ADDRESS_} --server=${_TFTP_ADDRESS_}" \
            "" >> ${MENU}
      print "" "" "" >> ${MENU}
      print "" "root (nd)" "" >> ${MENU}
      print "" "" "" >> ${MENU}
      ;;

    *)
      ;;
  esac

  # fills in an array from the modules list
  array=()
  i=0

  for m in ${_MODULES_} ; do
    array[${i}]=${m}

    let "i += 1"
  done

  # sets the kernel into the grub menu file.
  print "" "${array[0]}" "" |						\
    substitute "^.*\/(.*)$" "kernel \/modules\/\1" "all" >> ${MENU}

  print "" "" "" >> ${MENU}

  i=1
  while [ ${i} -lt ${#array[*]} ] ; do
    print "" "${array[${i}]}" "" |					\
      substitute "^.*\/(.*)$" "module \/modules\/\1" "all" >> ${MENU}

    let "i += 1"
  done
}



#
# INSTALL
#
# installs the distribution on the grub boot device.
#
install()
{
  local m

  case "${_BOOTMODE_}" in
    "floppy")
      if [ ! -e ${MENU} ] ; then
        display " ${MENU_LST}" "!"
      else
	device-copy "${MENU}" "${_MDEVICE_}" "/boot/grub/${MENU_LST}" ""
        display " ${menu_lst}" "+"
      fi

      for m in ${_MODULES_} ; do
        if [ ! -e ${_SRC_DIR_}/${m} ] ; then
          display " ${m}" "!"
        else
          device-copy "${_SRC_DIR_}/${m}" "${_MDEVICE_}" "/modules/" ""
          display " ${m}" "+"
        fi
      done

      ;;

    "tftp")
      if [ ! -e ${MENU} ] ; then
        display " ${MENU_LST}" "!"
      else
        device-copy "${MENU}" "${_MDEVICE_}" "/boot/grub/${MENU_LST}" ""
        display " ${MENU_LST}" "+"
      fi

      for m in ${_MODULES_} ; do
        if [ ! -e ${_SRC_DIR_}/${m} ] ; then
          display " ${m}" "!"
        else
          copy "${_SRC_DIR_}/${m}" "${_TFTP_DIRECTORY_}"
          display " ${m}" "+"
        fi
      done

      ;;

    "floppy-image")
      if [ ! -e ${MENU} ] ; then
        display " ${MENU_LST}" "!"
      else
	device-copy "${MENU}" "${_IMAGE_}" "/boot/grub/${MENU_LST}" "--image"
        display " ${MENU_LST}" "+"
      fi

      for m in ${_MODULES_} ; do
        if [ ! -e ${_SRC_DIR_}/${m} ] ; then
          display " ${m}" "!"
        else
          device-copy "${_SRC_DIR_}/${m}" "${_IMAGE_}" "/modules/" "--image"
          display " ${m}" "+"
        fi
      done

      ;;
  esac
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
  display ""
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
