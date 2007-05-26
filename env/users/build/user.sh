#!/bin/sh
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/env/users/matthieu.bucchianeri/user.sh
##
## created       matthieu bucchianeri   [tue dec 13 21:02:30 2005]
## updated       matthieu bucchianeri   [mon jan 23 20:10:30 2006]
##

#
# ---------- information ------------------------------------------------------
#
# this file contains specific user shell variables, functions etc.
#

#
# VIEW
#
# this function opens and displays a document.
#
view()
{
  document=$1

  xpdf $document
}
