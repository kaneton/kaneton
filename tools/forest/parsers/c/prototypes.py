##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/prototypes.py
##
## created       julien quintard   [tue sep  5 20:59:41 2006]
## updated       julien quintard   [sat sep 16 12:57:10 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the prototypes.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

import functions

#
# ----- regexp ----------------------------------------------------------------
#

s_prototype = functions.s_function

e_prototype = re.compile("(" + s_prototype + ";" + "[^\n]*" + "\n" + ")",
                         re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  prototypes = None
  p = None

  prototypes = e_prototype.findall(contents)

  for p in prototypes:
    contents = contents.replace(p, "", 1)

  return prototypes, contents
