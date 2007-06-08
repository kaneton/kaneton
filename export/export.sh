## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/export/export.sh
##
## created       julien quintard   [fri jul 14 14:16:22 2006]
## updated       julien quintard   [fri jul 14 22:34:55 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in the directory export/
#
# the argument 'dist' makes a backup of the current development tree
# including the cut lines but still without the subversion control directories
#
# the argument 'backup' makes a simple backup of the whole project
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

STAGES="k0 k1 k2 k3 k4 k5 k6 k7 k8 k9 dist backup"
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
  local s

  display " usage: export.sh [stage]" "!"
  display "" ""

  display " available stages:" "!"

  for s in ${STAGES} ; do
    display "   ${s}" "!"
  done
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
  display "" ""
  display "   stage:                    ${LOCATION}" "+"
  display "" ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key ""
}



#
# GENERATE
#
# this function regenerates the non coherent stuff including
# prototypes etc.
#
generate()
{
  local needless

  needless=$(print "" "" "" | launch "Makefile" "init" "")

  needless=$(launch "Makefile" "proto" "")

  needless=$(print "" "" "" | launch "Makefile" "clean" "")
}



#
# TAGS
#
# this function removes the cut lines.
#
tags()
{
  local tmp
  local f

  tmp=$(temporary "--file")

  for f in ${FILES} ; do
    contents "${f}" "" |						\
      substitute "^.*\[cut\].*$" "" "" > ${tmp}

    copy "${tmp}" "${f}" ""
  done

  remove "${tmp}" ""
}



#
# CLEAR
#
# this function clears the unwanted source code.
#
clear()
{
  local tmp
  local f
  local s
  local c

  tmp=$(temporary "--file")

  c=0
  for s in ${STAGES}; do
    if [ ${s} = ${LOCATION} ] ; then
      c=1
    fi

    if [ ${c} = "1" ] ; then
      for f in ${FILES} ; do
        contents "${f}" "" |						\
          cut "^.*\[cut\].*${s}.*$" "^.*\[cut\].*\/${s}.*$" "--delete"	\
          > ${tmp}

        copy "${tmp}" "${f}" ""
      done
    fi
  done

  remove "${tmp}" ""
}



#
# VERSIONING
#
# this function cleans the versioning directories.
#
versioning()
{
  local directories

  directories=$(search "." ".svn" "--directory")

  remove "${directories}" ""
}



#
# BUILD
#
# this function builds an exported distribution given the stage.
#
build()
{
  local c
  local h
  local S
  local asm

  # removes the temporary directory
  remove "${_EXPORT_}" ""

  # creates a tarball from the current working development tree.
  change-directory "${_SRC_DIR_}" ""
  pack "." "${TEMP}" ""
  change-directory "${_EXPORT_DIR_}" ""

  # cleans the copy of the current working development tree.
  make-directory "${_EXPORT_}" ""
  unpack "${_EXPORT_}" "${TEMP}" ""

  # enters directory.
  change-directory "${_EXPORT_}" ""

  # gets the list of the files
  c=$(search "." "*.c" "--file")
  h=$(search "." "*.h" "--file")
  S=$(search "." "*.S" "--file")
  asm=$(search "." "*.asm" "--file")

  FILES="${c} ${h} ${S} ${asm}"

  # make a choice from ${LOCATION}
  case ${LOCATION} in
      "backup")
	  ;;
      "dist")
	  versioning
	  tags

	  remove "${_HIDDEN_}" ""
	  ;;
      k*)
	  versioning
	  clear
	  generate

	  remove "${_HIDDEN_}" ""
	  ;;
  esac

  # leave directory
  change-directory "${_EXPORT_DIR_}" ""
}



#
# DIST
#
# this function makes a distribution from the exported version
#
dist()
{
  local d

  d=$(stamp "%Y%m%d" "")

  # make the distribution
  pack "${_EXPORT_}" "${_EXPORT_}-${d}-${LOCATION}.tar.bz2"

  # remove the working directory
  remove "${_EXPORT_}" ""
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display "" ""

# check the number of arguments.
if [ ${#} -lt 1 ] ; then
  usage
  display "" ""
  exit -1
fi

# gets the argument.
STAGE="${1}"

# preparing to export.
display " preparing to export" "+"

# locate the stage.
LOCATION=$(lookup "${STAGES}" "${STAGE}" "")

# checks the result
if [ "${LOCATION}" = "" ] ; then
  display " unknown stage \"${STAGE}\"" "!"
  display "" ""
  usage
  display "" ""
  exit -1
fi

# warns the user.
warning

# generates a temporary file.
TEMP=$(temporary "--file")

# build the exported subtree.
build

# removes the temporary file.
remove "${TEMP}" ""

# build a distribution from the subtree created.
dist

# end of export
display " ${LOCATION} exported successfully" "+"

# displays some stuff.
display "" ""
