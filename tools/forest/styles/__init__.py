##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/__init__.py
##
## created       julien quintard   [tue sep  5 22:27:41 2006]
## updated       julien quintard   [sat sep 16 14:25:27 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file is called when the module is imported.
#
# this file is a wrapper over the different styles.
#

#
# ----- imports ---------------------------------------------------------------
#

import c

#
# ----- interface -------------------------------------------------------------
#

#
# options()
#
# this function just forwards the call to the correct styles.
#
def		options(parser, style, optparse):
  if not parser or not style:
    return

  if (parser == "c"):
    return c.options(style, optparse)



#
# check()
#
# this function forwards the call to the correct styles.
#
def		check(parser, style, p, options):
  detail = None
  pure = None
  count = None

  if not parser or not style:
    return pure, detail, count

  if (parser == "c"):
    pure, detail, count = c.check(style, p, options)

  return pure, detail, count
