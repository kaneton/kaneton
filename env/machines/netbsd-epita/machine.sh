#!/bin/bash
## licence       kaneton licence
##
## project
##
## file          /home/buckman/kaneton/kaneton/env/machines/netbsd-epita/machine.sh
##
## created       julien quintard   [fri feb 11 02:08:31 2005]
## updated       matthieu bucchianeri   [wed dec 14 12:11:17 2005]
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
  case "${3}" in
      "--no-newline")
	  options="${options} -n"
	  ;;
      "")
	  ;;
      *)
	  print "red" "error in options !" ""
	  ;;
  esac

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
        ${_ECHO_} ${options} "${text}"
        ;;
    esac
  else
    ${_ECHO_} ${options} "${text}"
  fi
}



#
# CONTENT
#
# gets the contents of a file.
#
# ${1}:         file
#
contents()
{
  local file

  file="${1}"

  ${_CAT_} ${file}
}



#
# TEMPFILE
#
# gives an unique temporary file name.
#
#
tempfile()
{
  ${_MKTEMP_}
}



#
# WORKING-DIRECTORY
#
# prints working directory
#
working-directory()
{
  ${_PWD_}
}



#
# LOCATE-PROG
#
# locates a program on the system.
#
# ${1}:         program
#
locate-prog()
{
  local program

  program=${1}

  ${_WHICH_} ${program}
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
      print "yellow" "?" "--no-newline"
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
wait-key()
{
  local needless

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

  case "${file}" in
    Makefile)
      if [ -n ${file} ] ; then
        ${_MAKE_} ${_MAKEFLAGS_} ${options} -f ${file} ${arguments}
      else
	${_MAKE_} ${_MAKEFLAGS_} ${options} -f ${file} ${arguments}
      fi
      ;;
    *.sh)
      ${_SHELL_} ${options} ${file} ${arguments}
      ;;
    *)
      ;;
  esac
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
#
copy()
{
  local source
  local destination

  source="${1}"
  destination="${2}"

  ${_CP_} ${source} ${destination}
}



#
# LINK
#
# this function performs a link between two files.
#
# ${1}:		source file
# ${2}:		destination file
#
link()
{
  local source
  local destination

  source="${1}"
  destination="${2}"

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
#
list()
{
  local directory

  directory="${1}"

  ${_LS_} ${directory}
}



#
# CHANGE DIRECTORY
#
# this function just changes the current working directory.
#
# ${1}:		directory
#
change-directory()
{
  local directory

  directory="${1}"

  ${_CD_} ${directory}
}



#
# LOCATE
#
# this function tries to locate an element in an array.
#
# ${1}:		the array
# ${2}:		the element looked for
#
locate()
{
  array="${1}"
  element="${2}"

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
