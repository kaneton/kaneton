##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/monitor.sh
##
## created       julien quintard   [sat dec  2 04:53:58 2006]
## updated       julien quintard   [sat dec  2 14:23:42 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

##
## ---------- globals ---------------------------------------------------------
##

MENU="1 Install 2 Configure"

##
## ---------- script ----------------------------------------------------------
##

temporary=$(mktemp)

while [ true ] ; do

#
# display the menu
#

  kdialog --title "[kaneton] monitor"					\
          --menu ""							\
          10 50								\
          0								\
          ${MENU} 2> ${temporary}

  case ${?} in
    0)
      entry=$(cat ${temporary})
      ;;
    *)
      exit 0
      ;;
  esac

#
# call the corresponding script
#

  case ${entry} in
    "1")
      bash install/install.sh
      ;;
    "2")
      bash configure/configure.sh
      ;;
    *)
      exit -1
      ;;
  esac

done
