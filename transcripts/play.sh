#! /bin/bash

## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/transcripts/play.sh
##
## created       julien quintard   [mon apr 10 15:02:40 2006]
## updated       julien quintard   [mon apr 10 15:11:31 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: src/transcripts/
#

#
# ---------- dependencies -----------------------------------------------------
#

source			../env/.env.sh

#
# ---------- globalss =--------------------------------------------------------
#

FILE=""

TEMP=""
DIRECTORY=""

LOG=""
TIME=""

#
# ---------- functions --------------------------------------------------------
#

#
# PREPARE
#
# this function prepares the play.
#
prepare()
{
  local cwd

  TEMP=$(tempdirectory)

  DIRECTORY="${TEMP}/transcript"

  LOG="${DIRECTORY}/log"
  TIME="${DIRECTORY}/time"

  copy "${FILE}" "${TEMP}/transcript.tar.gz"

  cwd=$(working-directory)

  change-directory "${TEMP}"

  unpack "${TEMP}" "transcript.tar.gz"

  change-directory "${cwd}"
}

#
# PLAY
#
# this function plays the session.
#
play()
{
  # displays some stuff.
  display " playing the session ..." "+"

  ${_SCRIPTREPLAY_TOOL_} ${TIME} ${LOG} # XXX make a function for it
}

#
# CLEAN
#
# this function cleans the play.
#
clean()
{
  remove "${TEMP}"
}

#
# USAGE
#
# this function displays the usage but does not exit.
#
usage()
{
  display " usage: play.sh [file]" "!"
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display ""

# check the number of arguments.
if [ ${#} -ne 1 ] ; then
  usage
  display ""
  exit -1
fi

# gets the argument.
FILE=${1}

# prepares the play.
prepare

# launchs the play.
play

# cleans the play.
clean

# displays some stuff.
display " session played sucessfully" "+"
