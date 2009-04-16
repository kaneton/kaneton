#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/enguerrand/kaneton/tool/mbl/qemu-mips/qemu-mips.py
#
# created       enguerrand raymond   [mon nov 24 23:15:02 2008]
# updated       enguerrand raymond   [sat apr 11 12:41:00 2009]
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

import os
import sys
import re
import struct
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
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_INTERACTIVE,
              "to cancel press CTRL^C, otherwise press enter", env.OPTION_NONE)

  # wait for the user's approval
  env.input(env.OPTION_NONE)


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

  # creates binary files : bootloader and kaneton
  env.binary_extract(env.FIRMWARE, ".text .data .rdata .rodata .bss", env.FIRMWARE_BIN)

  # cat the binary files in the mips_bios.bin file
  env.concat_file(env.FIRMWARE_BIN, env.MIPS_BIOS)
  env.concat_file(env._SOURCE_DIR_ + "/header", env.MIPS_BIOS)
  env.concat_file(env.MODULES, env.MIPS_BIOS)

  #env.copy(env._KANETON_, env._IMAGE_, env.OPTION_NONE)


#
# build()
#
# this function builds, initializes the boot device.
#
def			build():
  handle = None
  mod_list = None
  header_file = None

  # warn the user before performing any action.
  warning()

  # creates the mips_bios.bin file
  env.push(env._SOURCE_DIR_ + "/mipsel_bios.bin", "", None)

  # creates the flag file with header information

  mod_list = env._INPUT_MOD_.split()

  header_file = open(env._SOURCE_DIR_ + "/header", "wb");

  # write the flag
  header_file.write(struct.pack("<l", 0x07070707))

  # write the number of module : min 2 (bootloader and kernel)
  header_file.write(struct.pack("<l", len(mod_list)))  

  for mod in mod_list:
    header_file.write(struct.pack("<l", os.path.getsize(mod)))
    env.concat_file(mod, env.MODULES)

  header_file.close()

  # display some stuff.
  env.display(env.HEADER_OK, "header file and mipsel_bios.bin file created",
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
