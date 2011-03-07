#! /usr/bin/python2
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
# updated       julien quintard   [sun mar  6 17:19:15 2011]
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
LogStore = StoreDirectory + "/log"

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
# this function computes a summary indicating the successfulness of the
# testing.
#
def                     Summary(report):
  passed = None
  failed = None
  total = None
  component = None
  test = None

  ktp.log.Record(LogStore,
                 "#(test) message(computing the summary)")

  # initialize the counters.
  passed = 0
  failed = 0
  total = 0

  if ("data" in report) and (report["data"]):
    # go through the report.
    for component in report["data"]:
      for test in report["data"][component]:
        # update the total number of tests
        total = total + 1

        # has this test been passed?
        if report["data"][component][test]["status"] == True:
          passed = passed + 1
        else:
          failed = failed + 1

  report["meta"]["summary"] = { "passed": passed,
                                "failed": failed,
                                "total": total }

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
  date = None

  try:
    # load the report.
    report = ktp.report.Load(ReportStore + "/" +                        \
                               namespace.report + ktp.report.Extension)

    # retrieve the current date.
    date = time.strftime("%Y/%m/%d %H:%M:%S")

    ktp.log.Record(LogStore,
                   "#(test) date(" + date + ")")

    # reset some of the report attributes.
    report["meta"]["date"] = date
    report["meta"]["duration"] = None

    report["data"] = None

    # remove any previous error.
    if "error" in report["meta"]:
      del report["meta"]["error"]

    # remove any previous summary.
    if "summary" in report["meta"]:
      del report["meta"]["summary"]

    # create a temporary files for the received snapshot, the about-to-be
    # generated image etc.
    image = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)
    bulletin = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # retrieve the current time.
    start = time.time()

    ktp.log.Record(LogStore,
                   "#(test) message(constructing the image '" +    \
                     image + "')");

    # try to build the kaneton image...
    for index in range(0, Rounds[ConstructPhase]):
      ktp.log.Record(LogStore,
                     "#(test) round(" + str(index) + ")")

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

      ktp.log.Record(LogStore,
                     "#(test) status(" + str(status) + ") " +           \
                       "output(" + str(output) + ")")

      # if the construction was successful, exit the loop.
      if status == ktp.StatusOk:
        break

    # test the success of the construct script invocation.
    if status == ktp.StatusError:
      Error(namespace, output + "\n[rounds] " + str(Rounds[ConstructPhase]))

    ktp.log.Record(LogStore,
                   "#(test) message(stressing the image)")

    for index in range(0, Rounds[StressPhase]):
      ktp.log.Record(LogStore,
                     "#(test) round(" + str(index) + ")")

      # launch the build script which generates a bootable image.
      status =                                                          \
        ktp.process.Invoke(StressScript,
                           [ "--name", report["meta"]["identifier"],
                             "--image", image,
                             "--environment",                           \
                                 report["meta"]["environments"]["stress"],
                             "--suite", report["meta"]["suite"],
                             "--bulletin", bulletin ],
                           stream = stream,
                           option = ktp.process.OptionNone)

      # retrieve the output.
      output = ktp.miscellaneous.Pull(stream)

      # remove the stream file.
      ktp.miscellaneous.Remove(stream)

      ktp.log.Record(LogStore,
                     "#(test) status(" + str(status) + ") " +           \
                       "output(" + str(output) + ")")

      # if the stress was successful, exit the loop.
      if status == ktp.StatusOk:
        break

    # test the success of the stress script invocation.
    if status == ktp.StatusError:
      Error(namespace, output + "\n[rounds] " + str(Rounds[StressPhase]))

    # retrieve the current time.
    end = time.time()

    ktp.log.Record(LogStore,
                   "#(test) duration(" + "%.3f" % (end - start) + ")")

    # complete the report according to the bulletin.
    report["meta"]["duration"] = "%.3f" % (end - start)
    report["data"] = ktp.bulletin.Load(bulletin)

    # compute a summary of the test.
    Summary(report)

    # store the report.
    ktp.report.Store(report,
                     ReportStore + "/" +                                \
                       report["meta"]["identifier"] + ktp.report.Extension)

    ktp.log.Record(LogStore,
                   "#(test) message(updated the report")
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

    ktp.log.Record(LogStore,
                   "#(test) message(cleaned the temporary files")

#
# this function initializes the script.
#
def                     Initialize(namespace):
  ktp.log.Record(LogStore,
                 "#(test) message(initializing)")

#
# this function cleans the script.
#
def                     Clean(namespace):
  ktp.log.Record(LogStore,
                 "#(test) message(cleaning)")

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

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # test the given report's snapshot i,e re-generate the report.
  Test(namespace)

  # clean the script.
  Clean(namespace)

  ktp.log.Record(LogStore,
                 "#(test) message(report stored in '" +                 \
                   namespace.report + "')")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
