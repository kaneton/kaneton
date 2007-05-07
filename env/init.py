#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/environment/init.py
#
# created       julien quintard   [fri dec 15 13:43:03 2006]
# updated       julien quintard   [wed feb 14 18:57:44 2007]
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
# warning()
#
# this function displays the current configuration and asks the user
# the permission to continue.
#
def			warning():
  env.display(env.HEADER_OK, "your current configuration:")
  env.display(env.HEADER_OK, "  user:                   " + env._USER_)
  env.display(env.HEADER_OK, "  machine:                " + env._MACHINE_)
  env.display(env.HEADER_OK, "  architecture:           " + env._ARCHITECTURE_)
  env.display(env.HEADER_NONE, "")
  env.display(env.HEADER_INTERACTIVE,
              "to cancel press CTRL^C, otherwise press enter")

  env.input()



#
# architecture()
#
# this function installs the links to the architecture dependent files
# and directories.
def			architecture():
  env.display(env.HEADER_OK,
              "installing links to machine-dependent directories")

  env.remove(env._MACHDEP_BOOTSTRAP_DIR_)
  env.link(env._MACHDEP_BOOTSTRAP_DIR_, env._ARCHITECTURE_)

  env.remove(env._MACHDEP_BOOTLOADER_DIR_)
  env.link(env._MACHDEP_BOOTLOADER_DIR_, env._ARCHITECTURE_)

  env.remove(env._MACHDEP_CORE_DIR_)
  env.link(env._MACHDEP_CORE_DIR_, env._ARCHITECTURE_)

  env.remove(env._MACHDEP_INCLUDE_DIR_)
  env.link(env._MACHDEP_INCLUDE_DIR_, env._ARCHITECTURE_)

  env.remove(env._MACHDEP_LINK_DIR_)
  env.link(env._MACHDEP_LINK_DIR_, env._ARCHITECTURE_)



#
# initialize()
#
# this function calls the machine and user specific initialization scripts.
#
def			initialize():
  if os.path.exists(env._MACHINE_DIR_ + "/init.py"):
    env.display(env.HEADER_OK, "calling the machine-specific init script")
    env.launch(env._MACHINE_DIR_ + "/init.py", "")

  if os.path.exists(env._USER_DIR_ + "/init.py"):
    env.display(env.HEADER_OK, "calling the user-specific init script")
    env.launch(env._USER_DIR_ + "/init.py", "")



#
# prototypes()
#
# this function generates the kaneton prototypes.
#
def			prototypes():
  env.display(env.HEADER_OK, "generating the kaneton prototypes")
# XXX  env.launch(env._SOURCE_DIR_ + "/Makefile", "proto")



#
# dependencies()
#
# this function generates the kaneton dependencies.
#
def			dependencies():
  env.display(env.HEADER_OK, "generating the kaneton dependencies")
# XXX  env.launch(env._SOURCE_DIR_ + "/Makefile", "dep")



#
# main()
#
# this function initializes the development environment.
#

def			main():
  # display some stuff.
  env.display(env.HEADER_NONE, "")
  env.display(env.HEADER_OK, "installing the kaneton development environment")
  env.display(env.HEADER_NONE, "")

  # display the current configuration and ask the user to continue.
  warning()

  # install the chosen architecture.
  architecture()

  # call to the machine and user specific initialization scripts.
  initialize()

  # generate the kaneton prototypes.
  prototypes()

  # generate the kaneton dependencies.
  dependencies()

  # display some stuff.
  env.display(env.HEADER_OK, "environment development installed successfully")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
