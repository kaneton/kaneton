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
TEMPORARY=""

##
## ---------- functions -------------------------------------------------------
##

#
# CLEAN
#
# this function cleans a string removing heading and trailing
# whitespaces and newlines.
#
clean()
{
  local string="${1}"
  local length=0
  local s

  while [ true ] ; do
    s=${string}

    s=${s# }
    s=${s#
}

    s=${s% }
    s=${s%
}

    if [ "${string}" = "${s}" ] ; then
      break
    fi

    string="${s}"
  done

  echo "${string}"
}



#
# GET-VALUE
#
# this function tries to locate a variable's value in the input files.
#
get-value()
{
  local files="${1}"
  local name="${2}"
  local value=""
  local v
  local f

  files=$(eval echo "${files}")

  for f in ${files} ; do
    v=$(contents "${f}" ""						| \
        substitute "^(((override)?${name}[ \t]*.?=[ \t]*(.*))|(.*))$" \
                   "\4"							\
                   "")

    if [ "${v}" != "" ] ; then
      value="${v}"
    fi
  done

  print "" "${value}" ""
}



#
# SET-VALUE
#
# this functions sets a new value for the given variable in the file OUTPUT.
#
set-value()
{
  local file="${1}"
  local variable="${2}"
  local value="${3}"
  local v

  file=$(eval echo "${file}")

  v=$(contents "${file}" ""						| \
      substitute "^(((override)?${name}[ \t]*.?=[ \t]*(.*))|(.*))$"	\
                 "\4"							\
                 "")

  if [ "${v}" != "" ] ; then
    $(contents "${file}" ""						| \
      substitute "^(((override)?${variable}[ \t]*.?=[ \t]*)(.*))$"	\
                 "\2${value}"						\
                 ""							> \
          ${TEMPORARY})

      copy "${TEMPORARY}" "${file}"
  else
    $(print ""								\
            "${variable}	:=	${value}"			\
            ""								>> \
      ${file})
  fi
}



#
# GENERATE-MENU
#
# this function generates a menu based on the configuration/description
# file FILE.
#
generate-menu()
{
  local entries="${1}"
  local menu=""
  local n=1
  local e

  for e in ${entries} ; do
    if [ "${e}" != "" ] ; then
      menu="${menu} ${n} ${e}"
      n=$(( ${n} + 1 ))
    fi
  done

  print "" "${menu}" ""
}



#
# CHOICE
#
# this function displays a radiolist box.
#
choice()
{
  local name="${1}"
  local input="${2}"
  local output="${3}"
  local choices="${4}"
  local values="${5}"
  local description="${6}"
  local v
  local n
  local x

#
# display the variable's description
#

  kdialog --title "[kaneton] monitor > ${TITLE}"			\
          --msgbox							\
									\
"

  ${description}

"									\
									\
          21 60

#
# display the radiolist box
#

  kdialog --title "[kaneton] monitor > ${TITLE}"			\
          --radiolist "Please choose a new value."			\
          15 60								\
          10 ${choices} 2> ${TEMPORARY}

#
# get the user's choice
#

    case ${?} in
      0)
        x=$(cat ${TEMPORARY})

        n=1
        for v in ${values} ; do
          if [ ${n} -eq ${x} ] ; then
            break
          fi

          n=$(( ${n} + 1 ))
        done

#
# check if something has changed
#

        x=$(get-value "${input}" "${name}")

        if [ "${v}" = "${x}" ] ; then
          return
        fi

#
# set the new variable's value
#

        set-value "${output}" "${name}" "${v}"
        ;;
      *)
        return
        ;;
    esac
}



#
# INPUT
#
# this function displays an input box.
#
input()
{
  local name="${1}"
  local input="${2}"
  local output="${3}"
  local value="${4}"
  local description="${5}"
  local v
  local x

#
# display the variable's description
#

  kdialog --title "[kaneton] monitor > ${TITLE}"			\
          --msgbox							\
									\
"

  ${description}

"									\
									\
          21 60

#
# display the radiolist box
#

    kdialog --title "[kaneton] monitor > ${TITLE}"			\
            --inputbox "Please enter a new value."			\
          7 60								\
          ${value} 2> ${TEMPORARY}

#
# get the user's choice
#

    case ${?} in
      0)
        v=$(cat ${TEMPORARY})

#
# check if something has changed
#

        x=$(clean "$(get-value "${input}" "${name}")")

        if [ "${v}" = "${x}" ] ; then
          return
        fi

#
# set the new variable's value
#

        set-value "${output}" "${name}" "${v}"
        ;;
      *)
        return
        ;;
    esac
}



#
# SUBSECTION
#
# this function calls another configure script for the selected subsection.
#
subsection()
{
  local section="${1}"

  section=$(print "" "${section}" "" | substitute "^\[(.*)\]$" "\1" "")

  ${_SHELL_} "${CONFIGURE_SH}" "${TITLE} > ${section}"			\
             "${DIRECTORY}/${section}" "${section}"
}



#
# VARIABLE
#
# this function asks the user to modify the variable.
#
variable()
{
  local entry="${1}"
  local name
  local type
  local input
  local output
  local choices
  local choice
  local values
  local description
  local content
  local length
  local value
  local c
  local v
  local n=1

  content=$(print "" "${CONTENT}" ""					| \
            cut "^${entry}$" "^\}$" "--print"				| \
            substitute "^[^ ].*$" "" "")

  name=$(clean							\
         "$(print "" "${content}" ""					| \
            substitute "^(([ \t]*\[variable\] (.*))|(.*))$" "\3" "")")

  type=$(clean							\
         "$(print "" "${content}" ""					| \
            substitute "^(([ \t]*\[type\] (.*))|(.*))$" "\3" "")")

  input=$(clean							\
          "$(print "" "${content}" ""					| \
             substitute "^(([ \t]*\[input\] (.*))|(.*))$" "\3" "")")

  output=$(clean							\
           "$(print "" "${content}" ""					| \
              substitute "^(([ \t]*\[output\] (.*))|(.*))$" "\3" "")")

  description=$(clean						\
                "$(print "" "${content}" ""				| \
                   substitute "^([ \t]*\[.*)$" "" "")")

  value=$(clean "$(get-value "${input}" "${name}")")

  choices=()
  values=()

  while [ true ] ; do
    choice=$(clean							\
             "$(print "" "${content}" ""				| \
                substitute "^(([ \t]*\[choice ${n}\] (.*))|(.*))$" "\3" "")")

    if [ "${choice}" = "" ] ; then
      break
    fi

    c=$(clean								\
        "$(print "" "${choice}" ""					| \
           substitute "^(.*)[ \t]*::[ \t]*(.*)$" "\1" "")")

    v=$(clean								\
        "$(print "" "${choice}" ""					| \
           substitute "^(.*)[ \t]*::[ \t]*(.*)$" "\2" "")")

    if [ "${value}" = "${v}" ] ; then
      choices="${choices} ${n} ${c} on"
    else
      choices="${choices} ${n} ${c} off"
    fi

    values="${values} ${v}"

    n=$(( ${n} + 1 ))
  done

  case "${type}" in
    "choice")
      choice "${name}" "${input}" "${output}"				\
             "${choices}" "${values}" "${description}"
      ;;
    "input")
      input "${name}" "${input}" "${output}" "${value}" "${description}"
    ;;
  esac
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
  local section
  local content
  local entries
  local entry
  local menu
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
# get the file's content and skip comments
#

  CONTENT=$(contents "${FILE}"						| \
            substitute "^#.*$" "" "")

#
# generate the menu
#

  entries=$(print "" "${CONTENT}" ""					| \
            cut "^\{$" "^\}$" "--delete"				| \
            substitute "^#.*$" "" "")

  menu=$(generate-menu "${entries}")

  while [ true ] ; do

#
# display the menu
#

    kdialog --title "[kaneton] monitor > ${TITLE}"			\
            --menu ""							\
            20 70							\
            15								\
            ${menu} 2> ${TEMPORARY}

#
# get the selected entry
#

    case ${?} in
      0)
        x=$(cat ${TEMPORARY})

        n=1
        for e in ${entries} ; do
          if [ "${e}" != "" ] ; then

            if [ ${n} -eq ${x} ] ; then
              entry="${e}"
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
# perform tha appropriate action
#

    section=$(lookup "${entry}" "^\[.*\]$" "")

    if [ "${section}" != ""  ] ; then
      subsection "${section}"
    else
      variable "${entry}"
    fi

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
  configure "configure" "${_CONFIGURATION_DIR_}" "configure"
else
  configure "${1}" "${2}" "${3}"
fi
