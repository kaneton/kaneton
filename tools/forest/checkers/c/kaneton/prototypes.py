##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/prototypes.py
##
## created       julien quintard   [fri sep  8 12:10:48 2006]
## updated       julien quintard   [sat sep 16 19:53:19 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file checks for the correctness of the prototypes declarations.
#
# the 'e' variable represents the 'c_kaneton' checker main variable, the 'z'
# variable contains everything necessary for the prototypes checking while
# the 'p' variable describes the current prototype checking process.
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
# prototypes
#

class c_prototypes:
  def __init__(self):
    self.needless = None

#
# prototype
#
PROTO_TYPE_STATIC = 1
PROTO_TYPE_EXTERN = 2

class c_prototype:
  def __init__(self):
    self.type = None
    self.name = None
    self.arguments = None
    self.a_arguments = None

#
# ----- regexp ----------------------------------------------------------------
#

#
# prototype
#

s_prototype = "^"							\
              "((?:extern|(static)|const|struct|enum|"			\
                  "unsigned|long|union| )*)"				\
              "([a-zA-Z_][a-zA-Z0-9_]*)"				\
                "(\**)"							\
              "( +)"							\
                "(\**)"							\
              "([a-zA-Z_][a-zA-Z0-9_]*)"				\
              "\("							\
                "("							\
                  "(?:"							\
                    "[^\)]*"						\
                  ")"							\
                    "|"							\
                  "(?:void)"						\
                ")"							\
              "\)"							\
              ";"

e_prototype = re.compile(s_prototype, re.MULTILINE)

#
# argument
#

s_a_argument = "^"							\
               "( *)"							\
               "((?:const|struct|enum|unsigned|long|union| )*)"		\
               "([a-zA-Z_][a-zA-Z0-9_]*)"				\
                 "(\**)"						\
               "( +)"							\
                 "\**"							\
               "([a-zA-Z_][a-zA-Z0-9_]*)"				\
                 "(?:\[[0-9]*\])*"					\
               "(?:,|$)"

e_a_argument = re.compile(s_a_argument, re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# options()
#
# this function adds specific prototypes options.
#
def		options(group):
  group.add_option("--kaneton-prototypes-exclude",
                   action="append",
                   dest="kaneton_prototypes_exclude",
                   metavar="PATTERN",
                   help="exclude prototypes matching PATTERN from the "
                        "style checking")



#
# scope()
#
# this functions verifies the prototypes are declared at the right place.
#
def		scope(e, z, p, error):
  if p.type == PROTO_TYPE_EXTERN:
    if e.parser.type != parsers.TYPE_HEADER:
      errors.add(e, error, errors.ERRORS_STYLE,
                 "prototypes must be defined in header files.\n")



#
# parse()
#
# this function tries to locate the different prototypes parts.
#
def		parse(e, z, error):
  p = c_prototype()
  match = None

  match = e_prototype.search(error.code)

  if not match:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the prototype format is absolutely not understandable.\n")
    return None

  if match.group(2):
    p.type = PROTO_TYPE_STATIC
  else:
    p.type = PROTO_TYPE_EXTERN

  p.name = match.group(7)
  p.arguments = match.group(8)
  p.a_arguments = len(match.group(1)) + len(match.group(3)) +		\
                  len(match.group(4)) + len(match.group(5)) +		\
                  len(match.group(6)) + len(match.group(7)) + len("(")

  return p



#
# arguments()
#
# this function verifies the prototype arguments are well placed.
#
def		arguments(e, z, p, error):
  match = None
  nlines = 0
  nargs = 0
  i = None

  match = e_a_argument.findall(p.arguments)
  if not match:
    return

  nlines = len(match)

  nargs = len(p.arguments.split(","))

  for i in range(0, nargs - nlines):
    errors.add(e, error, errors.ERRORS_STYLE,
               "one of the prototype's argument is not alone on its line.\n")



#
# alignment()
#
# this function verifies the prototype arguments alignment.
#
def		alignment(e, z, p, error):
  match = None
  type = -1
  name = -1
  m = None

  match = e_a_argument.findall(p.arguments)

  for m in match:
    if type == -1 and name == -1:
      type = p.a_arguments + len(m[0])
      name = type + len(m[1]) + len(m[2]) + len(m[3]) + len(m[4])
      continue

    if type != len(m[0]):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "prototype argument's type '" + m[2] +			\
                 "' is not aligned with the others.\n")

    if name != (len(m[0]) + len(m[1]) + len(m[2]) + len(m[3]) + len(m[4])):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "prototype argument's name '" + m[5] +			\
                 "' is not aligned with the others.\n")



#
# prototype()
#
# this function launches the prototype tests.
#
def		prototype(e, z, i):
  error = classes.c_error(i)
  p = None

  errors.open(e, error)

  checker.trailings(e, error)
  checker.columns(e, error)

  p = parse(e, z, error)
  if not p:
    errors.close(e, error)
    errors.commit(e, error)
    return

  scope(e, z, p, error)

  arguments(e, z, p, error)
  alignment(e, z, p, error)

  errors.close(e, error)

  errors.commit(e, error)



#
# check()
#
# this function checks every prototype of the list.
#
def		check(e):
  z = c_prototypes()
  launch = None
  i = None
  x = None

  for i in e.parser.prototypes:
    launch = 1

    if e.options.kaneton_prototypes_exclude:
      for x in e.options.kaneton_prototypes_exclude:
        if re.search(x, i, re.MULTILINE):
          launch = 0
          break

    if launch:
      prototype(e, z, i)
