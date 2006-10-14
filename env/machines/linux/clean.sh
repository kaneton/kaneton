## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/machines/gnu-unix/clean.sh
##
## created       julien quintard   [fri feb 11 02:58:21 2005]
## updated       julien quintard   [fri jul 14 13:54:02 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run into the directory: env/machines/linux/
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
  local needless

  # cleaning development tree.
  display "   cleaning development tree" "+"

  needless=$(launch "Makefile" "${_SRC_DIR_}" "clear")

  # destroys the architecture dependent links.
  display "   removing links to machine-dependent directories" "+"

  remove "${_MACHDEP_BOOTSTRAP_DIR_}" ""
  remove "${_MACHDEP_BOOTLOADER_DIR_}" ""
  remove "${_MACHDEP_CORE_DIR_}" ""
  remove "${_MACHDEP_INCLUDE_DIR_}" ""
  remove "${_MACHDEP_LINK_DIR_}" ""
}

#
# ---------- entry point ------------------------------------------------------
#

# cleans environment.
clean
