#! /bin/bash

## copyright quintard julien
## 
## kaneton
## 
## init.sh
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Mon Nov  7 18:43:50 2005   mycure
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

  directory $_SRC_DIR_
  makefile clear 2>&- 1>&-
  directory $_ENV_DIR_

  # destroys the architecture dependent links.
  display "   removing links to machine-dependent directories" "+"

  remove $_MACHDEP_BOOTSTRAP_DIR_
  remove $_MACHDEP_BOOTLOADER_DIR_
  remove $_MACHDEP_KANETON_DIR_
  remove $_MACHDEP_INCLUDE_DIR_
  remove $_MACHDEP_LDS_DIR_

}

#
# ---------- entry point ------------------------------------------------------
#

# cleans environment.
clean
