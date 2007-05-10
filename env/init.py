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
# updated       julien quintard   [thu may 10 16:41:28 2007]
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

  env.remove(env._CORE_PLATEFORM_DIR_, env.OPTION_NONE)
  env.link(env._CORE_PLATEFORM_DIR_,
           env._PLATEFORM_CORE_DIR_,
           env.OPTION_NONE)

  env.remove(env._CORE_ARCHITECTURE_DIR_, env.OPTION_NONE)
  env.link(env._CORE_ARCHITECTURE_DIR_,
           env._ARCHITECTURE_CORE_DIR_,
           env.OPTION_NONE)

  env.remove(env._INCLUDE_PLATEFORM_DIR_, env.OPTION_NONE)
  env.link(env._INCLUDE_PLATEFORM_DIR_,
           env._PLATEFORM_INCLUDE_DIR_, env.OPTION_NONE)

  env.remove(env._INCLUDE_ARCHITECTURE_DIR_, env.OPTION_NONE)
  env.link(env._INCLUDE_ARCHITECTURE_DIR_,
           env._ARCHITECTURE_INCLUDE_DIR_, env.OPTION_NONE)

  # XXX deprecated
  env.remove(env._MACHDEP_LINK_DIR_, env.OPTION_NONE)
  env.link(env._MACHDEP_LINK_DIR_, env._ARCHITECTURE_, env.OPTION_NONE)



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
    binary = binary.lstrip().rstrip()

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
  env.display(env.HEADER_OK,
              "generating the kaneton prototypes",
              env.OPTION_NONE)
# XXX  env.launch(env._SOURCE_DIR_ + "/Makefile", "proto")



#
# dependencies()
#
# this function generates the kaneton dependencies.
#
def			dependencies():
  env.display(env.HEADER_OK,
              "generating the kaneton dependencies",
              env.OPTION_NONE)
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

  # check the presence of the binaries used by the behaviour profile.
  check()

  # generate the kaneton prototypes.
  prototypes()

  # generate the kaneton dependencies.
  dependencies()

  # display some stuff.
  env.display(env.HEADER_OK,
              "environment development installed successfully",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
