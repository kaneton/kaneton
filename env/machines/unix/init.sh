#!/bin/bash
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/machines/unix/init.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       julien quintard   [sat apr  1 02:07:57 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run into the directory: src/env/
#
# this script installs operating system and architecture specific
# stuff.
#

#
# ---------- dependencies -----------------------------------------------------
#

source			.env.sh

#
# ---------- globals ----------------------------------------------------------
#

BINARIES="bash gcc gmake rm ar ranlib ld nasm ln touch wc date	\
          tail tar pdflatex cp cat sed echo ls date cpp mtools	\
          mcopy mkdir find xpdf mktemp mv"

#
# ---------- functions --------------------------------------------------------
#

#
# CHECK
#
# checks whether the binaries used to compile the kaneton kernel are
# present on the system.
#
check()
{
  local b

  display "   checking binaries" "+"

  for b in ${BINARIES} ; do
    locate-prog "${b}" 2>/dev/null 1>/dev/null
    if [ $? -ne 0 ] ; then
      display "   binary '${b}' not present on your system" "!"
    fi
  done
}



#
# INIT
#
# installs the environment for unix operating system.
#
init()
{
  # creates the links for architecture dependencies.
  display "   installing links to machine-dependent directories" "+"

  remove "${_MACHDEP_BOOTSTRAP_DIR_}"
  link "${_MACHDEP_BOOTSTRAP_DIR_}" "${_ARCHITECTURE_}"

  remove "${_MACHDEP_BOOTLOADER_DIR_}"
  link "${_MACHDEP_BOOTLOADER_DIR_}" "${_ARCHITECTURE_}"

  remove "${_MACHDEP_CORE_DIR_}"
  link "${_MACHDEP_CORE_DIR_}" "${_ARCHITECTURE_}"

  remove "${_MACHDEP_INCLUDE_DIR_}"
  link "${_MACHDEP_INCLUDE_DIR_}" "${_ARCHITECTURE_}"

  remove "${_MACHDEP_LDS_DIR_}"
  link "${_MACHDEP_LDS_DIR_}" "${_ARCHITECTURE_}"
}

#
# ---------- entry point ------------------------------------------------------
#

# checks the binaries.
check

# installs environment.
init
