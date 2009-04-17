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
# updated       julien quintard   [tue apr 14 18:54:57 2009]
#

#
# ---------- imports ----------------------------------------------------------
#

import critical

import env

import os

import sys

import getopt

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
              "  platform:               " + env._PLATFORM_,
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
# this function installs the links to the glue, platform and architecture
# dependent files and directories.
def			machine(options):
  if not options & env.OPTION_QUIET:
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
def			check(options):
  binaries = None
  binary = None

  if not options & env.OPTION_QUIET:
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
def			prototypes(options):
  useless = None

  if not options & env.OPTION_QUIET:
    env.display(env.HEADER_OK,
                "generating the kaneton prototypes",
                env.OPTION_NONE)
  env.launch(env._SOURCE_DIR_ + "/Makefile", "prototypes", env.OPTION_QUIET)



#
# headers()
#
# this function generates the kaneton header dependencies.
#
def			headers(options):
  if not options & env.OPTION_QUIET:
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

  options = env.OPTION_NONE

  opts,args = getopt.getopt(sys.argv[1:],"q",["quiet"])
  for o,a in opts:
    if o in ("-q", "--quiet"):
      options = env.OPTION_QUIET

  # display some stuff.
  if not options & env.OPTION_QUIET:
    env.display(env.HEADER_OK,
                "installing the kaneton development environment",
                env.OPTION_NONE)
    env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # display the current configuration and ask the user to continue.
  if not options & env.OPTION_QUIET:
    warning()

  # install the chosen machine: platform/architecture.
  machine(options)

  # check the presence of the binaries used by the behaviour profile.
  check(options)

  # generate the kaneton prototypes.
  prototypes(options)

  # generate the kaneton headers.
  headers(options)

  # display some stuff.
  if not options & env.OPTION_QUIET:
    env.display(env.HEADER_OK,
                "environment development installed successfully",
                env.OPTION_NONE)
    
#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
