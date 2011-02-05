#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/hooks/supervisor/watcher.py
#
# created       julien quintard   [thu feb  3 23:28:24 2011]
# updated       julien quintard   [fri feb  4 13:01:44 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this hook is triggered whenever the a snapshot is tested. the script
# updates its results bank by keeping the highest summary results
# for every student/environment/machine/suite.
#
# then, the 'notifier.py' script is triggered on a regular basis in order
# to provide the kaneton contributors of these results.
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
import yaml

import ktp

#
# ---------- constants --------------------------------------------------------
#

# directories
StoreDirectory = TestDirectory + "/store"
HooksDirectory = TestDirectory + "/hooks"

# stores
DatabaseStore = StoreDirectory + "/database"
CapabilityStore = StoreDirectory + "/capability"
SnapshotStore = StoreDirectory + "/snapshot"
ReportStore = StoreDirectory + "/report"
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
# this function updates the bank.
#
def                     Update(namespace):
  bank = None
  capability = None
  database = None
  report = None
  user = None
  environment = None
  machine = None
  champion = None
  match = None

  ktp.log.Record(LogStore,
                 "#(watcher) message(updating the bank according to " + \
                   "the new report)")

  ktp.log.Record(LogStore,
                 "#(watcher) identifier(" + namespace.identifier + ")")

  ktp.log.Record(LogStore,
                 "#(watcher) message(loading the report)")

  # load the report.
  report = ktp.report.Load(ReportStore + "/" +                          \
                             namespace.identifier +                     \
                             ktp.report.Extension)

  ktp.log.Record(LogStore,
                 "#(watcher) message(loading the capability)")

  # load the capability.
  capability = ktp.capability.Load(CapabilityStore + "/" +              \
                                     report["meta"]["user"] +           \
                                     ktp.capability.Extension)

  ktp.log.Record(LogStore,
                 "#(watcher) message(loading the database)")

  # load the database.
  database = ktp.database.Load(DatabaseStore + "/" +                    \
                                 report["meta"]["user"] +               \
                                 ktp.database.Extension)

  # apply the pattern onto the name, if required.
  if namespace.pattern:
    match = re.search(namespace.pattern,
                      capability["identifier"],
                      re.MULTILINE)

    # ignore the name should it fail to match the pattern.
    if not match:
      ktp.log.Record(LogStore,
                     "#(watcher) warning(non-matching pattern)")

      return

  # also ignore the suite if not matching the provided one.
  if namespace.suite and namespace.suite != report["meta"]["suite"]:
    ktp.log.Record(LogStore,
                   "#(watcher) warning(incompatible suite)")

    return

  # finally, ignore reports with errors.
  if "error" in report["meta"]["suite"]:
    ktp.log.Record(LogStore,
                   "#(watcher) warning(ignoring reports with errors)")

    return

  # load the statistics.
  statistics = Load()

  ktp.log.Record(LogStore,
                 "#(watcher) message(creating the necessary records)")

  # set the user
  user = capability["identifier"]

  # create the user record, if necessary.
  if not user in statistics:
    statistics[user] = {}

  # set the environment
  environment = report["meta"]["environments"]["stress"]

  # create the environment record, if necessary.
  if not environment in statistics[user]:
    statistics[user][environment] = {}

  # set the machine
  machine = report["meta"]["platform"] + "." + report["meta"]["architecture"]

  # create the machine record, if necessary.
  if not machine in statistics[user][environment]:
    statistics[user][environment][machine] = {}

  # set the suite
  suite = report["meta"]["suite"]

  # create the suite record, if necessary.
  if not suite in statistics[user][environment][machine]:
    statistics[user][environment][machine][suite] = {}

  ktp.log.Record(LogStore,
                 "#(watcher) message(updating the bank)")

  # update the suite record with the best report.
  if not statistics[user][environment][machine][suite]:
    statistics[user][environment][machine][suite] =                     \
      { "passed": report["meta"]["summary"]["passed"],
        "total": report["meta"]["summary"]["total"] }
  else:
    champion = statistics[user][environment][machine][suite]

    if report["meta"]["summary"]["passed"] > champion["passed"]:
      statistics[user][environment][machine][suite] =                   \
        { "passed": report["meta"]["summary"]["passed"],
          "total": report["meta"]["summary"]["total"] }

  # store the statistics.
  Store(statistics)

#
# this function loads the statistics file.
#
def                     Load():
  statistics = None
  path = None

  ktp.log.Record(LogStore,
                 "#(watcher) message(loading the bank)")

  path = HooksDirectory + "/statistics/bank.yml"

  if os.path.exists(path):
    statistics = yaml.load(file(path, 'r'))
  else:
    statistics = {}

  return statistics

#
# this function stores the statistics.
#
def                     Store(statistics):
  path = None

  ktp.log.Record(LogStore,
                 "#(watcher) message(storing the bank)")

  path = HooksDirectory + "/statistics/bank.yml"

  yaml.dump(statistics,
            file(path, 'w'))

#
# this is the main function
#
def                     Main():
  global g_parser

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script "         \
                                       "sends statistics to the "       \
                                       "kaneton contributors regarding "\
                                       "the current status of the "     \
                                       "student snapshots' testing")

  # set up the authorised arguments.
  g_parser.add_argument("--identifier", '-i',
                        required = True,
                        help = "the identifier of the snapshot having " \
                          "test",
                        dest = "identifier")
  g_parser.add_argument("--pattern", '-p',
                        default = "^.*$",
                        help = "the filter pattern to apply to the "    \
                          "names of the students to watch",
                        dest = "pattern")
  g_parser.add_argument("--suite", '-s',
                        default = None,
                        help = "the stage for which the students' "     \
                          "snapshots should be evaluated",
                        dest = "stage")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # update the bank.
  Update(namespace)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
