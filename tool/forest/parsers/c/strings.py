##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/strings.py
##
## created       julien quintard   [tue sep  5 17:07:08 2006]
## updated       julien quintard   [sat sep 16 12:58:12 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect and replace the strings by empty strings
# to avoid future fault-positive matchs.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_string = '"([^"]*\\\\")*[^"]*"'

e_string = re.compile(s_string)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  strings = None
  s = None

  strings = e_string.findall(contents)

  for s in e_string.finditer(contents):
#     contents = contents.replace(s.group(0), parser.forge(parser.FORGE_STRING,
#                                                 s.group(0),
#                                                 len(s.group(0))),
#                                 1)
    contents = contents.replace(s.group(0),
                                'string',
                                1)


  return strings, contents
