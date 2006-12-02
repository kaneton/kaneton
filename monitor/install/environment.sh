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
## updated       julien quintard   [sat dec  2 14:28:22 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

##
## ---------- script ----------------------------------------------------------
##

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
[kaneton-root-directory]/env/\n"					\
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
