##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/macros.py
##
## created       julien quintard   [tue sep  5 20:53:23 2006]
## updated       julien quintard   [sat sep 16 12:52:34 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the macro and macro-function definitions.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_macro = "^"								\
          "(?: *)"							\
          "#"								\
          "(?: *)"							\
          "(?:ifndef|ifdef|endif|define|undef|include|if|else)"		\
          "(?:(?:\\\\\n)|[^\n])*"  \
          "\n"

e_macro = re.compile("(" + s_macro + ")", re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  macros = None
  m = None

  macros = e_macro.findall(contents)

  for m in macros:
    contents = contents.replace(m, "", 1)

  return macros, contents
