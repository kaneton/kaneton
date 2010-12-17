#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/utilities/inventory.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [thu dec 16 22:25:12 2010]
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
/*
 * note that should the kaneton bundle be used through the test system,
 * the bundle must be generated with the _test_ module activated, in addition
 * to the _bundle_ module.
 *
 * the following prevents the generation of the array of tests so that
 * the test system fails to compile a kaneton snapshot with a bundle
 * which has not been generated with the _test_ module.
 *
 * therefore, using such an invalid bundle should result with an error
 * similar to the following:
 *
 *   [EXECUTABLE]            /home/user/kaneton/kaneton/kaneton
 *   kaneton/modules/modules.lo: In function `module_test_locate':
 *   (.text+0x55a1f): undefined reference to `_module_test_functions'
 *   kaneton/kaneton/modules/modules.lo: In function `module_test_locate':
 *   (.text+0x55a3c): undefined reference to `_module_test_functions'
 *   kaneton/kaneton/modules/modules.lo: In function `module_test_locate':
 *   (.text+0x55a58): undefined reference to `_module_test_functions'
 *   kaneton/kaneton/modules/modules.lo: In function `module_test_dump':
 *   (.text+0x55b1c): undefined reference to `_module_test_functions'
 *   kaneton/kaneton/modules/modules.lo: In function `module_test_dump':
 *   (.text+0x55b26): undefined reference to `_module_test_functions'
 *   kaneton/kaneton/modules/modules.lo:(.text+0x55b50): more undefined references to `_module_test_functions' follow
 *   collect2: ld returned 1 exit status
 *   make[3]: *** [/home/user/kaneton/kaneton/kaneton] Error 1
 *   make[2]: *** [_] Error 42
 *   make[1]: *** [main] Error 42
 *   make: *** [_] Error 42
 *
 * these errors show that the test system has not been able to link the
 * kaneton snapshot to test with the bundle because a specific definition
 * is missing: _module_test_functions. this is the test array definied
 * in this file!
 */

#ifdef MODULE_test

# include <kaneton.h>

%(prototypes)s

ms_test_function        _module_test_functions[] =
{
%(tests)s
  { NULL, NULL }
};

#endif
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
