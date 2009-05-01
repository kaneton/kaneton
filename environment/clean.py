#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/environment/clean.py
#
# created       julien quintard   [sat dec 16 20:57:38 2006]
# updated       julien quintard   [mon apr 20 03:33:17 2009]
#

#
# ---------- imports ----------------------------------------------------------
#

import os

import env

#
# ---------- functions --------------------------------------------------------
#

#
# machine()
#
# this function removes the links to the machine dependent files
# and directories.
#
def			machine():
  env.display(env.HEADER_OK,
              "removing links to machine-dependent directories",
              env.OPTION_NONE)

  env.remove(env._GLUE_CURRENT_, env.OPTION_NONE)
  env.remove(env._ARCHITECTURE_CURRENT_, env.OPTION_NONE)
  env.remove(env._PLATFORM_CURRENT_, env.OPTION_NONE)



#
# clear()
#
# this function clears the kaneton development tree.
#
def			clear():
  env.display(env.HEADER_OK,
              "clearing the kaneton development tree",
              env.OPTION_NONE)
  env.launch(env._SOURCE_DIR_ + "/Makefile", "clear", env.OPTION_QUIET)



#
# boot()
#
# this function removes everything that has been generated
# for booting the kernel like images and so forth.
#
def                     boot():
  if env.path(env._IMAGE_, env.OPTION_EXIST):
    env.remove(env._IMAGE_, env.OPTION_NONE)



#
# dependencies()
#
# this function removes the kaneton dependencies.
#
def			dependencies():
  dependencies = None
  dep = None

  env.display(env.HEADER_OK,
              "removing the kaneton header dependencies",
              env.OPTION_NONE)

  dependencies = env.search(env._SOURCE_DIR_,
                            env._DEPENDENCY_MK_,
                            env.OPTION_FILE | env.OPTION_RECURSIVE)

  for dep in dependencies:
    env.remove(dep, env.OPTION_NONE)



#
# clean()
#
# the function removes the generated kaneton development environment files.
#
def			clean():
  env.remove(env._ENV_MK_, env.OPTION_NONE)
  env.remove(env._ENV_PY_, env.OPTION_NONE)



#
# main()
#
# this function initializes the development environment.
#
import sys
def			main():
  # display some stuff.
  env.display(env.HEADER_OK,
              "cleaning the kaneton development environment",
              env.OPTION_NONE)

  # clear the kaneton development tree.
  clear()

  # remove the generated boot stuff.
  boot()

  # uninstall the chosen machine.
  machine()

  # generate the kaneton dependencies.
  dependencies()

  # remove the environment-specific files.
  clean()

  # display some stuff.
  env.display(env.HEADER_OK,
              "environment development cleaned successfully",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
