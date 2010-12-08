#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/scripts/evaluate.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [wed dec  8 22:08:20 2010]
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
# finally, noteworthy is that the --deadline option can be used in order to
# exclude---and assign a grade of zero---to any snapshot having been submitted
# after the given deadline date.
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

  try:
    # create a unique identifier for the test context.
    identifier = time.strftime("%Y%m%d:%H%M%S")

    # retrieve the current data.
    date = time.strftime("%Y/%m/%d %H:%M:%S")

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

    # store the report.
    ktp.report.Store(report,
                     ReportStore + "/" + identifier + ktp.report.Extension)

    # store the snapshot has being to one related to the report.
    ktp.miscellaneous.Copy(SnapshotStore + "/" +                        \
                             snapshot + ktp.snapshot.Extension,
                           SnapshotStore + "/" +                        \
                             identifier + ktp.snapshot.Extension)

    # generate a temporary file.
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # launch the test script.
    status =                                                            \
      ktp.process.Invoke(TestScript,
                         [ "--report", identifier ],
                         stream = stream,
                         option = ktp.process.OptionNone)

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    # test the success of the stress script invocation.
    if status == ktp.StatusError:
      raise Exception(output)
  except Exception, exception:
    # display the error, if verbose has been activated.
    if namespace.verbose:
      print("[trace] " + ktp.trace.Generate())
      print("[exception] " + str(exception))
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

  # initialize the score.
  score = 0

  # go through the report.
  if "data" in report:
    for component in report["data"]:
      for test in report["data"][component]:
        # has this test been passed?
        if report["data"][component][test]["status"] == True:
          # add the points associated with this test.
          score += manifests[test]["points"]

  return score

#
# this function returns true if the given delivery has reached to
# deadline.
#
def                     Dead(namespace, delivery):
  submission = None
  deadline = None

  # if not deadline has been provided, everything is alright.
  if not namespace.deadline:
    return False

  # retrieve the submission date.
  submission = time.strptime(delivery["date"], "%Y/%m/%d %H:%M:%S")

  # retrieve the deadline.
  deadline = time.strptime(namespace.deadline, "%Y/%m/%d %H:%M:%S")

  # if the delivery occured before the deadline, return OK.
  if submission < deadline:
    return False

  # verbose messaging.
  if namespace.verbose:
    print("  [deadline] reached")

  return True

#
# this function stores the submitted snapshot in the history directory,
# should this functionality have been activated.
#
def                     History(namespace, name, delivery):
  path = None

  # if no history directory has been provided, ignore this functionality.
  if not namespace.history:
    return

  # compute the destination path.
  path = os.path.abspath(namespace.history) + "/" +                     \
           name.replace("::", "/") + "/" +                              \
           "sources/" +                                                 \
           namespace.stage + ".tar.bz2"

  # create the directories to the destination path.
  ktp.miscellaneous.Dig(path)

  # copy the snapshot.
  ktp.miscellaneous.Copy(SnapshotStore + "/" +                          \
                           delivery["snapshot"] +                       \
                           ktp.snapshot.Extension,
                         path)

  # verbose messaging.
  if namespace.verbose:
    print("  [history] saved")

#
# this function evaluates the students' deliveries.
#
def                     Evaluate(namespace):
  statement = {}
  stage = None
  names = None
  name = None
  match = None
  capability = None
  database = None
  configuration = None
  suite = None
  manifests = None
  report = None

  # load the stage file.
  stage = ktp.stage.Load(StagesDirectory + "/" +                        \
                           namespace.stage + ktp.stage.Extension)

  # retrieve the capability names.
  names = ktp.capability.List(CapabilityStore)

  # treat every capability.
  for name in names:
    # apply the pattern onto the name.
    match = re.search(namespace.pattern, name, re.MULTILINE)

    # ignore the name should it fail to match the pattern.
    if not match:
      continue

    # verbose messaging.
    if namespace.verbose:
      print("[name] " + name)

    # load the capability.
    capability = ktp.capability.Load(CapabilityStore + "/" +            \
                                       name +                           \
                                       ktp.capability.Extension)

    # load the database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   capability["identifier"] +           \
                                   ktp.database.Extension)

    # if no deliveries, continue.
    if not "deliveries" in database:
      continue

    # initialize the statement.
    statement[capability["identifier"]] =                               \
      {
        "members": capability["members"],
        "date": database["deliveries"][namespace.stage]["date"],
        "snapshot": database["deliveries"][namespace.stage]["snapshot"],
        "configurations": {},
        "grade": 0.0
      }

    # store the snapshot in the history, if required.
    History(namespace, name, database["deliveries"][namespace.stage])

    # has the deadline been reached?
    if Dead(namespace, database["deliveries"][namespace.stage]) == True:
      continue

    # for every stage configuration, test the snapshot.
    for configuration in stage["configurations"]:
      # verbose messaging.
      if namespace.verbose:
        print("  [configuration] " + configuration["name"])

      # load the suite associated with this configuration.
      suite = ktp.suite.Load(SuitesDirectory + "/" +                    \
                             configuration["suite"] +                   \
                             ktp.suite.Extension)

      # load all the manifests related to this suite.
      manifests = ktp.suite.Manifests(suite)

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

  return statement

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
  g_parser.add_argument("--cap", '-c',
                        default = 20.0,
                        type = float,
                        help = "the grading cap",
                        dest = "cap")
  g_parser.add_argument("--history", '-y',
                        default = None,
                        help = "the path to a history directory "       \
                          "receiving the saved snapshots",
                        dest = "history")
  g_parser.add_argument("--deadline", '-d',
                        default = None,
                        help = "the deadline for which to consider "    \
                          "students' snapshots",
                        dest = "deadline")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # display a message.
  print("WARNING! the server should be stopped in order to prevent "    \
          "virtual machines to be run concurrently")

  # evaluate the submissions.
  statement = Evaluate(namespace)

  # store the generated statement.
  ktp.statement.Store({
                        "meta":
                          {
                            "stage": namespace.stage,
                            "cap": namespace.cap,
                            "deadline": namespace.deadline
                          },
                        "data":
                          statement
                      },
                      namespace.statement)

  # clean the script.
  Clean(namespace)

  # display a message.
  print("the statement has been saved in '" + namespace.statement + "'")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
