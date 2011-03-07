#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/scripts/evaluate.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [mon mar  7 07:43:50 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script evaluates the kaneton snapshots having been submitted.
#
# the script starts by loading the stage system's file associated with
# the --stage option's argument. this file contains the set of emulated
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
# note that the server should be put to rest for the duration of the
# evaluation in order to prevent multiple virtual machines from running
# concurrently.
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
StagesDirectory = TestDirectory + "/stages"
SuitesDirectory = TestDirectory + "/suites"
TestsDirectory = TestDirectory + "/tests"
ScriptsDirectory = TestDirectory + "/scripts"

# stores
DatabaseStore = StoreDirectory + "/database"
CapabilityStore = StoreDirectory + "/capability"
SnapshotStore = StoreDirectory + "/snapshot"
ReportStore = StoreDirectory + "/report"
StatementStore = StoreDirectory + "/statement"
LogStore = StoreDirectory + "/log"

# scripts
TestScript = ScriptsDirectory + "/test.py"

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
def                     Error(namespace, message):
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
def                     Test(namespace, snapshot, configuration):
  identifier = None
  date = None
  status = None
  output = None
  report = None
  stream = None

  ktp.log.Record(LogStore,
                 "#(evaluate) message(testing the snapshot in " +       \
                   "a specific configuration)")

  try:
    # create a unique identifier for the test context.
    while True:
      identifier = time.strftime("%Y%m%d:%H%M%S")

      if not os.path.exists(SnapshotStore + "/" + identifier +          \
                              ktp.snapshot.Extension):
        break

    ktp.log.Record(LogStore,
                   "#(evaluate) identifier(" + identifier + ")")

    # retrieve the current data.
    date = time.strftime("%Y/%m/%d %H:%M:%S")

    ktp.log.Record(LogStore,
                   "#(evaluate) date(" + date + ")")

    # initialize the report.
    report = {
               "meta":
                 {
                   "identifier": identifier,
                   "date": date,
                   "environments":
                     {
                       "construct": configuration["environments"]["construct"],
                       "stress":  configuration["environments"]["stress"]
                     },
                   "platform": configuration["platform"],
                   "architecture": configuration["architecture"],
                   "suite": configuration["suite"],
                   "duration": None
                 },
               "data":
                 None
             }

    ktp.log.Record(LogStore,
                   "#(evaluate) report(" + str(report) + ")")

    # store the report.
    ktp.report.Store(report,
                     ReportStore + "/" + identifier + ktp.report.Extension)

    ktp.log.Record(LogStore,
                   "#(evaluate) message(copying the snapshot)")

    # store the snapshot has being to one related to the report.
    ktp.miscellaneous.Copy(SnapshotStore + "/" +                        \
                             snapshot + ktp.snapshot.Extension,
                           SnapshotStore + "/" +                        \
                             identifier + ktp.snapshot.Extension)

    # generate a temporary file.
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    ktp.log.Record(LogStore,
                   "#(evaluate) message(launching the test script)")

    # launch the test script.
    status =                                                            \
      ktp.process.Invoke(TestScript,
                         [ "--report", identifier ],
                         stream = stream,
                         option = ktp.process.OptionNone)

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    ktp.log.Record(LogStore,
                   "#(evaluate) status(" + str(status) + ") " +         \
                     "output(" + str(output) + ")")

    # load the report.
    report = ktp.report.Load(ReportStore + "/" + identifier +           \
                               ktp.report.Extension)

    # update the report's meta section if the whole process failed.
    if status == ktp.StatusError:
      # include the error message within the report's meta data.
      report["meta"]["error"] = output

      ktp.log.Record(LogStore,
                     "#(evaluate) message(recorded the error in the report)");

    # change the report's state to 'done'.
    report["meta"]["state"] = ktp.report.StateDone

    # store the report.
    ktp.report.Store(report,
                     ReportStore + "/" + identifier + ktp.report.Extension)
  except Exception, exception:
    ktp.log.Record(LogStore,
                   "#(evaluate) exception(" +                           \
                     str(exception) + ") trace(" +                      \
                     ktp.trace.Generate() + ")")
  finally:
    # remove the temporary files.
    if stream:
      ktp.miscellaneous.Remove(stream)

  # load the final report in order to send it back to the client.
  report = ktp.report.Load(ReportStore + "/" + identifier +             \
                             ktp.report.Extension)

  return (report)

#
# this function returns the maximum number of points associated with
# a given set of tests.
#
def                     Points(namespace, manifests):
  manifest = None
  points = None

  ktp.log.Record(LogStore,
                 "#(evaluate) message(computing the points)")

  # initialize the points to zero.
  points = 0

  # go through the manifests.
  for manifest in manifests:
    points += manifests[manifest]["points"]

  return points

#
# this function returns the number of points acquired by passing
# the tests recorded in the report.
#
def                     Score(namespace, manifests, report):
  score = None

  ktp.log.Record(LogStore,
                 "#(evaluate) message(computing the score)")

  # initialize the score.
  score = 0

  # go through the report.
  if ("data" in report) and (report["data"]):
    for component in report["data"]:
      for test in report["data"][component]:
        # has this test been passed?
        if report["data"][component][test]["status"] == True:
          # add the points associated with this test.
          score += manifests[test]["points"]

  return score

#
# this function evaluates the students' deliveries.
#
def                     Evaluate(namespace):
  statement = {}
  stage = None
  deadline = None
  names = None
  name = None
  match = None
  capability = None
  database = None
  submission = None
  configuration = None
  suite = None
  manifests = None
  report = None

  # load the stage file.
  stage = ktp.stage.Load(StagesDirectory + "/" +                        \
                           namespace.stage + ktp.stage.Extension)

  ktp.log.Record(LogStore,
                 "#(evaluate) stage(" + str(stage) + ")")

  # retrieve the submission deadline.
  deadline = time.strptime(stage["deadline"], "%Y/%m/%d %H:%M:%S")

  # retrieve the capability names.
  names = ktp.capability.List(CapabilityStore)

  # treat every capability.
  for name in names:
    ktp.log.Record(LogStore,
                   "#(evaluate) name(" + name + ")")

    # apply the pattern onto the name.
    match = re.search(namespace.pattern, name, re.MULTILINE)

    # ignore the name should it fail to match the pattern.
    if not match:
      ktp.log.Record(LogStore,
                     "#(evaluate) warning(non-matching pattern)")

      continue

    # load the capability.
    capability = ktp.capability.Load(CapabilityStore + "/" +            \
                                       name +                           \
                                       ktp.capability.Extension)

    ktp.log.Record(LogStore,
                   "#(evaluate) capability(" + str(capability) + ")")

    # load the database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   capability["identifier"] +           \
                                   ktp.database.Extension)

    ktp.log.Record(LogStore,
                   "#(evaluate) database(" + str(database) + ")")

    # if no deliveries, continue.
    if (not "deliveries" in database) or                                \
       (not namespace.stage in database["deliveries"]):
      ktp.log.Record(LogStore,
                     "#(evaluate) warning(no delivery)")

      continue

    # initialize the statement.
    statement[capability["identifier"]] =                               \
      {
        "members": capability["members"],
        "date": None,
        "snapshot": None,
        "configurations": {},
        "grade": 0.0
      }

    # if nothing has been delivered, continue.
    if not database["deliveries"][namespace.stage]:
      ktp.log.Record(LogStore,
                     "#(evaluate) warning(no delivery)")

      continue

    # set the delivery.
    statement[capability["identifier"]]["date"] =                       \
      database["deliveries"][namespace.stage]["date"]
    statement[capability["identifier"]]["snapshot"] =                   \
      database["deliveries"][namespace.stage]["snapshot"]

    # retrieve the submission deadline.
    submission =                                                        \
      time.strptime(database["deliveries"][namespace.stage]["date"],
                    "%Y/%m/%d %H:%M:%S")

    # if the deadline has been reached, ignore this submission.
    if submission > deadline:
      ktp.log.Record(LogStore,
                     "#(evaluate) warning(deadline reached)")

      continue

    # for every stage configuration, test the snapshot.
    for configuration in stage["configurations"]:
      ktp.log.Record(LogStore,
                     "#(evaluate) configuration(" + str(configuration) + ")")

      # load the suite associated with this configuration.
      suite = ktp.suite.Load(SuitesDirectory + "/" +                    \
                             configuration["suite"] +                   \
                             ktp.suite.Extension)

      ktp.log.Record(LogStore,
                     "#(evaluate) suite(" + str(suite) + ")")

      # load all the manifests related to this suite.
      manifests = ktp.suite.Manifests(suite)

      ktp.log.Record(LogStore,
                     "#(evaluate) manifests(" + str(manifests) + ")")

      # launch the test.
      report = Test(namespace,
                    database["deliveries"][namespace.stage]["snapshot"],
                    configuration)

      # initialize this statement's configuration.
      statement[capability["identifier"]][                              \
        "configurations"][configuration["name"]] =                      \
          {
            "report": report["meta"]["identifier"],
            "score": Score(namespace, manifests, report),
            "points": Points(namespace, manifests)
          }

      # add some points to the grade according to the weight
      # of the configuration.
      statement[capability["identifier"]]["grade"] +=                   \
        statement[capability["identifier"]][                            \
          "configurations"][configuration["name"]]["score"] *           \
        (namespace.cap * configuration["weight"]) /                     \
          statement[capability["identifier"]][                          \
            "configurations"][configuration["name"]]["points"]

      ktp.log.Record(LogStore,
                     "#(evaluate) statement(" +                         \
                       str(statement[capability["identifier"]]) + ")")

  return statement

#
# this function initializes the script.
#
def                     Initialize(namespace):
  ktp.log.Record(LogStore,
                 "#(evaluate) message(initializing)")

#
# this function cleans the script.
#
def                     Clean(namespace):
  ktp.log.Record(LogStore,
                 "#(evaluate) message(cleaning)")

#
# this is the main function
#
def                     Main():
  global g_parser

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
                        default = time.strftime("%Y%m%d-%H%M%S"),
                        help = "the path to the output statement file",
                        dest = "statement")
  g_parser.add_argument("--cap", '-c',
                        default = 20.0,
                        type = float,
                        help = "the grading cap",
                        dest = "cap")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # evaluate the submissions.
  statement = Evaluate(namespace)

  # store the generated statement.
  ktp.statement.Store({
                        "meta":
                          {
                            "stage": namespace.stage,
                            "cap": namespace.cap,
                          },
                        "data":
                          statement
                      },
                      StatementStore + "/" +                            \
                        namespace.statement +                           \
                        ktp.statement.Extension)

  ktp.log.Record(LogStore,
                 "#(evaluate) message(statement stored in '" +          \
                   namespace.statement + "')")

  # clean the script.
  Clean(namespace)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
