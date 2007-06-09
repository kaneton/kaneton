##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/functions.py
##
## created       julien quintard   [tue sep  5 20:55:11 2006]
## updated       julien quintard   [sat sep 16 13:07:19 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file tries to detect the function declarations.
#

#
# ----- imports ---------------------------------------------------------------
#

import re
import parser

#
# ----- regexp ----------------------------------------------------------------
#

s_function = "^(?P<function>(?P<lead> *)"                                      \
             "(?!return|else|if)([\w_]+\s+)*[\w_]+\*?"                       \
             "[\s\*]*?\**"                                                  \
             "(?P<funcname>[\w_]+)(?P<incorrect_spce>\s*)"                     \
             "\("                                                              \
             "("                                                               \
               "[^;]*?"                                                           \
             ")"                                                               \
             "\)\s*\{)"

e_function = re.compile(s_function, re.MULTILINE | re.DOTALL)
#
# ----- functions -------------------------------------------------------------
#


#
# parse()
#
def		parse(contents):
  functions = []

  f_begin = e_function.search(contents)
  while f_begin:
    function_inside = parser.parse_matching(contents[f_begin.end():],
                                            '{', '}')
    newlines = parser.e_newlines.search(contents[f_begin.end() + len(function_inside):]).group(0)
    contents = contents.replace(f_begin.group(0) + function_inside + newlines, "", 1)
    functions += [(f_begin.group('function') + function_inside)]
    f_begin = e_function.search(contents)


  return functions, contents
