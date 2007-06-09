##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/tabulations.py
##
## created       julien quintard   [tue sep  5 16:21:01 2006]
## updated       julien quintard   [sat sep 16 13:00:01 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect and replace the tabulations by the equivalent
# amount of whitespaces.
#
# this action is performed to simplify the future detections of bad
# alignments.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_tabulation = "^([^\t\n]*)(\t)"

e_tabulation = re.compile("(" + s_tabulation + ")",
                          re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  length = None
  match = None
  text = None
  tab = None

  while 1:
    match = e_tabulation.search(contents)

    if match:
      text = match.group(2)
      tab = match.group(3)

      length = len(text)

      contents = contents.replace(tab,
                                  parser.forge(parser.FORGE_STREAM,
                                               "",
                                               8 - (length % 8)),
                                  1)
    else:
      break

  return contents
