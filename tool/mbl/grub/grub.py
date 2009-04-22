#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/tool/mbl/grub/grub.py
#
# created       julien quintard   [tue jun 26 11:33:57 2007]
# updated       julien quintard   [mon apr 20 03:31:32 2009]
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
  env.display(env.HEADER_ERROR, "usage: grub.py [action]", env.OPTION_NONE)

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
  env.display(env.HEADER_OK, "your current configuration:", env.OPTION_NONE)
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
# menu()
#
# this function generates the grub menu file.
#
def			menu():
  global g_inputs
  global g_menu
  content = None
  input = None

  # initialize the file content.
  content = "timeout 0\n" +						\
            "title kaneton\n"

  # add information about how to boot.
  if (env._BOOT_MODE_ == "peripheral") or (env._BOOT_MODE_ == "image"):
    if env._BOOT_DEVICE_ == "floppy":
      content += "root (fd0)\n"
    elif env._BOOT_DEVICE_ == "hard-drive":
      content += "root (hd0)\n"
    else:
      env.display(env.HEADER_ERROR,
                  "unknown boot device '" + env._BOOT_DEVICE_ + "'",
                  env.OPTION_NONE)
      sys.exit(42)
  elif env._BOOT_MODE_ == "network":
    content += "ifconfig --address=" + env._ADDRESS_ + " --server=" +	\
                 env._TFTP_ADDRESS_ + "\n" +				\
               "root (nd)\n"
  else:
    env.display(env.HEADER_ERROR, "unknown boot mode '" + env._BOOT_MODE_ +
                "'", env.OPTION_NONE)
    sys.exit(42)

  # retrieve the grub modules from the _INPUTS_ environment variables.
  g_inputs = re.split("[ \t]+", env._INPUTS_.strip())

  # set the first input as the grub kernel.
  content += re.sub("^.*\/", "kernel /modules/", g_inputs[0]) + "\n"

  # set the other inputs as grub modules.
  for input in g_inputs[1:]:
    content += re.sub("^.*\/", "module /modules/", input) + "\n"

  # create the temporary file and fill it.
  g_menu = env.temporary(env.OPTION_FILE)

  env.push(g_menu, content, env.OPTION_NONE)



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
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "installing kaneton binaries on the boot device",
              env.OPTION_NONE)

  # generates the grub menu file.
  menu()

  # depending on the boot mode and boot device, install the kaneton binaries
  # and the grub menu file.
  if env._BOOT_MODE_ == "peripheral":
    env.load(g_menu, env._MDEVICE_, "/boot/grub/menu.lst", env.OPTION_DEVICE)

    for input in g_inputs:
      if not env.path(input, env.OPTION_EXIST):
        env.display(env.HEADER_ERROR, "  " + input, env.OPTION_NONE)
      else:
        env.load(input, env._MDEVICE_, "/modules/", env.OPTION_DEVICE)
        env.display(env.HEADER_OK, "  " + input, env.OPTION_NONE)
  elif env._BOOT_MODE_ == "network":
    env.load(g_menu, env._MDEVICE_, "/boot/grub/menu.lst", env.OPTION_DEVICE)

    for input in g_inputs:
      if not env.path(input, env.OPTION_EXIST):
        env.display(env.HEADER_ERROR, "  " + input, env.OPTION_NONE)
      else:
        env.copy(input, env._TFTP_DIRECTORY_, env.OPTION_NONE)
        env.display(env.HEADER_OK, "  " + input, env.OPTION_NONE)
  elif env._BOOT_MODE_ == "image":
    env.load(g_menu, env._IMAGE_, "/boot/grub/menu.lst", env.OPTION_IMAGE)

    for input in g_inputs:
      if not env.path(input, env.OPTION_EXIST):
        env.display(env.HEADER_ERROR, "  " + input, env.OPTION_NONE)
      else:
        env.load(input, env._IMAGE_, "/modules/", env.OPTION_IMAGE)
        env.display(env.HEADER_OK, "  " + input, env.OPTION_NONE)
  else:
    env.display(env.HEADER_ERROR, "unknown boot mode '" + env._BOOT_MODE_ +
                "'", env.OPTION_NONE)
    sys.exit(42)



#
# build()
#
# this function builds, initializes the boot device.
#
def			build():
  # warn the user before performing any action.
  warning()

  # display some stuff.
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK, "initializing the boot device", env.OPTION_NONE)

  # for each boot mode, initialize the boot device.
  if (env._BOOT_MODE_ == "peripheral") or (env._BOOT_MODE_ == "network"):
    env.push(env._UDEVICE_,
             env.pull(g_image, env.OPTION_NONE),
             env.OPTION_NONE)
  elif env._BOOT_MODE_ == "image":
    env.copy(g_image, env._IMAGE_, env.OPTION_NONE)
  else:
    env.display(env.HEADER_ERROR, "unknown boot mode '" + env._BOOT_MODE_ +
                "'", env.OPTION_NONE)
    sys.exit(42)



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
