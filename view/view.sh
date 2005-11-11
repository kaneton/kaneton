#! /bin/bash

## copyright quintard julien
## 
## kaneton
## 
## view.sh
## 
## path          /home/mycure/kaneton/view
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Tue Nov  8 14:15:50 2005   mycure
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

_PAPERS_="								\
  papers/assignments papers/design papers/kaneton papers/seminar	\
  lectures/advanced-makefiles lectures/arch-ia32			\
  lectures/c-preprocessor lectures/distributed-operating-systems	\
  lectures/prerequisites lectures/inline-assembly lectures/kernels	\
  lectures/development-environment					\
  cursus/schedule"

_PATH_=""
_PAPER_=""

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
  $_MAKE_ -C $_PATH_ view
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
  display "   dvi viewer:               $_VIEWER_DVI_" "+"
  display "   ps viewer:                $_VIEWER_PS_" "+"
  display "   pdf viewer:               $_VIEWER_PDF_" "+"
  display "   format:                   $_FORMAT_" "+"
  display ""
  display "   paper:                    $_PATH_" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  NEEDLESS=""
  read NEEDLESS
}



#
# USAGE
#
# this function displays the usage but does not exit.
#
usage()
{
  display " papers:" "+"
  display ""

  for l in $_PAPERS_ ; do
    display "   $l" "+"
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
if [ $# -lt 1 ] ; then
  usage
  exit -1
fi

# gets the argument.
_PAPER_=$1

# prints some stuff.
display " cleaning the environment" "+"
display ""

# locate the paper.
_PATH_=$(locate $_PAPER_ "$_PAPERS_")

# checks the result
if [ -z $_PATH_ ] ; then
  display " unknown paper \"$_PAPER_\"" "!"
  display ""
  usage
  exit -1
fi

# warns the user.
warning

# launchs the viewer.
view

# end.
display " environment installed successfully" "+"

# displays some stuff.
display ""
