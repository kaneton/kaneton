##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/common/wrapper.sh
##
## created       julien quintard   [tue nov 28 21:12:32 2006]
## updated       julien quintard   [sat dec  2 06:14:59 2006]
##

#
# kaneton b?dialog wrapper
#
kdialog()
{
  if [ $(which dialog) != "0" ] ; then
    dialog "${@}"
  else
    bdialog "${@}"
  fi
}
