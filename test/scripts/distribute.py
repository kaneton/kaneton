#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/scripts/distribute.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [tue nov  2 05:25:45 2010]
#

#
# ---------- information ------------------------------------------------------
#
# the distribute script takes the set of capabilities--though this set can
# be controlled through the --pattern option---and sends an email to the
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
  content = None
  configuration = None
  message = None

  # create a temporary file.
  configuration = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # create a configuration file.
  content = """
set realname = "opaak admin"
set from = "admin@opaak.org"
set use_from = yes
"""

  # store the temporary mutt configuration file.
  ktp.miscellaneous.Push(content, configuration)

  # create a temporary file.
  message = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # retrieve the capability names.
  names = ktp.capability.List(CapabilityStore)

  # display a text.
  print("recipients:")

  # treat every capability.
  for name in names:
    # apply the pattern onto the name.
    match = re.search(namespace.pattern, name, re.MULTILINE)

    # ignore the name should it fail to match the pattern.
    if not match:
      continue

    # load the capability in order to retrieve the email.
    capability = ktp.capability.Load(CapabilityStore + "/" +            \
                                       name + ktp.capability.Extension)

    # create the message.
    content = """\
This email contains the capability of the student referred to as:

  %(identifier)s

In order to use this capability, please place it in your user profile
directory, at the location:

  kaneton/environment/profile/user/${KANETON_USER}/${KANETON_USER}.cap\
""" % { "identifier": capability["identifier"] }

    # store the temporary message.
    ktp.miscellaneous.Push(content, message)

    # initialize the emails variable.
    emails = []
    # compute the email addresses.
    for member in capability["members"]:
      emails += [ member["email"] ]

    # email the capability to the supposed recipient.
    (status, output) = ktp.process.Invoke("mutt",
                                          [ "-F", configuration,
                                            "-s", "'kaneton capability'",
                                            "-a", CapabilityStore + "/" + \
                                              name + ktp.capability.Extension,
                                            " ".join(emails),
                                            "<" + message ])

    # check the status.
    if status == ktp.StatusError:
      # display the output.
      print(output)

      # display the error message.
      print("an error occured while sending the email to '" +           \
              capability["email"] + "'")

    # display a message.
    print("  " + capability["identifier"])

  # remove the temporary files.
  ktp.miscellaneous.Remove(message)
  ktp.miscellaneous.Remove(configuration)

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
