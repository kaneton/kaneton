#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/env/init.py
#
# created       julien quintard   [fri dec 15 13:43:03 2006]
# updated       julien quintard   [thu may 10 13:57:42 2007]
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
  env.display(env.HEADER_OK, "your current configuration:", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  user:                   " + env._USER_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  host:                   " + env._HOST_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  plateform:              " + env._PLATEFORM_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  architecture:           " + env._ARCHITECTURE_,
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_INTERACTIVE,
              "to cancel press CTRL^C, otherwise press enter",
              env.OPTION_NONE)

  env.input(env.OPTION_NONE)



#
# machine()
#
# this function installs the links to the plateform and architecture
# dependent files and directories.
def			machine():
  env.display(env.HEADER_OK,
              "installing links to machine-dependent directories",
              env.OPTION_NONE)

XXX

  env.remove(env._MACHDEP_CORE_DIR_, env.OPTION_NONE)
  env.link(env._MACHDEP_CORE_DIR_, env._ARCHITECTURE_, env.OPTION_NONE)

  env.remove(env._MACHDEP_INCLUDE_DIR_, env.OPTION_NONE)
  env.link(env._MACHDEP_INCLUDE_DIR_, env._ARCHITECTURE_, env.OPTION_NONE)

  env.remove(env._MACHDEP_LINK_DIR_, env.OPTION_NONE)
  env.link(env._MACHDEP_LINK_DIR_, env._ARCHITECTURE_, env.OPTION_NONE)



#
# initialise()
#
# this function calls the machine and user specific initialisation scripts.
#
def			initialise():
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
# this function initialises the development environment.
#

def			main():
  # display some stuff.
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "installing the kaneton development environment",
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # display the current configuration and ask the user to continue.
  warning()

  # install the chosen machine: plateform/architecture.
  machine()

  sys.exit(1)

  # call to the machine and user specific initialisation scripts.
  initialise()

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
