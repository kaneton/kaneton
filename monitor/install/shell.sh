##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/install/shell.sh
##
## created       julien quintard   [sat dec  2 13:16:51 2006]
## updated       julien quintard   [sat dec  2 14:32:21 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

##
## ---------- script ----------------------------------------------------------
##

shell()
{

#
# ask the user to specify the location of the 'bash' binary program
#

  kdialog --title "[kaneton] monitor > install > shell"			\
          --inputbox							\
									\
"Please specify the location of the 'bash' binary on your system."	\
									\
          10 50								\
          ${KANETON_SHELL} 2> ${temporary}

  case ${?} in
    0)
      KANETON_SHELL=$(cat ${temporary})
      ;;
    *)
      return
      ;;
  esac

}

shell
