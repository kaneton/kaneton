#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/configure/environment.py
#
# created       julien quintard   [thu may 24 16:58:00 2007]
# updated       julien quintard   [sun mar 29 00:36:36 2009]
#

#
# ---------- imports ----------------------------------------------------------
#

import env

import re
import os
import sys
import yaml

#
# ---------- types ------------------------------------------------------------
#

#
# this class represents a variable with its actual value, its type,
# the values it can take and a little description of it.
#
class c_variable:
  def __init__(self):
    self.variable = None

    self.assignment = None
    self.value = None

    self.string = None
    self.type = None
    self.values = None
    self.description = None

#
# ---------- globals ----------------------------------------------------------
#

g_directories = (env._PROFILE_DIR_,
                 env._PROFILE_DIR_ + "/host",
                 env._PROFILE_HOST_DIR_,
                 env._PROFILE_DIR_ + "/kaneton",
                 env._PROFILE_DIR_ + "/kaneton/core",
                 env._PROFILE_DIR_ + "/kaneton/platform",
                 env._PROFILE_PLATFORM_DIR_,
                 env._PROFILE_DIR_ + "/kaneton/architecture",
                 env._PROFILE_ARCHITECTURE_DIR_,
                 env._PROFILE_DIR_ + "/kaneton/libc",
                 env._PROFILE_DIR_ + "/user",
                 env._PROFILE_USER_DIR_)

g_database = []

#
# ---------- options ----------------------------------------------------------
#

OPTION_INCLUDES = 1
OPTION_COMMENTS = 2

TYPE_ANY = 1
TYPE_SET = 2

#
# ---------- functions --------------------------------------------------------
#

#
# error()
#
# this function displays an error and quit.
#
def                     error(message):
  env.display(env.HEADER_ERROR, message, env.OPTION_NONE)
  sys.exit(42)



#
# warning()
#
# this function simply displays an error.
#
def                     warning(message):
  env.display(env.HEADER_ERROR, message, env.OPTION_NONE)



#
# comments()
#
# this function removes the comments from a given content.
#
def                     comments(content):
  comments = None
  c = None

  comments = re.findall("^#.*$", content, re.MULTILINE);

  for c in comments:
    content = content.replace(c, "", 1)

  return content



#
# load()
#
# this function takes an arbitray number of directories where pattern
# files could be located and load them in a single python string.
#
# the function considers these files can contain an 'include' statement
# in order to include other files.
#
def			load(directories, pattern, option):
  content = ""

  directory = None
  includes = None
  include = None
  handle = None
  files = None
  line = None
  file = None
  cwd = None

  for directory in directories:
    files = env.list(directory, env.OPTION_FILE)

    for file in files:
      if not re.match(pattern, file):
        continue

      content += env.pull(directory + "/" + file, env.OPTION_NONE) + "\n"

      if (option & OPTION_COMMENTS):
        content = comments(content)

      if (option & OPTION_INCLUDES):
        includes = re.findall("("					\
                                "^"					\
                                "include"				\
                                "[ \t]*"				\
                                "(.*)"					\
                                "\n"					\
                              ")", content, re.MULTILINE);

        for include in includes:
          content = content.replace(include[0],
                                    load([env.path(directory + "/" +
                                                   include[1],
                                                   env.OPTION_DIRECTORY)],
                                         "^" + env.path(directory +
                                                        "/" +
                                                        include[1],
                                                   env.OPTION_FILE) +
                                         "$",
                                         OPTION_COMMENTS | OPTION_INCLUDES))

  return content



#
# locate()
#
# this function tries to locate the name in the given array of tuple:
#   ( variable, operator, value )
#
def		        locate(name, array):
  element = None

  for element in array:
    if name == element[0]:
      return element

  return None



#
# expand()
#
# this function tries to expand the given variable.
#
# the variables assignments are supposed correct.
#
def			expand(variable, stack):
  references = None
  reference = None

  # get the object corresponding to the variable
  object = get(variable)

  if not object:
    error("the kaneton environment variable " + variable + " is not defined\n")

  # try to locate the variable in the stack, meaning the variable references
  # itself, maybe indirectly.
  if variable in stack:
    error("the environment variable " + variable +
          " recursively references itself.\n")

  # locate, expand and replace the kaneton environment variable references.
  references = re.findall("\$\{([^}]+)\}", object.value)
  if references:
    for reference in references:
      expand(reference, stack + [variable])
    for reference in references:
      object.value = object.value.replace("${" + reference + "}",
                                          get(reference).value)

  # locate, expand and replace the shell variable references.
  references = re.findall("\$\(([^}]+)\)", object.value)
  if references:
    for reference in references:
      if os.getenv(reference) == None:
        warning("shell user variable " + var + " is not defined\n")

        object.value = object.value.replace("$(" + reference + ")", "")
      else:
        object.value = object.value.replace("$(" + reference + ")",
                                            os.getenv(reference))



#
# extract()
#
# this function extracts the variables assignments from the given content
# and then returns a list of assignments: ( variable, operator, value )
#
# the variables assignments are supposed correct.
#
def			extract(content):
  assignments = []

  assignments = re.findall("^"						\
                           "[ \t]*"					\
                           "([a-zA-Z0-9_]+)"				\
                           "[ \t]*"					\
                           "(\+?=)"					\
                           "[ \t]*"					\
                           "((?:(?:\\\\\n)|[^\n])+)"			\
                           "\n", content, re.MULTILINE);

  return assignments



#
# get()
#
# this function returns a variable of the database.
#
def			get(variable):
  object = None

  for object in g_database:
    if variable == object.variable:
      return object

  return None



#
# build()
#
# this function starts building the variables database.
#
def			build(assignments):
  global g_database
  assignment = None
  object = None

  for assignment in assignments:
    # try to get an already registered variable with this name.
    object = get(assignment[0])

    if not object:
      object = c_variable()

      # if it is an assignment.
      if assignment[1] == "=":
        object.variable = assignment[0]
        object.assignment = assignment[2]
        object.value = assignment[2]
      # an appending.
      elif assignment[1] == "+=":
         error("appending to an undefined variable '" + assignment[0] +
              "' is not allowed\n")
      else:
        error("unknown assignment token '" + assignment[1] + "' for the "
              "variable '" + assignment[0] + "'.\n")

      g_database.append(object)
    else:
      # if it is an assignment, just override the previous declaration.
      if assignment[1] == "=":
        object.assignment = assignment[2]
        object.value = assignment[2]
      # if it is a concatenation, override the previous one with a
      # concatenation of the two values.
      elif assignment[1] == "+=":
        object.assignment = object.assignment + " " + assignment[2]
        object.value = object.value + " " + assignment[2]
      else:
        error("unknown assignment token '" + assignment[1] + "' for the "
              "variable '" + assignment[0] + "'.\n")



#
# configuration()
#
# this function loads the environment configuration files and then
# resolves the variables in order to build the variables database.
#
def			configuration():
  assignments = None
  content = None
  object = None

  # load the files in a single python string.
  content = "_SOURCE_DIR_	=	" + env._SOURCE_DIR_ + "\n"
  content += load(g_directories, "^.*\.conf$",
                  OPTION_COMMENTS | OPTION_INCLUDES)

  # extract the assignments from the content.
  assignments = extract(content)

  # build the variables database.
  build(assignments)

  # expands the variables.
  for object in g_database:
    expand(object.variable, [])



#
# description()
#
# this function completes the work achieved by the configuration()
# function by adding descriptive information to the variables.
#
def			description():
  content = None
  streams = None
  stream = None
  object = None
  entry = None

  # load the description files.
  content = load(g_directories, "^.*\.desc$", OPTION_COMMENTS)

  # parse the description content based on the YAML syntax.
  streams = yaml.load(content)

  # for each entry, complete the variables database.
  for stream in streams:
    try:
      object = get(stream["variable"])

      object.string = stream["string"]

      if stream["type"] == "set":
        object.type = TYPE_SET
        object.values = stream["values"]
      elif stream["type"] == "any":
        object.type = TYPE_ANY
      else:
        continue

      object.description = stream["description"]
    except:
      continue



#
# modify()
#
# this function takes a variable name and value and modify both the
# in-memory database entry as well as the user profile configuration file.
#
def			modify(variable, assignment):
  content = None
  matches = None
  object = None
  match = None

  # update the database
  for object in g_database:
    if variable == object.variable:

      # if the value is the same, just ignore the call
      if object.assignment == assignment:
        return

      object.assignment = assignment

      break

  # load the user profile configuration file.
  content = env.pull(env._PROFILE_USER_DIR_ + "/" + env._USER_ + ".conf",
                     env.OPTION_NONE) + "\n"

  # find the assignments related to the given variable
  matches = re.findall("(" +						\
                         "^" +						\
                         "(" +						\
                           "[ \t]*" +					\
                           variable +					\
                           "[ \t]*" +					\
                           "\+?=" +					\
                           "[ \t]*" +					\
                         ")" +						\
                         "((?:(?:\\\\\n)|[^\n])+)" +			\
                         "\n" +						\
                       ")", content, re.MULTILINE);

  if not matches:
    # add a new entry to the file.
    content = content +							\
              variable + "		=	" + assignment + "\n"
  else:
    # replace the value part with the given value.
    for match in matches:
      content = content.replace(match[0], match[1] + assignment + "\n")

  # finally, write back the content.
  env.push(env._PROFILE_USER_DIR_ + "/" + env._USER_ + ".conf",
           content, env.OPTION_NONE)



#
# init()
#
# this functions builds the variables database: variable, value, description
# etc.
#
def			init():
  # start to build the variables database by loading the configuration files.
  configuration()

  # complete the database with variable descriptions.
  description()
