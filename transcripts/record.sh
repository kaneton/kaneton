#! /bin/bash

## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/transcripts/record.sh
##
## created       julien quintard   [mon apr 10 15:01:23 2006]
## updated       julien quintard   [mon apr 10 15:11:56 2006]
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

  script -q -t ${LOG} 2> ${TIME} # XXX make a function for it
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

  pack "transcript" "${cwd}/${FILE}"

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
  display " usage: record.sh [file]" "!"
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

# prepares the record.
prepare

# launchs the record.
record

# builds the record.
build

# displays some stuff.
display " session recorded sucessfully" "+"
