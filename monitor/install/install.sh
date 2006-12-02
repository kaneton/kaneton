##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/install/install.sh
##
## created       julien quintard   [tue nov 28 21:12:53 2006]
## updated       julien quintard   [sat dec  2 14:24:07 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

##
## ---------- globals ---------------------------------------------------------
##

MENU="1 User 2 Environment 3 Machine 4 Shell 5 Save"

KANETON_USER=""
KANETON_ENVIRONMENT="${PWD}"
KANETON_MACHINE=""
KANETON_SHELL=$(which bash)

##
## ---------- script ----------------------------------------------------------
##

temporary=$(mktemp)

while [ true ] ; do

#
# display the menu
#

  kdialog --title "[kaneton] monitor > install"				\
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
# source the corresponding file
#

  case ${entry} in
    "1")
      source install/user.sh
      ;;
    "2")
      source install/environment.sh
      ;;
    "3")
      source install/machine.sh
      ;;
    "4")
      source install/shell.sh
      ;;
    "5")
      source install/save.sh
      ;;
    *)
      exit -1
      ;;
  esac

done
