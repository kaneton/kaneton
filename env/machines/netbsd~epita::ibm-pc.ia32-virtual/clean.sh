#!/bin/sh
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/env/machines/netbsd-epita/clean.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       matthieu bucchianeri   [tue jan 24 12:01:54 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run into the directory: src/env/
#

#
# ---------- dependencies -----------------------------------------------------
#

source			.env.sh

#
# ---------- functions --------------------------------------------------------
#

#
# CLEAN
#
# cleans the environment for unix operating system.
#
clean()
{
  # cleaning development tree.
  display "   cleaning development tree" "+"

  change-directory ${_SRC_DIR_}
  makefile "clear" 2>/dev/null 1>/dev/null
  change-directory ${_ENV_DIR_}

  # destroys the architecture dependent links.
  display "   removing links to machine-dependent directories" "+"

  remove ${_MACHDEP_BOOTSTRAP_DIR_}
  remove ${_MACHDEP_BOOTLOADER_DIR_}
  remove ${_MACHDEP_KANETON_DIR_}
  remove ${_MACHDEP_INCLUDE_DIR_}
  remove ${_MACHDEP_LDS_DIR_}
}

#
# ---------- entry point ------------------------------------------------------
#

# cleans environment.
clean