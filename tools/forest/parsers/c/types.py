##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/types.py
##
## created       julien quintard   [tue sep  5 21:04:05 2006]
## updated       julien quintard   [sat sep 16 13:10:05 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the type definitions.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_type = "^"								\
         "(?: *)"							\
         "typedef"							\
         "(?:[ \n]+)"							\
         "(?:"								\
           "(?:"							\
             "(?:struct|enum|union)"					\
             "(?: *)"							\
             "(?:"							\
               "(?:"							\
                 "(?:(?: +)(?:[a-zA-Z_][a-zA-Z0-9_]*))?"		\
                 "[ \n]*"						\
                 "^\{"							\
                   "(?:(?:\n|(?: +\})|[^\}])*)"				\
                 "^\}"							\
               ")"							\
                 "|"							\
               "(?:"							\
                 "(?:(?: +)(?:[a-zA-Z_][a-zA-Z0-9_]*))"			\
               ")"							\
             ")"							\
           ")"								\
             "|"							\
           "(?:"							\
             "(?:const|unsigned|long|static| )*"			\
             "(?:[a-zA-Z_][a-zA-Z0-9_]*)"				\
               "[ \n\*]*"						\
           ")"								\
         ")"								\
         "(?:[ \n]+)"							\
         "(?:"								\
           "(?:"							\
             "(?:[a-zA-Z_][a-zA-Z0-9_]*)"				\
               "(?:\[[0-9]*\])?"					\
           ")"								\
             "|"							\
           "(?:"							\
             "\("							\
             "\*"							\
             "(?:[a-zA-Z_][a-zA-Z0-9_]*)"				\
             "\)"							\
             "\("							\
             "[^;]*"							\
           ")"								\
         ")"								\
         ";"								\
         "[^\n]*"							\
         "\n"

e_type = re.compile("(" + s_type + ")",
                    re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  types = None
  t = None

  types = e_type.findall(contents)

  for t in types:
    contents = contents.replace(t, "", 1)

  return types, contents
