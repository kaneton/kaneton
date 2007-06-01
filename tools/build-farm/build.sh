#
# this file performs an automatic checkout of the kaneton repository
#
# then it tries to compile the microkernel and launches the testsuite
# making sure the microkernel behave well.
#
# finally, the script sends an email to the kaneton developers mailing
# list.
#

#
# global declarations
#

# set the PATH shell environment
export PATH="/usr/local/bin:/usr/bin:/bin"

# set the shell environment variables so that the kaneton
# environment engine can be set up properly
export KANETON_USER="build"
export KANETON_MACHINE='linux::ibm-pc.ia32-virtual'
export KANETON_SHELL='/bin/bash'

# the repository address
g_repository="svn+ssh://repositories.kaneton.org/kaneton"

# the kaneton microkernel image, once built
g_image="/home/mycure/kaneton/kaneton.img"

# the pts to use
g_pts=""

# the temporary directory
g_directory=$(mktemp -d)

# state: succes, failure or error
g_state=""

# the final report
g_report=""

# the debug message in case an error occured
g_debug=""

# the detail of the test suite execution
g_detail=""

# result variables
g_ntests=""
g_nsuccesses=""
g_nerrors=""

#
# ERROR
#
# this function is called whenever an error is detected
#
error()
{
  g_state="error"

  g_report="
------------------------------------------------------------------------ errors

${g_report}





------------------------------------------------------------------------- debug

${g_debug}
"

  email

  exit 0
}

#
# PTS
#
# this function determines the pts to use.
#
pts()
{
  output=""

  # go to the home directory: /home/mycure/kaneton
  cd "/home/mycure/kaneton"

  # launch qemu and quit, just to get the output.
  output=$(echo "
quit" | qemu -m 32m -fda pty.img -boot a -nographic -serial pty 2>&1)

  # get the pts from the output of qemu.
  g_debug=$(echo ${output} |  sed -rn "s/^.*(\/dev\/pts\/[0-9]*).*$/\1/p")

  if [ ${?} -ne 0 ] ; then
    g_report="
Unable to find any /dev/pts/X in the QEMU output
"
    error
  fi

  # set the pts device path.
  g_pts="${g_debug}"
}

#
# INITIALIZE
#
# this function initialize the build context
#
initialize()
{
  # kill any running QEMU
  killall -9 qemu

  # make sure of the umask
  umask 022

  # go to the temporary directory
  cd "${g_directory}"
}

#
# CLEAN
#
# this function cleans the previously created stuff
# like the temporary directory etc.
#
clean()
{
  pid=""

  # remove the temporary directory
  rm -Rf ${g_directory}

  # kill the running emulators
  killall -9 qemu
}

#
# REPOSITORY
#
# this function tries to get the repository
#
repository()
{
  # checkout the kaneton subversion repository based on the
  # ssh configuration: see ~/.ssh/
  g_debug=$(svn co --username build "${g_repository}" 2>&1)

  if [ ${?} -ne 0 ] ; then
    g_report="
Unable to checkout the kaneton repository at:

  ${g_repository}
"
    error
  fi
}

#
# ENVIRONMENT
#
# this function moves to the kaneton repository and tries to
# initialize the kaneton development environment for the
# 'build' user profile.
#
environment()
{
  # go into the kaneton directory
  cd "kaneton/"

  # initialize the kaneton environment
  g_debug=$(echo | make init 2>&1)

  if [ ${?} -ne 0 ] ; then
    g_report="
Unable to initialize the kaneton development environment.
"
    error
  fi
}

#
# KERNEL
#
# this function tries to build the kaneton microkernel
#
kernel()
{
  # try to build the kaneton microkernel
  g_debug=$(make kaneton 2>&1)

  if [ ${?} -ne 0 ] ; then
    g_report="
Unable to compile the kaneton microkernel.
"
    error
  fi
}

#
# IMAGE
#
# this function builds the kaneton boot image
#
image()
{
  # try to build the boot device
  g_debug=$(echo | make build 2>&1)

  if [ ${?} -ne 0 ] ; then
    g_report="
Unable to build the boot device.
"
    error
  fi

  # try to install the kaneton core and other stuff on the boot device
  g_debug=$(echo | make install 2>&1)

  if [ ${?} -ne 0 ] ; then
    g_report="
Unable to install the kaneton core and modules on the boot device.
"
    error
  fi

  g_debug=$(cp ${g_image} kaneton.img 2>&1)

  if [ ${?} -ne 0 ] ; then
    g_report="
Unable to copy the kaneton image.
"
    error
  fi
}

#
# PREPARE
#
# this function prepares the test suite.
#
prepare()
{
  # go into the python tool directory
  cd "tools/python/"

  # build a kaneton-sepcific module for serial communications
  g_debug=$(python mkmodules.py build 2>&1)

  if [ ${?} -ne 0 ] ; then
    g_report="
Unable to build the kserial python module.
"
    error
  fi

  # go back
  cd "../.."

  # finally, update the PYTHONPATH shell environment variable
  export PYTHONPATH="${g_directory}/kaneton/tools/python/build/lib.linux-i686-2.4/"
}

#
# LAUNCH
#
# this function launches the kaneton test suite.
#
launch()
{
  # go to the check/ directory.
  cd "check/"

  # launch the test suite.
  g_debug=$(python check_qemu.py "${g_pts}" build farm 2>&1)

  if [ ${?} -ne 0 ] ; then
    g_report="
An error occured during the kaneton test suite process
"
    error
  fi

  # kepp the detail
  g_detail="${g_debug}"

  # come back to the kaneton root directory
  cd "../"
}

#
# EVALUATE
#
# this function evaluates the test suite results.
#
evaluate()
{
  percentage=""
  file="check/build-farm.moul"

  # get the total number of lines
  g_ntests=$(cat "${file}" | grep " = " | wc -l)

  # get each type of test result
  g_nsuccesses=$(cat "${file}" | grep " = 1" | wc -l)
  g_nerrors=$(cat "${file}" | grep " = 0" | wc -l)

  if [ ${g_nerrors} -ne 0 ] ; then
    percentage=$(( ${g_nsuccesses} * 100 / ${g_ntests} ))

    g_state="failure --- ${percentage} %"
  else
    g_state="success"
  fi

  # build the file report
  g_report="
----------------------------------------------------------------------- summary

  ${g_ntests} tests

  ${g_nsuccesses} passed
  ${g_nerrors} failed





------------------------------------------------------------------------ detail

${g_detail}
"
}

#
# EMAIL
#
# this function sends an email to the kaneton developers mailing-list.
#
email()
{
  machine=""
  gcc=""
  ld=""

  # set the server name
  export SERVER_NAME='kaneton.org'

  # get information on the system
  machine=$(uname -rmpio)
  gcc=$(gcc --version)
  ld=$(ld --version)

  # add information to the report
  g_report="${g_report}





------------------------------------------------------------------------ system

${machine}

${gcc}

${ld}
"

  # finally email the report
  bash /home/mycure/kaneton/gmail.sh <<EOF
From: admin@kaneton.org
To: kaneton-developers@googlegroups.com
Subject: [build] ${g_state}

${g_report}
EOF
}

#
# MAIN
#
# this function performs the main work.
#
main()
{
  pts
  initialize
  repository
  environment
  kernel
  image
  prepare
  launch
  evaluate
  email
  clean
}

#
# ENTRY POINT
#
# call the main() function.
#

main
