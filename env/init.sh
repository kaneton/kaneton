#! /bin/bassh

## copyright quintard julien
## 
## kaneton
## 
## init.sh
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:58:21 2005   mycure
## last update   Mon Nov  7 18:43:43 2005   mycure
##

#
# ---------- information ------------------------------------------------------
#
# this script has to be run in its directory: src/env/
#

#
# ---------- globals ----------------------------------------------------------
#

MACHINE_CONF=""
USER_CONF="users/$KANETON_USER/user.conf"

MACHINE_MK=""
USER_MK=""

MACHINE_SH=""
USER_SH=""

ENV_MK=".env.mk"
ENV_SH=".env.sh"

SRC_DIR=""



#
# ---------- functions --------------------------------------------------------
#

#
# PROTO
#
# this function generates the prototypes.
#
proto()
{
  directory $_SRC_DIR_
  makefile proto 2>&- 1>&-
  directory $_ENV_DIR_
}



#
# DEP
#
# this function generates the dependencies.
#
dep()
{
  directory $_SRC_DIR_
  makefile dep 2>&- 1>&-
  directory $_ENV_DIR_
}



#
# MACHINES
#
# this function displays the supported machines.
#
machines()
{
  l=""

  machines=$(list $_MACHINES_DIR_)

  for m in $machines ; do
    if [ -d $_MACHINES_DIR_/$m ] ; then
      l="$l $m"
    fi
  done

  display " supported machines are:$l" "!"
}



#
# LINKS
#
# this function links the kernel configuration files.
#
links()
{
  display " linking kernel configuration files" "+"

  remove $_CORE_CONF_DIR_/conf.c
  link $_CORE_CONF_DIR_/conf.c $_USER_DIR_/conf.c

  remove $_CORE_INCLUDE_DIR_/kaneton/conf.h
  link $_CORE_INCLUDE_DIR_/kaneton/conf.h $_USER_DIR_/conf.h
}



#
# INIT
#
# this function installs the environment, calling the script depending
# of your operating system.
#
init()
{
  if [ ! -d $_MACHINE_DIR_ ] ; then
    display " unknown system: '$_MACHINE_'" "!"
    display ""
    display " please check your _MACHINE_ variable into '$_USER_CONF_'" "!"
    display ""
    machines
    display ""
    usage
    display ""
    exit
  fi

  if [ ! -e $_MACHINE_DIR_/init.sh ] ; then
    display " '$_MACHINE_' machine-specific init script not present" "!"
    display ""
    display " please check your _MACHINE_ variable into '$_USER_CONF_'" "!"
    display ""
    machines
    display ""
    usage
    display ""
    exit
  else
    launch $_MACHINE_DIR_/init.sh
  fi

  if [ -e $_USER_DIR_/init.sh ] ; then
    launch $_USER_DIR_/init.sh
  fi
}



#
# WARNING
#
# this function alerts the user, displaying information and asking to continue.
#
warning()
{
  # displays information and asks the user to continue or cancel.
  display " your current configuration:" "+"
  display "   user:                     $_USER_" "+"
  display "   machine:                  $_MACHINE_" "+"
  display "   architecture:             $_ARCHITECTURE_" "+"
  display "   multi-bootloader:         $_MBI_" "+"
  display ""
  display " to cancel press CTRL^C, otherwise press enter" "?"

  waitkey
}



#
# USER_CONF
#
# this function reads the user configuration file to get the machine
# variable.
#
user_conf()
{
  # gets the _MACHINE_ variable.
  machine=$(sed -n							\
            "s/^_MACHINE_[[:space:]]*\(=\|:=\)[[:space:]]*\(.*\)$/\2/p"	\
            $USER_CONF)

  # sets the configuration variables.
  MACHINE_CONF="machines/$machine/machine.conf"

  MACHINE_MK="machines/$machine/machine.mk"
  USER_MK="users/$KANETON_USER/user.mk"

  MACHINE_SH="machines/$machine/machine.sh"
  USER_SH="users/$KANETON_USER/user.sh"
}



#
# ENV_MK
#
# this function generates the env.mk makefile dependency.
#
env_mk()
{
  # removes the previous env.mk version.
  rm -f $ENV_MK

  # sets the source directory path.
  echo "_SRC_DIR_		:=		$SRC_DIR" >> $ENV_MK

  # copy the machine.conf contents into the env.mk makefile dependency.
  cat $MACHINE_CONF >> $ENV_MK

  # copy the user.conf contents into the env.mk makefile dependency.
  cat $USER_CONF >> $ENV_MK

  # copy the machine.mk contents into the env.mk makefile dependency.
  cat $MACHINE_MK >> $ENV_MK

  # copy the user.mk contents into the env.mk makefile dependency.
  cat $USER_MK >> $ENV_MK
}



#
# ENV_SH
#
# this function generates the env.sh file.
#
env_sh()
{
  # removes the previous env.conf version.
  rm -f $ENV_SH

  # creates an empty temporary file.
  makefile=$(mktemp)

  # generates the temporary makefile.
  echo "_SRC_DIR_		:=		$SRC_DIR" >> $makefile
  echo "include			$MACHINE_CONF" >> $makefile
  echo "include			$USER_CONF" >> $makefile
  echo "all:" >> $makefile

  # generates the env.sh shell script dependency.
  regexp="^\(_[[:alpha:]_]\+_\)[[:space:]]*\(=\|:=\)[[:space:]]*\(.*\)$"
  replacement="\1=\"\3\""

  make -p -f $makefile |						\
      sed -n "s/$regexp/$replacement/p" > $ENV_SH

  # appends the env.sh file the machine shell script specific code.
  cat $MACHINE_SH >> $ENV_SH

  # appends the env.sh file the user shell script specific code.
  cat $USER_SH >> $ENV_SH
}



#
# CHECK
#
# checks whether the user and machine directories are present to avoid
# problems.
#
check()
{
  # tries to find the user configuration file located in the user directory.
  if [ ! -e $USER_CONF ] ; then
    echo ""
    echo "[!] user configuration file '$USER_CONF' unreachable"
    echo ""
    echo "[!] please check your KANETON_USER environment variable"
    echo ""
    exit -1
  fi

  # read user configuration file.
  user_conf

  # tries to find the machine configuration file.
  if [ ! -e $MACHINE_CONF ] ; then
    echo ""
    echo "[!] machine configuration file '$MACHINE_CONF' unreachable"
    echo ""
    echo "[!] please check your _MACHINE_ variable into the '$USER_CONF' file"
    echo ""
    exit -1
  fi
}

#
# ---------- entry point ------------------------------------------------------
#

# checks the user and machine directory.
check

# gets the source directory path.
cd ..
SRC_DIR=$(pwd)
cd env

# generates the env.sh file.
env_sh

# generates the env.mk file.
env_mk

# from here include the env.sh to get access to every kaneton shell script
# functionalities.
source $ENV_SH

# displays some stuff.
display ""

display " environment files generated successfully" "+"
display ""

# asks the user to continue.
warning

# installs some links.
links

# calls the init function which will install machine-specific stuff.
init

# generates the prototypes.
proto

# generates the dependencies.
dep

# end.
display " environment installed successfully" "+"

# displays some stuff.
display ""
