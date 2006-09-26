##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/types.py
##
## created       julien quintard   [thu sep  7 06:59:19 2006]
## updated       julien quintard   [sat sep 16 18:13:04 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file checks for the correctness of the type declarations.
#
# the 'e' variable represents the 'c_epita' checker main variable, the 'z'
# variable contains everything necessary for the types checking while
# the 'p' variable describes the current type checking process.
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
# types
#

class c_types:
  def __init__(self):
    self.needless = None

#
# type
#
TYPE_STRUCTURE = 1
TYPE_ENUMERATION = 2
TYPE_UNION = 3
TYPE_BASIC = 4
TYPE_FUNCTION = 5

class c_type:
  def __init__(self):
    self.type = None
    self.definition = None
    self.body = None
    self.name = None
    self.a_members = None

#
# ----- regexp ----------------------------------------------------------------
#

#
# typedef
#

s_type = "^"								\
         "( *)"								\
         "typedef"							\
         "([ \n]+)"							\
         "("								\
           "("								\
             "(struct|enum|union)"					\
             "( *)"							\
             "(?:"							\
               "("							\
                 "(( +)([a-zA-Z_][a-zA-Z0-9_]*))?"			\
                 "\n"							\
                 "^\{\n"						\
                   "((?:\n|(?: +\})|[^\}])*)"				\
                 "^\}"							\
               ")"							\
                 "|"							\
               "("							\
                 "(( +)([a-zA-Z_][a-zA-Z0-9_]*))"			\
               ")"							\
             ")"							\
           ")"								\
             "|"							\
           "("								\
             "(const|unsigned|long|static| )*"				\
             "([a-zA-Z_][a-zA-Z0-9_]*)"					\
               "\**"							\
           ")"								\
         ")"								\
         "([ \n]+)"							\
         "("								\
           "("								\
             "([a-zA-Z_][a-zA-Z0-9_]*)"					\
               "(?:\[[0-9]*\])?"					\
           ")"								\
             "|"							\
           "("								\
             "\("							\
             "\*"							\
             "([a-zA-Z_][a-zA-Z0-9_]*)"					\
             "\)"							\
             "\("							\
             "[^;]*"							\
           ")"								\
         ")"								\
         ";"

e_type = re.compile(s_type, re.MULTILINE)

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
# values
#

s_a_values = "^"							\
             "( *)"							\
             "([a-zA-Z_][a-zA-Z0-9_]*)"					\
             "(?:,|[ \n]*)"

e_a_values = re.compile(s_a_values, re.MULTILINE)

#
# structure
#

s_structure = "^s_.*$"

e_structure = re.compile(s_structure)

#
# enumeration
#

s_enumeration = "^e_.*$"

e_enumeration = re.compile(s_enumeration)

#
# union
#

s_union = "^u_.*$"

e_union = re.compile(s_union)

#
# function
#

s_function = "^f_.*$"

e_function = re.compile(s_function)

#
# basic
#

s_basic = "^t_.*$"

e_basic = re.compile(s_basic)

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
  group.add_option("--epita-types-exclude",
                   action="append",
                   dest="epita_types_exclude",
                   metavar="PATTERN",
                   help="exclude types matching PATTERN from the "
                        "style checking")



#
# scope()
#
# this function verifies the type declaration is at the right place.
#
def		scope(e, z, error):
  if e.parser.type != parsers.TYPE_HEADER:
    errors.add(e, error, errors.ERRORS_STYLE,
               "types must be defined in header files.\n")



#
# parse()
#
# this function tries to locate the different type declaration parts.
#
def		parse(e, z, error):
  p = c_type()
  match = None

  match = e_type.search(error.code)

  if not match:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the type format is absolutely not understandable.\n")
    return None

  if len(match.group(1)) != 0:
    errors.add(e, error, errors.ERRORS_WARNING,
               "the keyword 'typedef' should be placed at the "		\
               "beginning of the line.\n")

  if match.group(4):
    if match.group(5) == "struct":
      p.type = TYPE_STRUCTURE
    elif match.group(5) == "enum":
      p.type = TYPE_ENUMERATION
    elif match.group(5) == "union":
      p.type = TYPE_UNION
  elif match.group(23):
    p.type = TYPE_FUNCTION
  else:
    p.type = TYPE_BASIC

  if p.type == TYPE_STRUCTURE or					\
     p.type == TYPE_ENUMERATION or					\
     p.type == TYPE_UNION:
    p.definition = match.group(4)
    p.name = match.group(19) + match.group(21)

    if match.group(7):
      p.body = match.group(11)
    else:
      p.body = None
  elif p.type == TYPE_BASIC:
    p.definition = match.group(16)
    p.name = match.group(19) + match.group(21)
    p.body = None
  elif p.type == TYPE_FUNCTION:
    p.definition = match.group(3) + match.group(19) + match.group(20)
    p.name = match.group(24)
    p.body = None

  if p.body:
    if match.group(10):
      p.members = len(match.group(1)) + len("typedef") +		\
                  len(match.group(2)) + len(match.group(5)) +		\
                  len(match.group(6)) + len(match.group(9))
    else:
      p.members = len("}") + len(match.group(19))
  else:
    p.members = None

  return p



#
# fields()
#
# this function verifies the type's fields, if present, are correctly
# declared.
#
def		fields(e, z, p, error):
  match = None
  nfields = 0
  nlines = 0
  m = None
  i = None

  if not p.body:
    return

  if p.type == TYPE_STRUCTURE or p.type == TYPE_UNION:
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
                 "one of the type's fields is not alone on its line.\n")

  elif p.type == TYPE_ENUMERATION:
    match = e_a_values.findall(p.body)
    if not match:
      return

    nlines = len(match)

    nfields = len(p.body.split(","))

    for i in range(0, nfields - nlines):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "one of the type's values is not alone on its line.\n")



#
# alignment()
#
# this function verifies the correct alignment of the type's fields, if
# present.
#
def		alignment(e, z, p, error):
  match = None

  if not p.body:
    return

  if p.members != (len("}") + (len(p.name) - len(p.name.lstrip()))):
    errors.add(e, error, errors.ERRORS_STYLE,
               "the type name '" + p.name.lstrip() +			\
               "' is not aligned with the definition's members.\n")

  if p.type == TYPE_STRUCTURE or p.type == TYPE_UNION:
    match = e_a_fields.findall(p.body)

    for m in match:
      if p.members != len(m[0]):
        errors.add(e, error, errors.ERRORS_STYLE,
                   "the type definition's member '" + m[1] +		\
                   "' is not aligned with the others.\n")

  elif p.type == TYPE_ENUMERATION:
    match = e_a_values.findall(p.body)

    for m in match:
      if p.members != len(m[0]):
        errors.add(e, error, errors.ERRORS_STYLE,
                   "the type definition's member '" + m[1] +		\
                   "' is not aligned with the others.\n")



#
# name()
#
# this function verifies the type name's correctness.
#
def		name(e, z, p, error):
  if p.type == TYPE_STRUCTURE:
    if not e_structure.search(p.name.lstrip()):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "structure aliased types must be prefixed by 's_'.\n")
  elif p.type == TYPE_ENUMERATION:
    if not e_enumeration.search(p.name.lstrip()):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "enumeration aliased types must be prefixed by 'e_'.\n")
  elif p.type == TYPE_UNION:
    if not e_union.search(p.name.lstrip()):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "union aliased types must be prefixed by 'u_'.\n")
  elif p.type == TYPE_FUNCTION:
    if not e_function.search(p.name.lstrip()):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "function pointers aliased types must be prefixed by 'f_'.\n")
  elif p.type == TYPE_BASIC:
    if not e_basic.search(p.name.lstrip()):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "basic types aliased types must be prefixed by 't_'.\n")



#
# type()
#
# this function launches the type's tests.
#
def		type(e, z, i):
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
  name(e, z, p, error)

  errors.close(e, error)

  errors.commit(e, error)



#
# check()
#
# this function checks every type of the list.
#
def		check(e):
  z = c_types()
  launch = None
  i = None
  x = None

  for i in e.parser.types:
    launch = 1

    if e.options.epita_types_exclude:
      for x in e.options.epita_types_exclude:
        if re.search(x, i, re.MULTILINE):
          launch = 0
          break

    if launch:
      type(e, z, i)
