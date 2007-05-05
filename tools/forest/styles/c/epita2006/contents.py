##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/contents.py
##
## created       julien quintard   [fri sep  8 20:40:00 2006]
## updated       julien quintard   [sat sep 16 15:16:27 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file dumps the undetected lines.
#

#
# ----- imports ---------------------------------------------------------------
#

import classes
import errors

#
# ----- functions -------------------------------------------------------------
#

#
# options()
#
# this function adds specific options()
#
def		options(group):
  return



#
# check()
#
# this function checks the contents.
#
def		check(e):
  error = classes.c_error(e.parser.contents)
  lines = None

  errors.open(e, error)

  if error.code:
    lines = error.code.split("\n")

    #if len(lines) > 0:
    #  errors.add(e, error, errors.ERRORS_FATAL,
    #             "there are " + str(len(lines)) + " meaningless lines "	\
    #             "of source code.\n")

  errors.close(e, error)

  errors.commit(e, error)
