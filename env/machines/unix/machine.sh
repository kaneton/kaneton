##
## copyright quintard julien
## 
## kaneton
## 
## machine.sh
## 
## path          /home/mycure/kaneton/view
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:08:31 2005   mycure
## last update   Tue Nov  8 14:13:04 2005   mycure
##

#
# ---------- information ------------------------------------------------------
# 
# this file is composed of functions needed by the kaneton shell scripts.
#

#
# ---------- functions --------------------------------------------------------
#

#
# PRINT
#
# prints a message using the user variable _DISPLAY_.
#
print()
{
  color=$1
  message=$2
  options=$3

  if [ $_DISPLAY_ = "color" ] ; then

    case "$color" in
      "red")
        $_ECHO_ -e $options '\E[;31m'"\033[1m$message\033[0m"
        ;;

      "green")
        $_ECHO_ -e $options '\E[;32m'"\033[1m$message\033[0m"
        ;;

      "yellow")
        $_ECHO_ -e $options '\E[;33m'"\033[1m$message\033[0m"
        ;;

      "blue")
        $_ECHO_ -e $options '\E[;34m'"\033[1m$message\033[0m"
        ;;

      "white")
        $_ECHO_ -e $options '\E[;37m'"\033[1m$message\033[0m"
        ;;

      *)
        ;;
    esac

  else

    $_ECHO_ $options "$message"

  fi
}



#
# DISPLAY A MESSAGE
#
# displays a message with a header.
#
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



#
# WAITKEY
#
# this function just waits for a key.
#
waitkey()
{
  $_READ_ needless
}



#
# LAUNCH
#
# this function just launches a new program, script etc..
#
launch()
{
  file=$1

  case $1 in
    *.sh)
      $_SHELL_ $file
      ;;
    *)
      ;;
  esac
}



#
# LINK
#
# this function performs a link between two files.
#
link()
{
  source=$1
  destination=$2

  $_LN_ $destination $source
}



#
# REMOVE
#
# this function just removes a file
#
remove()
{
  args=$*

  $_RM_ $args
}



#
# LIST
#
# this function returns the list of the directory entries.
#
list()
{
  directory=$1

  $_LS_ $directory
}



#
# DIRECTORY
#
# this function just changes the current working directory.
#
directory()
{
  directory=$1

  $_CD_ $directory
}



#
# MAKEFILE
#
# this function launches a makefile.
#
makefile()
{
  args=$*

  $_MAKE_ $args
}



#
# RUNTIME CONFIGURATION
#
# this function generates the kaneton runtime configuration file.
#
runtime_configuration()
{
  source=$1
  destination=$2

  directory $_SRC_DIR_

  # makes a temporary file
  kaneton_conf=$(mktemp)

  # removes the previous version.
  remove $destination

  # creates the temporary file.
  echo "! kaneton.conf" >> $kaneton_conf
  cat $source >> $kaneton_conf
  echo "! /kaneton.conf" >> $kaneton_conf

  # preprocess the temporary file to generate a correct runtime
  # configuration file.
  $_CPP_ $_INCLUDES_ -P -include kaneton.h $kaneton_conf |		\
    $_SED_ -n '/^! kaneton.conf$/,/^! \/kaneton.conf$/ p' |		\
    $_SED_ '/^!.*$/ d' > $destination

  directory $_ENV_DIR_
}



#
# LOCATE
#
# this function tries to locate an element in a list of elements.
#
locate()
{
  element=$1
  list=$2

  for l in $list ; do
    echo $l | grep $element 2>/dev/null 1>/dev/null

    if [ $? -eq 0 ] ; then
      echo $l
      return 0
    fi
  done
}
