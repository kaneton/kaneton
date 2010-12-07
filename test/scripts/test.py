#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/scripts/test.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [tue dec  7 17:15:35 2010]
#

#
# ---------- information ------------------------------------------------------
#
# This script takes a partially-, or fully-, completely report, extracts the
# identifier of the snapshot associated with this report and builds an
# image before stressing it in an virtual environment.
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
import argparse

import ktp

#
# ---------- constants --------------------------------------------------------
#

# directories
StoreDirectory = TestDirectory + "/store"
ScriptsDirectory = TestDirectory + "/scripts"

# stores
ReportStore = StoreDirectory + "/report"
SnapshotStore = StoreDirectory + "/snapshot"
CapabilityStore = StoreDirectory + "/capability"

# scripts
ConstructScript = ScriptsDirectory + "/construct.py"
StressScript = ScriptsDirectory + "/stress.py"

# phases
ConstructPhase = 0
StressPhase = 1

# rounds: qemu/xen.
Rounds = [ 3, 3 ]

#
# ---------- globals ----------------------------------------------------------
#

# the command-line parser.
g_parser = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function displays an error message, cleans the script before
# exiting.
#
def                     Error(namespace,
                              message):
  # print the message.
  if message:
    print(message)

  # clean the script.
  Clean(namespace)

  # exit with an error code.
  sys.exit(42)

#
# this function tests the given snapshot according to the configuration.
#
def                     Test(namespace):
  report = None
  image = None
  bulletin = None
  stream = None
  output = None
  start = None
  end = None
  index = None

  try:
    # load the report.
    report = ktp.report.Load(ReportStore + "/" +                        \
                               namespace.report + ktp.report.Extension)

    # create a temporary files for the received snapshot, the about-to-be
    # generated image etc.
    image = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)
    bulletin = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # retrieve the current time.
    start = time.time()

    if namespace.verbose:
      print("constructing the image '" + image + "'")

    # try to build the kaneton image...
    for index in range(0, Rounds[ConstructPhase]):
      # launch the construct script which generates a bootable image.
      status =                                                          \
        ktp.process.Invoke(ConstructScript,
                             [ "--name", report["meta"]["identifier"],
                               "--snapshot", SnapshotStore + "/" +      \
                                 report["meta"]["identifier"] +         \
                                 ktp.snapshot.Extension,
                               "--image", image,
                               "--environment",                         \
                                 report["meta"]["environments"]["construct"],
                               "--platform",                            \
                                 report["meta"]["platform"],
                               "--architecture",                        \
                                 report["meta"]["architecture"] ],
                           stream = stream,
                           option = ktp.process.OptionNone)

      # retrieve the output.
      output = ktp.miscellaneous.Pull(stream)

      # remove the stream file.
      ktp.miscellaneous.Remove(stream)

      # if the construction was successful, exit the loop.
      if status == ktp.StatusOk:
        break

    # test the success of the construct script invocation.
    if status == ktp.StatusError:
      Error(namespace, output)

    if namespace.verbose:
      print("stressing the image '" + image + "'")

    for index in range(0, Rounds[StressPhase]):
      # launch the build script which generates a bootable image.
      status =                                                          \
        ktp.process.Invoke(StressScript,
                           [ "--name", report["meta"]["identifier"],
                             "--image", image,
                             "--environment",                           \
                                 report["meta"]["environments"]["stress"],
                             "--suite", report["meta"]["suite"],
                             "--bulletin", bulletin,
                             "--verbose" ],
                           stream = stream,
                           option = ktp.process.OptionNone)

      # retrieve the output.
      output = ktp.miscellaneous.Pull(stream)

      # remove the stream file.
      ktp.miscellaneous.Remove(stream)

      # if the stress was successful, exit the loop.
      if status == ktp.StatusOk:
        break

    # test the success of the stress script invocation.
    if status == ktp.StatusError:
      Error(namespace, output)

    # retrieve the current time.
    end = time.time()

    # complete the report according to the bulletin.
    report["meta"]["duration"] = "%.3f" % (end - start)
    report["data"] = ktp.bulletin.Load(bulletin)

    # store the report.
    ktp.report.Store(report,
                     ReportStore + "/" +                                \
                       report["meta"]["identifier"] + ktp.report.Extension)
  except Exception, exception:
    Error(namespace, ktp.trace.Generate())
  finally:
    # remove the temporary files.
    if stream:
      ktp.miscellaneous.Remove(stream)
    if bulletin:
      ktp.miscellaneous.Remove(bulletin)
    if image:
      ktp.miscellaneous.Remove(image)

#
# this function emails the generated report to its owner.
#
def                     Email(namespace):
  report = None
  capability = None
  content = None
  emails = None
  message = None
  configuration = None

  try:
    # load the report.
    report = ktp.report.Load(ReportStore + "/" +                        \
                               namespace.report + ktp.report.Extension)

    # load the capability.
    capability = ktp.capability.Load(CapabilityStore + "/" +            \
                                       report["meta"]["user"] +         \
                                       ktp.capability.Extension)

    # create a temporary file.
    configuration = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # create a configuration file.
    content = """\
set realname = "opaak admin"
set from = "admin@opaak.org"
set use_from = yes\
"""

    # store the temporary mutt configuration file.
    ktp.miscellaneous.Push(content, configuration)

    # create the message.
    content = """\
This email contains the report referred to as:

  %(identifier)s

In order to consult this report, please place it in your store
directory, at the location:

  kaneton/test/store/report/\
""" % { "identifier": report["meta"]["identifier"] }

    # create a temporary file.
    message = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # store the temporary message.
    ktp.miscellaneous.Push(content, message)

    # initialize the emails variable.
    emails = []
    # compute the email addresses.
    for member in capability["members"]:
      emails += [ member["email"] ]

    # create a stream file.
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # email the capability to the supposed recipient.
    status = ktp.process.Invoke("mutt",
                                [ "-F", configuration,
                                  "-s", "'kaneton report'",
                                  "-a", ReportStore + "/" +             \
                                    report["meta"]["identifier"] +      \
                                    ktp.report.Extension,
                                  " ".join(emails),
                                  "<" + message ])

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    # check the status.
    if status == ktp.StatusError:
      raise Exception(output)
  except Exception, exception:
    # display the exception.
    if exception:
      print(str(exception))

    # display the error message.
    Error(namespace,
          "an error occured while sending the email to '" +             \
          report["meta"]["identifier"] + "'")
  finally:
    # remove the temporary files.
    ktp.miscellaneous.Remove(message)
    ktp.miscellaneous.Remove(configuration)

#
# this function initializes the script.
#
def                     Initialize(namespace):
  pass

#
# this function cleans the script.
#
def                     Clean(namespace):
  pass

#
# this is the main function
#
def                     Main():
  global g_parser

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script "         \
                                       "tests the snapshot associated " \
                                       "with the given report.")

  # set up the authorised arguments.
  g_parser.add_argument("--report", '-r',
                        required = True,
                        help = "the identifier of the report to evaluate",
                        dest = "report")
  g_parser.add_argument("--email", '-e',
                        default = False,
                        action = "store_true",
                        help = "this option tells the script to send "  \
                          "the report by email to its owner",
                        dest = "email")
  g_parser.add_argument("--verbose", '-v',
                        default = False,
                        action = "store_true",
                        help = "activate the verbose messaging",
                        dest = "verbose")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # test the given report's snapshot i,e re-generate the report.
  Test(namespace)

  # potentially email the report to its user.
  if namespace.email:
    Email(namespace)

  # clean the script.
  Clean(namespace)

  # display a message.
  print("the report has been completed or re-generated in '" +          \
            namespace.report + "'")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
