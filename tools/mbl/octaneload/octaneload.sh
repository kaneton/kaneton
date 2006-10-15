##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/tools/mbl/octaneload/octaneload.sh
##
## created       Enguerrand RAYMOND   [sun oct 15 20:29:10 2006]
## updated       Enguerrand RAYMOND   [sun oct 15 20:37:19 2006]
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
  display " usage: octaneload.sh [build|install]" "!"
}



#
# BUILD
#
# initialise the grub boot device.
#
build()
{

}

#
# INSTALL
#
# installs the distribution on the grub boot device.
#
install()
{
    cat > ${_IMAGE_}
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
  display "" ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key ""
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display "" ""

# checks the number of arguments.
if [ ${#} -lt 1 ] ; then
  usage
  display "" ""
  exit -1
fi

# gets the argument.
ACTION="${1}"

# choose what to do.
case ${ACTION} in
  "build")
    # initialising the boot system.
    display " initialising the boot system" "+"
    display "" ""

    # calls the warning function.
    warning

    # calls the build function.
    build

    ;;
  "install")
    # initialising the boot system.
    display " initialising the boot system" "+"
    display "" ""

    # calls the warning function.
    warning

    # generates a temporary file for the grub menu file.
    MENU=$(temporary "--file")

    # calls the menu function to generate the grub menu file.
    menu

    # calls the install function
    install

    # display some stuff.
    display "" ""

    ;;
  *)
    display " unknown action \"${ACTION}\"" "!"
    display "" ""
    usage
    exit -1
    ;;
esac

# boot system initialised.
display " boot system initialised successfully" "+"

# displays some stuff.
display "" ""
