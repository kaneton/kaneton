##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/macros.py
##
## created       julien quintard   [fri sep  8 10:43:53 2006]
## updated       julien quintard   [sat sep 16 17:46:38 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file checks for the correctness of the macro declarations.
#
# the 'e' variable represents the 'c_kaneton' checker main variable, the 'z'
# variable contains everything necessary for the macros checking while
# the 'p' variable describes the current macro checking process.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import checker
import classes
import errors

#
# ----- classes ---------------------------------------------------------------
#

#
# macros
#

class c_macros:
  def __init__(self):
    self.indentation = 0

#
# macro
#

MACRO_IF = 1
MACRO_ELSE = 2
MACRO_ENDIF = 3
MACRO_DEFINE = 4
MACRO_UNDEF = 5
MACRO_IFDEF = 6
MACRO_IFNDEF = 7
MACRO_INCLUDE = 8

class c_macro:
  def __init__(self):
    self.indentation = None
    self.type = None
    self.name = None
    self.arguments = None
    self.body = None

#
# ----- regexp ----------------------------------------------------------------
#

#
# macro
#

s_macro = "^"								\
          "( *)"							\
          "#"								\
          "( *)"							\
          "(ifndef|ifdef|endif|define|undef|include|if|else)"		\
          "(?:"								\
            "( +)"							\
              "("							\
                "((?:\"|<)[a-zA-Z_./ ]*(?:\"|>))"			\
                  "|"							\
                "(?:"							\
                  "([a-zA-Z_][a-zA-Z0-9_]*)"				\
                  "(?:"							\
                    "\("						\
                    "([^\)]*)"						\
                    "\)"						\
                  ")?"							\
                  "(?:"							\
                    " +"						\
                    "((?:(?:\\\\\n)|[^\n])*)"				\
                  ")?"							\
                ")"							\
              ")"							\
          ")?"								\
          "\n"

e_macro = re.compile(s_macro, re.MULTILINE)

#
# breakers
#

s_a_breakers = "^(.*)\\\\\n"

e_a_breakers = re.compile(s_a_breakers, re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# options()
#
# this function adds macro specific options.
#
def		options(group):
  group.add_option("--kaneton-macros-exclude",
                   action="append",
                   dest="kaneton_macros_exclude",
                   metavar="PATTERN",
                   help="exclude macros matching PATTERN from the "
                        "style checking")



#
# parse()
#
# this function tries to locate the different parts of the macro.
#
def		parse(e, z, error):
  p = c_macro()
  match = None

  match = e_macro.search(error.code)

  if not match:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the macro format is absolutely not understandable.\n")
    return None

  if len(match.group(1)) != 0:
    errors.add(e, error, errors.ERRORS_STYLE,
               "the '#' character must be placed at the beginning "	\
               "of the line.\n")

  p.indentation = len(match.group(2))

  if match.group(3) == "if":
    p.type = MACRO_IF
  elif match.group(3) == "else":
    p.type = MACRO_ELSE
  elif match.group(3) == "endif":
    p.type = MACRO_ENDIF
  elif match.group(3) == "define":
    p.type = MACRO_DEFINE
  elif match.group(3) == "undef":
    p.type = MACRO_UNDEF
  elif match.group(3) == "ifdef":
    p.type = MACRO_IFDEF
  elif match.group(3) == "ifndef":
    p.type = MACRO_IFNDEF
  elif match.group(3) == "include":
    p.type = MACRO_INCLUDE

  if p.type == MACRO_INCLUDE:
    p.name = match.group(6)
  elif p.type == MACRO_DEFINE or p.type == MACRO_UNDEF or		\
       p.type == MACRO_IFDEF or p.type == MACRO_IFNDEF:
    p.name = match.group(7)
  else:
    p.name = None

  if p.type == MACRO_DEFINE:
    p.arguments = match.group(8)
    p.body = match.group(9)
  else:
    p.arguments = None
    p.body = None

  return p



#
# alignment()
#
# this function verifies the correct alignments: lines breakers and
# indentation.
#
def		alignment(e, z, p, error):
  alignment = -1
  match = None
  m = None

  if p.type == MACRO_ENDIF:
    z.indentation = z.indentation - 1

  if p.type == MACRO_IFNDEF or p.type == MACRO_IFDEF or p.type == MACRO_IF:
    z.indentation = z.indentation + 1

  match = e_a_breakers.findall(error.code)

  for m in match:
    if alignment == -1:
      alignment = len(m)

    if alignment != len(m):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "one of the line breakers is not aligned.\n")



#
# name()
#
# this function verifies that the macro name is correct.
#
def		name(e, z, p, error):
  args = None
  a = None

  if not p.type == MACRO_DEFINE and not p.type == MACRO_UNDEF and	\
     not p.type == MACRO_IFDEF and not p.type == MACRO_IFNDEF:
    return



#
# macro()
#
# this function launches the macro tests.
#
def		macro(e, z, i):
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

  alignment(e, z, p, error)
  name(e, z, p, error)

  errors.close(e, error)

  errors.commit(e, error)



#
# check()
#
# this function checks every macro of the list.
#
def		check(e):
  z = c_macros()
  launch = None
  i = None
  x = None

  for i in e.parser.macros:
    launch = 1

    if e.options.kaneton_macros_exclude:
      for x in e.options.kaneton_macros_exclude:
        if re.search(x, i, re.MULTILINE):
          launch = 0
          break

    if launch:
      macro(e, z, i)