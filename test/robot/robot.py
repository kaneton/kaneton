#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/robot/robot.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [fri feb  4 15:37:16 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script is called on a regular basis in order to test the current
# kaneton research implementation.
#
# the script starts by retrieving the kaneton implementation by checking out
# the repository. then, several test suites are requested to the server.
#
# finally a report is build and sent to the kaneton contributors mailing list.
#

#
# ---------- requirement ------------------------------------------------------
#

import os
import sys
import re

TestDirectory = os.path.abspath(os.path.dirname(                        \
                  os.path.realpath(sys.argv[0])) + "/..")

sys.path.append(TestDirectory + "/packages")

#
# ---------- packages ---------------------------------------------------------
#

import time

import ktp

#
# ---------- constants --------------------------------------------------------
#

# the subversion address to the kaneton repository.
Repository = "svn+ssh://subversion@repositories.passeism.org/kaneton"

# environments.
Environments = [ ktp.environment.QEMU,
                 ktp.environment.Xen ]

#
# ---------- globals ----------------------------------------------------------
#

# the temporary directory.
g_directory = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function displays an error message, cleans the script before
# exiting.
#
def                     Error(message):
  # print the message.
  if message:
    print(message)

  # clean the script.
  Clean()

  # exit with an error code.
  sys.exit(42)

#
# this function retrieves the latest kaneton snapshot through
# Subversion.
#
def                     Checkout():
  # launch Subversion.
  if ktp.process.Invoke("svn",
                        [ "co",
                          Repository,
                          g_directory]) == ktp.StatusError:
    Error("unable to check out the kaneton repository")

#
# this function triggers some test through the kaneton
# test client.
#
def                     Test():
  report = None
  summary = None
  detail = None
  reports = None
  report = None
  stream = None
  status = None
  output = None

  # set the kaneton environment variables.
  os.putenv("KANETON_PYTHON", "/usr/bin/python")
  os.putenv("KANETON_HOST", "linux/ia32")
  os.putenv("KANETON_USER", "robot")
  os.putenv("KANETON_PLATFORM", "ibm-pc")
  os.putenv("KANETON_ARCHITECTURE", "ia32/educational")

  # initialize the summary and detail strings.
  summary = str()
  detail = str()

  # create a temporary file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # request a test suite through the test client
  status =                                                              \
    ktp.process.Invoke("make",
                       [ "-C", g_directory,
                         "initialize" ],
                       stream = stream,
                       option = ktp.process.OptionNone)

  # retrieve the output.
  output = ktp.miscellaneous.Pull(stream)

  # remove the stream file.
  ktp.miscellaneous.Remove(stream)

  # if the construction was successful, exit the loop.
  if status == ktp.StatusError:
    if not output:
      Error("an error occured while initializing the kaneton environment")
    else:
      Error(output)

  # for every environment to test...
  for environment in Environments:
    # create a temporary file.
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # request a test suite through the test client
    status =                                                            \
      ktp.process.Invoke("make",
                         [ "-C", g_directory + "/test/client",
                           "test-" + environment + "::kaneton" ],
                         stream = stream,
                         option = ktp.process.OptionNone)

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    # if the construction was successful, exit the loop.
    if status == ktp.StatusError:
      if not output:
        Error("an error occured while launching the test client")
      else:
        Error(output)

#
# this function initializes the script.
#
def                     Initialize():
  global g_directory

  # create a temporary directory.
  g_directory = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionDirectory)

#
# this function cleans what has been created by this script.
#
def                     Clean():
  # remove the disk directory.
  if g_directory:
    ktp.miscellaneous.Remove(g_directory)

#
# this is the main function
#
def                     Main():
  # initialize the script.
  Initialize()

  # checkout the kaneton repository.
  Checkout()

  # trigger the tests.
  Test()

  # clean the temporary stuff.
  Clean()

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
