##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/globals.py
##
## created       julien quintard   [fri sep  8 09:42:09 2006]
## updated       julien quintard   [sat sep 16 17:33:35 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file checks for the correctness of the global variable declarations.
#
# the 'e' variable represents the 'c_kaneton' checker main variable, the 'z'
# variable contains everything necessary for the globals checking while
# the 'p' variable describes the current global variable checking process.
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
# globals
#

class c_globals:
  def __init__(self):
    self.counter = 0

#
# global
#
GLOBAL_STATIC = 1
GLOBAL_EXTERN = 2

class c_global:
  def __init__(self):
    self.extern = None
    self.name = None
    self.value = None

#
# ----- regexp ----------------------------------------------------------------
#

#
# global
#

s_global = "^"								\
           "(?: *)"							\
           "(?:(extern)|static|const|struct|enum|"			\
             "unsigned|long|union| )*"					\
           "[a-zA-Z_][a-zA-Z0-9_]*"					\
             "\**"							\
           "[ \n]+"							\
             "\**"							\
           "([a-zA-Z_][a-zA-Z0-9_]*)"					\
             "(?:\[[0-9]*\])*"						\
             "(?:"							\
               "[ \n]*"							\
               "="							\
               "[ \n]*"							\
               "([^;]*)"						\
             ")?"							\
           ";"

e_global = re.compile(s_global, re.MULTILINE)

#
# name
#

s_name = "^g_.*$"

e_name = re.compile(s_name)

#
# call
#

s_call = "[a-zA-Z_][a-zA-Z0-9_]\("

e_call = re.compile(s_call)

#
# ----- functions -------------------------------------------------------------
#

#
# options()
#
# this function adds specific global variables options.
#
def		options(group):
  group.add_option("--kaneton-globals-exclude",
                   action="append",
                   dest="kaneton_globals_exclude",
                   metavar="PATTERN",
                   help="exclude global variables matching PATTERN from the "
                        "style checking")



#
# parse()
#
# this function tries to locate the different global variables parts.
#
def		parse(e, z, error):
  p = c_global()
  match = None

  match = e_global.search(error.code)

  if not match:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the global variable format is absolutely not "		\
               "understandable.\n")
    return None

  if match.group(1):
    p.extern = GLOBAL_EXTERN
  else:
    p.extern = GLOBAL_STATIC

  p.name = match.group(2)
  p.value = match.group(3)

  if p.extern == GLOBAL_STATIC:
    z.counter += 1

  return p



#
# name()
#
# this function verifies the global variable's name.
#
def		name(e, z, p, error):
  return



#
# initialization()
#
# this function verifies the correctness of the global variable
# initialization.
#
def		initialization(e, z, p, error):
  return



#
# global()
#
# this function launches the global variable tests.
#
def		globals(e, z, i):
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

  name(e, z, p, error)
  initialization(e, z, p, error)

  errors.close(e, error)

  errors.commit(e, error)



#
# check()
#
# this function checks every global variables of the list.
#
def		check(e):
  z = c_globals()
  launch = None
  counter = 0
  i = None
  x = None

  for i in e.parser.globals:
    launch = 1

    if e.options.kaneton_globals_exclude:
      for x in e.options.kaneton_globals_exclude:
        if re.search(x, i, re.MULTILINE):
          launch = 0
          break

    if launch:
      globals(e, z, i)
