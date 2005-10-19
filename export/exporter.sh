#!/bin/sh
## copyright quintard julien
## 
## kaneton
## 
## exporter.sh
## 
## path          /home/mycure/kaneton/papers/lectures/advanced-makefiles
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:18:00 2005   mycure
## last update   Wed Oct 19 00:04:50 2005   mycure
##

# INFORMATIONS
#
# this script has to be run in the directory src/export



# GLOBAL VALUES
#
# global values
_STAGES_="k1 k2 k3 k4 k5 k6 k7 k8 k9"
_STAGE_=""
_INPUT_=""



# CONFIGURATION FILE PATH
#
# configuration file
_CONF_="../conf/"$USER"/"$USER".conf"



# CONFIGURATION FILE VARIABLES
#
# default globals
_KANETON_EXPORT_="unknown"
_DISPLAY_="unknown"



# READ CONFIGURATION FILE
#
# function used to read the configuration file and to load
# important variables
read_kaneton_conf()
{
  # display
  _DISPLAY_=`cat $_CONF_ | grep -E "^_DISPLAY_ = .*$" | cut -b 13-`

  # kaneton export
  _KANETON_EXPORT_=`cat $_CONF_ | grep -E "^_KANETON_EXPORT_ = .*$" |	\
                    cut -b 20-`
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



# BUILD
#
# this function builds an exported distribution given the stage
build()
{
  # removes the old directory for this stage

  rm -f $_STAGE_.tar.gz
  rm -Rf $_STAGE_

  # creates a tarball from the current working development tree

  cd ../
  tar -czf /tmp/kaneton.tar.gz .
  cd export

  # cleans the copy of the current working development tree

  mkdir $_STAGE_
  tar -xzf /tmp/kaneton.tar.gz -C $_STAGE_

  cd $_STAGE_
  rm -Rf `find ./ -type d -name .svn`
  cd ..

  # cleans every unwanted source code given the stage

  cd $_STAGE_

  c_files=`find ./ -type f -name "*.c"`
  h_files=`find ./ -type f -name "*.h"`
  asm_files=`find ./ -type f -name "*.asm"`
  S_files=`find ./ -type f -name "*.S"`

  files="$c_files $h_files $asm_files $S_files"

  for f in $files ; do
    cat $f | sed "/^.*<<<.*$_STAGE_.*$/,/^.*>>>.*$_STAGE_.*$/ d" >	\
      /tmp/$_STAGE_
    cp /tmp/$_STAGE_ $f
  done

  cd ..
}



# DIST
#
# this function makes a distribution from the exported version
dist()
{
  tar -czf $_STAGE_.tar.gz $_STAGE_

  rm -Rf $_STAGE_
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
display " $stage exported successfully" "+"
