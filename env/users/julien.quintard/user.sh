## copyright quintard julien
##
## kaneton
##
## user.sh
##
## path          /home/mycure/kaneton/env
##
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
##
## started on    Fri Feb 11 02:18:00 2005   mycure
## last update   Tue Nov  8 12:03:39 2005   mycure
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
