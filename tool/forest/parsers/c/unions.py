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

s_union = "^\s*" \
                "(typedef\s+)?union\s+" \
                "[\w_]+\s*" \
                "\{"


e_union = re.compile("(" + s_union + ")",
                           re.MULTILINE)

s_newlines = '^\s*([\w_]+)?(;|)\s*'
e_newlines = re.compile(s_newlines)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):

  unions = []

  union = e_union.search(contents)
  while union:
    contents = contents.replace(union.group(0), "", 1)
    union_inside = parser.parse_matching(contents, '{', '}')
    contents = contents.replace(union_inside, "", 1)
    newlines = e_newlines.search(contents).group(0)
    contents = contents.replace(newlines, "", 1)
    unions += [(union.group(0) + union_inside)]
    union = e_union.search(contents)

  
  return unions, contents
