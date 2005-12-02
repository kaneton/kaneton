#! /bin/bash

## copyright quintard julien
## 
## kaneton
## 
## view.sh
## 
## path          /home/mycure/kaneton/view/lectures
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Fri Nov 18 20:47:01 2005   mycure
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: src/view
#

#
# ---------- dependencies -----------------------------------------------------
#

source			../env/.env.sh

#
# ---------- globalss =--------------------------------------------------------
#

PAPERS="								\
  papers/assignments papers/design papers/kaneton			\
  lectures/advanced-makefiles lectures/arch-mips lectures/arch-ia32	\
  lectures/c-preprocessor lectures/distributed-operating-systems	\
  lectures/prerequisites lectures/inline-assembly			\
  lectures/kernels lectures/development-environment lectures/seminar	\
  lectures/arch-mips							\
  cursus/schedule"

LOCATION=""
PAPER=""

#
# ---------- functions --------------------------------------------------------
#

#
# VIEW
#
# this function launch the viewer calling the makefile.
#
view()
{
  change-directory "${LOCATION}"
  makefile "view"
  change-directory "${_VIEW_DIR_}"
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
  display "   format:                   ${_FORMAT_}" "+"
  display ""
  display "   paper:                    ${LOCATION}" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key
}



#
# USAGE
#
# this function displays the usage but does not exit.
#
usage()
{
  local p

  display " papers:" "+"
  display ""

  for p in ${PAPERS} ; do
    display "   ${p}" "+"
  done

  display ""

  display " usage: viewer.sh [paper]" "!"
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display ""

# check the number of arguments
if [ ${#} -lt 1 ] ; then
  usage
  exit -1
fi

# gets the argument.
PAPER="${1}"

# prints some stuff.
display " preparing the paper" "+"
display ""

# locate the paper.
LOCATION=$(locate "${PAPERS}" "${PAPER}")

# checks the result
if [ ${?} -ne 0 ] ; then
  display " unknown paper \"${PAPER}\"" "!"
  display ""
  usage
  display ""
  exit -1
fi

# warns the user.
warning

# launchs the viewer.
view

# displays some stuff.
display ""
