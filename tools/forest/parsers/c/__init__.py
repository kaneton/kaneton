##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/__init__.py
##
## created       julien quintard   [tue sep  5 14:19:28 2006]
## updated       julien quintard   [sat sep 16 12:01:05 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file is called when the module is imported.
#

#
# ----- imports ---------------------------------------------------------------
#

from optparse import OptionParser, OptionGroup

import parsers

import parser

#
# ----- interface -------------------------------------------------------------
#

#
# options()
#
# this function add the specific 'c' parser options to the 'optparse'
# object.
#
def		options(optparse):
  group = None

  group = OptionGroup(optparse,
                      "parser package specific options",
                      "# these options enable specific syntaxical "
                         "parts debugging #")

  group.add_option("--c-debug-strings",
                   action="store_true",
                   dest="parser_strings",
                   help="enable the parser's strings debugging")

  group.add_option("--c-debug-characters",
                   action="store_true",
                   dest="parser_characters",
                   help="enable the parser's characters debugging")

  group.add_option("--c-debug-comments",
                   action="store_true",
                   dest="parser_comments",
                   help="enable the parser's comments debugging")

  group.add_option("--c-debug-macros",
                   action="store_true",
                   dest="parser_macros",
                   help="enable the parser's macros debugging")

  group.add_option("--c-debug-functions",
                   action="store_true",
                   dest="parser_functions",
                   help="enable the parser's functions debugging")

  group.add_option("--c-debug-prototypes",
                   action="store_true",
                   dest="parser_prototypes",
                   help="enable the parser's prototypes debugging")

  group.add_option("--c-debug-types",
                   action="store_true",
                   dest="parser_types",
                   help="enable the parser's types debugging")

  group.add_option("--c-debug-structures",
                   action="store_true",
                   dest="parser_structures",
                   help="enable the parser's structures debugging")

  group.add_option("--c-debug-enumerations",
                   action="store_true",
                   dest="parser_enumerations",
                   help="enable the parser's enumerations debugging")

  group.add_option("--c-debug-unions",
                   action="store_true",
                   dest="parser_unions",
                   help="enable the parser's unions debugging")

  group.add_option("--c-debug-globals",
                   action="store_true",
                   dest="parser_globals",
                   help="enable the parser's globals debugging")

  group.add_option("--c-debug-blanks",
                   action="store_true",
                   dest="parser_blanks",
                   help="enable the parser's blanks debugging")

  group.add_option("--c-debug-contents",
                   action="store_true",
                   dest="parser_contents",
                   help="for each debugging step, dump the undetected content")

  optparse.add_option_group(group)

  return optparse



#
# parse()
#
# this function just calls the real parser function.
#
def		parse(p, options):
  parser.parse(p)

  return p
