#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## exporter.sh
## 
## path          /home/mycure/kaneton/export
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:18:00 2005   mycure
## last update   Mon Oct 31 11:56:47 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in the directory src/export
#
# the argument 'kn' makes a valid distribution, meaning that
# there will be no cut line in the distribution. this distribution
# includes all the kaneton levels, all the source code
#
# the argument 'dist' makes a backup of the current development tree
# including the cut lines but still without the subversion control directories
#
# kn is used to always hide source code parts from the distribution
#
# the hidden variable contains the list of directories to not include in
# exported distributions



# GLOBAL VALUES
#
# global values
_STAGES_="k0 k1 k2 k3 k4 k5 k6 k7 k8 k9 kn dist"
_STAGE_=""
_INPUT_=""
_FILES_=""



# CONFIGURATION FILE PATH
#
# configuration file
_CONF_="../conf/"$USER"/"$USER".conf"



# CONFIGURATION FILE VARIABLES
#
# default globals
_DISPLAY_="unknown"
_EXPORT_="unknown"
_HIDDEN_="unknown"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # display
  _DISPLAY_=`cat $_CONF_ | sed -n "s/^_DISPLAY_ = \(.*\)$/\1/p"`

  # export
  _EXPORT_=`cat $_CONF_ | sed -n "s/^_EXPORT_ = \(.*\)$/\1/p"`

  # hidden
  _HIDDEN_=`cat $_CONF_ | sed -n "s/^_HIDDEN_ = \(.*\)$/\1/p"`
}



# USAGE
#
# this function displays the usage but does not exit
usage()
{
  display " usage: exporter.sh [stage]" "!"
  display ""
  display " available stages: $_STAGES_"
}



# WARNING
#
# this function alerts the user, displaying information and asking to continue
warning()
{
  # display information and ask the user to continue or cancel
  display " your current configuration" "+"
  display "   export:                   $_EXPORT_" "+"
  display "   hidden:                   $_HIDDEN_" "+"
  display ""
  display "   stage:                    $_STAGE_" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  NEEDLESS=""
  read NEEDLESS
}



# LOCATE
#
# this functions locates the stage
locate()
{
  for s in $_STAGES_ ; do
    echo $s | grep $_INPUT_ 2> /dev/null > /dev/null

    if [ $? -eq 0 ] ; then
      _STAGE_=$s
    fi
  done

  if [ -z $_STAGE_ ] ; then
    display " unknown stage \"$_INPUT_\"" "!"
    display ""
    usage
    exit -1
  fi
}



# TAGS
#
# this function removes tags from the source code files
tags()
{
  # finally cleans the file from the cut tags
  for f in $_FILES_ ; do
    cat $f | sed "/^.*\[cut\].*$/ d" > /tmp/$_EXPORT_
    cp /tmp/$_EXPORT_ $f
  done
}



# clean
#
# this function cuts the unwanted source code
clean()
{
  # for each file, cut the unwanted source code
  flag=0
  for s in $_STAGES_; do
    if [ $s = $_STAGE_ ] ; then
      flag=1
    fi

    if [ $flag = "1" ] ; then
      for f in $_FILES_ ; do
        cat $f | sed "/^.*\[cut\].*$s.*$/,/^.*\[cut\].*\/$s.*$/	\
                      { /^.*\[cut\].*$s.*$/ !d }" > /tmp/$_EXPORT_
        cp /tmp/$_EXPORT_ $f
      done
    fi
  done
}



# BUILD
#
# this function builds an exported distribution given the stage
build()
{
  # removes the old directory for this stage
  rm -f $_EXPORT_-*-$_STAGE_.tar.gz
  rm -f /tmp/$_EXPORT_.tar.gz

  # creates a tarball from the current working development tree
  cd ../
  tar -czf /tmp/$_EXPORT_.tar.gz .
  cd export

  # cleans the copy of the current working development tree
  mkdir $_EXPORT_
  tar -xzf /tmp/$_EXPORT_.tar.gz -C $_EXPORT_

  # enter directory
  cd $_EXPORT_

  # remove svn control directories
  rm -Rf `find ./ -type d -name .svn`

  # remove the hidden directories
  rm -Rf $_HIDDEN_

  # gets the list of the files
  c_files=`find libs/ core/ drivers/ services/ programs/		\
           -type f -name "*.c"`
  h_files=`find libs/ core/ drivers/ services/ programs/		\
           -type f -name "*.h"`
  asm_files=`find libs/ core/ drivers/ services/ programs/		\
             -type f -name "*.asm"`
  S_files=`find libs/ core/ drivers/ services/ programs/		\
           -type f -name "*.S"`

  _FILES_="$c_files $h_files $asm_files $S_files"

  # make a choice from $_STAGE_
  case $_STAGE_ in
    "dist")
      ;;

    "kaneton")
      tags
      ;;

    *)
      clean
      tags
      ;;
  esac

  # we have to re-generate prototypes because the header files
  # still contain our prototypes

  echo "
" | gmake init > /dev/null 2> /dev/null

  gmake proto > /dev/null 2> /dev/null

  echo "
" | gmake clean > /dev/null 2> /dev/null

  # leave directory
  cd ..
}



# DIST
#
# this function makes a distribution from the exported version
dist()
{
  d=`date +"%Y%m%d"`

  # make the distribution
  tar -czf $_EXPORT_-$d-$_STAGE_.tar.gz $_EXPORT_

  # remove the working directory
  rm -Rf $_EXPORT_
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

# check the number of arguments
if [ $# -lt 1 ] ; then
    usage
    exit -1
fi

_INPUT_="$1"
shift

# preparing to export
display " preparing to export" "+"
display ""

# call the locate function
locate

# call the warning function
warning

# call the build function
build

# call the dist function
dist

# end of export
display " $_STAGE_ exported successfully" "+"
