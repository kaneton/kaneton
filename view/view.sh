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

PAPERS=""

LOCATION=""
PAPER=""

#
# ---------- functions --------------------------------------------------------
#

#
# BUILD
#
# this function builds the file calling the makefile.
#
build()
{
  change-directory "${LOCATION}"
  makefile "build"
  change-directory "${_VIEW_DIR_}"
}



#
# VIEW
#
# this function launchs the viewer calling the makefile.
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
  display "   viewer:                   ${_VIEWER_}" "+"
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

  display " usage: viewer.sh [paper]" "!"

  display ""

  display " papers:" "+"
  display ""

  for p in ${PAPERS} ; do
    display "   ${p}" "+"
  done
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display ""

# gets the papers list.
curriculum=$(list "${_CURRICULUM_DIR_}" "--directories")
for e in ${curriculum} ; do
  PAPERS="${PAPERS} curriculum::${e}"
done

exams=$(list "${_EXAMS_DIR_}" "--directories")
for e in ${exams} ; do
  archives=$(list "exams/${e}" "--directories")
  for a in ${archives} ; do
    PAPERS="${PAPERS} exams::${e}::${a}"
  done
done

feedbacks=$(list "${_FEEDBACKS_DIR_}" "--directories")
for e in ${feedbacks} ; do
  PAPERS="${PAPERS} feedbacks::${e}"
done

lectures=$(list "${_LECTURES_DIR_}" "--directories")
for e in ${lectures} ; do
  PAPERS="${PAPERS} lectures::${e}"
done

papers=$(list "${_PAPERS_DIR_}" "--directories")
for e in ${papers} ; do
  PAPERS="${PAPERS} papers::${e}"
done

books=$(list "${_BOOKS_DIR_}" "--directories")
for e in ${books} ; do
  PAPERS="${PAPERS} books::${e}"
done

internships=$(list "${_INTERNSHIPS_DIR_}" "--directories")
for e in ${internships} ; do
  PAPERS="${PAPERS} internships::${e}"
done

# check the number of arguments.
if [ ${#} -ne 1 ] ; then
  usage
  display ""
  exit -1
fi

# gets the argument.
PAPER=${1}

# prints some stuff.
display " preparing the paper" "+"
display ""

# locate the paper.
LOCATION=$(locate "${PAPERS}" "${PAPER}" | substitute "::" "\/" "all")

# checks the result
if [ ${?} -ne 0 ] || [ "${LOCATION}" = "" ] ; then
  display " unknown paper \"${PAPER}\"" "!"
  display ""
  usage
  display ""
  exit -1
fi

# warns the user.
warning

# builds the file.
build

# launchs the viewer.
view

# displays some stuff.
display ""
