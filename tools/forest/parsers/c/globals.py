##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/globals.py
##
## created       julien quintard   [tue sep  5 21:20:06 2006]
## updated       julien quintard   [sat sep 16 13:07:48 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the global variable declarations.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_global = "^"								\
           "(?:extern|static|const|struct|enum|"			\
             "unsigned|long|union| )*"					\
           "[a-zA-Z_][a-zA-Z0-9_]*"					\
           "[ \n\*]*"							\
           "[a-zA-Z_][a-zA-Z0-9_]*"					\
             "(?:\[[0-9]*\])*"						\
             "(?:"							\
               "[ \n]*"							\
               "="							\
               "[ \n]*"							\
               "[^;]*"							\
             ")?"							\
           ";"								\
           "[^\n]*"							\
           "\n"

e_global = re.compile("(" + s_global + ")",
                      re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  globals = None
  g = None

  globals = e_global.findall(contents)

  for g in globals:
    contents = contents.replace(g, "", 1)

  return globals, contents
