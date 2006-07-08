## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/env/users/julien.quintard/user.sh
##
## created       julien quintard   [sat jul  8 03:08:34 2006]
## updated       julien quintard   [sat jul  8 03:08:39 2006]
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
  local document=${1}

  ${VIEWER} ${document}
}
