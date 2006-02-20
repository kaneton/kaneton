#!/bin/bash
## licence       kaneton licence
##
## project
##
## file          /home/mycure/kaneton/export/export.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       julien quintard   [sat feb 18 22:58:34 2006]
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

STAGES="k0 k1 k2 k3 k4 k5 k6 k7 k8 k9 kn dist"
STAGE=""
LOCATION=""
FILES=""
TEMP=""

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
  display " available stages: ${STAGES}"
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
  display "   export:                   ${_EXPORT_}" "+"
  display "   hidden:                   ${_HIDDEN_}" "+"
  display ""
  display "   stage:                    ${LOCATION}" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key
}



#
# EXTRACT
#
# this function cuts the unwanted source code.
#
extract()
{
  local cutted

  cutted=$(tempfile)
  # for each file, cut the unwanted source code
  flag=0
  for s in ${STAGES}; do
    if [ $s = ${LOCATION} ] ; then
      flag=1
    fi

    if [ ${flag} = "1" ] ; then
      for f in ${FILES} ; do
        contents "${f}" | cut-code "${s}" > ${cutted}
        copy "${cutted}" "${f}"
      done
    fi
  done
}



#
# BUILD
#
# this function builds an exported distribution given the stage.
#
build()
{
  # removes the old directory for this stage.
  remove "${_EXPORT_}-*-${LOCATION}.tar.gz"
  remove "${_EXPORT_}"

  # creates a tarball from the current working development tree.
  change-directory "${_SRC_DIR_}"
  pack "." "${TEMP}"
  change-directory "${_EXPORT_DIR_}"

  # cleans the copy of the current working development tree.
  make-directory "${_EXPORT_}"
  unpack "${_EXPORT_}" "${TEMP}"

  # enters directory.
  change-directory "${_EXPORT_}"

  # removes svn control directories.
  svn-clean "${_EXPORT_}"

  # gets the list of the files
  c_files=$(find-files "libs/ core/ drivers/ services/ programs/" "*.c"	\
	"--file")
  h_files=$(find-files "libs/ core/ drivers/ services/ programs/" "*.h"	\
	"--file")
  asm_files=$(find-files "libs/ core/ drivers/ services/ programs/"	\
	"*.asm" "--file")
  S_files=$(find-files "libs/ core/ drivers/ services/ programs/" "*.S"	\
	"--file")

  FILES="${c_files} ${h_files} ${asm_files} ${S_files}"

  # make a choice from ${LOCATION}
  case ${LOCATION} in
    "dist")
      ;;
    "kaneton")
      tags-clean "${FILES}"
      ;;
    *)
      extract
      tags-clean "${FILES}"
      ;;
  esac

  # we have to re-generate prototypes because the header files
  # still contain our prototypes

  print "" "" "" | makefile "init" > /dev/null 2> /dev/null

  makefile "proto" > /dev/null 2> /dev/null

  print "" "" "" | makefile "clean" > /dev/null 2> /dev/null

  # remove the hidden directories
  remove "${_HIDDEN_}"

  # leave directory
  change-directory ".."
}

# DIST
#
# this function makes a distribution from the exported version
dist()
{
  d=$(format-date "%Y%m%d")

  # make the distribution
  pack "${_EXPORT_}" "${_EXPORT_}-${d}-${LOCATION}.tar.gz"

  # remove the working directory
  remove "${_EXPORT_}"
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display ""

# check the number of arguments.
if [ ${#} -lt 1 ] ; then
  usage
  exit -1
fi

# gets the argument.
STAGE="${1}"

# preparing to export.
display " preparing to export" "+"

# locate the stage.
LOCATION=$(locate "${STAGES}" "${STAGE}")

# checks the result
if [ ${?} -ne 0 ] ; then
  display " unknown stage \"${STAGE}\"" "!"
  display ""
  usage
  display ""
  exit -1
fi

# warns the user.
warning

# generates a temporary file.
TEMP=$(tempfile)

# build the exported subtree.
build

# build a distribution from the subtree created.
dist

# end of export
display " ${LOCATION} exported successfully" "+"

# displays some stuff.
display ""
