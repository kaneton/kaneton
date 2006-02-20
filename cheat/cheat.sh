#! /bin/bash

## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/cheat/cheat.sh
##
## created       julien quintard   [sat feb 18 19:09:32 2006]
## updated       julien quintard   [mon feb 20 22:48:23 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in the directory src/cheat
#
# this script looks for cheats into students kaneton implementations
#

#
# ---------- dependencies -----------------------------------------------------
#

source			../env/.env.sh

#
# ---------- globals ----------------------------------------------------------
#

YEARS="2005 2006"
STAGES="k1 k2 k3 k4 k5 k6"

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
  display " usage: cheat.sh [year] [stage]" "!"
  display ""
  display " available years: ${YEARS}"
  display " available stages: ${STAGES}"
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
  display "   year:                     ${YEAR}" "+"
  display "   stage:                    ${STAGE}" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  wait-key
}

#
# GENERATE
#
# this function generates the missing token fingerprints.
#
generate()
{
  local y
  local g

  for y in ${YEARS} ; do
    display "   ${y}" "+"

    check=$(contents ${y}/${GFILE})

    for g in ${check} ; do
      if [ ! -e "${y}/${g}/fingerprints/${STAGE}.ctf" ] ; then
        if [ -e "${y}/${g}/sources/${STAGE}.tar.gz" ] ; then
          make-directory "${TEMP}/${y}-${g}/" ""

          unpack "${TEMP}/${y}-${g}/" "${y}/${g}/sources/${STAGE}.tar.gz" ""

          if [ -d "${TEMP}/${y}-${g}/kaneton" ] ; then
            display "     ${g}" "+"

            ${_CTBUILD_TOOL_} "${TEMP}/${y}-${g}/kaneton/"		\
                              "${y}/${g}/fingerprints/${STAGE}.ctf"
          else
            display " no \"kaneton\" directory found once extracted" "!"
          fi
        fi
      fi
    done

    if [ ${y} = ${YEAR} ] ; then
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
  local check
  local y
  local g
  local c

  groups=$(contents ${YEAR}/${GFILE})

  for g in ${groups} ; do
    display "   ${g}" "+"

    for y in ${YEARS} ; do
      check=$(contents ${_CHEAT_DIR_}/${y}/${GFILE})

      for c in ${check} ; do
        if [ -e "${_CHEAT_DIR_}/${YEAR}/${g}/fingerprints/${STAGE}.ctf" ] &&
           [ -e "${_CHEAT_DIR_}/${y}/${c}/fingerprints/${STAGE}.ctf" ] ; then
          if [ ! "${y}/${c}" = "${YEAR}/${g}" ] ; then
            display "     ${y}/${c}" "+"

            ${_CTCOMPARE_TOOL_} -n "${_CHEAT_IGNORE_}"			\
              "${_CHEAT_DIR_}/${YEAR}/${g}/fingerprints/${STAGE}.ctf"	\
              "${_CHEAT_DIR_}/${y}/${c}/fingerprints/${STAGE}.ctf"	| \
              grep -v ".h:"						| \
              sort -rn							| \
              substitute "^([0-9]+)  (.*)  (.*)$"			\
                         "---\| \1\n   \| \2\n   \| \3\n"		\
                         "" >						\
              "${_CHEAT_DIR_}/${YEAR}/${g}/cheats/${STAGE}.${y}-${c}"
          fi
        fi
      done

      if [ ${y} = ${YEAR} ] ; then
        break;
      fi

    done
  done
}

#
# ---------- entry point ------------------------------------------------------
#

# displays some stuff.
display ""

# check the number of arguments.
if [ ${#} -lt 2 ] ; then
  usage
  exit -1
fi

# gets the arguments.
year="${1}"
stage="${2}"

# locate the year.
YEAR=$(locate "${YEARS}" "${year}")

# checks the result
if [ ${?} -ne 0 ] ; then
  display " unknown year \"${year}\"" "!"
  display ""
  usage
  display ""
  exit -1
fi

# locate the stage.
STAGE=$(locate "${STAGES}" "${stage}")

# checks the result
if [ ${?} -ne 0 ] ; then
  display " unknown stage \"${stage}\"" "!"
  display ""
  usage
  display ""
  exit -1
fi

# builds a temporary directory.
TEMP=$(tempdirectory)

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
display ""
