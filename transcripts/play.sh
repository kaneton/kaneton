#! /bin/bash

## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/transcripts/play.sh
##
## created       julien quintard   [mon apr 10 15:02:40 2006]
## updated       julien quintard   [mon apr 10 16:13:20 2006]
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

TRANSCRIPTS=""

TRANSCRIPT=""

TEMP=""
DIRECTORY=""

LOG=""
TIME=""

#
# ---------- functions --------------------------------------------------------
#

#
# WARNING
#
# this function alerts the user, displaying information and asking to continue.
#
warning()
{
  # display information and ask the user to continue or cancel
  display " your current configuration" "+"
  display "   transcript:               ${LOCATION}" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key
}

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

  copy "${LOCATION}" "${TEMP}/transcript.tar.gz"

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
  local t

  display " usage: play.sh [transcript]" "!"

  display ""

  display " transcripts:" "+"
  display ""

  for t in ${TRANSCRIPTS} ; do
    display "   ${t}" "+"
  done
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display ""

# gets the transcripts list.
sessions=$(list "${_SESSIONS_DIR_}" "")
for e in ${sessions} ; do
  TRANSCRIPTS="${TRANSCRIPTS} sessions::${e}"
done

# check the number of arguments.
if [ ${#} -ne 1 ] ; then
  usage
  display ""
  exit -1
fi

# gets the argument.
TRANSCRIPT=${1}

# locate the paper.
LOCATION=$(locate "${TRANSCRIPTS}" "${TRANSCRIPT}" |
           substitute "::" "\/" "all")

# checks the result
if [ ${?} -ne 0 ] || [ "${LOCATION}" = "" ] ; then
  display " unknown transcript \"${TRANSCRIPT}\"" "!"
  display ""
  usage
  display ""
  exit -1
fi

# warns the user.
warning

# prepares the play.
prepare

# launchs the play.
play

# cleans the play.
clean

# displays some stuff.
display " session played sucessfully" "+"
