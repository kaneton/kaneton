##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/enumerations.py
##
## created       julien quintard   [thu sep  7 04:46:39 2006]
## updated       julien quintard   [sat sep 16 17:21:52 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file checks for the correctness of the enumeration definitions.
#
# the 'e' variable represents the 'c_epita' checker main variable, the 'z'
# variable contains everything necessary for the enumerations checking while
# the 'p' variable describes the current enumeration checking process.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parsers

import checker
import classes
import errors

#
# ----- classes ---------------------------------------------------------------
#

#
# enumerations
#

class c_enumerations:
  def __init__(self):
    self.needless = None

#
# enumeration
#

class c_enumeration:
  def __init__(self):
    self.name = None
    self.body = None
    self.a_values = None

#
# ----- regexp ----------------------------------------------------------------
#

#
# enumeration
#

s_enumeration = "^"							\
                "( *)"							\
                "enum"							\
                "( +)"							\
                "([a-zA-Z_][a-zA-Z0-9_]*)"				\
                "[ \n]*"						\
                "\{"							\
                  "(.*)"						\
                "\}"
#                ";"

e_enumeration = re.compile(s_enumeration, re.MULTILINE)

#
# values
#

s_a_values = "^"							\
             "( *)"							\
             "([a-zA-Z_][a-zA-Z0-9_]*)"					\
             "(?:,|[ \n]*)"

e_a_values = re.compile(s_a_values, re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# options()
#
# this function adds specific options.
#
def		options(group):
  group.add_option("--epita-enumerations-exclude",
                   action="append",
                   dest="epita_enumerations_exclude",
                   metavar="PATTERN",
                   help="exclude enumerations matching PATTERN from the "
                        "style checking")



#
# scope()
#
# this function verifies that enumeration definitions are well declared
# in a header file.
#
def		scope(e, z, error):
  if e.parser.type != parsers.TYPE_HEADER:
    errors.add(e, error, errors.ERRORS_STYLE,
               "enumerations must be defined in header files.\n")



#
# parse()
#
# this function tries to locate the different enumeration definition
# parts.
#
def		parse(e, z, error):
  p = c_enumeration()
  match = None

  match = e_enumeration.search(error.code)

  if not match:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the enumeration format is absolutely not understandable.\n")
    return None

  if len(match.group(1)) != 0:
    errors.add(e, error, errors.ERRORS_WARNING,
               "the keyword 'enum' should be placed at the "		\
               "beginning of the line.\n")

  p.name = match.group(3)
  p.body = match.group(4)
  p.a_values = len(match.group(1)) + len("enum") + len(match.group(2))

  return p



#
# values()
#
# this function verifies the enumeration values are correctly placed.
#
def		values(e, z, p, error):
  match = None
  nvalues = 0
  nlines = 0
  m = None
  i = None

  match = e_a_values.findall(p.body)
  if not match:
    return

  nlines = len(match)

  nvalues = len(p.body.split(","))

  for i in range(0, nvalues - nlines):
    errors.add(e, error, errors.ERRORS_STYLE,
               "one of the enumeration's values is not alone on its line.\n")



#
# enumeration()
#
# this function launches the differents tests.
#
def		enumeration(e, z, i):
  error = classes.c_error(i)
  p = None

  errors.open(e, error)

  scope(e, z, error)

  checker.trailings(e, error)
  checker.columns(e, error)

  p = parse(e, z, error)
  if not p:
    errors.close(e, error)
    errors.commit(e, error)
    return

  values(e, z, p, error)

  errors.close(e, error)

  errors.commit(e, error)



#
# check()
#
# this function checks every enumeration of the list.
#
def		check(e):
  z = c_enumerations()
  launch = None
  i = None
  x = None

  for i in e.parser.enumerations:
    launch = 1

    if e.options.epita_enumerations_exclude:
      for x in e.options.epita_enumerations_exclude:
        if re.search(x, i, re.MULTILINE):
          launch = 0
          break

    if launch:
      enumeration(e, z, i)
