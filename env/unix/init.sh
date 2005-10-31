#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## init.sh
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Mon Oct 31 12:14:34 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in the directory src/



# CONFIGURATION FILE PATH
#
# the configuration file
_CONF_="conf/"$USER"/"$USER".conf"



# CONFIGURATION FILE VARIABLES
#
# default globals
_DISPLAY_="unknown"
_ENVIRONMENT_="unknown"
_ARCHITECTURE_="unknwon"



# INSTALL SPECIFIC VARIABLES
#
# generic names for this script
_KANETON_MK_=".kaneton.mk"



# BINARIES
#
# the list of binaries used to compile the kaneton kernel on
# unix operating systems.
_BINARIES_="gcc rm ar ranlib ld nasm ln touch tail wc tar sed		\
            mtools cat cut which gmake latex dvips pdflatex cp		\
            grep basename mkdir date cpp"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # display
  _DISPLAY_=`cat $_CONF_ | sed -n "s/^_DISPLAY_ = \(.*\)$/\1/p"`

  # environment
  _ENVIRONMENT_=`cat $_CONF_ | sed -n "s/^_ENVIRONMENT_ = \(.*\)$/\1/p"`

  # architecture
  _ARCHITECTURE_=`cat $_CONF_ | sed -n "s/^_ARCHITECTURE_ = \(.*\)$/\1/p"`
}



# CHECK
#
# checks wheter the binaries used to compile the kaneton kernel are
# present on the system.
check()
{
  display "   checking binaries" "+"

  for i in $_BINARIES_ ; do
    which $i 2> /dev/null > /dev/null
    if [ $? -ne 0 ] ; then
      display "    binary '$i' not present on your system" "!"
    fi
  done
}



# INIT
#
# installs the environment for unix operating system
init()
{
  # makefile dependencies
  generic_kaneton_mk="env/"$_ENVIRONMENT_"/kaneton.mk"

  # sed
  display "   generating makefile dependencies" "+"

  rm -f $_KANETON_MK_

  sed s/___kaneton_sed___/$(pwd | sed 's/\//\\\//g')/g $generic_kaneton_mk \
      > $_KANETON_MK_

  # create the symbolic link for architecture dependencies
  display "   installing links to machine-dependent directories" "+"

  rm -f core/bootstrap/arch/machdep
  rm -f core/bootloader/arch/machdep
  rm -f core/include/arch/machdep
  rm -f core/kaneton/arch/machdep
  rm -f tools/scripts/ld/arch/machdep

  ln -s $_ARCHITECTURE_ core/bootstrap/arch/machdep
  ln -s $_ARCHITECTURE_ core/bootloader/arch/machdep
  ln -s $_ARCHITECTURE_ core/include/arch/machdep
  ln -s $_ARCHITECTURE_ core/kaneton/arch/machdep
  ln -s $_ARCHITECTURE_ tools/scripts/ld/arch/machdep
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

# call the read_kaneton_conf function
read_kaneton_conf

# install environment
init

# check the binaries
check
