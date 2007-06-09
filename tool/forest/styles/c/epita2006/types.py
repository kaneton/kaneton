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
import common

#
# ----- classes ---------------------------------------------------------------
#

#
# types
#

class CTypes:
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

class CType:
  def __init__(self):
    self.type = None
    self.definition = None
    self.body = None
    self.name = None
    self.align = None
    self.typedefed = False

#  ___
# | . \ ___  ___  ___ __   ___  ___
# |   // ._>/ . |/ ._>\ \/| . \<_-<
# |_\_\\___.\_. |\___./\_\|  _//__/
#           <___'         |_|
#

# typedef

s_type_complex = "^"                                                    \
                 "(?P<align>"                                           \
                 "(?P<lead_spce> *)"                                    \
                 "(?P<typedef>typedef\s+)?"                             \
                 "(?P<type>enum|struct|union)"                          \
                 "\s+)(?#End of the alignment)"                         \
                 "(?P<identifier>[\w_]+)?"                              \
                 "(?P<brace_spce>\s*)\{"                                \
                 "(?P<body>.*?)"                                        \
                 "( *\n)*(?P<align_end>(?P<end_spce> *)\}"              \
                 "\s*)(?P<new_identifier>[\w_]+)?\s*;"
e_type_complex = re.compile(s_type_complex, re.DOTALL)

s_type_simple = "^"                                                     \
                "typedef\s+"                                            \
                "(?P<base_type>union|struct|enum)?"                     \
                ".*?"                                                   \
                "(?P<new_identifier>[\w_]+)"                            \
                "(?P<incorrect_spaces>\s*);"
e_type_simple = re.compile(s_type_simple, re.DOTALL)

s_type_funcp = "^"                                                      \
               "typedef"                                                \
               ".*?"                                                    \
               "\(\*(?P<new_identifier>[\w_]+)\)\(.*\)\s*;"
e_type_funcp = re.compile(s_type_funcp, re.DOTALL)

# fields

s_struct_union_field = "(?P<align>(?P<type_align>^ *)"            \
                       "(?P<type>(?P<qualifier>[\w_]+\s+)*[\w_]+)\s+)"        \
                       "(?P<id>(\*[\*\s]*)?[\w_]+)\s*;(?P<end_spce> *\n?)"
e_struct_union_field = re.compile(s_struct_union_field, re.MULTILINE)

s_enum_field = "^(?P<field_align> *)(?P<id>[\w_]+)\s*,? *\n?"
e_enum_field = re.compile(s_enum_field, re.MULTILINE)

# structure

s_structure = "^s_.*$"

e_structure = re.compile(s_structure)

# enumeration

s_enumeration = "^e_.*$"

e_enumeration = re.compile(s_enumeration)

# union

s_union = "^u_.*$"

e_union = re.compile(s_union)

# function

s_function = "^f_.*$"

e_function = re.compile(s_function)

# basic

s_basic = "^t_.*$"

e_basic = re.compile(s_basic)





#  ___                 _    _
# | __>_ _ ._ _  ___ _| |_ <_> ___ ._ _  ___
# | _>| | || ' |/ | ' | |  | |/ . \| ' |<_-<
# |_| `___||_|_|\_|_. |_|  |_|\___/|_|_|/__/
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
  p = CType()
  m_complex = None

  m_complex = e_type_complex.search(error.code)
  if m_complex:
    if m_complex.group('type') == 'enum':
      p.type = TYPE_ENUMERATION
    elif m_complex.group('type') == 'struct':
      p.type = TYPE_STRUCTURE
    elif m_complex.group('type') == 'union':
      p.type = TYPE_UNION

    if m_complex.group('typedef'):        # Are we in a typedef statement ?
      p.name = m_complex.group('new_identifier')
      p.typedefed = True
    else:
      p.name = m_complex.group('identifier')

    if m_complex.group('identifier'):
      p.align = len(m_complex.group('align'))
    else:
      p.align = len(m_complex.group('align_end'))
#       type_error(e, error, p.name, "typedefed structs or enum MUST" +
#                  " have a name")

    # typedef declaration and alignment of the new type
    if p.typedefed and m_complex.group('identifier') and len(m_complex.group('align_end')) != p.align:
      type_error(e, error, p.name, "the type declaration \'" +
                 m_complex.group('new_identifier') + "\' MUST be " +
                 "aligned with \'" + m_complex.group('identifier') + "\'")

    p.body = m_complex.group('body')
    return p

  m_function = e_type_funcp.search(error.code)
  if m_function:
    p.type = TYPE_FUNCTION
    p.typedefed = True
    p.name = m_function.group('new_identifier')

  m_simple = e_type_simple.search(error.code)
  if m_simple:
    p.type = TYPE_BASIC
    if m_simple.group('base_type'):
      base = m_simple.group('base_type')
      if base == 'enum':
        p.type = TYPE_ENUMERATION
      elif base == 'union':
        p.type = TYPE_UNION
      elif base == 'struct':
        p.type = TYPE_STRUCTURE

    p.typedefed = True
    p.name = m_simple.group('new_identifier')

  return p


def type_error(e, error, typename, msg):
  errors.add(e, error, errors.ERRORS_STYLE,
             "In type " + typename + ": " +
             msg + "\n")


#
# fields()
#
# this function verifies the type's fields, if present, are correctly
# declared.
#
def		fields(e, z, p, error):

  if not p.body:
    return

  if p.type == TYPE_STRUCTURE or p.type == TYPE_UNION:
    type_align = -1
    for m_field in e_struct_union_field.finditer(p.body):
      if type_align == -1:
        type_align = len(m_field.group('type_align'))

      if len(m_field.group('type_align')) != type_align:
        type_error(e, error, p.name, "the type field \'" +
                   m_field.group('type') + "\' is not correctly" \
                   " aligned")

      if len(m_field.group('align')) != p.align:
        type_error(e, error, p.name, "the field \'" +
                   m_field.group('id') + "\' is not correctly" \
                   " aligned")

      common.check_id_validity(e, error, m_field.group('id'))

  if p.type == TYPE_ENUMERATION:
    field_align = -1
    for m_field in e_enum_field.finditer(p.body):
      if field_align == -1:
        field_align = len(m_field.group('field_align'))

      if len(m_field.group('field_align')) != field_align:
        type_error(e, error, p.name, "the enumeration field \'" +
                   m_field.group('id') + "\' is not correctly" \
                   " aligned")



#
# name()
#
# this function verifies the type name's correctness.
#
def		name(e, z, p, error):
  if p.type == TYPE_STRUCTURE:
    if not e_structure.search(p.name):
      type_error(e, error, p.name,
                 "structure aliased types must be prefixed by 's_'")
  elif p.type == TYPE_ENUMERATION:
    if not e_enumeration.search(p.name):
      type_error(e, error, p.name,
                 "enumeration aliased types must be prefixed by 'e_'")
  elif p.type == TYPE_UNION:
    if not e_union.search(p.name):
      type_error(e, error, p.name,
                 "union aliased types must be prefixed by 'u_'")
  elif p.type == TYPE_FUNCTION:
    if not e_function.search(p.name):
      type_error(e, error, p.name,
                 "function pointers aliased types must be prefixed by 'f_'")
  elif p.type == TYPE_BASIC:
    if not e_basic.search(p.name):
      type_error(e, error, p.name,
                 "basic types aliased types must be prefixed by 't_'")



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

  p = parse(e, z, error)
  if not p:
    errors.close(e, error)
    errors.commit(e, error)
    return

  if p.typedefed:
    name(e, z, p, error)

  fields(e, z, p, error)

  errors.close(e, error)

  errors.commit(e, error)



#
# check()
#
# this function checks every type of the list.
#
def		check(e):
  z = CTypes()
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
