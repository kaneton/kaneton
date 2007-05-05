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
# the 'e' variable represents the 'c_epita' checker main variable, the 'z'
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
	      "(?P<align>"						\
              "(?P<qual>extern|static)?(const|struct|enum|"		\
                  "unsigned|long|union|short| )*"			\
              "([a-zA-Z_][a-zA-Z0-9_]*)"				\
                "(\*| )*?"						\
                "\**"							\
              "(?P<name>[a-zA-Z_][a-zA-Z0-9_]*)(?P<spc1>\s*)"		\
              "\()"							\
                "(?P<args>"						\
                ".*)"							\
              "\)"							\
              "(?P<spc2>\s*);"

e_prototype = re.compile(s_prototype, re.MULTILINE | re.DOTALL)

#
# argument
#

s_a_argument = "^"							\
	       "(?P<a_var>"						\
	       "(?P<a_type>"						\
               "(?P<spc0> *))"						\
               "(?P<type>(const|struct|enum|unsigned|union| )*"		\
               "([a-zA-Z_][a-zA-Z0-9_]*\s*?)+)"				\
                 "(\**)"						\
               "(?P<spc1> *)"						\
                 ")"							\
               "(?P<name>\**([a-zA-Z_][a-zA-Z0-9_]*))?"			\
                 "(?:\[.*?\])*(?P<spc2>\s*)"				\
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
  group.add_option("--epita-prototypes-exclude",
                   action="append",
                   dest="epita_prototypes_exclude",
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

  p.code = error.code
  if not match:
    print p.code
    errors.add(e, error, errors.ERRORS_FATAL,
               "the prototype format is absolutely not understandable.\n")
    return None

  if match.group('qual') == 'static':
    p.type = PROTO_TYPE_STATIC
  else:
    p.type = PROTO_TYPE_EXTERN

  if match.group('name'):
    p.name = match.group('name')
  p.arguments = match.group('args')
  p.a_arguments = len(match.group('align'))
  if match.group('spc1'):
    errors.add(e, error, errors.ERRORS_STYLE,
	       "there MUST NOT be spaces between the function name"
	       " and the (\n")
  if match.group('spc2'):
    errors.add(e, error, errors.ERRORS_STYLE,
	       "there MUST NOT be spaces between the ) and ;\n")
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
  for match in e_a_argument.finditer(p.arguments):
    nlines += 1
    if match.group('spc2'):
      errors.add(e, error, errors.ERRORS_STYLE,
		 "%s : there MUST NOT be any spaces before the , or the"
		 ")\n" % p.name)
    if not match.group('spc1') and match.group('name'):
      errors.add(e, error, errors.ERRORS_STYLE,
		 "there MUST be spaces between a type and an identifier\n")

  nargs = len(p.arguments.split(","))

  for i in range(0, nargs - nlines):
    errors.add(e, error, errors.ERRORS_STYLE,
               "%s : one of the prototype's argument is not alone on its"
               " line.\n" % p.name)



#
# alignment()
#
# this function verifies the prototype arguments alignment.
#
def		alignment(e, z, p, error):
  match = None
  a_type = -1
  a_name = -1
  m = None

  for m in e_a_argument.finditer(p.arguments):
    if a_type == -1 and a_name == -1:
      a_type = p.a_arguments + len(m.group('a_type'))
      a_name = a_type + len(m.group('a_var'))
      continue

    if a_type != len(m.group('a_type')):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "prototype argument's type '" + m.group('type') +
                 "' is not aligned with the others.\n")

    if m.group('name') and a_name != len(m.group('a_var')):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "prototype argument's name '" + m.group('name') +
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

    if e.options.epita_prototypes_exclude:
      for x in e.options.epita_prototypes_exclude:
        if re.search(x, i, re.MULTILINE):
          launch = 0
          break

    if launch:
      prototype(e, z, i)
