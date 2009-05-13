#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/tool/mbl/octaneload/octaneload.py
#
# created       julien quintard   [tue jun 26 11:33:57 2007]
# updated       julien quintard   [mon may 11 21:44:19 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this script is used to build and install the kaneton microkernel boot
# device.
#

#
# ---------- imports ----------------------------------------------------------
#

import env

import sys
import re
import subprocess

#
# ---------- globals ----------------------------------------------------------
#

g_image="data/kaneton.img"
g_menu = None
g_inputs = None
g_action = None

#
# ---------- functions --------------------------------------------------------
#

#
# usage()
#
# this function displays the usage.
#
def			usage():
  env.display(env.HEADER_ERROR, "usage: octaneload.py [action]",
              env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  env.display(env.HEADER_ERROR, "actions:", env.OPTION_NONE)
  env.display(env.HEADER_ERROR, "  build", env.OPTION_NONE)
  env.display(env.HEADER_ERROR, "  install", env.OPTION_NONE)



#
# warning()
#
# this function warns the user about its configuration.
#
def			warning():
  env.display(env.HEADER_OK, "configuration:", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  bootmode:               " + env._BOOT_MODE_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  address:                " + env._ADDRESS_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  tftp address:           " + env._TFTP_ADDRESS_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  tftp directory:         " + env._TFTP_DIRECTORY_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  UNIX device:            " + env._UDEVICE_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  mtool device:           " + env._MDEVICE_,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  image path:             " + env._IMAGE_,
              env.OPTION_NONE)


#
# install()
#
# this function installs the kaneton binaries onto the boot device.
#
def			install():
  input = None

  # warn the user before performing any action.
  warning()

  # display some stuff.
  env.display(env.HEADER_OK,
              "installing kaneton binaries on the boot device",
              env.OPTION_NONE)

  env.copy(env._KANETON_, env._IMAGE_, env.OPTION_NONE)


#
# build()
#
# this function builds, initializes the boot device.
#
def			build():
  # warn the user before performing any action.
  warning()

  # display some stuff.
  env.display(env.HEADER_ERROR, "nothing to build for this architecture",
              env.OPTION_NONE)


#
# main()
#
# this function performs the main work.
#
def			main():
  global g_action

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # set the action.
  g_action = sys.argv[1]

  # act according to the action argument.
  if g_action == "build":
    build()
  elif g_action == "install":
    install()
  else:
    env.display(env.HEADER_ERROR, "unknown action \'" + g_action + "'",
                env.OPTION_NONE)
    usage()
    sys.exit(42)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
