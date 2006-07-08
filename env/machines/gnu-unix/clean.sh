## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/machines/gnu-unix/clean.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       julien quintard   [sat jul  8 03:10:01 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run into the directory: env/machines/gnu-unix/
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
  remove ${_MACHDEP_CORE_DIR_}
  remove ${_MACHDEP_INCLUDE_DIR_}
  remove ${_MACHDEP_LINK_DIR_}
}

#
# ---------- entry point ------------------------------------------------------
#

# cleans environment.
clean
