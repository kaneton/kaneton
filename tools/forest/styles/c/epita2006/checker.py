##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/checker.py
##
## created       julien quintard   [tue sep  5 23:00:37 2006]
## updated       julien quintard   [sat sep 16 21:29:42 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file contains the checker main functions.
#

#
# ----- import ----------------------------------------------------------------
#

import sys
import re

import errors

import comments
import structures
import enumerations
import unions
import types
import globals
import macros
import functions
import prototypes
import contents
import classes

#
# ----- functions -------------------------------------------------------------
#

#
# trailings()
#
# this function tries to detect the trailing whitespaces.
#
s_whitespace = "^.* +$"
e_whitespace = re.compile("(" + s_whitespace + ")",
                          re.MULTILINE)

def		trailings(e, error):
  match = None
  m = None

  match = e_whitespace.findall(error.code)

  for m in match:
    errors.add(e, error, errors.ERRORS_STYLE,
               "a line is containing one or more trailing whitespaces.\n")



#
# columns()
#
# this function detects whether a line is exceeding the columns limit.
#
COLUMNS_LIMIT = 80

def		columns(e, error):
  lines = None
  l = None

  lines = error.code.split("\n")

  e_header_fuckup = re.compile('^\*\* .+ for .+ in .+$')
  for l in lines:
    if len(l) >= COLUMNS_LIMIT and not e_header_fuckup.search(l):
      errors.add(e, error, errors.ERRORS_STYLE,
                 "a line is exceeding the " + str(COLUMNS_LIMIT) +	\
                 " columns limit.\n")



#
# check()
#
# this is the main checker's function.
#
# this function calls the checker's subparts.
#
def		check(e):
##   structures.check(e)
##   enumerations.check(e)
##   unions.check(e)

#   trailings(e.code)
#   columns(e.code)

  error = classes.c_error(e.parser.old_contents)
  errors.open(e, error)
  if (e.parser.windows_style):
    errors.add(e, error, errors.ERRORS_STYLE,
               "This file contains \\r at end of lines !\n")
  columns(e, error)
  trailings(e, error)
  errors.close(e, error)
  errors.commit(e, error)

  types.check(e)
  #print "types done"
  globals.check(e)
  #print "globals done"
  comments.check(e)
  #print "comments done"
  macros.check(e)
  #print "macros done"
  functions.check(e)
  #print "func done"
  prototypes.check(e)
  #print "proto done"
  contents.check(e)
  #print "contents done"
