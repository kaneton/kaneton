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
# updated       julien quintard   [sat mar  5 09:19:34 2011]
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
g_components = None
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
# menu()
#
# this function generates the grub menu file.
#
def			menu():
  global g_components
  global g_menu
  content = None
  component = None

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

  # retrieve the grub modules from the _COMPONENTS_ environment variables.
  g_components = re.split("[ \t]+", env._COMPONENTS_.strip())

  # set the first component as the grub kernel.
  content += re.sub("^.*\/", "kernel /modules/", g_components[0]) + "\n"

  # set the other components as grub modules.
  for component in g_components[1:]:
    content += re.sub("^.*\/", "module /modules/", component) + "\n"

  # create the temporary file and fill it.
  g_menu = env.temporary(env.OPTION_FILE)

  env.push(g_menu, content, env.OPTION_NONE)



#
# install()
#
# this function installs the kaneton binaries onto the boot device.
#
def			install():
  component = None

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
    if env.load(g_menu, env._MDEVICE_, "/boot/grub/menu.lst",
                env.OPTION_DEVICE) != 0:
      env.display(env.HEADER_ERROR,
                  "unable to load the menu.lst file",
                  env.OPTION_NONE)
      sys.exit(42)

    for component in g_components:
      if not env.path(component, env.OPTION_EXIST):
        env.display(env.HEADER_ERROR, "  " + component, env.OPTION_NONE)
      else:
        if env.load(component, env._MDEVICE_, "/modules/",
                    env.OPTION_DEVICE) != 0:
          env.display(env.HEADER_ERROR,
                      "unable to load the component '" + component + "'",
                      env.OPTION_NONE)
          sys.exit(42)

        env.display(env.HEADER_OK, "  " + component, env.OPTION_NONE)
  elif env._BOOT_MODE_ == "network":
    if env.load(g_menu, env._MDEVICE_, "/boot/grub/menu.lst",
                env.OPTION_DEVICE) != 0:
      env.display(env.HEADER_ERROR,
                  "unable to load the menu.lst file",
                  env.OPTION_NONE)
      sys.exit(42)

    for component in g_components:
      if not env.path(component, env.OPTION_EXIST):
        env.display(env.HEADER_ERROR, "  " + component, env.OPTION_NONE)
      else:
        env.copy(component, env._TFTP_DIRECTORY_, env.OPTION_NONE)
        env.display(env.HEADER_OK, "  " + component, env.OPTION_NONE)
  elif env._BOOT_MODE_ == "image":
    if env.load(g_menu, env._IMAGE_, "/boot/grub/menu.lst",
                env.OPTION_IMAGE) != 0:
      env.display(env.HEADER_ERROR,
                  "unable to load the menu.lst file",
                  env.OPTION_NONE)
      sys.exit(42)

    for component in g_components:
      if not env.path(component, env.OPTION_EXIST):
        env.display(env.HEADER_ERROR, "  " + component, env.OPTION_NONE)
      else:
        if env.load(component, env._IMAGE_, "/modules/",
                    env.OPTION_IMAGE) != 0:
          env.display(env.HEADER_ERROR,
                      "unable to load the component '" + component + "'",
                      env.OPTION_NONE)
          sys.exit(42)

        env.display(env.HEADER_OK, "  " + component, env.OPTION_NONE)
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
