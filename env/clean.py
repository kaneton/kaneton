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
# updated       julien quintard   [sun dec 17 03:13:46 2006]
#

#
# ---------- imports ----------------------------------------------------------
#

import os

import critical
import env

#
# ---------- functions --------------------------------------------------------
#

#
# architecture()
#
# this function removes the links to the architecture dependent files
# and directories.
def			architecture():
  env.display(env.HEADER_OK,
              "removing links to machine-dependent directories")

  env.remove(env._MACHDEP_BOOTSTRAP_DIR_)
  env.remove(env._MACHDEP_BOOTLOADER_DIR_)
  env.remove(env._MACHDEP_CORE_DIR_)
  env.remove(env._MACHDEP_INCLUDE_DIR_)
  env.remove(env._MACHDEP_LINK_DIR_)



#
# clear()
#
# this function clears the kaneton development tree.
#
def			clear():
  env.display(env.HEADER_OK, "clearing the kaneton development tree")
# XXX  env.launch(env._SOURCE_DIR_ + "/Makefile", "clear")



#
# dependencies()
#
# this function removes the kaneton dependencies.
#
def			dependencies():
  dependencies = None
  dep = None

  env.display(env.HEADER_OK, "removing the kaneton dependencies")

  dependencies = env.search(env._SOURCE_DIR_,
                            env._DEPENDENCY_MK_,
                            env.OPTION_FILE)

  for dep in dependencies:
    env.remove(dep)



#
# clean()
#
# this function calls the machine and user specific clean scripts.
#
# finally the function removes the generated kaneton development
# environment files.
#
def			clean():
  if os.path.exists(env._MACHINE_DIR_ + "/clean.py"):
    env.display(env.HEADER_OK, "calling the machine-specific clean script")
    env.launch(env._MACHINE_DIR_ + "/clean.py", "")

  if os.path.exists(env._USER_DIR_ + "/clean.py"):
    env.display(env.HEADER_OK, "calling the user-specific clean script")
    env.launch(env._USER_DIR_ + "/clean.py", "")

  env.remove(env._ENV_MK_)
  env.remove(env._ENV_PY_)



#
# main()
#
# this function initializes the development environment.
#

def			main():
  # display some stuff.
  env.display(env.HEADER_NONE, "")
  env.display(env.HEADER_OK, "environment files generated successfully")
  env.display(env.HEADER_NONE, "")

  # uninstall the chosen architecture.
  architecture()

  # clear the kaneton development tree.
  clear()

  # generate the kaneton dependencies.
  dependencies()

  # call to the machine and user specific clean scripts.
  clean()

  # display some stuff.
  env.display(env.HEADER_OK, "environment development cleaned successfully")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
