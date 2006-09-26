##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/unions.py
##
## created       julien quintard   [tue sep  5 21:18:33 2006]
## updated       julien quintard   [sat sep 16 13:01:35 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the union definitions.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_union = "^"								\
          "(?: *)"							\
          "union"							\
          "[ \n]+"							\
          "[a-zA-Z_][a-zA-Z0-9_]*"					\
          "[ \n]*"							\
          "^\{"								\
            "(?:(?:\n|(?: +\})|[^\}])*)"				\
          "^\}"								\
          "[ \n]*"							\
          ";"								\
          "[^\n]*"							\
          "\n"

e_union = re.compile("(" + s_union + ")",
                     re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  unions = None
  u = None

  unions = e_union.findall(contents)

  for u in unions:
    contents = contents.replace(u, "", 1)

  return unions, contents
