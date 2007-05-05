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

#              "(typedef\s*|)"							\
s_structure = "^\s*"         \
              "(typedef\s+)?struct"							\
              "\s+"							\
              "[\w_]+"					\
              "\s*"							\
              "\{"

e_structure = re.compile("(" + s_structure + ")",
                         re.MULTILINE | re.DOTALL)

#
# ----- functions -------------------------------------------------------------
#

s_newlines = '^\s*([\w_]+)?(;|)\s*?'
e_newlines = re.compile(s_newlines)

#
# parse()
#
def		parse(contents):
  structures = []

  structure = e_structure.search(contents)
  while structure:
#  for structure in e_structure.finditer(contents):
    contents = contents.replace(structure.group(0), "", 1)
    structure_inside = parser.parse_matching(contents, '{', '}')
    contents = contents.replace(structure_inside, "", 1)
    newlines = e_newlines.search(contents).group(0)
    contents = contents.replace(newlines, "", 1)
    structures += [(structure.group(0) + structure_inside + newlines)]
    structure = e_structure.search(contents)

  return structures, contents
