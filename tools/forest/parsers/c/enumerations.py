##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/enumerations.py
##
## created       julien quintard   [tue sep  5 21:16:19 2006]
## updated       julien quintard   [sat sep 16 12:48:18 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the enumeration definitions.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_enumeration = "^"							\
                "(?: *)"						\
                "enum"							\
                "[ \n]+"						\
                "[a-zA-Z_][a-zA-Z0-9_]*"				\
                "[ \n]*"						\
                "\{"							\
                  "[^\}]*"						\
                "\}"							\
                "[ \n]*"						\
                ";"							\
                "[^\n]*"						\
                "\n"

e_enumeration = re.compile("(" + s_enumeration + ")",
                           re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  enumerations = None
  e = None

  enumerations = e_enumeration.findall(contents)

  for e in enumerations:
    contents = contents.replace(e, "", 1)

  return enumerations, contents
