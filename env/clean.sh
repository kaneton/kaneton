#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## clean
## 
## path          /home/mycure/kaneton/papers/lectures/advanced-makefiles
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:14:03 2005   mycure
## last update   Tue Oct 18 23:57:44 2005   mycure
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
_DISPLAY="unknown"
_ENVIRONMENT="unknown"
_ARCHITECTURE_="unknown"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # display
  _DISPLAY_=`cat $_CONF_ | grep -E "^_DISPLAY_ = .*$" | cut -b 13-`

  # environment
  _ENVIRONMENT_=`cat $_CONF_ | grep -E "^_ENVIRONMENT_ = .*$" | cut -b 17-`

  # architecture
  _ARCHITECTURE_=`cat $_CONF_ | grep -E "^_ARCHITECTURE_ = .*$" | cut -b 18-`
}



# USAGE
#
# this function displays the usage but does not exit
usage()
{
  display " usage: clean" "!"
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



# CLEAN
#
# this function installs the environment, calling the function(s) depending
# of your operating system
clean()
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

  if [ ! -e env/$_ENVIRONMENT_/clean ] ; then
    display " unknown system: $_ENVIRONMENT_" "!"
    display ""
    display " please check your ENVIRONMENT variable in $_CONF_" "!"
    display ""
    environments
    display ""
    usage
    exit
  fi

  ./env/$_ENVIRONMENT_/clean
}



# CONFIGURATION
#
# this function unlinks the kernel configuration files.
conf()
{
  display " unlinking kernel configuration files" "+"

  if [ -e core/kaneton/conf/conf.c ] ; then
    rm core/kaneton/conf/conf.c
  fi

  if [ -e core/include/kaneton/conf.h ] ; then
    rm core/include/kaneton/conf.h
  fi
}



# DEPENDENCIES
#
# this function cleans C files dependencies.
dep()
{
  display " cleaning of makefile dependencies" "+"

  makefiles=`find ./ -name .makefile.mk`

  for m in $makefiles ; do
    rm -f $m
  done
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

# start of clean

# call the read_kaneton_conf function
read_kaneton_conf

display " cleaning environment" "+"
display ""

# call the warning function
warning

# go into the src/ directory
cd ..

# clean environment
clean

# unlink the configuration files
conf

# clean the files dependencies
dep

# return into the env/ directory
cd env

# end of clean
display " environment cleaned successfully" "+"
