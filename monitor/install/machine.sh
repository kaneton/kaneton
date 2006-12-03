##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/monitor/install/machine.sh
##
## created       julien quintard   [tue nov 28 22:03:54 2006]
## updated       julien quintard   [sun dec  3 10:55:52 2006]
##

##
## ---------- source ----------------------------------------------------------
##

source common/common.sh

##
## ---------- script ----------------------------------------------------------
##

#
# MACHINE
#
# this function asks the user to specify the kaneton machine profile to use.
#
machine()
{
  local accepted
  local machines
  local files
  local f
  local n
  local x

  accepted=0
  machines=""
  n=0

#
# get the list of the kaneton machine profiles located in the
# environment directory previously specified by the user
#

  files=$(ls ${KANETON_ENVIRONMENT}/machines/)

  for f in ${files} ; do
    if [ -d "${KANETON_ENVIRONMENT}/machines/${f}" ] ; then
      n=$(( ${n} + 1 ))
      machines="${machines} ${n} ${f}"
    fi
  done

#
# if no profile is found, alert the user there is something wrong
# with the environment directory location.
#

  if [ "${machines}" = "" ] ; then
    kdialog --title "[kaneton] monitor > install > machine"		\
            --msgbox							\
									\
"The environment path:\n\
\n\
   ${KANETON_ENVIRONMENT}\n\
\n\
you previously set seems wrong since there is no machine \
profile in here."							\
									\
          10 65

    return
  fi

#
# explain what is a machine profile
#

  kdialog --title "[kaneton] monitor > install > machine"		\
          --msgbox							\
									\
"Please specify the kaneton machine profile you want to use.\n\
\n\
A kaneton machine profile is composed of the following components:\n\
\n\
1. The operating system you are running on.\n\
2. Eventually the microprocessor on which runs your operating system.\n\
\n\
3. The target machine for which kaneton will be developed.\n\
4. Eventually the target microprocessor for which kaneton will be \
developed.\n\
\n\
The nomenclature followed by the machine profiles is:\n\
\n\
[1-os].[2-processor]::[3-os].[4-processor]\n\
\n\
Some valid examples:\n\
\n\
   linux::pc-ibm\n\
   linux.ia32::sgi-o2.mips32\n\
   macos.ppc::pc-ibm.mips64"						\
									\
          20 70

  while [ ${accepted} -eq 0 ] ; do

#
# ask the user to choose a machine profile and finally set the
# KANETON_MACHINE based on his choice
#

    kdialog --title "[kaneton] monitor > install > machine"		\
            --menu ""							\
            15 50							\
            10								\
            ${machines} 2> ${temporary}

    case ${?} in
      0)
        x=$(cat ${temporary})

        n=0
        for f in ${files} ; do
          if [ -d "${KANETON_ENVIRONMENT}/machines/${f}" ] ; then
            n=$(( ${n} + 1 ))
            if [ ${n} -eq ${x} ] ; then
              KANETON_MACHINE=${f}
              break
            fi
          fi
        done
        ;;
      *)
        return
        ;;
    esac

#
# display the description file of the chosen machine profile
#

    kdialog --title "[kaneton] monitor > install > machine"		\
            --textbox							\
      ${KANETON_ENVIRONMENT}/machines/${KANETON_MACHINE}/Description	\
            20 75 2> chiche

#
# ask confirmation about the machine profile choice
#

    kdialog --title "[kaneton] monitor > install > machine"		\
            --yesno "Are you sure you want to use this machine profile?"\
            5 70

    case ${?} in
      0)
        accepted=1
        ;;
      1)
        accepted=0
	;;
      *)
        return
        ;;
    esac

  done
}

machine
