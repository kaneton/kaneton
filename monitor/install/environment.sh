##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/install/environment.sh
##
## created       julien quintard   [tue nov 28 22:03:54 2006]
## updated       julien quintard   [mon dec  4 11:37:43 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

##
## ---------- script ----------------------------------------------------------
##

#
# ENVIRONMENT
#
# this function asks the user to locate the environment directory.
#
environment()
{

#
# indicate what is the environment directory
#

  kdialog --title "[kaneton] monitor > install > environment"		\
          --msgbox							\
									\
"Please specify the kaneton environement directory\n\
\n\
This directory should be located at:\n\
\n\
[kaneton]/env/\n"							\
									\
          10 50 

#
# ask the user to specify the environment directory
#

  kdialog --title "[kaneton] monitor > install > environment"		\
          --fselect ${KANETON_ENVIRONMENT}				\
          10 50 2> ${temporary}

  case ${?} in
    0)
      KANETON_ENVIRONMENT=$(cat ${temporary})
      ;;
    *)
      return
      ;;
  esac
}

environment
