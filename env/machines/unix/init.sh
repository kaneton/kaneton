#! /bin/bash

## copyright quintard julien
## 
## kaneton
## 
## init.sh
## 
## path          /home/mycure/kaneton/view
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Tue Nov  8 14:08:50 2005   mycure
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

_BINARIES_="bash gcc gmake rm ar ranlib ld nasm ln touch wc date	\
            tail tar latex dvips pdflatex cp cat sed echo ls cpp"

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
  display "   checking binaries" "+"

  for b in $_BINARIES_ ; do
    which $b 2>/dev/null 1>/dev/null
    if [ $? -ne 0 ] ; then
      display "   binary '$b' not present on your system" "!"
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

  remove $_MACHDEP_BOOTSTRAP_DIR_
  link $_MACHDEP_BOOTSTRAP_DIR_ $_ARCHITECTURE_

  remove $_MACHDEP_BOOTLOADER_DIR_
  link $_MACHDEP_BOOTLOADER_DIR_ $_ARCHITECTURE_

  remove $_MACHDEP_KANETON_DIR_
  link $_MACHDEP_KANETON_DIR_ $_ARCHITECTURE_

  remove $_MACHDEP_INCLUDE_DIR_
  link $_MACHDEP_INCLUDE_DIR_ $_ARCHITECTURE_

  remove $_MACHDEP_LDS_DIR_
  link $_MACHDEP_LDS_DIR_ $_ARCHITECTURE_
}

#
# ---------- entry point ------------------------------------------------------
#

# checks the binaries.
check

# installs environment.
init
