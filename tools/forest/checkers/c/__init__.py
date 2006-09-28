##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/kaneton/tools/forest/styles/c/__init__.py
##
## created       julien quintard   [sat sep 16 01:47:33 2006]
## updated       julien quintard   [mon sep 25 19:54:46 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file is another wrapper for the 'c' parser's styles.
#

#
# ----- imports ---------------------------------------------------------------
#

import kaneton

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

  if (style == "kaneton"):
    return kaneton.options(optparse)

#
# check()
#
# this function forwards the call to the correct style.
#
def		check(style, p, options):
  summary = None
  count = None

  if not style:
    return summary, count

  if (style == "kaneton"):
    summary, count = kaneton.check(p, options)

  return summary, count
