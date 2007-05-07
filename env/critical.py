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
# updated       julien quintard   [mon may  7 16:42:34 2007]
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

g_directories = None
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
# this function takes an arbitray number of directories where pattern
# files could be located and load them in a single python string.
#
def			load(directories, pattern):
  content = ""

  directory = None
  handle = None
  files = None
  line = None
  file = None
  cwd = None

  for directory in directories:
    files = os.listdir(directory);

    for file in files:
      if not os.path.isfile(directory + "/" + file):
        continue

      if not re.match(pattern, os.path.basename(directory + "/" + file)):
        continue

      try:
        handle = open(directory + "/" + file, "r")
      except IOError:
        error("unable to open the file " + directory + "/" + file + "\n")

      for line in handle.readlines():
        content += line

      handle.close()

  return content


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
  registered = None
  variables = None
  position = None
  value = None
  var = None
  new = None

  print "--- " + variable

  # try to get the variable's value from the already expanded variables.
  try:
    registered = g_variables[variable]
  except:
    pass

  # but also from the not yet expanded variables.
  new = get(variable)

  print "-- reg | new"
  print registered
  print new

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
    print "---------------------------------------- " + g_assignments[0][0]
    expand(g_assignments[0][0], [])



#
# generate()
#
# this function generates the kaneton development environment files.
#
def			generate(mfile, mcontent, pfile, pcontent):
  mhandle = None
  phandle = None
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
  mhandle.write(mcontent)

  # append the python environment files to the python environment file.
  phandle.write(pcontent)

  # close the files.
  phandle.close()
  mhandle.close()



#
# main()
#
# this function builds a kaneton development environment.
#
def			main():
  global g_directories
  global g_contents
  architecture = None
  source_dir = None
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

  # set the configuration directories based on the user variables.
  g_directories = ("profile/",
                   "profile/environment/",
                   "profile/environment/behaviour/",
                   "profile/environment/behaviour/" + host + "/",
                   "profile/environment/behaviour/" + host + "/" +
                                                      architecture + "/",
                   "profile/environment/user/",
                   "profile/environment/user/" + user + "/",
                   "profile/kaneton/")

  # first, set a virtual kaneton variable containing the location of
  # the kaneton source directory.
  # this directory is expected to be relatively located at: ../
  # since this script should have been launched in the environment/
  # directory
  cwd = os.getcwd()
  os.chdir("..")
  source_dir = os.getcwd()
  os.chdir(cwd)

  # build the global content variable adding the _SOURCE_DIR_ variable.
  # the load() function is used to load the content of the configuration
  # files *.conf.
  g_contents = "_SOURCE_DIR_		=		" + source_dir + "\n"
  g_contents += load(g_directories, "^.*\.conf$")

  # removes the comments
  comments()

  # extract the assignments from the files.
  extract()

  # resolve the variables in assignments.
  resolve()

  # generate the development environment files.
  generate("env.mk", load(g_directories, "^.*\.mk$"),
           "env.py", load(g_directories, "^.*\.py$"))

#
# ---------- entry point ------------------------------------------------------
#

main()
