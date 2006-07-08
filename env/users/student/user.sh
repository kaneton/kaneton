## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/users/student/user.sh
##
## created       matthieu bucchianeri   [tue dec 13 21:02:30 2005]
## updated       julien quintard   [sat jul  8 03:08:06 2006]
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

  xpdf $document
}
