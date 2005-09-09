#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## viewer.sh
## 
## path          /home/mycure/kaneton/papers
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:18:00 2005   mycure
## last update   Fri Sep  9 17:55:02 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in the directory src/papers



# GLOBAL VALUES
#
# global values
PAPERS="								\
  design kaneton assignments						\
  lectures/advanced-makefiles lectures/arch-ia32 lectures/asm-x86	\
  lectures/c lectures/c-preprocessor					\
  lectures/distributed-operating-systems lectures/inline-assembly	\
  lectures/kernels"



# CONFIGURATION FILE PATH
#
# configuration file
CONF="../conf/"$USER"/"$USER".conf"



# CONFIGURATION FILE VARIABLES
#
# default globals
DVI_VIEWER="unknown"
PS_VIEWER="unknown"
PDF_VIEWER="unknown"
VIEW_FORMAT="unknown"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # dvi viewer
  DVI_VIEWER=`cat $CONF | grep -E "^_DVI_VIEWER_ = .*$" | cut -b 16-`

  # ps viewer
  PS_VIEWER=`cat $CONF | grep -E "^_PS_VIEWER_ = .*$" | cut -b 15-`

  # pdf viewer
  PDF_VIEWER=`cat $CONF | grep -E "^_PDF_VIEWER_ = .*$" | cut -b 16-`

  # view format
  VIEW_FORMAT=`cat $CONF | grep -E "^_VIEW_FORMAT_ = .*$" | cut -b 17-`
}



# USAGE
#
# this function displays the usage but does not exit
usage()
{
  print " papers:" "+"

  for i in $PAPERS ; do
    name=`basename $i`
    print "   $name" "+"
  done

  print ""

  print " usage: viewer.sh paper" "!"
}



# WARNING
#
# this function alerts the user, displaying information and asking to continue
warning()
{
  # display information and ask the user to continue or cancel
  print " your current configuration" "+"
  print "   dvi viewer:               $DVI_VIEWER" "+"
  print "   ps viewer:                $PS_VIEWER" "+"
  print "   pdf viewer:               $PDF_VIEWER" "+"
  print "   view format:              $VIEW_FORMAT" "+"
  print ""
  print " to cancel press CTRL^C, otherwise press enter" "?"

  NEEDLESS=""
  read NEEDLESS
}



# FIND
#
# this functions finds the paper
find()
{
  path=""

  for i in $PAPERS ; do
    name=`basename $i`
    echo $name | grep $paper 2> /dev/null > /dev/null

    if [ $? -eq 0 ] ; then
      path=$i
    fi
  done

  if [ -z $path ] ; then
    print " unknown paper \"$paper\"" "!"
    print ""
    usage
    exit -1
  fi
}



# BUILD
#
# this function builds the paper
build()
{
  make -C $path $VIEW_FORMAT
}



# VIEW
#
# this function displays the paper
view()
{
  name=`basename $path`

  case "$VIEW_FORMAT" in
    "dvi")
      $DVI_VIEWER $path/$name.dvi
      ;;

    "ps")
      $PS_VIEWER $path/$name.ps
      ;;

    "pdf")
      $PDF_VIEWER $path/$name.pdf
      ;;

    *)
      print " unknown view format: \"$VIEW_FORMAT\"" "!"
      print ""
      usage
      exit -1
      ;;
  esac
}



# PRINT A MESSAGE
#
# print a message with a header
print()
{
  msg=$1
  header=$2

  case "$header" in
    "+")
      echo -n -e '\E[;34m'"\033[1m[\033[0m"
      echo -n -e '\E[;32m'"\033[1m+\033[0m"
      echo -n -e '\E[;34m'"\033[1m]\033[0m"
      ;;

    "!")
      echo -n -e '\E[;34m'"\033[1m[\033[0m"
      echo -n -e '\E[;31m'"\033[1m!\033[0m"
      echo -n -e '\E[;34m'"\033[1m]\033[0m"
      ;;

    "?")
      echo -n -e '\E[;34m'"\033[1m[\033[0m"
      echo -n -e '\E[;33m'"\033[1m\033[5m?\033[0m\033[0m"
      echo -n -e '\E[;34m'"\033[1m]\033[0m"
      ;;

    *)
      ;;
  esac

  echo -e "\033[1m$msg\033[0m"
  tput sgr0
}



# ENTRY POINT
#
# entry point of this script

# check the number of arguments
if [ $# -lt 1 ] ; then
    usage
    exit -1
fi

paper="$1"
shift

# preparing the paper
print " preparing the paper" "+"
print ""

# call the read_kaneton_conf function
read_kaneton_conf

# call the warning function
warning

# call the find function
find

# call the build function
build

# call the view function
view
