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
## last update   Mon Oct 31 22:22:23 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in its directory: src/env/



# CONFIGURATION FILE PATH
#
# the configuration file
_CONF_="../conf/"$USER"/"$USER".conf"



# CONFIGURATION FILE VARIABLES
#
# default globals
_DISPLAY_="unknown"
_ENVIRONMENT_="unknown"
_ARCHITECTURE_="unknwon"
_MULTIBOOTLOADER_="unknown"



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

  # multibootloader
  _MULTIBOOTLOADER_=`cat $_CONF_ |					\
                     sed -n "s/^_MULTIBOOTLOADER_ = \(.*\)$/\1/p"`
}



# USAGE
#
# this function displays the usage but does not exit
usage()
{
  display " usage: init.sh" "!"
}



# WARNING
#
# this function alerts the user, displaying information and asking to continue
warning()
{
  # display information and ask the user to continue or cancel
  display " your current configuration:" "+"
  display "   environment:              $_ENVIRONMENT_" "+"
  display "   architecture:             $_ARCHITECTURE_" "+"
  display "   multi-bootloader:         $_MULTIBOOTLOADER_" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  NEEDLESS=""
  read NEEDLESS
}



# ENVIRONMENTS
#
# this function displays the supported environments
environments()
{
  list=""

  directory=`ls env`

  for i in env/$directory ; do
    if [ -d env/$i ] ; then
      list="$list $i"
    fi
  done

  display " supported environments are:$list" "!"
}



# INIT
#
# this function installs the environment, calling the script depending
# of your operating system
init()
{
  if [ ! -d env/$_ENVIRONMENT_ ] ; then
    display " unknown system: $_ENVIRONMENT_" "!"
    display ""
    display " please check your ENVIRONMENT variable in $_CONF_" "!"
    display ""
    environments
    display ""
    usage
    exit
  fi

  if [ ! -e env/$_ENVIRONMENT_/init.sh ] ; then
    display " unknown system: $_ENVIRONMENT_" "!"
    display ""
    display " please check your ENVIRONMENT variable in $_CONF_" "!"
    display ""
    environments
    display ""
    usage
    exit
  fi

  ./env/$_ENVIRONMENT_/init.sh
}



# CONFIGURATION
#
# this function links the kernel configuration files
conf()
{
  display " linking kernel configuration files" "+"

  rm -f core/kaneton/conf/conf.c
  rm -f core/include/kaneton/conf.h

  ln -s ../../../conf/$USER/conf.c core/kaneton/conf/conf.c
  ln -s ../../../conf/$USER/conf.h core/include/kaneton/conf.h
}



# PROTO
#
# this function generates the prototypes
proto()
{
  make proto 2> /dev/null > /dev/null
}



# DEP
#
# this function generates the dependencies
dep()
{
  make dep 2> /dev/null > /dev/null
}



# PRINT A MESSAGE
#
# prints a message using the user variable _DISPLAY_
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

# start of installation

# call the read_kaneton_conf function
read_kaneton_conf

display " installing environment" "+"
display ""

# call the warning function
warning

# go into the src/ directory
cd ../

# install environment
init

# link kernel configuration
conf

# generate prototypes
proto

# generate dependencies
dep

# return into the env/ directory
cd env/

# end of installation
display " environment installed successfully" "+"
