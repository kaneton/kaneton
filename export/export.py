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
# updated       julien quintard   [thu may 31 22:55:26 2007]
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

import env

#
# ---------- globals ----------------------------------------------------------
#

g_types = [ "dist", "backup" ]
g_stages = [ "0", "1", "2", "3", "4" ]

g_type = None
g_from = None
g_to = None

g_file = None
g_directory = None

g_distribution = None

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
  type = None

  env.display(env.HEADER_ERROR, "usage: export.py [type]|k[stage,stage]",
              env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  env.display(env.HEADER_ERROR, "types:", env.OPTION_NONE)

  for type in g_types:
    env.display(env.HEADER_ERROR, "  " + type, env.OPTION_NONE)

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

  if g_type:
    env.display(env.HEADER_OK,
                "  type:                   " + g_type,
                env.OPTION_NONE)
  else:
    env.display(env.HEADER_OK,
                "  stages:                 [" + g_from + ", " + g_to + "]",
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
  global g_type
  type = None

  if g_type:
    for type in g_types:
      if re.search(g_type, type) != None:
        g_type = type
        return

    env.display(env.HEADER_ERROR, "unknown type '" + g_type + "'",
                env.OPTION_NONE)
    env.display(env.HEADER_NONE, "", env.OPTION_NONE)
    usage()
    sys.exit(42)
  else:
    if not (g_from in g_stages) or not (g_to in g_stages):
      env.display(env.HEADER_ERROR, "unknown stage: '" + g_from + "' or '" +
                  g_to + "'", env.OPTION_NONE)
      env.display(env.HEADER_NONE, "", env.OPTION_NONE)
      usage()
      sys.exit(42)



#
# dist()
#
# this function creates a distribution tarball and places it in the
# export/ directory.
#
def			dist():
  global g_distribution
  name = None

  # build a name depending on the argument.
  if g_type:
    name = g_type
  else:
    name = "k" + g_from + "," + g_to

  g_distribution = env._EXPORT_ + "-" +					\
                   env.stamp(env.OPTION_NONE) + "-" + name

  # move to the directory parent to the kaneton repository copy.
  env.cd(g_directory, env.OPTION_NONE)

  # create an archive.
  env.pack(env._EXPORT_,
           env._EXPORT_DIR_ + "/" + g_distribution, env.OPTION_NONE)

  # remove the temporary stuff.
  env.remove(g_file, env.OPTION_NONE)
  env.remove(g_directory, env.OPTION_NONE)



#
# stages()
#
# this function removes the pieces of code tagged as a stage comprised
# between g_from and g_to, included.
#
def			stages():
  content = None
  stage = None
  files = None
  file = None

  # look for any source file which could possibly contain unwanted pieces
  # of code.
  files = env.search(g_directory, "^.*\.(c|S|h|asm)$", env.OPTION_FILE)

  # for each file, remove the tags
  for file in files:
    content = env.pull(file, env.OPTION_NONE)

    # for each stage in the range [g_from, g_to]
    for stage in range(g_stages.index(g_from), g_stages.index(g_to) + 1):
      # find the pieces of code
      markings = re.findall("("						\
                              "^.*\[cut\] k" + str(stage) + ".*\n"	\
                              "(?:.|\n)*"				\
                              "^.*\[cut\] /k" + str(stage) + ".*\n"	\
                            ")",
                            content, re.MULTILINE)

      if not markings:
        continue

      # remove the pieces of code
      for marking in markings:
        content = content.replace(marking, "")

    # finally rewrite the file
    env.push(file, content, env.OPTION_NONE)



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
  content = None
  files = None
  file = None

  # look for any source file which could possibly contain unwanted pieces
  # of code.
  files = env.search(g_directory, "^.*\.(c|S|h|asm)$", env.OPTION_FILE)

  # for each file, remove the tags
  for file in files:
    content = env.pull(file, env.OPTION_NONE)

    # locate the tags
    markings = re.findall("^(.*\[cut\].*)$", content, re.MULTILINE)

    if not markings:
      continue

    # remove the tags
    for marking in markings:
      content = content.replace(marking, "")

    # rewrite the file
    env.push(file, content, env.OPTION_NONE)



#
# clean()
#
# this function removes unwanted stuff i.e the env._HIDDEN_ stuff as well
# as the subversion directories.
#
def			clean():
  entries = None
  entry = None

  # remove the subversion directories
  entries = env.search(g_directory, "^\.svn$",
                       env.OPTION_DIRECTORY | env.OPTION_RECURSIVE)

  for entry in entries:
    env.remove(entry, env.OPTION_NONE)

  # remove the env._HIDDEN_ stuff
  entities = re.findall("([^\t ]+)", env._HIDDEN_)

  for entity in entities:
    env.remove(entity, env.OPTION_NONE)



#
# build()
#
# this function creates a copy of the repository in a temporary directory.
#
def			build():
  # go to the root directory.
  env.cd(env._SOURCE_DIR_, env.OPTION_NONE)

  # creates a tarball of the kaneton root directory.
  env.pack(env.info(env.OPTION_CURRENT_DIRECTORY), g_file, env.OPTION_NONE)

  # make a directory named env._EXPORT_
  env.mkdir(g_directory + "/" + env._EXPORT_, env.OPTION_NONE)

  # extract the just created tarball in order to clean it from
  # unwanted files.
  env.unpack(g_file, g_directory + "/" + env._EXPORT_, env.OPTION_NONE)

  # go to this directory in order to avoid bad manipulations
  env.cd(g_directory + "/" + env._EXPORT_, env.OPTION_NONE)

  # depending on the type of export, performs more or less steps.
  if g_type:
    if g_type == "backup":
      pass
    elif g_type == "dist":
      clean()
      tags()
  else:
    clean()
    stages()
    tags()



#
# main()
#
# this function performs the main stuff.
#
def			main():
  global g_directory
  global g_file
  global g_type
  global g_from
  global g_to
  arg = None

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # set the stage.
  arg = sys.argv[1]

  if arg[0] == "k":
    g_from = arg[1:].split(",")[0]
    g_to = arg[1:].split(",")[1]
  else:
    g_type = arg

  # a message.
  env.display(env.HEADER_OK, "preparing the exportation", env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # try to lookup the given stage in the known stages.
  lookup()

  # warn the user about what is about to happen.
  warning()

  # create a temporary file and a temporary directory.
  g_file = env.temporary(env.OPTION_FILE)
  g_directory = env.temporary(env.OPTION_DIRECTORY)

  # build the tarball.
  build()

  # create a tarball distribution.
  dist()

  # a final message.
  env.display(env.HEADER_OK, "distribution '" + g_distribution +
              "' built successfully", env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
