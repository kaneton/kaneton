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

s_character = "(\'..\'|\'.?\')"
#s_character = "\'(\\\'|\\ *\n|.)*?\'"

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

  for c in e_character.finditer(contents):
    contents = contents.replace(c.group(0), parser.forge(parser.FORGE_CHARACTER,
                                                c.group(0),
                                                len(c.group(0))),
                                1)

  return characters, contents
