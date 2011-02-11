#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/environment/initialize.py
#
# created       julien quintard   [fri dec 15 13:43:03 2006]
# updated       julien quintard   [thu feb 10 19:24:31 2011]
#

#
# ---------- imports ----------------------------------------------------------
#

import critical

import env

import sys
import time

#
# ---------- functions --------------------------------------------------------
#

#
# warning()
#
# this function displays the current configuration.
#
def			warning():
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK, "configuration:", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  user:                   " + env._USER_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  host:                   " + env._HOST_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  platform:               " + env._PLATFORM_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  architecture:           " + env._ARCHITECTURE_,
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)



#
# machine()
#
# this function installs the links to the glue, platform and architecture
# dependent files and directories.
def			machine():
  env.display(env.HEADER_OK,
              "installing links to machine-dependent directories",
              env.OPTION_NONE)

  env.remove(env._GLUE_CURRENT_, env.OPTION_NONE)
  env.link(env._GLUE_CURRENT_, env._GLUE_DIR_, env.OPTION_NONE)

  env.remove(env._PLATFORM_CURRENT_, env.OPTION_NONE)
  env.link(env._PLATFORM_CURRENT_, env._PLATFORM_DIR_, env.OPTION_NONE)

  env.remove(env._ARCHITECTURE_CURRENT_, env.OPTION_NONE)
  env.link(env._ARCHITECTURE_CURRENT_, env._ARCHITECTURE_DIR_, env.OPTION_NONE)



#
# check()
#
# this function looks for every binary the behaviour profile needs to work
# properly.
#
def			check():
  binaries = None
  binary = None

  env.display(env.HEADER_OK,
              "looking for programs needed by the development environment",
              env.OPTION_NONE)

  binaries = env._BINARIES_.split(",")

  for binary in binaries:
    binary = binary.replace('\t', ' ')
    binary = binary.strip()

    binary = binary.split(' ')

    if env.locate(binary[0], env.OPTION_NONE) != 0:
      env.display(env.HEADER_ERROR,
                  "  the program " + binary[0] + " is not present "	\
                    "on your system",
                  env.OPTION_NONE)



#
# prototypes()
#
# this function generates the kaneton prototypes.
#
def			prototypes():
  useless = None

  env.display(env.HEADER_OK,
              "generating the kaneton prototypes",
              env.OPTION_NONE)
  env.launch(env._SOURCE_DIR_ + "/Makefile", "prototypes", env.OPTION_QUIET)



#
# headers()
#
# this function generates the kaneton header dependencies.
#
def			headers():
  env.display(env.HEADER_OK,
              "generating the kaneton header dependencies",
              env.OPTION_NONE)
  env.launch(env._SOURCE_DIR_ + "/Makefile", "headers", env.OPTION_QUIET)



#
# main()
#
# this function initializes the development environment.
#
def			main():
  # display some stuff.
  env.display(env.HEADER_OK,
              "installing the kaneton development environment",
              env.OPTION_NONE)

  # display the current configuration and ask the user to continue.
  warning()

  # install the chosen machine: platform/architecture.
  machine()

  # check the presence of the binaries used by the behaviour profile.
  check()

  # generate the kaneton prototypes.
  prototypes()

  # generate the kaneton headers.
  headers()

  # display some stuff.
  env.display(env.HEADER_OK,
              "environment development installed successfully",
              env.OPTION_NONE)
    
#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
