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
# updated       enguerrand raymond   [mon may 18 03:49:44 2009]
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

g_image			="data/kaneton.img"
g_menu			= None
g_inputs		= None
g_action		= None

g_firmware_bin		= env._COMPONENT_FIRMWARE_ + ".bin"
g_components_file	= env._SOURCE_DIR_ + "/components"
g_qemu_mips_bios	= env._SOURCE_DIR_ + "/mipsel_bios.bin"
g_header_file		= env._SOURCE_DIR_ + "/header"

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
# padding()
#
# this function write padding information to file following alignment and update adr
#
def			padding(file, alignment, adr):
  wfile = open(file, "ab")
  
  while (adr % alignment) <> 0:
    wfile.write('\0')
    adr += 1

  return adr



#
# module_name()
#
# this function writes the module name in the modules file
#
def			module_name(modules, name):
  mod_file = open(modules, "ab")
  mod_file.write(name)
  mod_file.write('\0')

  mod_file.close()



#
# header_write()
#
# this function upadtes the header file with module to load
#
def			header_write(module, name, image, header_file, modules):
  header_file.write(struct.pack("<Q", os.path.getsize(module)))
  
  image = padding(modules, 4, image)

  header_file.write(struct.pack("<Q", image))
  image += 1 + len(name)

  module_name(modules, name)

  image = padding(modules, 4, image)

  header_file.write(struct.pack("<Q", image))
  image += os.path.getsize(module)
  
  return image



#
# install()
#
# this function installs the kaneton binaries onto the boot device.
#
def			install():
  input = None
  mod_list = None
  header_file = None
  image = 0

  # warn the user before performing any action.
  warning()

  # display some stuff.
  env.display(env.HEADER_OK,
              "installing kaneton binaries on the boot device",
              env.OPTION_NONE)

  # creates binary files : bootloader and kaneton
  env.binary_extract(env._COMPONENT_FIRMWARE_, ".text .data .rdata .rodata .bss", g_firmware_bin)

  mod_list = env._COMPONENTS_.split()

  image = int(env._QEMU_MIPS_START_ADDRESS_, 16) + ((len(mod_list) -1) * 24) + os.path.getsize(g_firmware_bin) + 8

  # open the header file to write header information
  header_file = open(g_header_file, "ab");

  # write the number of module : min 2 (bootloader and kernel)
  header_file.write(struct.pack("<l", len(mod_list) - 1))  

  for mod in mod_list:
    if mod.split("/")[len(mod.split("/")) - 1] != "firmware":
      image = header_write(mod, mod.split("/")[len(mod.split("/")) - 1], image, header_file, g_components_file)
      env.concat_file(mod, g_components_file)

  header_file.close()

  # cat the binary files in the mips_bios.bin file
  env.concat_file(g_firmware_bin, g_qemu_mips_bios)
  env.concat_file(g_header_file, g_qemu_mips_bios)
  env.concat_file(g_components_file, g_qemu_mips_bios)



#
# build()
#
# this function builds, initializes the boot device.
#
def			build():
  handle = None
  header_file = None

  # warn the user before performing any action.
  warning()

  # creates the mips_bios.bin and the modules files
  env.push(g_qemu_mips_bios, "", None)
  env.push(g_components_file, "", None)

  # creates the flag file
  header_file = open(g_header_file, "wb");

  # write the flag
  header_file.write(struct.pack("<l", 0x07070707))

  header_file.close()

  # display some stuff.
  env.display(env.HEADER_OK, "header, components and mipsel_bios.bin files created",
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
