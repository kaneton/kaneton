#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/scripts/file.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [sun mar  6 16:48:53 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script takes, for every student, the submission associated with
# a stage and copies it in an history directory.
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
# this function files the students' deliveries.
#
def                     File(namespace):
  stage = None
  deadline = None
  names = None
  name = None
  match = None
  capability = None
  database = None
  submission = None

  ktp.log.Record(LogStore,
                 "#(file) message(cleaning)")

  # load the stage file.
  stage = ktp.stage.Load(StagesDirectory + "/" +                        \
                           namespace.stage + ktp.stage.Extension)

  ktp.log.Record(LogStore,
                 "#(file) stage(" + str(stage) + ")")

  # retrieve the submission deadline.
  deadline = time.strptime(stage["deadline"], "%Y/%m/%d %H:%M:%S")

  # retrieve the capability names.
  names = ktp.capability.List(CapabilityStore)

  # treat every capability.
  for name in names:
    # apply the pattern onto the name.
    match = re.search(namespace.pattern, name, re.MULTILINE)

    # ignore the name should it fail to match the pattern.
    if not match:
      continue

    ktp.log.Record(LogStore,
                   "#(file) name(" + name + ")")

    # load the capability.
    capability = ktp.capability.Load(CapabilityStore + "/" +            \
                                       name +                           \
                                       ktp.capability.Extension)

    ktp.log.Record(LogStore,
                   "#(file) capability(" + str(capability) + ")")

    # load the database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   capability["identifier"] +           \
                                   ktp.database.Extension)

    ktp.log.Record(LogStore,
                   "#(file) database(" + str(database) + ")")

    # if no deliveries for this stage, continue.
    if (not "deliveries" in database) or                                \
       (not namespace.stage in database["deliveries"]) or               \
       (not database["deliveries"][namespace.stage]):
      ktp.log.Record(LogStore,
                     "#(file) message(no submission)")

      continue

    # retrieve the submission deadline.
    submission =                                                        \
      time.strptime(database["deliveries"][namespace.stage]["date"],
                    "%Y/%m/%d %H:%M:%S")

    # if the deadline has been reached, ignore this submission.
    if submission > deadline:
      ktp.log.Record(LogStore,
                     "#(file) warning(deadline reached)")

      continue

    # initialize the path based on the stage file.
    path = namespace.history + "/" + stage["history"]

    # compute the path depending on the variables and the
    # capability attributes.
    for attribute in capability["attributes"]:
      path = path.replace("%" + attribute + "%",
                          capability["attributes"][attribute])

    ktp.log.Record(LogStore,
                   "#(file) path(" + path + ")")

    # create the path.
    ktp.miscellaneous.Dig(path)

    # copy the submitted snapshot into the history.
    ktp.miscellaneous.Copy(                                             \
      SnapshotStore + "/" +                                             \
        database["deliveries"][namespace.stage]["snapshot"] +           \
        ktp.snapshot.Extension,
      path)

    ktp.log.Record(LogStore,
                   "#(file) message(submitted snapshot copied)")

#
# this function initializes the script.
#
def                     Initialize(namespace):
  ktp.log.Record(LogStore,
                 "#(file) message(initializing)")

#
# this function cleans the script.
#
def                     Clean(namespace):
  ktp.log.Record(LogStore,
                 "#(file) message(cleaning)")

#
# this is the main function
#
def                     Main():
  global g_parser

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script "         \
                                       "files the submitted snapshots " \
                                       "according to a given stage.")

  # set up the authorised arguments.
  g_parser.add_argument("--pattern", '-p',
                        default = "^.*$",
                        help = "the filter pattern to apply to the "    \
                          "names of the students to evaluate",
                        dest = "pattern")
  g_parser.add_argument("--stage", '-s',
                        required = True,
                        help = "the stage for which the students' "     \
                          "snapshots should be filed",
                        dest = "stage")
  g_parser.add_argument("--history", '-y',
                        required = True,
                        help = "the path to the history directory in "  \
                          "which the snapshots must be stored",
                        dest = "history")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # file the submissions.
  File(namespace)

  # clean the script.
  Clean(namespace)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
