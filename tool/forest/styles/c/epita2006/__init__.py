##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/__init__.py
##
## created       julien quintard   [tue sep  5 22:29:18 2006]
## updated       julien quintard   [sat sep 16 14:36:50 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file contains the first called functions 'options()' and 'parse()'.
#

#
# ----- imports ---------------------------------------------------------------
#

from optparse import OptionParser, OptionGroup

import checker

import classes

#
# ----- interface -------------------------------------------------------------
#

#
# options()
#
# this function permits the 'epita2006' module to provide additional
# specific options to the user.
#
# moreover, this function calls the checker subparts' 'options()' function.
# these subparts indeed maybe want to add some options too.
#
def		options(optparse):
  group = None

  group = OptionGroup(optparse,
                      "[epita2006] checker package specific options",
                      "# these options enable/disable specific checkings #")

  #
  # enable specific checkings
  #

  group.add_option("--epita-structures",
                   action="store_true",
                   dest="epita_structures",
                   help="enable the structures style checking")

  group.add_option("--epita-enumerations",
                   action="store_true",
                   dest="epita_enumerations",
                   help="enable the enumerations style checking")

  group.add_option("--epita-unions",
                   action="store_true",
                   dest="epita_unions",
                   help="enable the unions style checking")

  group.add_option("--epita-types",
                   action="store_true",
                   dest="epita_types",
                   help="enable the types style checking")

  group.add_option("--epita-globals",
                   action="store_true",
                   dest="epita_globals",
                   help="enable the global variables style checking")

  group.add_option("--epita-comments",
                   action="store_true",
                   dest="epita_comments",
                   help="enable the comments style checking")

  group.add_option("--epita-macros",
                   action="store_true",
                   dest="epita_macros",
                   help="enable the macros style checking")

  group.add_option("--epita-functions",
                   action="store_true",
                   dest="epita_functions",
                   help="enable the functions style checking")

  group.add_option("--epita-prototypes",
                   action="store_true",
                   dest="epita_prototypes",
                   help="enable the prototypes style checking")

  group.add_option("--epita-contents",
                   action="store_true",
                   dest="epita_contents",
                   help="enable the contents style checking")

  #
  # call checker's subparts
  #

  structures.options(group)
  enumerations.options(group)
  unions.options(group)
  types.options(group)
  globals.options(group)
  comments.options(group)
  macros.options(group)
  functions.options(group)
  prototypes.options(group)
  contents.options(group)

  optparse.add_option_group(group)

  return optparse



#
# check()
#
# this function just builds an object 'c_epita' named 'e'.
#
# this object contains everything needed by the checker.
#
# then, this function calls the checker.
#
def		check(p, options):
  e = classes.c_epita()

  e.parser = p
  e.options = options

  checker.check(e)

  errors.summary(e)

  return e.pure, e.detail, e.count
