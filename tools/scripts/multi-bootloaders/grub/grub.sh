#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## grub.sh
## 
## path          /home/mycure/kaneton/papers/lectures/development-environment
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:18:00 2005   mycure
## last update   Fri Oct 21 23:39:53 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in the directory
# src/tools/scripts/multi-bootloaders/grub/



# CONFIGURATION FILE PATH
#
# configuration file
_CONF_="../../../../conf/"$USER"/"$USER".conf"
_MODULES_=`sed "s/#.*//g" ../../../../conf/"$USER"/modules.conf | grep -v "^$"`



# CONFIGURATION FILE VARIABLES
#
# default globals
_DISPLAY__="unknown"
_ADDRESS_="unknown"
_TFTP_ADDRESS_="unknown"
_TFTP_DIRECTOR_="unknown"
_BOOTMODE_="unknown"
_UDEVICE_="unknown"
_MDEVICE_="unknown"
_IMAGE_="unknown"

_KANETON_IMAGE_="data/kaneton.img"
_MENU_="/tmp/menu.lst"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # address
  _DISPLAY_=`cat $_CONF_ | grep -E "^_DISPLAY_ = .*$" | cut -b 13-`

  # address
  _ADDRESS_=`cat $_CONF_ | grep -E "^_ADDRESS_ = .*$" | cut -b 13-`

  # tftp address
  _TFTP_ADDRESS_=`cat $_CONF_ | grep -E "^_TFTP_ADDRESS_ = .*$" | cut -b 18-`

  # tftp directory
  _TFTP_DIRECTORY_=`cat $_CONF_ | grep -E "^_TFTP_DIRECTORY_ = .*$" |	\
                    cut -b 20-`

  # bootmode
  _BOOTMODE_=`cat $_CONF_ | grep -E "^_BOOTMODE_ = .*$" | cut -b 14-`

  # unix device
  _UDEVICE_=`cat $_CONF_ | grep -E "^_UDEVICE_ = .*$" | cut -b 13-`

  # mtools device
  _MDEVICE_=`cat $_CONF_ | grep -E "^_MDEVICE_ = .*$" | cut -b 13-`

  # image
  _IMAGE_=`cat $_CONF_ | grep -E "^_IMAGE_ = .*$" | cut -b 11-`
}



# USAGE
#
# this function displays the usage but does not exit
usage()
{
  display " usage: grub.sh [build|install]" "!"
}



# WARNING
#
# this function alerts the user, displaying information and asking to continue
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

  NEEDLESS=""
  read NEEDLESS
}



# MENU
#
# this function creates the grub menu file
menu()
{
  # remove old version
  rm -f $_MENU_ 2>&1 > /dev/null

  # create new version
  echo "title kaneton" >> $_MENU_
  echo "" >> $_MENU_

  # insert each module in menu

  case "$_BOOTMODE_" in
    "floppy"|"floppy-image")
      echo "root (fd0)" >> $_MENU_
      echo "" >> $_MENU_
      ;;

    "tftp")
      echo "ifconfig --address=$_ADDRESS_ --server=$_TFTP_ADDRESS_" >> $_MENU_
      echo "" >> $_MENU_
      echo "root (nd)" >> $_MENU_
      echo "" >> $_MENU_
      ;;

    *)
      display " unknown mode \"$_BOOTMODE_\"" "!"
      display ""
      display " please check your BOOTMODE variable in $_CONF_" "!"
      display ""
      usage
      display " supported boot modes for grub are:" "!"
      display "      floppy-image" "!"
      display "      floppy" "!"
      display "      tftp" "!"
      exit
      ;;
  esac

  i=0
  for module in $_MODULES_ ; do
    if [ $i = 0 ] ; then
      kernel=$module
    else
      modules="$modules $module"
    fi
    i=1
  done

  echo $kernel | sed "s/^.*\/\(.*\)$/kernel \/modules\/\1/g" >> $_MENU_

  echo "" >> $_MENU_

  for module in $modules
  do
      echo $module | sed "s/^.*\/\(.*\)$/module \/modules\/\1/g" >> $_MENU_
  done
}



# BUILD
#
# initialise the boot device
build()
{
  case "$_BOOTMODE_" in
    "floppy"|"tftp")
      cat $_KANETON_IMAGE_ > $_UDEVICE_
      ;;

    "floppy-image")
      cp $_KANETON_IMAGE_ $_IMAGE_
      ;;

    *)
      display " unknown mode \"$_BOOTMODE_\"" "!"
      display ""
      display " please check your BOOTMODE variable in $_CONF_" "!"
      display ""
      usage
      display " current supported boot modes for grub are:" "!" 
      display "      floppy-image" "!"
      display "      floppy" "!"
      display "      tftp" "!"
      exit
      ;;
  esac
}



# INSTALL
#
# installs the distribution on the boot device
install()
{
  case "$_BOOTMODE_" in
    "floppy")
      if [ ! -f $_MENU_ ] ; then
        display " $_MENU_" "!"
      else
        mcopy -n -o $_MENU_ $_MDEVICE_/boot/grub/
        display " $_MENU_" "+"
      fi

      for module in $_MODULES_ ; do
        if [ ! -f ../../../../$module ] ; then
          display " $module" "!"
        else
          mcopy -n -o ../../../../$module $_MDEVICE_/modules/
          display " $module" "+"
        fi
      done
      display ""
      ;;

    "tftp")
      if [ ! -f $_MENU_ ] ; then
        display " $_MENU_" "!"
      else
        mcopy -n -o $_MENU_ $_MDEVICE_/boot/grub/
        display " $_MENU_" "+"
      fi

      for module in $_MODULES_ ; do
        if [ ! -f ../../../../$module ] ; then
          display " $module" "!"
        else
          cp ../../../../$module $_TFTP_DIRECTORY_
          display " $module" "+"
        fi
      done
      display ""
      ;;

    "floppy-image")
      if [ ! -f $_MENU_ ] ; then
        display " $_MENU_" "!"
      else
        mcopy -n -o $_MENU_ -i $_IMAGE_ ::/boot/grub/
        display " $_MENU_" "+"
      fi

      for module in $_MODULES_ ; do
        if [ ! -f ../../../../$module ] ; then
          display " $module" "!"
        else
          mcopy -n -o ../../../../$module -i $_IMAGE_ ::/modules/
          display " $module" "+"
        fi
      done
      display ""
      ;;

    *)
      display " unknown mode \"$_BOOTMODE_\"" "!"
      display ""
      display " please check your BOOTMODE variable in $_CONF_" "!"
      display ""
      usage
      display " current supported boot modes for grub are:" "!"
      display "      floppy-image" "!"
      display "      floppy" "!"
      display "      tftp" "!"
      exit
      ;;
  esac
}



# PRINT A MESSAGE
#
# prints a message using the user variable DISPLAY
print()
{
  color=$1
  message=$2
  options=$3

  if [ $_DISPLAY_ = "color" ] ; then

    case "$color" in
      "red")
        echo -e $options '\E[;31m'"\033[1m$message\033[0m"
	;;

      "green")
        echo -e $options '\E[;32m'"\033[1m$message\033[0m"
	;;

      "yellow")
        echo -e $options '\E[;33m'"\033[1m$message\033[0m"
	;;

      "blue")
        echo -e $options '\E[;34m'"\033[1m$message\033[0m"
	;;

      "white")
        echo -e $options '\E[;37m'"\033[1m$message\033[0m"
	;;

      *)
	;;
    esac

  else

    echo $options "$message"

  fi
}



# DISPLAY A MESSAGE
#
# displays a message with a header
display()
{
  msg=$1
  header=$2

  case "$header" in
    "+")
      print "blue" "[" "-n"
      print "green" "+" "-n"
      print "blue" "]" "-n"
      ;;

    "!")
      print "blue" "[" "-n"
      print "red" "!" "-n"
      print "blue" "]" "-n"
      ;;

    "?")
      print "blue" "[" "-n"
      print "yellow" "?" "-n"
      print "blue" "]" "-n"
      ;;

    *)
      ;;
  esac

  print "white" "$msg" ""
}



# ENTRY POINT
#
# entry point of this script

# check the number of arguments
if [ $# -lt 1 ] ; then
    usage
    exit -1
fi

action="$1"
shift

case "$action" in
  "build")
    # initialising boot system

    # call the read_kaneton_conf function
    read_kaneton_conf

    display " initialising boot system" "+"
    display ""

    # call the warning function
    warning

    # call the init function
    build

    # boot system initialised
    display " boot system initialised successfully" "+"
    ;;

  "install")
    # initialising boot system

    # call the read_kaneton_conf function
    read_kaneton_conf

    display " initialising boot system" "+"
    display ""

    # call the warning function
    warning

    # call the menu function
    menu

    # call the install function
    install

    # boot system initialised
    display " boot system initialised successfully" "+"
    ;;

  *)
    display " unknown action \"$action\"" "!"
    display ""
    usage
    exit
    ;;
esac
