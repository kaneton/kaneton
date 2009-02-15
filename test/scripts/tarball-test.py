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

yaml_filename = None
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

  env.display(env.HEADER_NONE, "usage: tarball-tests.py [filename]",
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

  # remove object file
  entries = env.search(g_directory, "(\.o|\.lo)$", env.OPTION_FILE |  env.OPTION_RECURSIVE)
  for entry in entries:
    env.remove(entry, env.OPTION_NONE)

def                     dist():
    global yaml_filename
    global g_directory

    g_directory = env.temporary(env.OPTION_DIRECTORY)

    fd = open(yaml_filename, "r")
    content = ""
    while fd:
        line = fd.readline()
        if line:
            content += line
        else:
            break
    fd.close()

    env.copy(env._CHECK_DIR_ + '/kaneton',
             g_directory + "/test/kaneton", env.OPTION_RECURSIVE)
    
    env.copy(env._CHECK_DIR_ + '/Makefile',
             g_directory + "/test/Makefile", env.OPTION_NONE)

    env.copy(env._CHECK_DIR_ + '/list',
             g_directory + "/test/list", env.OPTION_NONE)
    
    env.copy(env._CHECK_DIR_ + '/check',
             g_directory + "/test/check", env.OPTION_RECURSIVE)

    env.mkdir(g_directory + "/test", env.OPTION_NONE)
    env.copy(env._TEST_DIR_ + '/tools',
             g_directory + "/test/tools", env.OPTION_RECURSIVE)
    env.copy(env._TESTSUITE_,
             g_directory + "/test/test.yml" , env.OPTION_NONE)

    env.copy(env._SOURCE_DIR_ + '/environment/profile/user/test',
             g_directory + "/environment/profile/user", env.OPTION_RECURSIVE)

    clean()

    tarball_name = "tests.tar.bz2"
    env.cd(g_directory, env.OPTION_NONE)
    
    env.display(env.HEADER_OK,
                "packing the check files...",
                env.OPTION_NONE)

    env.pack(".",
             env._TEST_DIR_ + "/" + tarball_name,
             env.OPTION_NONE)
             
    env.display(env.HEADER_OK,
                "removing tamporary directory...",
                env.OPTION_NONE)

    env.remove(g_directory, env.OPTION_NONE)

#
# main()
#
# this function performs the main stuff.
#
def			main():
  global yaml_filename

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  yaml_filename = sys.argv[1]

  # a message.
  env.display(env.HEADER_OK, "tarball for %s" % sys.argv[1], env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  dist()

  # a final message.
  env.display(env.HEADER_OK, "tarball done", env.OPTION_NONE);

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
