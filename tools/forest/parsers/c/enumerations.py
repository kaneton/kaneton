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

## s_enumeration = "^"							\
##                 "(?: *)"						\
##                 "enum"							\
##                 "[ \n]+"						\
##                 "[a-zA-Z_][a-zA-Z0-9_]*"				\
##                 "[ \n]*"						\
##                 "\{"							\
##                   "[^\}]*"						\
##                 "\}"							\
##                 "[ \n]*"						\
##                 ";"							\
##                 "[^\n]*"						\
##                 "\n"

s_enumeration = "^\s*" \
                "(typedef\s+)?enum\s+" \
                "[\w_]+\s*" \
                "\{"


e_enumeration = re.compile("(" + s_enumeration + ")",
                           re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

s_newlines = '^\s*([\w_]+)?(;|)\s*'
e_newlines = re.compile(s_newlines)

#
# parse()
#
def		parse(contents):

  enumerations = []

  enumeration = e_enumeration.search(contents)
  while enumeration:
    contents = contents.replace(enumeration.group(0), "", 1)
    enumeration_inside = parser.parse_matching(contents, '{', '}')
    contents = contents.replace(enumeration_inside, "", 1)
    newlines = e_newlines.search(contents).group(0)
    contents = contents.replace(newlines, "", 1)
    enumerations += [(enumeration.group(0) + enumeration_inside)]
    enumeration = e_enumeration.search(contents)

  return enumerations, contents
