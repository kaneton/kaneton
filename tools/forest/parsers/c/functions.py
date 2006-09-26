##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/functions.py
##
## created       julien quintard   [tue sep  5 20:55:11 2006]
## updated       julien quintard   [sat sep 16 13:07:19 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the function declarations.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_function = "^"							\
             "(?:extern|static|const|struct|enum|"			\
               "unsigned|long|union| )*"				\
             "[a-zA-Z_][a-zA-Z0-9_]*"					\
             "[ \n\*]*"							\
             "[a-zA-Z_][a-zA-Z0-9_]*"					\
             "\("							\
               "(?:"							\
                 "(?:"							\
                   "(?:const|struct|enum|unsigned|long|union| )*"	\
                   "[a-zA-Z_][a-zA-Z0-9_]*"				\
                   "[ \n\*]*"						\
                   "[a-zA-Z_][a-zA-Z0-9_]*"				\
                     "(?:\[[0-9]*\])*"					\
                   ",[ \n]+"						\
                 ")*"							\
                 "(?:const|struct|enum|unsigned|long|union| )*"		\
                 "[a-zA-Z_][a-zA-Z0-9_]*"				\
                 "[ \n\*]*"						\
                 "[a-zA-Z_][a-zA-Z0-9_]*"				\
                   "(?:\[[0-9]*\])*"					\
                 "|"							\
                 "void"							\
               ")"							\
             "\)"

e_function = re.compile("(" + s_function + "[ \n]*" +
                          "\{" +
                            "(?:\n|(?: +\})|[^\}])*" +
                          "^\}" +
                          "[^\n]*" +
                          "\n" +
                        ")",
                        re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  functions = None
  f = None

  functions = e_function.findall(contents)

  for f in functions:
    contents = contents.replace(f, "", 1)

  return functions, contents
