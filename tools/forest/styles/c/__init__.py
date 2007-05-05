##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/__init__.py
##
## created       julien quintard   [sat sep 16 01:47:33 2006]
## updated       julien quintard   [sat sep 16 14:26:35 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file is another wrapper for the 'c' parser's styles.
#

#
# ----- imports ---------------------------------------------------------------
#

import epita2006

#
# ----- interface -------------------------------------------------------------
#

#
# options()
#
# this function forwards the call to the correct style.
#
def		options(style, optparse):
  if not style:
    return

  if (style == "epita2006"):
    return epita2006.options(optparse)



#
# check()
#
# this function forwards the call to the correct style.
#
def		check(style, p, options):
  detail = None
  pure = None
  count = None

  if not style:
    return pure, detail, count

  if (style == "epita2006"):
    pure, detail, count = epita2006.check(p, options)

  return pure, detail, count
