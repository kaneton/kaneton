##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/functions_parser.py
##
## created       julien quintard   [sat sep  9 21:35:58 2006]
## updated       julien quintard   [sun sep 17 09:39:25 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the functions' body parts.
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
# function
#

s_function = "^"							\
             "((?:extern|(static)|const|struct|enum|"			\
               "unsigned|long|union| )*)"				\
             "([a-zA-Z_][a-zA-Z0-9_]*)"					\
               "(\**)"							\
             "( +)"							\
               "(\**)"							\
             "([a-zA-Z_][a-zA-Z0-9_]*)"					\
             "\("							\
               "("							\
                 "(?:"							\
                   "(?:const|struct|enum|unsigned|long|union| )*"	\
                   "[a-zA-Z_][a-zA-Z0-9_]*"				\
                     "\**"						\
                   "[ \n]+"						\
                     "\**"						\
                   "[a-zA-Z_][a-zA-Z0-9_]*"				\
                     "(?:\[[0-9]*\])*"					\
                   ",[ \n]+"						\
                 ")*"							\
                 "(?:const|struct|enum|unsigned|long|union| )*"		\
                 "[a-zA-Z_][a-zA-Z0-9_]*"				\
                   "\**"						\
                 "[ \n]+"						\
                   "\**"						\
                 "[a-zA-Z_][a-zA-Z0-9_]*"				\
                   "(?:\[[0-9]*\])*"					\
                 "|"							\
                 "void"							\
               ")"							\
             "\)"							\
             "[ \n]*"							\
             "^\{\n"							\
               "("							\
                 "(?:"							\
                   "(?:extern|const|struct|enum|unsigned|"		\
                      "long|union|[ \n])*"				\
                   "[a-zA-Z_][a-zA-Z0-9_]*"				\
                     "\**"						\
                   "[ \n]+"						\
                     "\**"						\
                   "[a-zA-Z_][a-zA-Z0-9_]*"				\
                     "(?:\[[0-9]*\])*"					\
                   "[^;]*"						\
                   ";"							\
                   "(?: *\n)*"						\
                 ")*"							\
               ")"							\
               "((?:\n|(?: +\})|[^\}])*)"				\
             "^\}"

e_function = re.compile(s_function, re.MULTILINE)

#
# fors
#

s_for = "for"								\
        "( *)"								\
        "\("								\
          "([^;]*);"							\
          "([^;]*);"							\
          "((?:[^\)]|(?:\)(?:[^\n])))*)"				\
        "\)"								\
        "( *)"								\
        "\n"								\
        "( *)\{"							\
          "(\n*)"							\
        "\1\}"

e_for = re.compile(s_for, re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# fors()
#
# this function tries to locate the 'for' statements.
#
def		fors(e, z, p, error):
  match = None

  match = e_for.search(p.body)

  print match

  if not match:
    print "XXX"
    return

  print match



#
# parse()
#
# this function tries to locate the different function's parts.
#
def		parse(e, z, error):
  p = functions.c_function()
  match = None

  match = e_function.search(error.code)

  if not match:
    errors.add(e, error, errors.ERRORS_FATAL,
               "the function format is absolutely not understandable.\n")
    return None

  p.name = match.group(7)
  p.arguments = match.group(8)

  p.a_arguments = len(match.group(1)) + len(match.group(3)) +		\
                  len(match.group(4)) + len(match.group(5)) +		\
                  len(match.group(6)) + len(match.group(7)) +		\
                  len("(")

  p.a_declarations = len(match.group(1)) + len(match.group(3)) +	\
                     len(match.group(4)) + len(match.group(5)) +	\
                     len(match.group(6))

  p.declarations = match.group(9)
  p.body = match.group(10)

  return # XXX
  # XXX
  print "---"
  print p.name
  print "---"
  print p.arguments
  print "---"
  print p.declarations
  print "---"
  print p.body
  print "---"

  fors(e, z, p, error)
  # XXX puis tous les autres

  if match.group(2):
    z.locals += 1
  else:
    z.exports += 1

    if z.exports > functions.FUNCTIONS_EXPORT_LIMIT:
      errors.add(e, error, errors.ERRORS_STYLE,
                 "the file contains more than " +			\
                 str(functions.FUNCTIONS_EXPORT_LIMIT) +		\
                 " exported functions.\n")

  if (z.locals + z.exports) > functions.FUNCTIONS_FILE_LIMIT:
    errors.add(e, error, errors.ERRORS_STYLE,
               "the file contains more than " +				\
               str(functions.FUNCTIONS_FILE_LIMIT) + " functions.\n")

  return p
