#! /bin/bash

## copyright quintard julien
## 
## kaneton
## 
## clean.sh
## 
## path          /home/mycure/kaneton/export
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Tue Nov  8 05:56:23 2005   mycure
##

#
# ---------- information ------------------------------------------------------
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
#

#
# ---------- dependencies -----------------------------------------------------
#

source			../env/.env.sh

#
# ---------- globals ----------------------------------------------------------
#

_STAGES_="k0 k1 k2 k3 k4 k5 k6 k7 k8 k9 kn dist"
_STAGE_=""
_INPUT_=""
_FILES_=""



#
# ---------- functions --------------------------------------------------------
#

#
# USAGE
#
# this function displays the usage but does not exit.
#
usage()
{
  display " usage: export.sh [stage]" "!"
  display ""
  display " available stages: $_STAGES_"
}



#
# WARNING
#
# this function alerts the user, displaying information and asking to continue.
#
warning()
{
  # display information and ask the user to continue or cancel
  display " your current configuration" "+"
  display "   export:                   $_EXPORT_" "+"
  display "   hidden:                   $_HIDDEN_" "+"
  display "   view format:		$_VIEW_FORMAT_" "+"
  display "   viewer:                   $_VIEWER_" "+"
  display ""
  display "   stage:                    $_STAGE_" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  NEEDLESS=""
  read NEEDLESS
}



#
# CHECK
#
# this functions checks the stage.
#
check()
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



#
# TAGS
#
# this function removes tags from the source code files.
#
tags()
{
  # finally cleans the file from the cut tags
  for f in $_FILES_ ; do
    cat $f | sed "/^.*\[cut\].*$/ d" > /tmp/$_EXPORT_
    cp /tmp/$_EXPORT_ $f
  done
}



#
# EXTRACT
#
# this function cuts the unwanted source code.
#
extract()
{
  # for each file, cut the unwanted source code
  flag=0
  for s in $_STAGES_; do
    if [ $s = $_STAGE_ ] ; then
      flag=1
    fi

    if [ $flag = "1" ] ; then
      for f in $_FILES_ ; do
        cat $f | sed "/^.*\[cut\].*$s.*$/,/^.*\[cut\].*\/$s.*$/ \
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
  c_files=`find libs/ core/ drivers/ services/ programs/                \
           -type f -name "*.c"`
  h_files=`find libs/ core/ drivers/ services/ programs/                \
           -type f -name "*.h"`
  asm_files=`find libs/ core/ drivers/ services/ programs/              \
             -type f -name "*.asm"`
  S_files=`find libs/ core/ drivers/ services/ programs/                \
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
      extract
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

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display ""

# check the number of arguments.
if [ $# -lt 1 ] ; then
    usage
    exit -1
fi

_INPUT_="$1"

# preparing to export.
display " preparing to export" "+"

# checks the input argument.
check

# warns the user.
warning

# build the exported subtree.
build

# build a distribution from the subtree created.
dist

# end of export
display " $_STAGE_ exported successfully" "+"

# displays some stuff.
display ""
