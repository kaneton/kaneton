## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/cheat/cheat.sh
##
## created       julien quintard   [sat feb 18 19:09:32 2006]
## updated       julien quintard   [sun jul 16 18:56:37 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in the directory cheat/
#
# this script looks for cheats into students kaneton implementations
#

#
# ---------- dependencies -----------------------------------------------------
#

source			../environment/.env.sh

#
# ---------- globals ----------------------------------------------------------
#

STAGES="k0 k1 k2 k3 k4 k5 k6 k7 k8 k9"

YEAR=""
STAGE=""

TEMP=""

GFILE="groups"

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
  local y
  local s

  display " usage: cheat.sh [year].[stage]" "!"
  display "" ""

  display " available years:" "!"

  for y in ${YEARS} ; do
    display "   ${y}" "!"
  done

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
  display "   filter:                   ${_FILTER_}" "+"
  display "" ""
  display "   year:                     ${YEAR}" "+"
  display "   stage:                    ${STAGE}" "+"
  display "" ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key ""
}



#
# GENERATE
#
# this function generates the missing token fingerprints.
#
generate()
{
  local groups
  local y
  local g

  for y in ${YEARS} ; do
    display "   ${y}" "+"

    groups=$(list "${y}" "--directory")

    for g in ${groups} ; do
      if [ ! -e "${y}/${g}/fingerprints/${STAGE}.ctf" ] ; then
        if [ -e "${y}/${g}/sources/${STAGE}.tar.gz" ] ; then
          make-directory "${TEMP}/${y}-${g}/" ""

          unpack "${TEMP}/${y}-${g}/" "${y}/${g}/sources/${STAGE}.tar.gz" ""

          if [ -d "${TEMP}/${y}-${g}/kaneton" ] ; then
            display "     ${g}" "+"

            launch "${_CTBUILD_TOOL_}"					\
                   "${TEMP}/${y}-${g}/kaneton/				\
                    ${y}/${g}/fingerprints/${STAGE}.ctf" ""
          else
            display " no \"kaneton\" directory found once extracted" "!"
          fi
        fi
      fi
    done

    if [ "${y}" = "${YEAR}" ] ; then
      break;
    fi
  done
}



#
# CHEAT
#
# this function checks for any kind of cheat between different kaneton
# sources.
#
cheat()
{
  local groups
  local grps
  local y
  local g
  local h

  groups=$(list "${YEAR}" "--directory")

  for g in ${groups} ; do
    display "   ${g}" "+"

    for y in ${YEARS} ; do
      grps=$(list "${_CHEAT_DIR_}/${y}/" "--directory")

      for h in ${grps} ; do
        if [ -e "${_CHEAT_DIR_}/${YEAR}/${g}/fingerprints/${STAGE}.ctf" ] &&
           [ -e "${_CHEAT_DIR_}/${y}/${h}/fingerprints/${STAGE}.ctf" ] ; then
          if [ ! "${y}/${h}" = "${YEAR}/${g}" ] ; then
            display "     ${y}/${h}" "+"

            launch "${_CTCOMPARE_TOOL_}"				\
              "-s ${_CHEAT_DIR_}/${YEAR}/${g}/fingerprints/${STAGE}.ctf	\
               ${_CHEAT_DIR_}/${y}/${h}/fingerprints/${STAGE}.ctf" ""	\
	      > "${_CHEAT_DIR_}/${YEAR}/${g}/cheats/${STAGE}.${y}-${h}"
          fi
        fi
      done

      if [ "${y}" = "${YEAR}" ] ; then
        break;
      fi
    done
  done
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display "" ""

# gets the years liste.
YEARS=$(list "${_CHEAT_DIR_}" "--directory")

# check the number of arguments.
if [ ${#} -ne 1 ] ; then
  usage
  display "" ""
  exit -1
fi

# prepares the tools.
launch "Makefile" "${_CTCOMPARE_DIR_}" "" ""

# displays some stuff.
display "" ""

# relocates and gets the arguments
args=$(print "" "${1}" "" | substitute "\." " " "--everywhere")

declare -a array
i=0
for a in ${args} ; do
  array[${i}]="${a}"

  let "i += 1"
done

# checks the number of arguments.
if [ ${#array[*]} -ne 2 ] ; then
  usage
  display "" ""
  exit -1
fi

year="${array[0]}"
stage="${array[1]}"

# locate the year.
YEAR=$(lookup "${YEARS}" "${year}" "")

# checks the result
if [ "${YEAR}" = "" ] ; then
  display " unknown year \"${year}\"" "!"
  display "" ""
  usage
  display "" ""
  exit -1
fi

# locate the stage.
STAGE=$(lookup "${STAGES}" "${stage}" "")

# checks the result
if [ "${STAGE}" = "" ] ; then
  display " unknown stage \"${stage}\"" "!"
  display "" ""
  usage
  display "" ""
  exit -1
fi

# builds a temporary directory.
TEMP=$(temporary "--directory")

# warns the user.
warning

# display some stuff.
display " generating missing fingerprints" "+"

# generates the missing token fingerprints.
generate

# display some stuff.
display " looking for any kind of cheat" "+"

# looks for any kind of cheat.
cheat

# end of script
display " cheats checked successfully" "+"

# displays some stuff.
display "" ""
