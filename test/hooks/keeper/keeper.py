#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/hooks/monitor/monitor.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [sat mar  5 20:38:40 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script sends an email regarding the loop-mounted devices as it
# seems that a bug remains in the test system which leaves devices
# mounted.
#
# eventually, the maximum number of loopback devices is reached which results
# in the impossibility for the test system to operate normally.
#

#
# ---------- requirement ------------------------------------------------------
#

import os
import sys
import re

TestDirectory = os.path.abspath(os.path.dirname(                        \
                  os.path.realpath(sys.argv[0])) + "/../..")

sys.path.append(TestDirectory + "/packages")

#
# ---------- packages ---------------------------------------------------------
#

import time
import argparse

import ktp

#
# ---------- constants --------------------------------------------------------
#

# directories
StoreDirectory = TestDirectory + "/store"

# stores
DatabaseStore = StoreDirectory + "/database"
CapabilityStore = StoreDirectory + "/capability"
ReportStore = StoreDirectory + "/report"
LogStore = StoreDirectory + "/log"

# email address to which the sheet must be sent.
EmailAddress = "admin@opaak.org"

#
# ---------- globals ----------------------------------------------------------
#

# the command-line parser.
g_parser = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function emails the sheet to the administrators for instance.
#
def                     Email(namespace, content):
  # do nothing if the content is null.
  if not content:
    return

  ktp.log.Record(LogStore,
                 "#(keeper) message(emailing)")

  # send the email.
  ktp.miscellaneous.Email("admin@opaak.org",
                          [ EmailAddress ],
                          "'kaneton hook(keeper)'",
                          content)

#
# this function monitors the loopback resources.
#
def                     Keep(namespace):
  stream = None
  output = None

  # generate a temporary file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  ktp.log.Record(LogStore,
                 "#(keeper) message(launching the command)")

  # launch the losetup comman.
  status =                                                              \
    ktp.process.Invoke("losetup",
                       [ "-a" ],
                       stream = stream,
                       option = ktp.process.OptionNone)

  # retrieve the output.
  output = ktp.miscellaneous.Pull(stream)

  # remove file.
  ktp.miscellaneous.Remove(stream)

  return output

#
# this function initializes the script.
#
def                     Initialize(namespace):
  ktp.log.Record(LogStore,
                 "#(keeper) message(initializing)")

#
# this function cleans the script.
#
def                     Clean(namespace):
  ktp.log.Record(LogStore,
                 "#(keeper) message(cleaning)")

#
# this is the main function
#
def                     Main():
  global g_parser
  content = None

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script "         \
                                       "monitors the loopback resources")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # monitors the resources.
  content = Keep(namespace)

  # email the content.
  Email(namespace, content)

  # clean the script.
  Clean(namespace)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
