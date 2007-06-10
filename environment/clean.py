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
# updated       julien quintard   [sun jun 10 18:36:29 2007]
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
# XXX  env.launch(env._SOURCE_DIR_ + "/Makefile", "clear")



#
# dependencies()
#
# this function removes the kaneton dependencies.
#
def			dependencies():
  dependencies = None
  dep = None

  env.display(env.HEADER_OK,
              "removing the kaneton dependencies",
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

def			main():
  # display some stuff.
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "environment files generated successfully",
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # uninstall the chosen machine.
  machine()

  # clear the kaneton development tree.
  clear()

  # generate the kaneton dependencies.
  dependencies()

  # call to the machine and user specific clean scripts.
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
