#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/scripts/evaluate.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [tue nov  2 05:42:22 2010]
#

#
# ---------- information ------------------------------------------------------
#
# this script evaluates the kaneton snapshots having been submitted.
#
# the script starts by loading the grading system's file associated with
# the --grade option's argument. this file contains the set of emulated
# configurations in which the snapshots should be stressed against a specific
# test suite.
#
# the snapshot is then tested in each of these configuration, generating
# a set of reports. every one of these reports is then evaluated according
# to the points referenced in the grading system so that a score is calculated.
#
# note that not every configuration has the same importance. indeed, the
# grading system associates a _weight_ to every configuration. for instance,
# a test configuration with a weight of 0.6 would not be able to generate
# points above 60/100 or 12/20
#
# upon completion, the student's final grade is computed by adding the
# multiple weighted points of the test configurations.
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
GradesDirectory = TestDirectory + "/grades"
HooksDirectory = TestDirectory + "/hooks"
PointsDirectory = TestDirectory + "/points"
SuitesDirectory = TestDirectory + "/suites"
TestsDirectory = TestDirectory + "/tests"

# stores
DatabaseStore = StoreDirectory + "/database"
CapabilityStore = StoreDirectory + "/capability"
SnapshotStore = StoreDirectory + "/snapshot"
ReportStore = StoreDirectory + "/report"
StatementStore = StoreDirectory + "/statement"


# hooks
ConstructHook = "construct.py"
StressHook = "stress.py"

#
# ---------- globals ----------------------------------------------------------
#

# the command-line parser.
g_parser = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function tests the given snapshot according to the configuration.
#
def                     Test(namespace,
                             snapshot,
                             configuration):
  identifier = None
  date = None
  image = None
  bulletin = None
  status = None
  text = None
  report = None

  try:
    # create a unique identifier for the test context.
    identifier = time.strftime("%Y%m%d-%H%M%S")

    # retrieve the current data.
    date = time.strftime("%Y/%m/%d %H:%M:%S")

    # create a temporary files for the received snapshot, the about-to-be
    # generated image etc.
    image = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)
    bulletin = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # initialize the report.
    report = { "meta":
                 { "identifier": identifier,
                   "date": date,
                   "environments":
                     { "construct": configuration["environments"]["construct"],
                       "stress":  configuration["environments"]["stress"] },
                   "platform": configuration["platform"],
                   "architecture": configuration["architecture"],
                   "suite": configuration["suite"],
                   "duration": None },
               "data":
                 None }

    # retrieve the current time.
    start = time.time()

    # launch the build hook which generates a bootable image.
    (status, text) =                                                    \
      ktp.process.Invoke(HooksDirectory +                               \
                           "/" +                                        \
                           configuration["environments"]["construct"] + \
                           "/" +                                        \
                           configuration["platform"] +                  \
                           "." +                                        \
                           configuration["architecture"] +              \
                           "/" +                                        \
                           configuration["suite"] +                     \
                           "/" +                                        \
                           ConstructHook,
                         [ "--name", identifier,
                           "--snapshot", SnapshotStore + "/" +          \
                                           snapshot + ktp.snapshot.Extension,
                           "--image", image,
                           "--environment",
                             configuration["environments"]["construct"],
                           "--platform", configuration["platform"],
                           "--architecture", configuration["architecture"] ],
                         stream = stream,
                         option = ktp.process.OptionNone)

    # test the success of the construct hook invocation.
    if status == ktp.StatusError:
      raise Exception(text)

    # launch the build hook which generates a bootable image.
    (status, text) =                                                    \
      ktp.process.Invoke(HooksDirectory +                               \
                           "/" +                                        \
                           configuration["environments"]["stress"] +    \
                           "/" +                                        \
                           configuration["platform"] +                  \
                           "." +                                        \
                           configuration["architecture"] +              \
                           "/" +                                        \
                           configuration["suite"] +                     \
                           "/" +                                        \
                           StressHook,
                         [ "--name", identifier,
                           "--image", image,
                           "--environment",
                             configuration["environments"]["stress"],
                           "--suite", configuration["suite"],
                           "--bulletin", bulletin ],
                         stream = stream,
                         option = ktp.process.OptionNone)

    # test the success of the construct hook invocation.
    if status == ktp.StatusError:
      raise Exception(text)

    # retrieve the current time.
    end = time.time()

    # set the duration.
    report["meta"]["duration"] = "%.3f" % (end - start)

    # set the data.
    report["data"] = ktp.bulletin.Load(bulletin)
  except Exception, exception:
    # display the error, if verbose has been activated.
    if namespace.verbose:
      print("  " + ktp.trace.Generate())
  finally:
    # remove the temporary files.
    if stream:
      ktp.miscellaneous.Remove(stream)
    if bulletin:
      ktp.miscellaneous.Remove(bulletin)
    if image:
      ktp.miscellaneous.Remove(image)

    # store the report.
    ktp.report.Store(report,
                     ReportStore + "/" + identifier + ktp.report.Extension)

  return (report)

#
# this function takes a report and counts the points according to the
# given report and grade system.
#
def                     Rate(namespace, configuration, report):
  score = None
  notch = None
  stream = None
  component = None
  manifests = None
  manifest = None
  test = None
  name = None

  # read the suite file.
  stream = ktp.suite.Load(SuitesDirectory + "/" +                       \
                            configuration["suite"] + ktp.suite.Extension)

  # initialize the notch and score.
  score = 0
  notch = 0

  # load the manifests.
  if "components" in stream:
    for component in stream["components"]:
      # search for manifest files.
      manifests = ktp.miscellaneous.Search(TestsDirectory + "/" +       \
                                             stream["components"][component],
                                           "^.*\.mnf$",
                                           ktp.miscellaneous.OptionFile |
                                           ktp.miscellaneous.OptionRecursive)

      for manifest in manifests:
        # load the manifest.
        test = ktp.manifest.Load(manifest)

        # compute the proper name according to the defined component:
        # for instance, given the "segment" component, the test name
        # segment/permissions/01 will be transformed into permissions/01
        if component == test["name"][:len(component)]:
          name = test["name"][len(component) + 1:]
        else:
          name = test["name"]

        # add the points related to this test.
        notch += namespace.points[component + "/" + name]

  # return zero points should the test have failed.
  if not report["data"]:
    return (score, notch)

  # go through the components and tests composing the report.
  for component in  report["data"]:
    for test in report["data"][component]:
      if report["data"][component][test]["status"] == True:
        score += namespace.points[component + "/" + test]

  return (score, notch)

#
# this function evaluates the students' snapshots.
#
def                     Evaluate(namespace, statement):
  names = None
  name = None
  match = None
  capability = None
  database = None
  configuration = None
  report = None
  points = None
  point = None
  symbol = None

  # load the grade file.
  namespace.grade = ktp.grade.Load(GradesDirectory + "/" +              \
                                     namespace.stage + ktp.grade.Extension)

  # initialize the points.
  namespace.points = {}

  # load the points associated with this grading system.
  for point in namespace.grade["points"]:
    # load the points file.
    points = ktp.point.Load(PointsDirectory + "/" + point +             \
                              ktp.point.Extension)

    # merge the points with the global data structure.
    for symbol in points:
      namespace.points[symbol] = points[symbol]

  # retrieve the capability names.
  names = ktp.capability.List(CapabilityStore)

  # treat every capability.
  for name in names:
    # apply the pattern onto the name.
    match = re.search(namespace.pattern, name, re.MULTILINE)

    # ignore the name should it fail to match the pattern.
    if not match:
      continue

    # load the capability.
    capability = ktp.capability.Load(CapabilityStore + "/" +            \
                                       name + ktp.capability.Extension)

    # load the database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   capability["identifier"] +           \
                                   ktp.database.Extension)

    # initialize the student's statement.
    statement[capability["identifier"]] =                               \
        { "members": capability["members"],
          "date": database["deliveries"][namespace.stage]["date"],
          "snapshot": database["deliveries"][namespace.stage]["snapshot"],
          "configurations": {},
          "grade": 0.0 }

    # verbose messaging.
    if namespace.verbose:
      print(capability["identifier"] + ":")

    # for every grade configuration, test the snapshot.
    for configuration in namespace.grade["configurations"]:
      # verbose messaging.
      if namespace.verbose:
        print("  " + configuration["name"])

      # initialize this configuration.
      statement[capability["identifier"]]["configurations"][            \
        configuration["name"]] = { "report": None,
                                   "score": None,
                                   "notch": None }

      # launch the test.
      report = Test(namespace,
                    database["deliveries"][namespace.stage]["snapshot"],
                    configuration)

      # set the report in the statement.
      statement[capability["identifier"]]["configurations"][            \
        configuration["name"]]["report"] = report["meta"]["identifier"]

      # compute the score.
      (statement[capability["identifier"]]["configurations"][           \
         configuration["name"]]["score"],
       statement[capability["identifier"]]["configurations"][           \
         configuration["name"]]["notch"]) =                             \
           Rate(namespace, configuration, report)

      # add some points to the grade according to the weight
      # of the configuration.
      statement[capability["identifier"]]["grade"] +=                   \
        statement[capability["identifier"]]["configurations"][          \
          configuration["name"]]["score"] *                             \
        (namespace.reference * configuration["weight"]) /               \
         statement[capability["identifier"]]["configurations"][         \
           configuration["name"]]["notch"]

#
# this is the main function
#
def                     Main():
  global g_parser

  statement = {}

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script "         \
                                       "evaluates the submitted "       \
                                       "snapshots according to a "      \
                                       "grading system.")

  # set up the authorised arguments.
  g_parser.add_argument("--pattern", '-p',
                        default = "^.*$",
                        help = "the filter pattern to apply to the "    \
                          "names of the students to evaluate",
                        dest = "pattern")
  g_parser.add_argument("--stage", '-s',
                        required = True,
                        help = "the stage for which the students' "     \
                          "snapshots should be evaluated",
                        dest = "stage")
  g_parser.add_argument("--statement", '-t',
                        default = StatementStore + "/" +                \
                          time.strftime("%Y%m%d-%H%M%S") +              \
                          ktp.statement.Extension,
                        help = "the path to the output statement file",
                        dest = "statement")
  g_parser.add_argument("--verbose", '-v',
                        default = False,
                        action = "store_true",
                        help = "activate the verbose messaging",
                        dest = "verbose")
  g_parser.add_argument("--reference", '-r',
                        default = 20.0,
                        type = float,
                        help = "the grading reference",
                        dest = "reference")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # evaluate the work of the students matching the pattern.
  Evaluate(namespace, statement)

  # store the generated statement.
  ktp.statement.Store({ "meta":
                          { "stage": namespace.stage,
                            "reference": namespace.reference },
                        "data":
                          statement }, namespace.statement)

  # display a message.
  print("the statement has been saved in '" + namespace.statement + "'")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
