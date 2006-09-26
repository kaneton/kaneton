##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/characters.py
##
## created       julien quintard   [tue sep  5 20:38:11 2006]
## updated       julien quintard   [sat sep 16 12:33:50 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to replace every character by an empty character: ' '.
#
# the goal is here to disable future fault-positive matchs.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_character = "\'.?\'"

e_character = re.compile("(" + s_character + ")")

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  characters = None
  c = None

  characters = e_character.findall(contents)

  for c in characters:
    contents = contents.replace(c, parser.forge(parser.FORGE_CHARACTER,
                                                c,
                                                len(c)),
                                1)

  return characters, contents
