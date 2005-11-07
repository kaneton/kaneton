##
## copyright quintard julien
## 
## kaneton
## 
## machine.sh
## 
## path          /home/mycure/kaneton/env
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:08:31 2005   mycure
## last update   Mon Nov  7 14:41:19 2005   mycure
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
