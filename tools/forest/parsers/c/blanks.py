##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/blanks.py
##
## created       julien quintard   [tue sep  5 21:27:54 2006]
## updated       julien quintard   [sat sep 16 12:08:06 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to remove the blank lines to 1) simplify the output
# of the remaining content 2) be able to count the remaining content lines.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_blank = "\n+"

e_blank = re.compile("(" + s_blank + ")")

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  blanks = None
  b = None

  blanks = e_blank.findall(contents)

  for b in blanks:
    contents = contents.replace(b, "\n", 1)

  contents = contents.lstrip()
  contents = contents.rstrip()

  return blanks, contents
