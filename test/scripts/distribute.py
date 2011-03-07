#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/scripts/distribute.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [fri feb 25 21:17:50 2011]
#

#
# ---------- information ------------------------------------------------------
#
# the distribute script takes the set of capabilities--though this set can
# be controlled through the '--pattern' option---and sends an email to the
# capability owner so that it can be used for triggering tests and submitting
# snapshots.
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

import signal
import time
import argparse

import ktp

#
# ---------- constants --------------------------------------------------------
#

# directories
StoreDirectory = TestDirectory + "/store"

# stores
CapabilityStore = StoreDirectory + "/capability"
LogStore = StoreDirectory + "/log"

#
# ---------- globals ----------------------------------------------------------
#

# the command-line parser.
g_parser = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function retrieves the capabilities and distributes them if they
# match the filtering pattern.
#
def                     Distribute(namespace):
  names = None
  name = None
  match = None
  capability = None
  message = None

  ktp.log.Record(LogStore,
                 "#(distribute) message(distributing the capabilities)")

  # retrieve the capability names.
  names = ktp.capability.List(CapabilityStore)

  # treat every capability.
  for name in names:
    ktp.log.Record(LogStore,
                   "#(distribute) name(" + name + ")")

    # apply the pattern onto the name.
    match = re.search(namespace.pattern, name, re.MULTILINE)

    # ignore the name should it fail to match the pattern.
    if not match:
      ktp.log.Record(LogStore,
                     "#(distribute) warning(non-matching pattern)")

      continue

    # load the capability in order to retrieve the email.
    capability = ktp.capability.Load(CapabilityStore + "/" +            \
                                       name + ktp.capability.Extension)

    ktp.log.Record(LogStore,
                   "#(distribute) capability(" + str(capability) + ")")

    # create the message.
    message = """\
This email contains an attached capability for the student or group referred
to as:

  %(identifier)s

In order to use this capability, please place it in your user profile
directory, at the location:

  environment/profile/user/${KANETON_USER}/${KANETON_USER}.cap\
""" % { "identifier": capability["identifier"] }

    ktp.log.Record(LogStore,
                   "#(distribute) body(" + message + ")")

    # initialize the emails variable.
    emails = []

    # compute the email addresses.
    for member in capability["members"]:
      emails += [ member["email"] ]

    ktp.log.Record(LogStore,
                   "#(distribute) emails(" + str(emails) + ")")

    # send the email.
    ktp.miscellaneous.Email("admin@opaak.org",
                            emails,
                            "'kaneton capability'",
                            message,
                            CapabilityStore + "/" +                     \
                              name + ktp.capability.Extension)

#
# this is the main function
#
def                     Main():
  global g_parser

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script builds "  \
                                       "a bootable image from the "     \
                                       "given kaneton snapshot.")

  # set up the authorised arguments.
  g_parser.add_argument("--pattern", '-p',
                        default = "^.*$",
                        help = "the filter pattern to apply to the "    \
                          "names of the capabilities to distribute",
                        dest = "pattern")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # distribute the capabilities matching the pattern.
  Distribute(namespace)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
