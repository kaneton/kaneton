#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/export/export.py
#
# created       julien quintard   [thu may 24 01:40:40 2007]
# updated       julien quintard   [thu may 24 02:47:03 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this tool creates tarball of the kaneton project.
#
# the argument 'dist' makes a backup of the current development tree
# including the cut lines but still without the subversion control directories.
#
# the argument 'backup' makes a simple backup of the whole project.
#
# the _HIDDEN_ variable contains the list of directories not to include in
# exported distributions.
#

#
# ---------- imports ----------------------------------------------------------
#

import sys
import re

import os # XXX

import env

#
# ---------- globals ----------------------------------------------------------
#

g_stages = [ "k0", "k1", "k2", "k3", "k4", "k5", "dist", "backup" ]

g_stage = None
g_file = None
g_directory = None

#
# ---------- functions --------------------------------------------------------
#

#
# usage()
#
# this function displays the usage.
#
def			usage():
  stage = None

  env.display(env.HEADER_ERROR, "usage: export.py [stage]", env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  env.display(env.HEADER_ERROR, "stages:", env.OPTION_NONE)

  for stage in g_stages:
    env.display(env.HEADER_ERROR, "  " + stage, env.OPTION_NONE)



#
# warning()
#
# this function asks the user if everything is right.
#
def			warning():
  env.display(env.HEADER_OK, "your current configuration:", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  stage:                  " + g_stage,
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_INTERACTIVE,
              "to cancel press CTRL^C, otherwise press enter", env.OPTION_NONE)

  env.input(env.OPTION_NONE)



#
# lookup()
#
# this function tries to match the given stage name in the stages.
#
def			lookup():
  global g_stage
  stage = None

  for stage in g_stages:
    if re.search(g_stage, stage) != None:
      g_stage = stage
      return

  g_stage = None



#
# build()
#
# XXX
#
def			build():
  # go to the root directory.
  env.cd(env._SOURCE_DIR_, env.OPTION_NONE)

  # creates a tarball of the kaneton root directory.
  env.pack(".", g_file, env.OPTION_NONE)

  # extract the just created tarball in order to clean it from
  # unwanted files.
  env.unpack(g_file, g_directory, env.OPTION_NONE)



#
# tags()
#
# this function removes the cut lines.
#
# these lines indicate the location of pieces of code which need to be
# remove depending on the stage.
#
# this function does not cut the pieces of code but the lines indicating
# where to cut the code off.
#
def			tags():
  pass

#
# clean()
#
# XXX
#
def			clean():
  files = None

  if g_stage == "backup":
    # nothing to do
    pass
#  elif g_stage == 

  # look for any source file which could possibly contain unwanted pieces
  # of code.
#  files = env.search(g_directory, "^.*\.(c|S|h|asm)$", env.OPTION_FILE)


#
# main()
#
# this function performs the main stuff.
#
def			main():
  global g_directory
  global g_stage
  global g_file

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # set the stage.
  g_stage = sys.argv[1]

  # a message.
  env.display(env.HEADER_OK, "preparing the exportation for '" + g_stage + "'",
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # try to lookup the given stage in the known stages.
  lookup()

  # if no document is found, display an error.
  if g_stage == None:
    env.display(env.HEADER_ERROR, "unknown stage", env.OPTION_NONE)
    env.display(env.HEADER_NONE, "", env.OPTION_NONE)
    usage()
    sys.exit(42)

  # warn the user about what is about to happen.
  warning()

  # create a temporary file and a temporary directory.
  g_file = env.temporary(env.OPTION_FILE)
  g_directory = env.temporary(env.OPTION_DIRECTORY)

  # build the tarball.
#  build()

  # clean the tarball directory from unwanted file and pieces of code.
#  clean()

  print "suce" > "T"

  # XXX
  print g_file
  print g_directory

  os.system("rm -Rf " + g_file + " " + g_directory)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
