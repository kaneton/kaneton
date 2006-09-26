##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/structures.py
##
## created       julien quintard   [tue sep  5 21:13:24 2006]
## updated       julien quintard   [sat sep 16 12:58:41 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the structure definitions.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_structure = "^"							\
              "(?: *)"							\
              "struct"							\
              "[ \n]+"							\
              "[a-zA-Z_][a-zA-Z0-9_]*"					\
              "[ \n]*"							\
              "^\{"							\
                "(?:(?:\n|(?: +\})|[^\}])*)"				\
              "^\}"							\
              "[ \n]*"							\
              ";"							\
              "[^\n]*"							\
              "\n"

e_structure = re.compile("(" + s_structure + ")",
                         re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  structures = None
  s = None

  structures = e_structure.findall(contents)

  for s in structures:
    contents = contents.replace(s, "", 1)

  return structures, contents
