##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/configure/configure.sh
##
## created       julien quintard   [sat dec  2 14:59:36 2006]
## updated       julien quintard   [mon dec  4 11:25:16 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

source configure/detect.sh

source ../env/.env.sh

##
## ---------- globals ---------------------------------------------------------
##

CONFIGURE_SH="configure/configure.sh"

TITLE=""
DIRECTORY=""
SECTION=""
FILE=""
CONTENT=""
ENTRIES=""
INPUT=""
OUTPUT=""
ENTRY=""
MENU=""
TEMPORARY=""

##
## ---------- functions -------------------------------------------------------
##

#
# GET_VALUE
#
# this function tries to locate a variable's value in the files INPUT/OUTPUT.
#
get_value()
{
  local variable="${1}"
  local i
  local o

  o=$(contents "${OUTPUT}" ""						| \
      substitute "^(((override)?${variable}[ \t]*:?=[ \t]*(.*))|(.*))$"	\
                   "\4"							\
                   "")

  if [ "${o}" != "" ] ; then
    print "" "${o}" ""
  else
    i=$(contents "${INPUT}" ""						| \
        substitute "^(((override)?${variable}[ \t]*:?=[ \t]*(.*))|(.*))$" \
                   "\4"							\
                   "")

    if [ "${i}" != "" ] ; then
      print "" "${i}" ""
    fi
  fi
}



#
# SET_VALUE
#
# this functions sets a new value for the given variable in the file OUTPUT.
#
set_value()
{
  local variable="${1}"
  local value="${2}"
  local o

  o=$(contents "${OUTPUT}" ""						| \
      substitute "^(((override)?${variable}[ \t]*:?=[ \t]*(.*))|(.*))$" \
                   "\4"							\
                   "")

  if [ "${o}" != "" ] ; then
    $(contents "${OUTPUT}" ""						| \
      substitute "^(((override)?${variable}[ \t]*:?=[ \t]*)(.*))$"	\
                 "\2${value}"						\
                 ""							> \
          ${TEMPORARY})

      copy "${TEMPORARY}" "${OUTPUT}"
  else
    $(print ""								\
            "${variable}			:=		${value}"  \
            ""								>> \
      ${OUTPUT})
  fi
}



#
# MENU
#
# this function generates a menu based on the configuration/description
# file FILE.
#
menu()
{
  local menu=""
  local n=1
  local e

  for e in ${ENTRIES} ; do
    if [ "${e}" != "" ] ; then
      menu="${menu} ${n} ${e}"
      n=$(( ${n} + 1 ))
    fi
  done

  print "" "${menu}" ""
}



#
# HANDLE
#
# this function launch an action corresponding on the selected menu entry.
#
handle()
{
  local description
  local entry
  local type
  local text
  local v

#
# is the selected entry a subsection?
#

  type=$(lookup "${ENTRY}" "^\[.*\]$" "")

  if [ "${type}" != ""  ] ; then

#
# launch a new configure.sh for the subsection
#

    ENTRY=$(print "" "${ENTRY}" "" | substitute "^\[(.*)\]$" "\1" "")

    ${_SHELL_} "${CONFIGURE_SH}" "${TITLE} > ${ENTRY}"			\
               "${DIRECTORY}/${ENTRY}" "${ENTRY}"

  else

#
# display a description of the variable and ask the user to enter the
# new value.
#
# then update the value in the corresponding configuration file
#

    description=$(print "" "${CONTENT}" ""				| \
                  cut "${ENTRY}" "^\}$" "--print"			| \
                  substitute "^[^ ].*$" "" "")

#
# get the variable's current value
#
    v=$(get_value "${ENTRY}")

#
# display the variable's description
#

    kdialog --title "[kaneton] monitor > ${TITLE}"			\
            --msgbox "${description}"					\
            21 60

#
# get a new value from the user
#

    kdialog --title "[kaneton] monitor > ${TITLE}"			\
            --inputbox "Please enter a new value for ${ENTRY}."		\
          7 70								\
          ${v} 2> ${TEMPORARY}

    case ${?} in
      0)

#
# set the new variable's value
#

        set_value "${ENTRY}" "$(cat ${TEMPORARY})"
        ;;
      *)
        return
        ;;
    esac

  fi
}



#
# CONFIGURE
#
# this is the core function.
#
# its function is first to display the menu, then to get the user's choice
# and finally to call the handle function.
#
configure()
{
  local content
  local e
  local x
  local n

#
# set the globals
#
  TITLE="${1}"
  DIRECTORY="${2}"
  SECTION="${3}"
  FILE="${DIRECTORY}/${SECTION}.conf"

#
# get the file's content
#

  CONTENT=$(contents "${FILE}")

#
# skip comments
#

  CONTENT=$(print "" "${CONTENT}" ""					| \
            substitute "^#.*$" "" "")

#
# locate the input/output configuration files in relation with this section
#

  INPUT=$(print "" "${CONTENT}" ""					| \
          substitute "^((<input>[ \t]*(.+))|(.*))$" "\3" "")
  INPUT=$(eval echo ${INPUT})

  OUTPUT=$(print "" "${CONTENT}" ""					| \
           substitute "^((<output>[ \t]*(.+))|(.*))$" "\3" "")
  OUTPUT=$(eval echo ${OUTPUT})

#
# remove the input/output directives
#

  CONTENT=$(print "" "${CONTENT}" ""					| \
            substitute "^((<.*>.*)|(.*))$" "\3" "")

#
# generate the menu
#

  ENTRIES=$(print "" "${CONTENT}" ""					| \
            cut "^\{$" "^\}$" "--delete"				| \
            substitute "^#.*$" "" "")
  MENU=$(menu)

  while [ true ] ; do

#
# display the menu
#

    kdialog --title "[kaneton] monitor > ${TITLE}"			\
            --menu ""							\
            20 70							\
            15								\
            ${MENU} 2> ${TEMPORARY}

#
# get the selected entry
#

    case ${?} in
      0)
        x=$(cat ${TEMPORARY})

        n=1
        for e in ${ENTRIES} ; do
          if [ "${e}" != "" ] ; then

            if [ ${n} -eq ${x} ] ; then
              ENTRY="${e}"
              break
            fi

            n=$(( ${n} + 1 ))

          fi
        done
        ;;
      *)
        exit 0
        ;;
    esac

#
# handle the entry
#

    handle

  done
}

#
# create a temporary file
#

TEMPORARY=$(temporary)

#
# call the configure function
#

if [ ${#} -eq 0 ] ; then
  configure "configure" "configure" "configure"
else
  configure "${1}" "${2}" "${3}"
fi
