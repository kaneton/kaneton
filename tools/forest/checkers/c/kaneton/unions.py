##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/unions.py
##
## created       julien quintard   [thu sep  7 04:46:39 2006]
## updated       julien quintard   [sat sep 16 18:48:03 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file checks for the correctness of the type declarations.
#
# the 'e' variable represents the 'c_epita' checker main variable, the 'z'
# variable contains everything necessary for the unions checking while
# the 'p' variable describes the current union checking process.
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
# unions
#

class c_unions:
  def __init__(self):
    self.needless = None

#
# union
#

class c_union:
  def __init__(self):
    self.name = None
    self.body = None
    self.a_fields = None

#
# ----- regexp ----------------------------------------------------------------
#

#
# union
#

s_union = "^"								\
          "( *)"							\
          "union"							\
          "( +)"							\
          "([a-zA-Z_][a-zA-Z0-9_]*)"					\
          "\n"								\
          "^\{\n"							\
            "((?:\n|(?: +\})|[^\}])*)"					\
          "^\}"								\
          ";"

e_union = re.compile(s_union, re.MULTILINE)

#
# fields
#

s_a_field = "^"								\
            "("								\
              "(?:extern|static|const|struct|enum|"			\
              "unsigned|long|union| )*"					\
              "[a-zA-Z_][a-zA-Z0-9_]*"					\
                "\**"							\
              "[ \n]+"							\
            ")"								\
              "\**"							\
            "([a-zA-Z_][a-zA-Z0-9_]*)"					\
              "(?:\[[0-9]*\])*"						\
            "(?:;|,)"

e_a_field = re.compile(s_a_field, re.MULTILINE)

#
# fields
#

s_fields = "(?:"							\
             "(?:extern|static|const|struct|enum|"			\
             "unsigned|long|union| )*"					\
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
  group.add_option("--epita-unions-exclude",
                   action="append",
                   dest="epita_unions_exclude",
                   metavar="PATTERN",
                   help="exclude unions matching PATTERN from the "
                        "style checking")



#
# scope()
#
# this function verifies the union is declared at the right place.
#
def		scope(e, z, error):
  if e.parser.type != parsers.TYPE_HEADER:
    errors.add(e, error, errors.ERRORS_STYLE,
               "unions must be defined in header files.\n")



#
# parse()
#
# this function tries to locate the different union's parts.
#
def		parse(e, z, error):
  p = c_union()
  match = None

  match = e_union.search(error.code)

  if not match:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the union format is absolutely not understandable.\n")
    return None

  if len(match.group(1)) != 0:
    errors.add(e, error, errors.ERRORS_WARNING,
               "the keyword 'union' should be placed at the "		\
               "beginning of the line.\n")

  p.name = match.group(3)
  p.body = match.group(4)
  p.a_fields = len(match.group(1)) + len("union") + len(match.group(2))

  return p



#
# fields()
#
# this function verifies the union's fields are well placed.
#
def		fields(e, z, p, error):
  match = None
  nfields = 0
  nlines = 0
  m = None
  i = None

  match = e_a_field.findall(p.body)
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
               "one of the union's fields is not alone on its line.\n")



#
# alignment()
#
# this function verifies the union's argument alignment.
#
def		alignment(e, z, p, error):
  match = None
  m = None

  match = e_a_field.findall(p.body)

  for m in match:
    if p.a_fields != len(m[0]):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "the union definition's member '" + m[1] +		\
                 "' is not aligned with the others.\n")



#
# union()
#
# this function launches the union's tests.
#
def		union(e, z, i):
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
# this function checks every union of the list.
#
def		check(e):
  z = c_unions()
  launch = None
  i = None
  x = None

  for i in e.parser.unions:
    launch = 1

    if e.options.epita_unions_exclude:
      for x in e.options.epita_unions_exclude:
        if re.search(x, i, re.MULTILINE):
          launch = 0
          break

    if launch:
      union(e, z, i)
