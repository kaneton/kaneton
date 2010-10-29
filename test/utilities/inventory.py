#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repo...neton.STABLE/test/utilities/inventory.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [wed oct 27 13:32:49 2010]
#

#
# ---------- information ------------------------------------------------------
#
# this program generates an inventory of the available tests.
#

#
# ---------- packages ---------------------------------------------------------
#

import env

import sys
import re
import yaml

import ktp

#
# ---------- globals ----------------------------------------------------------
#

g_school = None
g_year = None

#
# ---------- functions --------------------------------------------------------
#

#
# usage()
#
# this function displays the usage.
#
def			usage():
  env.display(env.HEADER_ERROR,
              "usage: inventory.py [directory]",
              env.OPTION_NONE)

#
# locate()
#
# this function locates the test manifests.
#
def                     locate(directory):
  functions = []
  manifest = None

  # display.
  env.display(env.HEADER_OK,
              "locating the test manifests",
              env.OPTION_NONE)

  # retrieve the list of tests.
  manifests = env.search(directory,
                         "^.*.mnf$",
                         env.OPTION_FILE | env.OPTION_RECURSIVE)

  # load the manifests.
  for manifest in manifests:
    # read the file.
    stream = ktp.manifest.Load(manifest)

    # record the function symbol.
    functions += [ stream["symbol"] ]

  return functions

#
# generate()
#
# this function generates an C array containing the
# list of test functions.
#
def                     generate(functions,
                                 inventory):
  prototypes = []
  tests = []
  content = None

  # display.
  env.display(env.HEADER_OK,
              "generating the inventory file",
              env.OPTION_NONE)

  # build the array content.
  for function in functions:
    prototypes += [ "void               %s(void);" % function ]
    tests += [ '  { "%(symbol)s", %(function)s },' % { "symbol": function,
                                                     "function": function } ]

  # generate the fil content.
  content = """
#include <kaneton.h>

%(prototypes)s

s_test_function test_functions[] =
{
%(tests)s
  { NULL, NULL }
};
""" % { "prototypes": "\n".join(prototypes),
        "tests": "\n".join(tests) }

  # finally, write the file
  env.push(inventory, content, env.OPTION_NONE)

#
# main()
#
# this is the main function.
#
def                     main():
  directory = None

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # set the global variables.
  directory = sys.argv[1]

  # display.
  env.display(env.HEADER_OK,
              "generating the inventory for '" + directory + "'",
              env.OPTION_NONE)

  # locate the tests.
  functions = locate(directory)

  # generate the inventory.
  generate(functions,
           directory + "/inventory.c")

  # display.
  env.display(env.HEADER_OK,
              "inventory generated in '" + directory + "/inventory.c" + "'",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
