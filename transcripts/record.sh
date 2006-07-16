## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/transcripts/record.sh
##
## created       julien quintard   [mon apr 10 15:01:23 2006]
## updated       julien quintard   [sun jul 16 13:41:23 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: transcripts/
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
  display "" ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key ""
}



#
# PREPARE
#
# this function prepares the record.
#
prepare()
{
  TEMP=$(temporary "--directory")

  DIRECTORY="${TEMP}/transcript"

  make-directory "${DIRECTORY}" ""

  LOG="${DIRECTORY}/log"
  TIME="${DIRECTORY}/time"
}



#
# BUILD
#
# this function builds the record.
#
build()
{
  local cwd

  cwd=$(working-directory "")

  change-directory "${TEMP}" ""

  pack "transcript" "${cwd}/${LOCATION}" ""

  change-directory "${cwd}" ""

  remove "${TEMP}" ""
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

  display " already existing transcripts:" "!"

  for t in ${TRANSCRIPTS} ; do
    display "   ${t}" "!"
  done
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display "" ""

# gets the transcripts list.
basics=$(list "${_BASICS_DIR_}" "--file")
for e in ${basics} ; do
  TRANSCRIPTS="${TRANSCRIPTS} basics::${e}"
done

# check the number of arguments.
if [ ${#} -ne 1 ] ; then
  usage
  display "" ""
  exit -1
fi

# gets the argument.
TRANSCRIPT="${1}"

# locate the paper.
LOCATION=$(print "" "${TRANSCRIPT}" "" |				\
           substitute "::" "\/" "--everywhere")

# checks the location
if [ -e ${LOCATION} ] ; then
  display " transcript \"${TRANSCRIPT}\" already exists" "!"
  display "" ""
  usage
  display "" ""
  exit -1
fi

# warns the user.
warning

# prepares the record.
prepare

# launchs the record.
display " recording the session ..." "+"
record "${LOG}" "${TIME}" ""

# builds the record.
build

# displays some stuff.
display " session recorded sucessfully" "+"

# displays some stuff.
display "" ""
