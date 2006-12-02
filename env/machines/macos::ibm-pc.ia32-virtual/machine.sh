#!/bin/bash
## licence       kaneton licence
##
## project
##
## file          /home/buckman/kaneton/kaneton/env/machines/unix/machine.sh
##
## created       julien quintard   [fri feb 11 02:08:31 2005]
## updated       matthieu bucchianeri   [sun dec 18 15:54:37 2005]
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

  color="${1}"
  text="${2}"
  for opt in ${3}; do
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

  file="${1}"
  for opt in ${2}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_CAT_} ${file}
}



#
# TEMPFILE
#
# gives an unique temporary file name.
#
# ${1}:		options
#
tempfile()
{
  for opt in ${1}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_MKTEMP_}
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
  for opt in ${1}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_PWD_}
}



#
# LOCATE-PROG
#
# locates a program on the system.
#
# ${1}:         program
# ${2}:		options
#
locate-prog()
{
  local program

  program=${1}
  for opt in ${2}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_WHICH_} ${program}
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

  old="${1}"
  new="${2}"

  case "${3}" in
      "all")
	  opt="g"
	  ;;
      "[1-9]+")
	  opt="${3}"
	  ;;
      *)
	  opt=""
	  ;;
  esac

  ${_SED_} "s/${old}/${new}/${opt}"
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
# WAIT KEY
#
# this function just waits for a key.
#
# ${1}:		options
#
wait-key()
{
  local needless

  for opt in ${1}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_READ_} needless
}



#
# LAUNCH
#
# this function just launches a new program, script etc..
#
# ${1}:		file
# ${2}:		arguments
# ${3}:		options
#
launch()
{
  local file
  local arguments
  local options

  file="${1}"
  arguments="${2}"
  options="${3}"

  ${_SHELL_} ${options} ${file} ${arguments}
}



#
# MAKEFILE
#
# launches a makefile.
#
# ${1}:		options
#
makefile()
{
  local options

  options="${1}"

  ${_MAKE_} ${_MAKEFLAGS_} ${options}
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

  source="${1}"
  destination="${2}"
  for opt in ${3}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_CP_} ${source} ${destination}
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

  source="${1}"
  destination="${2}"
  for opt in ${3}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_LN_} ${destination} ${source}
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
  local options

  local f

  files="${1}"
  options="${2}"
  for opt in ${2}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  for f in ${files} ; do
    if [ -d ${f} ] ; then
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

  directory="${1}"
  for opt in ${2}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_LS_} ${directory}
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

  directory="${1}"
  for opt in ${2}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_CD_} ${directory}
}



#
# LOCATE
#
# this function tries to locate an element in an array.
#
# ${1}:		the array
# ${2}:		the element looked for
# ${3}:		options
#
locate()
{
  array="${1}"
  element="${2}"
  for opt in ${3}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  for e in ${array} ; do
    ${_ECHO_} ${e} | ${_GREP_} ${element} 2>/dev/null 1>/dev/null

    if [ ${?} -eq 0 ] ; then
      ${_ECHO_} ${e}
      return 0
    fi
  done

  return -1
}



#
# PREPROCESS
#
# this function preprocess a file
#
# ${1}:		file to process
# ${2}:		additional include
# ${3}:		tag start
# ${4}:		tag end
# ${5}:		options
#
preprocess()
{
  local source
  local options
  local includes
  local tags
  local tage

  source="${1}"
  tags="${3}"
  tage="${4}"
  for opt in ${5}; do
      case "${opt}" in
	  "--no-markers")
	      options="${options} -P"
	      ;;
	  *)
	      ;;
      esac
  done

  for inc in ${2}; do
      includes="${includes} -include ${inc}"
  done

  ${_CPP_} ${_INCLUDES_} ${options} ${includes} ${source} |		\
    ${_SED_} -n "/^${tags}$/,/^${tage}$/p" |				\
    ${_SED_} '/^!.*$/d'

}



#
# FIND-FILES
#
# find files given a pattern
#
# ${1}:		place to search for
# ${2}:		files to match
# ${3}:		options
#
find-files()
{
  local source
  local pattern
  local options

  source="${1}"
  pattern="${2}"
  for opt in ${3}; do
      case "${opt}" in
	  "--file")
	      options="${options} -type f"
	      ;;
	  *)
	      ;;
      esac
  done

  ${_FIND_} ${source} -name "${pattern}" ${options}
}



#
# TAGS CLEAN
#
# this function removes tags from the source code files.
#
# ${1}:		files
#
tags-clean()
{
  local files
  local temp
  local f

  files="${1}"

  temp=$(tempfile)

  for f in ${files} ; do
    ${_CAT_} ${f} | ${_SED_} "/^.*\[cut\].*$/ d" > ${temp}
    ${_CP_} ${temp} ${f}
  done
}



#
# PACK
#
# this function creates a package from a source directory.
#
# ${1}:		directory
# ${2}:		destination file
#
pack()
{
  local directory
  local file

  directory="${1}"
  file="${2}"

  ${_TAR_} -czf ${file} ${directory}
}



#
# UNPACK
#
# this function unpackages a file.
#
# ${1}:		directory
# ${2}:		destination file
#
unpack()
{
  local directory
  local file

  directory="${1}"
  file="${2}"

  if [ -n ${directory} ] ; then
    ${_TAR_} -xzf ${file} -C ${directory}
  else
    ${_TAR_} -xzf ${file}
  fi
}



#
# MAKE DIRECTORY
#
# this function creates a directory.
#
# ${1}:		directory
#
make-directory()
{
  local directory

  directory="${1}"

  ${_MKDIR_} ${directory}
}



#
# DEVICE-COPY
#
# copies a file on a device (normally a floppy).
#
# ${1}:		source file
# ${2}:		destination device or image
# ${3}:		destination path
# ${4}:		options
#
device-copy()
{
  local source
  local device
  local dest

  source="${1}"
  device="${2}"
  dest="${3}"

  case "${4}" in
      "--image")
	  ${_MCOPY_} -o -n -i ${device} ${source} ::/${dest}
	  ;;
      *)
	  ${_MCOPY_} -o -n ${source} ${device}/${dest}
	  ;;
  esac
}



#
# FORMAT-DATE
#
# this function return current date formatted
#
# ${1}:		format
# ${2}:		options
#
format-date()
{
  local format

  format="${1}"
  for opt in ${2}; do
      case "${opt}" in
	  *)
	      ;;
      esac
  done

  ${_DATE_} +${format}
}



#
# CUT-CODE
#
# cut code for exporting.
#
# ${1}:		stage
#
cut-code()
{
  local stage

  stage="${1}"
  ${_SED_} -e "/^.*\[cut\].*${stage}.*$/,/^.*\[cut\].*\/${stage}.*$/ \
                      { /^.*\[cut\].*${stage}.*$/ !d }"
}



#
# SVN CLEAN
#
# this function cleans the svn contol directories.
#
# ${1}:		directory
#
svn-clean()
{
  ${_RM_} -Rf `${_FIND_} ./ -type d -name .svn`
}
