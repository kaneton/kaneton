##
## copyright quintard julien
## 
## kaneton
## 
## machine.sh
## 
## path          /home/mycure/kaneton/env
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:08:31 2005   mycure
## last update   Sun Nov 13 13:16:35 2005   mycure
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
  options="${3}"

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
        ;;
    esac
  else
    ${_ECHO_} ${options} "${text}"
  fi
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
      print "blue" "[" "-n"
      print "green" "+" "-n"
      print "blue" "]" "-n"
      ;;

    "!")
      print "blue" "[" "-n"
      print "red" "!" "-n"
      print "blue" "]" "-n"
      ;;

    "?")
      print "blue" "[" "-n"
      print "yellow" "?" "-n"
      print "blue" "]" "-n"
      ;;

    *)
      ;;
  esac

  print "white" "${message}" ""
}



#
# WAITKEY
#
# this function just waits for a key.
#
waitkey()
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
      ${_MAKE_} ${options} ${file} ${arguments}
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
# ${1}:		file
# ${2}:		options
#
remove()
{
  local file
  local options

  file="${1}"
  options="${2}"

  ${_RM_} ${options} ${file}
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
# DIRECTORY
#
# this function just changes the current working directory.
#
# ${1}:		directory
#
directory()
{
  local directory

  directory="${1}"

  ${_CD_} ${directory}
}



#
# RUNTIME CONFIGURATION
#
# this function generates the kaneton runtime configuration file.
#
# ${1}:		source file
# ${2}:		destination file
#
runtime-configuration()
{
  local source
  local destination

  source="${1}"
  destination="${2}"

  # changes the directory.
  directory ${_SRC_DIR_}

  # makes a temporary file.
  kaneton_conf=$(mktemp)

  # removes the previous version.
  remove ${destination}

  # creates the temporary file.
  ${_ECHO_} "! kaneton.conf" >> ${kaneton_conf}
  ${_CAT_} ${source} >> ${kaneton_conf}
  ${_ECHO_} "! /kaneton.conf" >> ${kaneton_conf}

  # preprocess the temporary file to generate a correct runtime
  # configuration file.
  ${_CPP_} ${_INCLUDES_} -P -include kaneton.h ${kaneton_conf} |	\
    ${_SED_} -n '/^! kaneton.conf$/,/^! \/kaneton.conf$/ p' |		\
    ${_SED_} '/^!.*$/ d' > ${destination}

  # returns in the env directory.
  directory ${_ENV_DIR_}
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
# GRUB BUILD
#
# initialise the grub boot device.
#
# ${1}:		bootmode
# ${2}:		kaneton image
# ${3}:		udevice
# ${4}:		image
#
grub-build()
{
  bootmode="${1}"
  kaneton_image="${2}"
  udevice="${3}"
  image="${4}"

  case "${bootmode}" in
    "floppy"|"tftp")
      ${_CAT_} ${kaneton_image} > ${udevice}
      ;;

    "floppy-image")
      ${_CP_} ${kaneton_image} ${image}
      ;;

    *)
      return -1
      ;;
  esac

  return 0
}



#
# GRUB MENU
#
# this function creates the grub menu file.
#
# ${1}:		menu output file
# ${2}:		bootmode
# ${3}:		address
# ${4}:		tftp_address
# ${5}:		modules
#
grub-menu()
{
  local menu
  local bootmode
  local address
  local tftp_address
  local modules

  local array
  local m
  local i

  menu="${1}"
  bootmode="${2}"
  address="${3}"
  tftp_address="${4}"
  modules="${5}"

  # cleans the menu file contents.
  ${_ECHO_} -n "" > ${menu}

  # creates new version.
  ${_ECHO_} "title kaneton" >> ${menu}
  ${_ECHO_} "" >> ${menu}

  # inserts each module in the menu file.
  case "${bootmode}" in
    "floppy"|"floppy-image")
      ${_ECHO_} "root (fd0)" >> ${menu}
      ${_ECHO_} "" >> ${menu}
      ;;

    "tftp")
      ${_ECHO_} "ifconfig --address=${address}"				\
                "--server=${tftp_address}" >> ${menu}
      ${_ECHO_} "" >> ${menu}
      ${_ECHO_} "root (nd)" >> ${menu}
      ${_ECHO_} "" >> ${menu}
      ;;

    *)
      return -1
      ;;
  esac

  # fills in an array from the modules list
  array=()
  i=0

  for m in ${modules} ; do
    array[${i}]=${m}

    let "i += 1"
  done

  # sets the kernel into the grub menu file.
  ${_ECHO_} ${array[0]} |						\
    ${_SED_} "s/^.*\/\(.*\)$/kernel \/modules\/\1/g" >> ${menu}

  ${_ECHO_} "" >> ${menu}

  i=1
  while [ ${i} -lt ${#array[*]} ] ; do
    ${_ECHO_} ${array[${i}]} |						\
      ${_SED_} "s/^.*\/\(.*\)$/module \/modules\/\1/g" >> ${menu}

    let "i += 1"
  done

  return 0
}



#
# GRUB INSTALL
#
# installs the distribution on the grub boot device.
#
# ${1}:		bootmode
# ${2}:		menu
# ${3}:		mdevice
# ${4}:		tftp directory
# ${5}:		image
# ${6}:		modules
#
grub-install()
{
  local bootmode
  local menu
  local mdevice
  local tftp_directory
  local image
  local modules

  local m

  local menu_lst

  bootmode="${1}"
  menu="${2}"
  mdevice="${3}"
  tftp_directory="${4}"
  image="${5}"
  modules="${6}"

  menu_lst="menu.lst"

  case "${bootmode}" in
    "floppy")
      if [ ! -e ${menu} ] ; then
        display " ${menu_lst}" "!"
      else
        ${_MCOPY_} -n -o ${menu} ${mdevice}/boot/grub/${menu_lst}
        display " ${menu_lst}" "+"
      fi

      for m in ${modules} ; do
        if [ ! -e ${_SRC_DIR_}/${m} ] ; then
          display " ${m}" "!"
        else
          ${_MCOPY_} -n -o ${_SRC_DIR_}/${m} ${mdevice}/modules/
          display " ${m}" "+"
        fi
      done

      ;;

    "tftp")
      if [ ! -e ${menu} ] ; then
        display " ${menu_lst}" "!"
      else
        ${_MCOPY_} -n -o ${menu} ${mdevice}/boot/grub/${menu_lst}
        display " ${menu_lst}" "+"
      fi

      for m in ${modules} ; do
        if [ ! -e ${_SRC_DIR_}/${m} ] ; then
          display " ${m}" "!"
        else
          ${_CP_} ${_SRC_DIR_}/${m} ${tftp_directory}
          display " ${m}" "+"
        fi
      done

      ;;

    "floppy-image")
      if [ ! -e ${menu} ] ; then
        display " ${menu_lst}" "!"
      else
        ${_MCOPY_} -n -o ${menu} -i ${image} ::/boot/grub/${menu_lst}
        display " ${menu_lst}" "+"
      fi

      for m in ${modules} ; do
        if [ ! -e ${_SRC_DIR_}/${m} ] ; then
          display " ${m}" "!"
        else
          ${_MCOPY_} -n -o ${_SRC_DIR_}/${m} -i ${image} ::/modules/
          display " ${m}" "+"
        fi
      done

      ;;
  esac
}

