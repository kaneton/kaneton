## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/view.sh
##
## created       julien quintard   [fri jul 14 14:02:03 2006]
## updated       julien quintard   [fri jul 14 22:29:13 2006]
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
  launch "Makefile" "${LOCATION}" "build" ""
}



#
# VIEW
#
# this function launchs the viewer calling the makefile.
#
view()
{
  launch "Makefile" "${LOCATION}" "view" ""
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
  display "" ""
  display "   paper:                    ${LOCATION}" "+"
  display "" ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key ""
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

  display "" ""

  display " papers:" "!"

  for p in ${PAPERS} ; do
    display "   ${p}" "!"
  done
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display "" ""

# gets the papers list.
curriculum=$(list "${_CURRICULUM_DIR_}" "--directory")
for e in ${curriculum} ; do
  PAPERS="${PAPERS} curriculum::${e}"
done

exams=$(list "${_EXAMS_DIR_}" "--directory")
for e in ${exams} ; do
  archives=$(list "${_EXAMS_DIR_}/${e}" "--directory")
  for a in ${archives} ; do
    PAPERS="${PAPERS} exam::${e}::${a}"
  done
done

feedbacks=$(list "${_FEEDBACKS_DIR_}" "--directory")
for e in ${feedbacks} ; do
  PAPERS="${PAPERS} feedback::${e}"
done

lectures=$(list "${_LECTURES_DIR_}" "--directory")
for e in ${lectures} ; do
  PAPERS="${PAPERS} lecture::${e}"
done

papers=$(list "${_PAPERS_DIR_}" "--directory")
for e in ${papers} ; do
  PAPERS="${PAPERS} paper::${e}"
done

books=$(list "${_BOOKS_DIR_}" "--directory")
for e in ${books} ; do
  PAPERS="${PAPERS} book::${e}"
done

internships=$(list "${_INTERNSHIPS_DIR_}" "--directory")
for e in ${internships} ; do
  PAPERS="${PAPERS} internship::${e}"
done

# check the number of arguments.
if [ ${#} -ne 1 ] ; then
  usage
  display ""
  exit -1
fi

# gets the argument.
PAPER="${1}"

# prints some stuff.
display " preparing the paper" "+"
display "" ""

# locate the paper.
LOCATION=$(lookup "${PAPERS}" "${PAPER}" "" |				\
           substitute "::" "\/" "--everywhere")

# checks the result
if [ "${LOCATION}" = "" ] ; then
  display " unknown paper \"${PAPER}\"" "!"
  display "" ""
  usage
  display "" ""
  exit -1
fi

# warns the user.
warning

# builds the file.
build

# launchs the viewer.
view

# displays some stuff.
display "" ""
