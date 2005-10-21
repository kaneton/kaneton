#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## viewer.sh
## 
## path          /data/kaneton/papers/lectures
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:18:00 2005   mycure
## last update   Fri Oct 21 09:11:58 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in the directory src/papers



# GLOBAL VALUES
#
# global values
_PAPERS_="								\
  assignments design kaneton seminar					\
  lectures/advanced-makefiles lectures/arch-ia32			\
  lectures/c-preprocessor lectures/distributed-operating-systems	\
  lectures/requirements lectures/inline-assembly lectures/kernels	\
  lectures/development-environment"
_PATH_=""
_PAPER_=""



# CONFIGURATION FILE PATH
#
# configuration file
_CONF_="../conf/"$USER"/"$USER".conf"


# CONFIGURATION FILE VARIABLES
#
# default globals
_DISPLAY_="unknown"
_DVI_VIEWER_="unknown"
_PS_VIEWER_="unknown"
_PDF_VIEWER_="unknown"
_VIEW_FORMAT_="unknown"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # display
  _DISPLAY_=`cat $_CONF_ | grep -E "^_DISPLAY_ = .*$" | cut -b 13-`

  # dvi viewer
  _DVI_VIEWER_=`cat $_CONF_ | grep -E "^_DVI_VIEWER_ = .*$" | cut -b 16-`

  # ps viewer
  _PS_VIEWER_=`cat $_CONF_ | grep -E "^_PS_VIEWER_ = .*$" | cut -b 15-`

  # pdf viewer
  _PDF_VIEWER_=`cat $_CONF_ | grep -E "^_PDF_VIEWER_ = .*$" | cut -b 16-`

  # view format
  _VIEW_FORMAT_=`cat $_CONF_ | grep -E "^_VIEW_FORMAT_ = .*$" | cut -b 17-`
}



# USAGE
#
# this function displays the usage but does not exit
usage()
{
  display " papers:" "+"
  display ""

  for i in $_PAPERS_ ; do
    name=`basename $i`
    display "   $name" "+"
  done

  display ""

  display " usage: viewer.sh [paper]" "!"
}



# WARNING
#
# this function alerts the user, displaying information and asking to continue
warning()
{
  # display information and ask the user to continue or cancel
  display " your current configuration" "+"
  display "   dvi viewer:               $_DVI_VIEWER_" "+"
  display "   ps viewer:                $_PS_VIEWER_" "+"
  display "   pdf viewer:               $_PDF_VIEWER_" "+"
  display "   view format:              $_VIEW_FORMAT_" "+"
  display ""
  display "   paper:                    $_PATH_" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  NEEDLESS=""
  read NEEDLESS
}



# LOCATE
#
# this functions locates the paper
locate()
{
  for i in $_PAPERS_ ; do
    name=`basename $i`
    echo $name | grep $_PAPER_ 2> /dev/null > /dev/null

    if [ $? -eq 0 ] ; then
      _PATH_=$i
    fi
  done

  if [ -z $_PATH_ ] ; then
    display " unknown paper \"$_PAPER_\"" "!"
    display ""
    usage
    exit -1
  fi
}



# BUILD
#
# this function builds the paper
build()
{
  display " building the paper: $_PATH_" "+"
  display ""

  make -s -C $_PATH_ $_VIEW_FORMAT_
}



# VIEW
#
# this function displays the paper
view()
{
  name=`basename $_PATH_`

  case "$_VIEW_FORMAT_" in
    "dvi")
      $_DVI_VIEWER_ $_PATH_/$name.dvi
      ;;

    "ps")
      $_PS_VIEWER_ $_PATH_/$name.ps
      ;;

    "pdf")
      $_PDF_VIEWER_ $_PATH_/$name.pdf
      ;;

    *)
      display " unknown view format: \"$_VIEW_FORMAT_\"" "!"
      display ""
      usage
      exit -1
      ;;
  esac
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

# check the number of arguments
if [ $# -lt 1 ] ; then
    usage
    exit -1
fi

_PAPER_="$1"
shift

# preparing the paper
display " preparing the paper" "+"
display ""

# call the locate function
locate

# call the warning function
warning

# call the build function
build

# call the view function
view
