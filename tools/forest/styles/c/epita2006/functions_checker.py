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

import functions



#
# ----- regexp ----------------------------------------------------------------
#

#
# declarations
#

s_declarations = "(?:"							\
                   "(?:extern|static|const|struct|enum|"		\
                      "unsigned|long|union| )*"				\
                   "[\w_]+"				\
                     "\**"						\
                   "[ \n]+"						\
                 ")"							\
                   "\**"						\
                 "[\w_]+"				\
                   "(?:\[.*?\])*"					\
                 "[^,;]*"						\
                 "("							\
                   "(?:"						\
                     "[ \n]*"						\
                     ","						\
                     "[ \n]*"						\
                       "\**"						\
                     "[\w_]+"				\
                       "(?:\[.*?\])*"				\
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
               "( *)(?!return|do)"						\
               "((?:const|struct|enum|unsigned|long|union| )*)"		\
               "([\w_]+)"				\
                 "(\**)"						\
               "( +)"							\
                 "\**"							\
               "([\w_]*)"				\
                 "(?:\[.*?\])*"					\
               "(?:,|$)"

e_a_argument = re.compile(s_a_argument, re.MULTILINE)

#
# declarations
#

s_a_declarations = "^"							\
                   "("							\
                     "(?:extern|static|const|struct|enum|"		\
                     "unsigned|long|union| )*"				\
                     "[\w_]+"				\
                       "\**"						\
                     "[ \n]+"						\
                   ")"							\
                     "(\**"						\
                   "[\w_]+)"				\
                     "(?:\[.*?\])*"					\
                   "[^;,]*"						\
                   "(?:;|,)"

e_a_declarations = re.compile(s_a_declarations, re.MULTILINE)

#
# call
#

# s_call = "(?:"								\
#            "("								\
#              "(for|while|switch|return|sizeof|if)"			\
#              "([ \n]*)"							\
#              "(?:\(|;)"							\
#            ")"								\
#              "|"							\
#            "("								\
#              "([\w_]+)"					\
#              "([ \n]*)"							\
#              "\("							\
#            ")"								\
#          ")"

s_call = "= *[\w_]+\s*\("

e_call = re.compile(s_call)

s_empty_line = "^\s*$"
e_empty_line = re.compile(s_empty_line)

#
# ----- functions -------------------------------------------------------------
#

# XXX

#
# lines()
#
def		lines(e, z, p, error):
  nlines = None
  d = None
  b = None

  d = e_lines.sub("", p.declarations).rstrip()
  b = e_lines.sub("", p.body).rstrip()

  nlines = -1
  for line in d.split("\n") + b.split("\n"):
    match = e_empty_line.search(line)
    if not match:
      nlines += 1

  if nlines > functions.FUNCTIONS_LINE_EXTREME_LIMIT:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the function contains more than " +			\
               str(functions.FUNCTIONS_LINE_EXTREME_LIMIT) + " lines. are you "	\
               "kidding us? (%i faults !!!!)\n" % errors.ERRORS_FATAL)
  elif nlines > functions.FUNCTIONS_LINE_LIMIT:
    errors.add(e, error, nlines - functions.FUNCTIONS_LINE_LIMIT,
               "the function contains %i lines. expected %i (%i faults)\n"
               % (nlines,
                  functions.FUNCTIONS_LINE_LIMIT,
                  nlines - functions.FUNCTIONS_LINE_LIMIT))



#
# arguments_format()
#
def		arguments_format(e, z, p, error):
  match = None
  nlines = 0
  nargs = 0
  i = None

  match = e_a_argument.findall(p.arguments)
  if not match:
    return

  nlines = len(match)

  nargs = len(p.arguments.split(","))

  if nargs > functions.FUNCTIONS_ARGUMENT_LIMIT:
    errors.add(e, error, errors.ERRORS_STYLE,
               "the function contains more than " +			\
               str(functions.FUNCTIONS_ARGUMENT_LIMIT) + " arguments.\n")

  for i in range(0, nargs - nlines):
    errors.add(e, error, errors.ERRORS_STYLE,
               "one of the function's argument is not alone on its line.\n")



#
# arguments_alignment()
#
def		arguments_alignment(e, z, p, error):
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
                 "function argument's type '" + m[2] +			\
                 "' is not aligned with the others.\n")

    if name != (len(m[0]) + len(m[1]) + len(m[2]) + len(m[3]) + len(m[4])):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "function argument's name '" + m[5] +			\
                 "' is not aligned with the others.\n")



#
# arguments()
#
def		arguments(e, z, p, error):
  arguments_format(e, z, p, error)
  arguments_alignment(e, z, p, error)



#
# declarations_blanks()
#
def		declarations_blanks(e, z, p, error):
  declarations = None

  e_blank = re.compile('(?P<all>(?P<declarations>.*;)[ \t]*\n(?P<ending_blanks>[ \n]*))', re.DOTALL)
  e_blank_line = re.compile('^[ \t]*$')

  m = e_blank.search(p.declarations)
  if m:
    declarations = p.declarations.replace(m.group('all'), m.group('declarations'), 1)
    if m.group('ending_blanks').count('\n') > 1:
      errors.add(e, error, errors.ERRORS_STYLE,
                 "the declaration block and the function's body must "	\
                 "be separated by a single blank line.\n")
  else:
    errors.add(e, error, errors.ERRORS_STYLE,
               "the declaration block and the function's body must "	\
               "be separated by a single blank line.\n")
    declarations = p.declarations

##   if p.declarations[len(p.declarations) - 2] != '\n':
##     errors.add(e, error, errors.ERRORS_STYLE,
##                "the declaration block and the function's body must "	\
##                "be separated by a blank line.\n")

##   if p.declarations[len(p.declarations) - 3] == '\n':
##     errors.add(e, error, errors.ERRORS_STYLE,
##                "the declaration block and the function's body must "	\
##                "be separated by a single blank line.\n")

  #declarations = e_lines.sub("", p.declarations)


##   if p.declarations.rstrip("\n") != declarations.rstrip("\n"):
##     errors.add(e, error, errors.ERRORS_STYLE,
##                "the declaration block must not contain blank lines.\n")
##     p.declarations = declarations;

  if e_blank_line.search(declarations):
    errors.add(e, error, errors.ERRORS_STYLE,
               "the declaration block must not contain blank lines.\n")
    #p.declarations = declarations;
  p.declarations = declarations



#
# declarations_count()
#
def		declarations_format(e, z, p, error):
  match = None
  nlines = 0
  ndecs = 0
  m = None
  i = None

  match = e_a_declarations.findall(p.declarations)
  if not match:
    return

  diff = p.declarations.count(';') - p.declarations.count('\n') - 1

  if diff > 0:
    for i in range(0, diff):
      errors.add(e, error, errors.ERRORS_STYLE,
                "one of the declaration is not alone on its line.\n")

#   nlines = len(match)

#   match = e_declarations.findall(p.declarations)
#   if not match:
#     return

#   for m in match:
#     ndecs += 1
#     if m:
#       ndecs += len(m.lstrip(",").split(","))

#   for i in range(0, ndecs - nlines):
#     errors.add(e, error, errors.ERRORS_STYLE,
#                "one of the declaration is not alone on its line.\n")



#
# declarations_alignment()
#
def		declarations_alignment(e, z, p, error):
  match = None
  type = -1
  name = -1
  m = None

  match = e_a_declarations.findall(p.declarations)

  for m in match:
    if p.a_declarations != len(m[0]):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "the block declaration's member '" + m[1] +		\
                 "' is not aligned with the function name.\n")



#
# declarations_init()
#
def		declarations_init(e, z, p, error):
  match = None
  m = None

  match = e_call.findall(p.declarations)
  if not match:
    return

  for m in match:
    errors.add(e, error, errors.ERRORS_STYLE,
               "local variables must not be initialized by function "	\
               "or macro-function calls.\n")



#
# declarations()
#
def		declarations(e, z, p, error):
  if not p.declarations:
    return

  declarations_blanks(e, z, p, error)
  declarations_format(e, z, p, error)
  declarations_alignment(e, z, p, error)
  declarations_init(e, z, p, error)



#
# calls_function()
#
## def		calls_function(e, z, p, error, functions):
##   f = None

##   for f in functions:
##     if len(f[5]) != 0:
##       errors.add(e, error, errors.ERRORS_STYLE,
##                  "function or macro-function call to '" + f[4] +	\
##                  "' should not contain any whitespace before the open "	\
##                  "parenthesis.\n")



## #
## # calls_keyword()
## #
## def		calls_keyword(e, z, p, error, keywords):
##   k = None

##   for k in keywords:
##     if len(k[2]) == 0:
##       errors.add(e, error, errors.ERRORS_STYLE,
##                  "the control structure '" + k[1] +			\
##                  "' should contain a single whitespace before the "	\
##                  "open parenthesis.\n")



## #
## # calls()
## #
## def		calls(e, z, p, error):
##   declarations = None
##   contents = None
##   functions = []
##   keywords = []
##   matchs = None
##   m = None

##   declarations = e_call.findall(p.declarations)
##   contents = e_call.findall(p.body)

##   matchs = declarations + contents

##   for m in matchs:
##     if m[0]:
##       keywords.append(m)
##     else:
##       functions.append(m)

##   calls_function(e, z, p, error, functions)
##   calls_keyword(e, z, p, error, keywords)



#
# check()
#
# this function launches the functions tests.
#
def		check(e, z, p, error):
#  pass
  lines(e, z, p, error)
  arguments(e, z, p, error)
  declarations(e, z, p, error)
#  calls(e, z, p, error)
