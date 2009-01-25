#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#

#
# ---------- information ------------------------------------------------------
#
# this tool creates a tarball with the test files
#
# the argument 'filename' is the yaml file that describe the test suite.
#

#
# ---------- imports ----------------------------------------------------------
#

import sys
import re
import os

import env

#
# ---------- globals ----------------------------------------------------------
#

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
  type = None

  env.display(env.HEADER_NONE, "usage: tarball-kaneton.py [filename]",
              env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

#
# clean()
#
# this function removes unwanted stuff i.e the env._EXPORT_FILTER_
# stuff as well as the subversion directories.
#
def			clean():
  entries = None
  entry = None

  # a message.
  env.display(env.HEADER_OK,
              "cleaning the distribution from unwanted/private stuff...",
              env.OPTION_NONE)

  # remove the subversion directories
  entries = env.search(g_directory, "^\.svn$",
                       env.OPTION_DIRECTORY | env.OPTION_RECURSIVE)

  for entry in entries:
    env.remove(entry, env.OPTION_NONE)

  # remove the env._EXPORT_FILTER_ stuff
  entities = re.findall("([^\t ]+)", env._EXPORT_FILTER_)

  for entity in entities:
    env.remove(entity, env.OPTION_NONE)


def                     dist():
    global g_directory

    g_directory = env.temporary(env.OPTION_DIRECTORY)

    env.copy(env._IMAGE_, g_directory + '/kaneton.img',
             env.OPTION_NONE)

    clean()

    tarball_name = "kaneton.tar.bz2"
    env.cd(g_directory, env.OPTION_NONE)
    
    env.display(env.HEADER_OK,
                "packing the kaneton files...",
                env.OPTION_NONE)

    env.pack(".",
             env._TEST_DIR_ + "/" + tarball_name,
             env.OPTION_NONE)
             
    env.display(env.HEADER_OK,
                "removing tomporary directory...",
                env.OPTION_NONE)

    env.remove(g_directory, env.OPTION_NONE)

#
# main()
#
# this function performs the main stuff.
#
def			main():
  # a message.
  env.display(env.HEADER_OK, "tarball for kaneton image", env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  dist()

  # a final message.
  env.display(env.HEADER_OK, "tarball done", env.OPTION_NONE);

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
