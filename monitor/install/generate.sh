##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/install/generate.sh
##
## created       julien quintard   [sat dec  2 12:53:12 2006]
## updated       julien quintard   [sun dec  3 11:01:40 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

##
## ---------- script ----------------------------------------------------------
##

#
# GENERATE
#
# this function asks the user confirmation about the configuration
#
generate()
{

#
# summarize the configuration
#

  kdialog --title "[kaneton] monitor > install > generate"		\
          --msgbox							\
									\
"Your current configuration is decribed below:\n\
\n\
   [developer]      ${KANETON_USER}\n\
   [machine]        ${KANETON_MACHINE}\n\
   [shell]          ${KANETON_SHELL}"					\
									\
          10 65

#
# ask confirmation before creating the user profile
#

  kdialog --title "[kaneton] monitor > install > generate"		\
          --yesno "Are you sure you want to use this configuration?"	\
          5 70

    case ${?} in
      0)
        ;;
      *)
        return
        ;;
    esac

#
# indicate what is going to happen
#

  kdialog --title "[kaneton] monitor > install > generate"		\
          --msgbox							\
									\
"A user profile will now be created at the following location:\n\
\n\
   ${KANETON_ENVIRONMENT}/users/${KANETON_USER}/"			\
									\
          10 65

#
# effectively create the user profile based on the student user profile
#

  if [ -d "${KANETON_ENVIRONMENT}/users/${KANETON_USER}" ] ; then
    rm -Rf "${KANETON_ENVIRONMENT}/users/${KANETON_USER}"
  fi

  cp -R "${KANETON_ENVIRONMENT}/users/student"				\
        "${KANETON_ENVIRONMENT}/users/${KANETON_USER}"

#
# indicate that everything is right
#

  kdialog --title "[kaneton] monitor > install > generate"		\
          --msgbox							\
									\
"The ${KANETON_USER}'s user profile was created successfully:\n\
\n\
   ${KANETON_ENVIRONMENT}/users/${KANETON_USER}/"			\
									\
          10 65

#
# finally, display what the user has to do to use this newly
# created profile
#

  kdialog --title "[kaneton] monitor > install > generate"		\
          --msgbox							\
									\
"To use this profile you have to set the following environment \
variables in your shell configuration file:\n\
\n\
   export KANETON_USER='${KANETON_USER}'\n\
   export KANETON_MACHINE='${KANETON_MACHINE}'\n\
   export KANETON_SHELL='${KANETON_SHELL}'\n\
\n\
Once this is done, you will be able to use the kaneton development \
environment."								\
									\
          15 65

}

generate
