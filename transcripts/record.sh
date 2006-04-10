#! /bin/bash

## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/transcripts/record.sh
##
## created       julien quintard   [mon apr 10 15:01:23 2006]
## updated       julien quintard   [mon apr 10 16:23:16 2006]
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
# this function prepares the record.
#
prepare()
{
  TEMP=$(tempdirectory)

  DIRECTORY="${TEMP}/transcript"

  make-directory "${DIRECTORY}"

  LOG="${DIRECTORY}/log"
  TIME="${DIRECTORY}/time"
}

#
# RECORD
#
# this function records the session.
#
record()
{
  # displays some stuff.
  display " recording the session ..." "+"

  # XXX make a function for it
  script -q -t ${LOG} -c ${_TRANSCRIPTS_CMD_} 2> ${TIME}
}

#
# BUILD
#
# this function builds the record.
#
build()
{
  local cwd

  cwd=$(working-directory)

  change-directory "${TEMP}"

  pack "transcript" "${cwd}/${LOCATION}"

  change-directory "${cwd}"

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

  display " usage: record.sh [transcript]" "!"

  display ""

  display " already existing transcripts:" "+"
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
LOCATION=$(print "" "${TRANSCRIPT}" "" |
           substitute "::" "\/" "all")

# checks the location
if [ -e ${LOCATION} ] ; then
  display " transcript \"${TRANSCRIPT}\" already exist" "!"
  display ""
  usage
  display ""
  exit -1
fi

# warns the user.
warning

# prepares the record.
prepare

# launchs the record.
record

# builds the record.
build

# displays some stuff.
display " session recorded sucessfully" "+"
