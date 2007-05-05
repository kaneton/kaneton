##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/structures.py
##
## created       julien quintard   [tue sep  5 23:13:37 2006]
## updated       julien quintard   [sat sep 16 17:57:13 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file checks for the correctness of the structure declarations.
#
# the 'e' variable represents the 'c_epita' checker main variable, the 'z'
# variable contains everything necessary for the structures checking while
# the 'p' variable describes the current structure checking process.
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
# structures
#

class c_structures:
  def __init__(self):
    self.needless = None

#
# structure
#

class c_structure:
  def __init__(self):
    self.name = None
    self.body = None
    self.a_fields = None

#
# ----- regexp ----------------------------------------------------------------
#

#
# structure
#

#                "((?:\n|(?: +\})|[^\}])*)"				\
s_structure = "^"							\
              "( *)"							\
              "struct"							\
              "(\s+)"							\
              "([a-zA-Z_][\w_]*)"      \
              "\s*"							\
              "\{(?:\n|)"							\
              "(.*?)"                  \
              "\}\s*"
#              ";"

e_structure = re.compile(s_structure, re.MULTILINE | re.DOTALL)

#
# fields
#

s_a_fields = "^"							\
             "("							\
               "(?:extern|static|const|struct|enum|"			\
               "unsigned|long|union| )*"				\
               "[a-zA-Z_][a-zA-Z0-9_]*"					\
                 "\**"							\
               "[ \n]+"							\
             ")"							\
               "\**"							\
             "([a-zA-Z_][a-zA-Z0-9_]*)"					\
               "(?:\[[0-9]*\])*"					\
             "(?:;|,)"

e_a_fields = re.compile(s_a_fields, re.MULTILINE)

#
# fields
#

s_fields = "(?:"							\
             "(?:extern|static|const|struct|enum|"			\
                "unsigned|long|union| )*"				\
             "[a-zA-Z_][a-zA-Z0-9_]*"					\
               "\**"							\
             "[ \n]+"							\
           ")"								\
             "\**"							\
           "[a-zA-Z_][a-zA-Z0-9_]*"					\
             "(?:\[[0-9]*\])*"						\
           "("								\
             "(?:"							\
               "[ \n]*"							\
               ","							\
               "[ \n]*"							\
                 "\**"							\
               "[a-zA-Z_][a-zA-Z0-9_]*"					\
                 "(?:\[[0-9]*\])*"					\
             ")*"							\
           ")"								\
           "[ \n]*"							\
           ";"

e_fields = re.compile(s_fields, re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# options()
#
# this function adds specific options.
#
def		options(group):
  group.add_option("--epita-structures-exclude",
                   action="append",
                   dest="epita_structures_exclude",
                   metavar="PATTERN",
                   help="exclude structures matching PATTERN from the "
                        "style checking")



#
# scope()
#
# this function verifies the structure is defined at the right place.
#
def		scope(e, z, error):
  if e.parser.type != parsers.TYPE_HEADER:
    errors.add(e, error, errors.ERRORS_STYLE,
               "structures must be defined in header files.\n")



#
# parse()
#
# this function tries to locate the different structure definition parts.
#
def		parse(e, z, error):
  p = c_structure()
  match = None

  match = e_structure.search(error.code)

  if not match:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the structure format is absolutely not understandable.\n")
    print error.code
    return None

  if len(match.group(1)) != 0:
    errors.add(e, error, errors.ERRORS_WARNING,
               "the keyword 'struct' should be placed at the "		\
               "beginning of the line.\n")

  p.name = match.group(3)
  p.body = match.group(4)
  p.a_fields = len(match.group(1)) + len("struct") + len(match.group(2))

  return p



#
# fields()
#
# this function verifies the structure's fields are well declared.
#
def		fields(e, z, p, error):
  match = None
  nfields = 0
  nlines = 0
  m = None
  i = None

  match = e_a_fields.findall(p.body)
  if not match:
    return

  nlines = len(match)

  match = e_fields.findall(p.body)
  if not match:
    return

  for m in match:
    nfields += 1
    if m:
      nfields += len(m.lstrip(",").split(","))

  for i in range(0, nfields - nlines):
    errors.add(e, error, errors.ERRORS_STYLE,
               "one of the structure's fields is not alone on its line.\n")



#
# alignment()
#
# this function verifies the structure's fields alignment.
#
def		alignment(e, z, p, error):
  match = None
  m = None

  match = e_a_fields.findall(p.body)

  for m in match:
    if p.a_fields != len(m[0]):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "the structure definition's member '" + m[1] +		\
                 "' is not aligned with the others.\n")



#
# structure()
#
# this function launches all the structure tests.
#
def		structure(e, z, i):
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

  fields(e, z, p, error)
  alignment(e, z, p, error)

  errors.close(e, error)

  errors.commit(e, error)



#
# check()
#
# this function checks every structure of the list.
#
def		check(e):
  z = c_structures()
  launch = None
  i = None
  x = None

  for i in e.parser.structures:
    launch = 1

    if e.options.epita_structures_exclude:
      for x in e.options.epita_structures_exclude:
        if re.search(x, i, re.MULTILINE):
          launch = 0
          break

    if launch:
      structure(e, z, i)
