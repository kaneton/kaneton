## licence       kaneton licence
##
## project
##
## file          /home/mycure/kaneton/env/machines/gnu-unix/machine.sh
##
## created       julien quintard   [fri feb 11 02:08:31 2005]
## updated       julien quintard   [sun jul 30 22:49:58 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this file is composed of functions needed by the kaneton shell scripts.
#

#
# ---------- functions --------------------------------------------------------
#

#
# PRINT
#
# prints a message using the user variable _DISPLAY_.
#
# ${1}:		color
# ${2}:		text
# ${3}:		options
#
print()
{
  local color
  local text
  local options
  local opt

  color="${1}"
  text="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  "--no-newline")
	      options="${options} -n"
	      ;;
	  "--flickering")
	      text="\E[5m${text}\033[0m"
	      ;;
	  *)
	      ;;
      esac
  done

  if [ "${_DISPLAY_}" = "color" ] ; then
    case "$color" in
      "red")
        ${_ECHO_} -e ${options} '\E[;31m'"\033[1m${text}\033[0m"
        ;;

      "green")
        ${_ECHO_} -e ${options} '\E[;32m'"\033[1m${text}\033[0m"
        ;;

      "yellow")
        ${_ECHO_} -e ${options} '\E[;33m'"\033[1m${text}\033[0m"
        ;;

      "blue")
        ${_ECHO_} -e ${options} '\E[;34m'"\033[1m${text}\033[0m"
        ;;

      "white")
        ${_ECHO_} -e ${options} '\E[;37m'"\033[1m${text}\033[0m"
        ;;

      *)
        ${_ECHO_} -e ${options} "${text}"
        ;;
    esac
  else
    ${_ECHO_} -e ${options} "${text}"
  fi
}



#
# CONTENTS
#
# gets the contents of a file.
#
# ${1}:         file
# ${2}:		options
#
contents()
{
  local file
  local options
  local opt

  file="${1}"

  for opt in ${2} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_CAT_} ${options} ${file}
}



#
# TEMPORARY
#
# creates a temporary file or directory.
#
# ${1}:		options
#
temporary()
{
  local options
  local opt

  for opt in ${1} ; do
      case "${opt}" in
	  "--file")
	      ;;
	  "--directory")
	      options="${options} -d";
	      ;;
	  *)
	      ;;
      esac
  done

  ${_MKTEMP_} ${options}
}



#
# WORKING-DIRECTORY
#
# prints working directory
#
# ${1}:		options
#
working-directory()
{
  local options
  local opt

  for opt in ${1} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_PWD_} ${options}
}



#
# LOCATE
#
# locates a program on the system.
#
# ${1}:         program name
# ${2}:		options
#
locate()
{
  local program
  local options
  local opt

  program=${1}

  for opt in ${2} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_WHICH_} ${options} ${program} 2>&1
}

#
# SUBSTITUTE
#
# substitute matches with another one (like sed 's').
#
# ${1}:		to substitute
# ${2}:		substitute string
# ${3}:		options
#
substitute()
{
  local old
  local new
  local commands
  local options
  local opt

  old="${1}"
  new="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  "--everywhere")
	      commands="${commands}g"
	      ;;
	  *)
	      ;;
      esac
  done

  ${_SED_} ${options} "s/${old}/${new}/${commands}"
}



#
# DISPLAY
#
# displays a message with a header.
#
# ${1}:		message
# ${2}:		header
#
display()
{
  local message
  local header

  message="${1}"
  header="${2}"

  case "${header}" in
    "+")
      print "blue" "[" "--no-newline"
      print "green" "+" "--no-newline"
      print "blue" "]" "--no-newline"
      ;;
    "!")
      print "blue" "[" "--no-newline"
      print "red" "!" "--no-newline"
      print "blue" "]" "--no-newline"
      ;;
    "?")
      print "blue" "[" "--no-newline"
      print "yellow" "?" "--no-newline --flickering"
      print "blue" "]" "--no-newline"
      ;;
    *)
      ;;
  esac

  print "white" "${message}" ""
}



#
# WAIT-KEY
#
# this function just waits for a key.
#
# ${1}:		options
#
wait-key()
{
  local needless
  local options
  local opt

  for opt in ${1} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_READ_} ${options} needless
}



#
# LAUNCH-SHELL
#
# this function just launches a shell script.
#
# ${1}:		the shell script file
# ${2}:		arguments
# ${3}:		options
#
launch-shell()
{
  local file
  local arguments
  local options
  local opt

  file="${1}"
  arguments="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_SHELL_} ${options} ${file} ${arguments}
}



#
# LAUNCH-PYTHON
#
# this function just launches a new python script.
#
# ${1}:		the python script file
# ${2}:		arguments
# ${3}:		options
#
launch-python()
{
  local file
  local arguments
  local options
  local opt

  file="${1}"
  arguments="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_PYTHON_} ${options} ${file} ${arguments}
}



#
# LAUNCH-PERL
#
# this function just launches a new perl script.
#
# ${1}:		the perl script file
# ${2}:		arguments
# ${3}:		options
#
launch-perl()
{
  local file
  local arguments
  local options
  local opt

  file="${1}"
  arguments="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_PERL_} ${options} ${file} ${arguments}
}



#
# LAUNCH-MAKE
#
# launches a new makefile.
#
# ${1}:		the make file to launch
# ${2}:		the directory list
# ${3}:		arguments
# ${4}:		options
#
launch-make()
{
  local file
  local wd
  local directories
  local dir
  local arguments
  local options
  local opt

  file="${1}"
  directories="${2}"
  arguments="${3}"

  for opt in ${4} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  for dir in "${directories}" ; do
    wd=$(working-directory)
    change-directory "${dir}" ""
    if [ ${?} -ne 0 ] ; then
      exit -1
    fi
    ${_MAKE_} -f ${file} ${options} ${_MAKEFLAGS_} ${arguments}
    change-directory "${wd}" ""
  done
}



#
# LAUNCH-PROGRAM
#
# launches a new program
#
# ${1}:		the file to launch
# ${2}:		arguments
# ${3}:		options
#
launch-program()
{
  local file
  local arguments
  local options
  local opt

  file="${1}"
  arguments="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${file} ${options} ${arguments}
}



#
# LAUNCH
#
# this function just launches a new program, script etc.
#
launch()
{
  case ${1} in
      *.sh)
	  launch-shell "${1}" "${2}" "${3}"
	  ;;
      *.py)
	  launch-python "${1}" "${2}" "${3}"
	  ;;
      *.pl)
	  launch-perl "${1}" "${2}" "${3}"
	  ;;
      Makefile)
	  launch-make "${1}" "${2}" "${3}" "${4}"
	  ;;
      *)
	  launch-program "${1}" "${2}" "${3}"
	  ;;
  esac
}



#
# COPY
#
# this function copies a file.
#
# ${1}:		source file
# ${2}:		destination file
# ${3}:		options
#
copy()
{
  local source
  local destination
  local options
  local opt

  source="${1}"
  destination="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_CP_} ${options} ${source} ${destination}
}



#
# LINK
#
# this function performs a link between two files.
#
# ${1}:		source file
# ${2}:		destination file
# ${3}:		options
#
link()
{
  local source
  local destination
  local options
  local opt

  source="${1}"
  destination="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_LN_} ${options} ${destination} ${source}
}



#
# REMOVE
#
# this function just removes a file.
#
# ${1}:		files
# ${2}:		options
#
remove()
{
  local files
  local f
  local options
  local opt

  files="${1}"

  for opt in ${2} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  for f in ${files} ; do
    if [ -d "${f}" ] ; then
      ${_RM_} ${options} -Rf ${f}
    else
      ${_RM_} ${options} -f ${f}
    fi
  done
}



#
# LIST
#
# this function returns the list of the directory entries.
#
# ${1}:		directory
# ${2}:		options
#
list()
{
  local directory
  local options
  local opt
  local output
  local o
  local e

  directory="${1}"

  for opt in ${2} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  o=$(${_LS_} ${options} ${directory})

  for opt in ${2} ; do
      case "${opt}" in
	  "--file")
	      for e in ${o} ; do
		  if [ -f "${directory}/${e}" ] ; then
		      output="${output} ${e}"
		  fi
	      done
	      ;;
	  "--directory")
	      for e in ${o} ; do
		  if [ -d "${directory}/${e}" ] ; then
		      output="${output} ${e}"
		  fi
	      done
	      ;;
	  *)
	      ;;
      esac
  done

  echo ${output}
}



#
# CHANGE DIRECTORY
#
# this function just changes the current working directory.
#
# ${1}:		directory
# ${2}:		options
#
change-directory()
{
  local directory
  local options
  local opt

  directory="${1}"

  for opt in ${2} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_CD_} ${options} ${directory}
}



#
# PREPROCESS
#
# this function preprocess a file
#
# ${1}:		file to process
# ${2}:		additional includes
# ${3}:		options
#
preprocess()
{
  local file
  local includes
  local inc
  local options
  local opt

  file="${1}"

  for opt in ${3} ; do
      case "${opt}" in
	  "--no-markers")
	      options="${options} -P"
	      ;;
	  *)
	      ;;
      esac
  done

  for inc in ${2} ; do
      includes="${includes} -include ${inc}"
  done

  ${_CPP_} ${_INCLUDES_} ${options} ${includes} ${file}
}



#
# SEARCH
#
# search files given a pattern
#
# ${1}:		place to search for
# ${2}:		files to match
# ${3}:		options
#
search()
{
  local path
  local pattern
  local options
  local opt

  path="${1}"
  pattern="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  "--file")
	      options="${options} -type f"
	      ;;
	  "--directory")
	      options="${options} -type d"
	      ;;
	  *)
	      ;;
      esac
  done

  ${_FIND_} ${path} -name "${pattern}" ${options}
}



#
# PACK
#
# this function creates a package from a source directory.
#
# ${1}:		directory
# ${2}:		destination file
# ${3}:		options
#
pack()
{
  local directory
  local file
  local options
  local opt

  directory="${1}"
  file="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_TAR_} -czf ${options} ${file} ${directory}
}



#
# UNPACK
#
# this function unpackages a file.
#
# ${1}:		directory
# ${2}:		destination file
# ${3}:		options
#
unpack()
{
  local directory
  local file
  local options
  local opt

  directory="${1}"
  file="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  if [ -n ${directory} ] ; then
    ${_TAR_} -xzf ${options} ${file} -C ${directory}
  else
    ${_TAR_} -xzf ${options} ${file}
  fi
}



#
# MAKE DIRECTORY
#
# this function creates a directory.
#
# ${1}:		directory
# ${2}:		options
#
make-directory()
{
  local directory
  local options
  local opt

  directory="${1}"

  for opt in ${2} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_MKDIR_} ${options} ${directory}
}



#
# CUT
#
# cut a text part.
#
# ${1}:		head pattern
# ${2}:		tail pattern
# ${3}:		options
#
cut()
{
  local head
  local tail
  local options
  local opt
  local mode

  head="${1}"
  tail="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  "--delete")
	      mode=${opt}
	      ;;
	  "--print")
	      mode=${opt}
	      ;;
	  *)
	      ;;
      esac
  done

  case "${mode}" in
      "--delete")
	  ${_SED_} -e "/${head}/,/${tail}/d"
	  ;;
      "--print")
	  ${_SED_} -ne "/${head}/,/${tail}/p"
	  ;;
      *)
	  ;;
  esac
}



#
# LOAD
#
# copies a file on a device.
#
# ${1}:		source file
# ${2}:		destination device or image
# ${3}:		destination path
# ${4}:		options
#
load()
{
  local source
  local device
  local destination
  local mode
  local options
  local opt

  source="${1}"
  device="${2}"
  destination="${3}"

  for opt in ${4} ; do
      case "${opt}" in
	  "--image")
	      mode=${opt}
	      ;;
	  "--device")
	      mode=${opt}
	      ;;
      esac
  done

  case "${mode}" in
      "--image")
	  ${_MCOPY_} -o -n ${options} -i ${device} ${source} ::/${destination}
	  ;;
      "--device")
	  ${_MCOPY_} -o -n ${options} ${source} ${device}/${destination}
	  ;;
      *)
	  ;;
  esac
}



#
# STAMP
#
# this function return current date formatted
#
# ${1}:		format
# ${2}:		options
#
stamp()
{
  local format
  local options
  local opt

  format="${1}"

  for opt in ${2} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_DATE_} ${options} +${format}
}



#
# RECORD
#
# this function runs the program used to record a session.
#
# ${1}:		log file
# ${2}:		time file
# ${3}:		options
#
record()
{
  local log
  local time
  local options
  local opt

  log="${1}"
  time="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_SCRIPT_} -q -t ${log} -c ${_TRANSCRIPTS_CMD_} 2> ${time}
}



#
# PLAY
#
# this function runs the program used to play a session.
#
# ${1}:		log file
# ${2}:		time file
# ${3}:		options
#
play()
{
  local log
  local time
  local options
  local opt

  log="${1}"
  time="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  launch "${_SCRIPTREPLAY_TOOL_}" "${time} ${log}" ""
}



#
# LOOKUP
#
# this function tries to locate an element in an array.
#
# ${1}:		the array
# ${2}:		the element looked for
# ${3}:		options
#
lookup()
{
  local array
  local element
  local options
  local opt

  array="${1}"
  element="${2}"

  for opt in ${3} ; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  for e in ${array} ; do
    ${_ECHO_} ${e} | ${_GREP_} ${element} 2>/dev/null 1>/dev/null

    if [ ${?} -eq 0 ] ; then
      ${_ECHO_} ${e}
      return
    fi
  done
}
