##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/install/user.sh
##
## created       julien quintard   [tue nov 28 21:18:45 2006]
## updated       julien quintard   [sat dec  2 14:32:32 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

##
## ---------- script ----------------------------------------------------------
##

user()
{

#
# ask the user to specify his developer name
#

  kdialog --title "[kaneton] monitor > install > user"			\
          --inputbox							\
									\
"The user name must be composed of\n\
your first and last name in lower case.\n\
\n\
Examples could be:\n\
\n\
julien.quintard\n\
robert.smith\n"								\
									\
          15 50								\
          ${KANETON_USER} 2> ${temporary}

  case ${?} in
    0)
      KANETON_USER=$(cat ${temporary})
      ;;
    *)
      return
      ;;
  esac
}

user
