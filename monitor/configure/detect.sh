##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/configure/detect.sh
##
## created       julien quintard   [sat dec  2 18:53:18 2006]
## updated       julien quintard   [sat dec  2 18:55:42 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

##
## ---------- script ----------------------------------------------------------
##

detect()
{
  if [ "${KANETON_USER}" = "" ] ||					\
     [ "${KANETON_MACHINE}" = "" ] ||					\
     [ "${KANETON_SHELL}" = "" ] ; then

    kdialog --title "[kaneton] monitor > configure"			\
            --msgbox							\
									\
"No kaneton configuration seems to be used since one of the \
following environment variables is not set:\n\
\n\
   \$KANETON_USER = ${KANETON_USER}
   \$KANETON_MACHINE = ${KANETON_MACHINE}
   \$KANETON_SHELL = ${KANETON_SHELL}
\n\
Did you set up a new kaneton configuration by creating a \
user profile?"								\
									\
          15 50

    exit 0

  fi

  if [ ! -f "../env/.env.sh" ] ; then

    kdialog --title "[kaneton] monitor > configure"			\
            --msgbox							\
									\
"The kaneton shell environment was not found at the following \
location:\n\
\n\
   ${PWD}/../env/.env.sh\n\
\n\
Either you launched the kaneton monitor from a bad directory \
or you simply forgot to initialize the kaneton development \
environment.\n\
\n\
The kaneton monitor must be launched in the monitor/ directory.\n\
\n\
To initialize the kaneton development directory, move to the \
kaneton root directory and launch the following command:\n\
\n\
   $> make init"							\
									\
          21 65

    exit 0

  fi
}

detect
