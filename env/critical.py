#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/env/critical.py
#
# created       julien quintard   [fri dec 15 13:43:03 2006]
# updated       julien quintard   [mon may  7 13:16:06 2007]
#

#
# ---------- imports ----------------------------------------------------------
#

import os
import sys
import re

#
# ---------- globals ----------------------------------------------------------
#

g_source_dire = None
g_contents = None
g_assignments = None

g_variables = {}

#
# ---------- functions --------------------------------------------------------
#

#
# error()
#
# this function simply displays an error.
#
def			error(msg):
  sys.stderr.write("[!] " + msg)
  sys.exit(42)



#
# load()
#
# this function takes an arbitray number of files and load them in a
# single python string.
#
def			load(files):
  global g_contents

  handle = None
  line = None
  file = None
  cwd = None

  g_contents = "_SOURCE_DIR_		=		" + g_source_dir + "\n"

  # load each file
  for file in files:
    if not os.path.exists(file):
      continue

    try:
      handle = open(file, "r")
    except IOError:
      error("unable to open the file " + file + "\n")

    for line in handle.readlines():
      g_contents += line

    handle.close()



#
# comments()
#
# this function removes the comments from the file
#
def			comments():
  global g_contents
  comments = None
  c = None

  comments = re.findall("^#.*$", g_contents, re.MULTILINE);

  for c in comments:
    g_contents = g_contents.replace(c, "", 1)



#
# extract()
#
# this function extracts the variables assignments from the environment
# configuration files previously loaded.
#
def			extract():
  global g_assignments

  g_assignments = re.findall("^"					\
                             "[ \t]*"					\
                             "([a-zA-Z0-9_]+)"				\
                             "[ \t]*"					\
                             "="					\
                             "[ \t]*"					\
                             "((?:(?:\\\\\n)|[^\n])+)"			\
                             "\n", g_contents, re.MULTILINE);



#
# get()
#
# this function tries to locate the variable's value in the not yet
# expanded variables list g_assignments
#
def		get(variable):
  a = None

  for a in g_assignments:
    if variable == a[0]:
      return a[1]



#
# expand()
#
# this function tries to expand the given variable.
#
def		expand(variable, stack):
  variables = None
  value = None
  var = None
  registered = None
  new = None
  position = None

  # try to get the variable's value from the already expanded variables.
  try:
    registered = g_variables[variable]
  except:
    pass

  # but also from the not yet expanded variables.
  new = get(variable)

  # check if the variable is defined somewhere.
  if registered == None and new == None:
      error("the kaneton environment variable " + variable +
            " is not defined\n")

  # in this case, just return 'cause the variable is already registered
  # and up-to-date.
  if new == None:
    return

  # check recursion assignments.
  try:
    position = stack.index(variable)
  except:
    pass

  if position != None:
    error("the kaneton environment variable " + variable +
          " recursively references itself.\n")

  # in the other cases, expand the value and register this new variable.

  # backup the 'new' value.
  value = new

  # remove the variable from the not yet expanded list.
  g_assignments.remove((variable, new))

  # locate the variables used in the variable's value and try
  # to expand them too.
  # finally, replace the variables by their values.
  variables = re.findall("\$\{([^}]+)\}", value)
  if variables:
    for var in variables:
      expand(var, stack + [variable])
    for var in variables:
      value = value.replace("${" + var + "}", g_variables[var])

  # locate the shell environment variables used in the variable's value
  # and replace these variables by their values.
  variables = re.findall("\$\(([^}]+)\)", value)
  if variables:
    for var in variables:
      if os.getenv(var) == None:
        error("shell user variable " + var + " is not defined\n")
      value = value.replace("$(" + var + ")", os.getenv(var))

  # finally register the new variable with its expanded value.
  g_variables[variable] = value



#
# resolve()
#
# this function resolves the variable assignments.
def			resolve():
  while len(g_assignments) != 0:
    expand(g_assignments[0][0], [])



#
# generate()
#
# this function generates the kaneton development environment files.
#
def			generate(mfile, mdependencies, pfile, pdependencies):
  mhandle = None
  phandle = None
  handle = None
  file = None
  line = None
  var = None

  # open the make environment file.
  try:
    mhandle = open(mfile, "w")
  except IOError:
    error("unable to open the file " + mfile + "\n")

  # open the python environment file.
  try:
    phandle = open(pfile, "w")
  except IOError:
    error("unable to open the file " + pfile + "\n")

  # inject the kaneton environment configuration for the make and python
  # environment.
  for var in g_variables:
    mhandle.write(var + " := " + g_variables[var] + "\n")
    phandle.write(var + " = " + "\"" + g_variables[var] + "\"" + "\n")

  # append the make environment files to the make environment file.
  for file in mdependencies:
    if not os.path.exists(file):
      continue

    try:
      handle = open(file, "r")
    except:
      error("unable to open the file " + file + "\n")

    mhandle.writelines(handle.readlines())

    handle.close()

  # append the python environment files to the python environment file.
  for file in pdependencies:
    if not os.path.exists(file):
      continue

    try:
      handle = open(file, "r")
    except:
      error("unable to open the file " + file + "\n")

    phandle.writelines(handle.readlines())

    handle.close()

  phandle.close()
  mhandle.close()



#
# main()
#
# this function builds a kaneton development environment.
#
def			main():
  global g_source_dir
  architecture = None
  plateform = None
  machine = None
  python = None
  host = None
  user = None

  # get the shell environment variables.
  user = os.getenv("KANETON_USER")
  host = os.getenv("KANETON_HOST")
  python = os.getenv("KANETON_PYTHON")
  plateform = os.getenv("KANETON_PLATEFORM")
  architecture = os.getenv("KANETON_ARCHITECTURE")

  # check the presence of the shell environment variable.
  if user == None:
    error("the shell environment variable KANETON_USER is not set")

  if host == None:
    error("the shell environment variable KANETON_HOST is not set")

  if python == None:
    error("the shell environment variable KANETON_PYTHON is not set")

  if plateform == None:
    error("the shell environment variable KANETON_PLATEFORM is not set")

  if architecture == None:
    error("the shell environment variable KANETON_ARCHITECTURE is not set")

  # first, set a virtual kaneton variable containing the location of
  # the kaneton source directory.
  # this directory is expected to be relatively located at: ../
  # since this script should have been launched in the environment/
  # directory
  cwd = os.getcwd()
  os.chdir("..")
  g_source_dir = os.getcwd()
  os.chdir(cwd)

  # build a string containing all the directories where configuration
  # files could be located.
  load(("default/environment/environment.conf",
        "default/boot/boot.conf",
        "default/machine/machine.conf",
        "default/core/core.conf",
        "machines/" + machine + "/machine.conf",
        "users/" + user + "/user.conf"))

  # removes the comments
  comments()

  # extract the assignments from the files.
  extract()

  # resolve the variables in assignments.
  resolve()

  # generate the development environment files.
  generate("env.mk", ("default/environment/environment.mk",
                      "default/boot/boot.mk",
                      "default/machine/machine.mk",
                      "default/core/core.mk",
                      "machines/" + machine + "/machine.mk",
                      "users/" + user + "/user.mk"),
           "env.py", ("default/environment/environment.py",
                      "default/boot/boot.py",
                      "default/machine/machine.py",
                      "default/core/core.py",
                      "machines/" + machine + "/machine.py",
                      "users/" + user + "/user.py"))

#
# ---------- entry point ------------------------------------------------------
#

main()
