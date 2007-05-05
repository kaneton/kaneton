##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/prototypes.py
##
## created       julien quintard   [tue sep  5 20:59:41 2006]
## updated       julien quintard   [sat sep 16 12:57:10 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the prototypes.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import parser

import functions

#
# ----- regexp ----------------------------------------------------------------
#

s_prototype = "(?P<total>\s*?^(?P<function> *"                           \
             "(?!else|return|typedef)([\w_]+\**?\s+)+?"                        \
             "(\*(\s|))*?\**"                                                  \
             "(?P<funcname>[\w_]+)(?P<incorrect_spce>\s*)"                     \
             "\("                                                              \
             "("                                                               \
               ".*?"                                                           \
             ")"                                                               \
             "\)\s*;))"

e_prototype = re.compile(s_prototype, re.DOTALL | re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# parse()
#
def		parse(contents):
  prototypes = []
  p = None

  for p in e_prototype.finditer(contents):
    contents = contents.replace(p.group(0), "", 1)
#    print p.group(0), "---"
    prototypes += [p.group(0)]

  return prototypes, contents
