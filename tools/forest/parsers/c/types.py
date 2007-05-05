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

s_type = "^"							\
         "( *)"                                                 \
         "("                                                    \
             "(typedef\s+)(struct|enum|union)?"			\
             ".*?(\{.*?\}.*?\s*;|;)\s*"                         \
         "|"                                                    \
             "(struct|enum|union).*?\{.*?\}.*?\s*;\s*"          \
         ")"

e_type = re.compile(s_type,
                    re.MULTILINE | re.DOTALL)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  types = []
  t = None

  for t in e_type.finditer(contents):
    contents = contents.replace(t.group(0), "", 1)
    types += [t.group(0)]

  return types, contents
