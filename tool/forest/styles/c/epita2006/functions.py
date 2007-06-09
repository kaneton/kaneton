##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/functions.py
##
## created       julien quintard   [sat sep  9 21:35:58 2006]
## updated       julien quintard   [sun sep 17 09:37:07 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file checks for the correctness of the function declarations.
#
# the 'e' variable represents the 'c_epita' checker main variable, the 'z'
# variable contains everything necessary for the functions checking while
# the 'p' variable describes the current function checking process.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parsers

import checker
import classes
import errors

import functions_parser
import functions_checker

#
# ----- constants -------------------------------------------------------------
#

FUNCTIONS_EXPORT_LIMIT = 5
FUNCTIONS_FILE_LIMIT = 10

FUNCTIONS_LINE_LIMIT = 25
FUNCTIONS_LINE_EXTREME_LIMIT = 42

FUNCTIONS_ARGUMENT_LIMIT = 4


#
# ----- classes ---------------------------------------------------------------
#

#
# functions
#

class c_functions: # z
  def __init__(self):
    self.locals = 0
    self.exports = 0

#
# function
#

class c_function: # p
  def __init__(self):
    self.name = None
    self.arguments = None
    self.declarations = None
    self.body = None
    self.a_arguments = None
    self.a_declarations = None
    self.fors = None
    self.whiles = None
    self.dos = None
    self.switchs = None
    self.ifs = None
    self.returns = None
    self.breaks = None
    self.continues = None
    self.gotos = None

#
# ----- functions -------------------------------------------------------------
#

#
# options()
#
# this function adds specific options.
#
def		options(group):
  group.add_option("--c-functions-exclude",
                   action="append",
                   dest="epita_functions_exclude",
                   metavar="PATTERN",
                   help="exclude functions matching PATTERN from the "
                        "style checking")
  group.add_option("--c-functions-cast-ignore",
                   action="append",
                   dest="cast_ignore",
                   metavar="PATTERN",
                   help="exclude functions matching PATTERN from the "
                        "return type cast checking")


#
# scope()
#
# this function verifies the function si declared at the right place.
#
def		scope(e, z, error):
  if e.parser.type != parsers.TYPE_SOURCE:
    errors.add(e, error, errors.ERRORS_STYLE,
               "functions must be defined in header files.\n")



#
# function()
#
# this function launches the function tests.
#
def		function(e, z, i):
  error = classes.c_error(i)
  p = None

  errors.open(e, error)

  scope(e, z, error)

  p = functions_parser.parse(e, z, error)
  if not p:
    errors.close(e, error)
    errors.commit(e, error)
    return

  functions_checker.check(e, z, p, error)

  errors.close(e, error)

  errors.commit(e, error)
  error.name = None



#
# check()
#
# this function checks every function of the list.
#
def		check(e):
  z = c_functions()
  launch = None
  i = None
  x = None

  for i in e.parser.functions:
    launch = 1
    if e.options.epita_functions_exclude:
      for x in e.options.epita_functions_exclude:
        if re.search(x, i, re.MULTILINE):
          launch = 0
          break

    if launch:
      function(e, z, i)
