#!/bin/sh
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/env/users/matthieu.bucchianeri/user.sh
##
## created       matthieu bucchianeri   [tue dec 13 21:02:30 2005]
## updated       matthieu bucchianeri   [tue dec 13 21:02:35 2005]
##

#
# ---------- information ------------------------------------------------------
#
# this file contains specific user shell variables, functions etc..
#

#
# VIEW
#
# this function opens and displays a document.
#
view()
{
  document=$1

  evince $document
}
