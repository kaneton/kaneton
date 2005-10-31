#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## confer.sh
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:18:00 2005   mycure
## last update   Mon Oct 31 12:15:32 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in the directory src/conf



# CONFIGURATION FILE PATH
#
# configuration file
_CONF_="../conf/"$USER"/"$USER".conf"



# GLOBALS
#
# global variables
_OUTPUT_="conf/kaneton.conf"
_KANETON_CONF_="conf/$USER/kaneton.conf"



# CONFIGURATION FILE VARIABLES
#
# default globals
_DISPLAY_="unknown"
_CFLAGS_="unknown"
_KFLAGS_="unknown"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # display
  _DISPLAY_=`cat $_CONF_ | sed -n "s/^_DISPLAY_ = \(.*\)$/\1/p"`

  # cflags
  _CFLAGS_=`cat $_CONF_ | sed -n "s/^_CFLAGS_ = \(.*\)$/\1/p"`

  # display
  _KFLAGS_=`cat $_CONF_ | sed -n "s/^_KFLAGS_ = \(.*\)$/\1/p"`
}



# USAGE
#
# this function displays the usage but does not exit
usage()
{
  display " usage: confer.sh" "!"
}



# CONF
#
# this function generate the correct configuration files
conf()
{
  cd ..

  echo "! kaneton.conf" > /tmp/kaneton.conf
  cat $_KANETON_CONF_ >> /tmp/kaneton.conf
  echo "! /kaneton.conf" >> /tmp/kaneton.conf

  cpp -D___kaneton -nostdinc -nostdlib -fno-builtin			\
      -Icore/include $_CFLAGS_ $_KFLAGS_				\
      -include core/include/kaneton.h -P				\
      /tmp/kaneton.conf |						\
  sed -n '/^! kaneton.conf$/,/^! \/kaneton.conf$/ p' |			\
  sed '/^!.*$/ d' > $_OUTPUT_

  cd conf/
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

# call the read_kaneton_conf function
read_kaneton_conf

# call the conf function
conf

# configuration files generated
display " configuration files generated" "+"
