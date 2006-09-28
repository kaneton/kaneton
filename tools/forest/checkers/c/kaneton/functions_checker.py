##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/functions_checker.py
##
## created       julien quintard   [sat sep  9 21:35:58 2006]
## updated       julien quintard   [sun sep 17 09:38:41 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to verify the correctness of the different function's parts.
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

#
# ----- regexp ----------------------------------------------------------------
#

#
# declarations
#

s_declarations = "(?:"							\
                   "(?:extern|static|const|struct|enum|"		\
                      "unsigned|long|union| )*"				\
                   "[a-zA-Z_][a-zA-Z0-9_]*"				\
                     "\**"						\
                   "[ \n]+"						\
                 ")"							\
                   "\**"						\
                 "[a-zA-Z_][a-zA-Z0-9_]*"				\
                   "(?:\[[0-9]*\])*"					\
                 "[^,;]*"						\
                 "("							\
                   "(?:"						\
                     "[ \n]*"						\
                     ","						\
                     "[ \n]*"						\
                       "\**"						\
                     "[a-zA-Z_][a-zA-Z0-9_]*"				\
                       "(?:\[[0-9]*\])*"				\
                     "[^,;]*"						\
                   ")*"							\
                 ")"							\
                 "[ \n]*"						\
                 ";"

e_declarations = re.compile(s_declarations, re.MULTILINE)

#
# lines
#

s_lines = "^\n"

e_lines = re.compile(s_lines, re.MULTILINE)

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
# declarations
#

s_a_declarations = "^"							\
                   "("							\
                     "(?:extern|static|const|struct|enum|"		\
                     "unsigned|long|union| )*"				\
                     "[a-zA-Z_][a-zA-Z0-9_]*"				\
                       "\**"						\
                     "[ \n]+"						\
                   ")"							\
                     "\**"						\
                   "([a-zA-Z_][a-zA-Z0-9_]*)"				\
                     "(?:\[[0-9]*\])*"					\
                   "[^;,]*"						\
                   "(?:;|,)"

e_a_declarations = re.compile(s_a_declarations, re.MULTILINE)

#
# call
#

s_call = "(?:"								\
           "("								\
             "(for|while|switch|return|sizeof|if)"			\
             "([ \n]*)"							\
             "(?:\(|;)"							\
           ")"								\
             "|"							\
           "("								\
             "([a-zA-Z_][a-zA-Z0-9_]*)"					\
             "([ \n]*)"							\
             "\("							\
           ")"								\
         ")"

e_call = re.compile(s_call)

#
# ----- functions -------------------------------------------------------------
#

# XXX

#
# lines()
#
def		lines(g, p, error):
  nlines = None
  d = None
  b = None

  d = e_lines.sub("", p.declarations).rstrip("\n")
  b = e_lines.sub("", p.body).rstrip("\n")

  nlines = len(d.split("\n")) + len(b.split("\n"))

  if nlines > FUNCTIONS_LINE_EXTREME_LIMIT:
    errors.add(error, errors.ERRORS_FATAL,
               "the function contains more than " +			\
               str(FUNCTIONS_LINE_EXTREME_LIMIT) + " lines. are you "	\
               "kidding us?\n")
  elif nlines > FUNCTIONS_LINE_LIMIT:
    errors.add(error, errors.ERRORS_STYLE,
               "the function contains more than " +			\
               str(FUNCTIONS_LINE_LIMIT) + " lines.\n")



#
# arguments_format()
#
def		arguments_format(g, p, error):
  match = None
  nlines = 0
  nargs = 0
  i = None

  match = e_a_argument.findall(p.arguments)
  if not match:
    return

  nlines = len(match)

  nargs = len(p.arguments.split(","))

  if nargs > FUNCTIONS_ARGUMENT_LIMIT:
    errors.add(error, errors.ERRORS_STYLE,
               "the function contains more than " +			\
               str(FUNCTIONS_ARGUMENT_LIMIT) + " arguments.\n")

  for i in range(0, nargs - nlines):
    errors.add(error, errors.ERRORS_STYLE,
               "one of the function's argument is not alone on its line.\n")



#
# arguments_alignment()
#
def		arguments_alignment(g, p, error):
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
      errors.add(error, errors.ERRORS_STYLE,
                 "function argument's type '" + m[2] +			\
                 "' is not aligned with the others.\n")

    if name != (len(m[0]) + len(m[1]) + len(m[2]) + len(m[3]) + len(m[4])):
      errors.add(error, errors.ERRORS_STYLE,
                 "function argument's name '" + m[5] +			\
                 "' is not aligned with the others.\n")



#
# arguments()
#
def		arguments(g, p, error):
  arguments_format(g, p, error)
  arguments_alignment(g, p, error)



#
# declarations_blanks()
#
def		declarations_blanks(g, p, error):
  declarations = None

  if p.declarations[len(p.declarations) - 2] != '\n':
    errors.add(error, errors.ERRORS_STYLE,
               "the declaration block and the function's body must "	\
               "be separated by a blank line.\n")

  if p.declarations[len(p.declarations) - 3] == '\n':
    errors.add(error, errors.ERRORS_STYLE,
               "the declaration block and the function's body must "	\
               "be separated by a single blank line.\n")

  declarations = e_lines.sub("", p.declarations)

  if p.declarations.rstrip("\n") != declarations.rstrip("\n"):
    errors.add(error, errors.ERRORS_STYLE,
               "the declaration block must not contain blank lines.\n")
    p.declarations = declarations;



#
# declarations_count()
#
def		declarations_format(g, p, error):
  match = None
  nlines = 0
  ndecs = 0
  m = None
  i = None

  match = e_a_declarations.findall(p.declarations)
  if not match:
    return

  nlines = len(match)

  match = e_declarations.findall(p.declarations)
  if not match:
    return

  for m in match:
    ndecs += 1
    if m:
      ndecs += len(m.lstrip(",").split(","))

  for i in range(0, ndecs - nlines):
    errors.add(error, errors.ERRORS_STYLE,
               "one of the declaration is not alone on its line.\n")



#
# declarations_alignment()
#
def		declarations_alignment(g, p, error):
  match = None
  type = -1
  name = -1
  m = None

  match = e_a_declarations.findall(p.declarations)

  for m in match:
    if p.a_declarations != len(m[0]):
      errors.add(error, errors.ERRORS_STYLE,
                 "the block declaration's member '" + m[1] +		\
                 "' is not aligned with the function name.\n")



#
# declarations_init()
#
def		declarations_init(g, p, error):
  match = None
  m = None

  match = e_call.findall(p.declarations)
  if not match:
    return

  for m in match:
    errors.add(error, errors.ERRORS_STYLE,
               "local variables must not be initialized by function "	\
               "or macro-function calls.\n")



#
# declarations()
#
def		declarations(g, p, error):
  if not p.declarations:
    return

  declarations_blanks(g, p, error)
  declarations_format(g, p, error)
  declarations_alignment(g, p, error)
  declarations_init(g, p, error)



#
# calls_function()
#
def		calls_function(g, p, error, functions):
  f = None

  for f in functions:
    if len(f[5]) != 0:
      errors.add(error, errors.ERRORS_STYLE,
                 "function or macro-function call to '" + f[4] +	\
                 "' should not contain any whitespace before the open "	\
                 "parenthesis.\n")



#
# calls_keyword()
#
def		calls_keyword(g, p, error, keywords):
  k = None

  for k in keywords:
    if len(k[2]) == 0:
      errors.add(error, errors.ERRORS_STYLE,
                 "the control structure '" + k[1] +			\
                 "' should contain a single whitespace before the "	\
                 "open parenthesis.\n")



#
# calls()
#
def		calls(z, p, error):
  declarations = None
  contents = None
  functions = []
  keywords = []
  matchs = None
  m = None

  declarations = e_call.findall(p.declarations)
  contents = e_call.findall(p.body)

  matchs = declarations + contents

  for m in matchs:
    if m[0]:
      keywords.append(m)
    else:
      functions.append(m)

  calls_function(g, p, error, functions)
  calls_keyword(g, p, error, keywords)



#
# check()
#
# this function launches the functions tests.
#
def		check(e, z, p, error):
  return
#  lines(g, p, error)
#  arguments(g, p, error)
#  declarations(g, p, error)
#  calls(g, p, error)

