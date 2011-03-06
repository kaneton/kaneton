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
# this script explores the reports generated so far for a given suite and
# keeps only the best report for every student/group.
#
# a sheet summarising these results is build and sent to the administrators.
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
EmailAddress = "contributors@kaneton.opaak.org"

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
def                     Email(namespace, sheet):
  content = None

  ktp.log.Record(LogStore,
                 "#(monitor) message(emailing)")

  # initalize the content.
  content = str()

  # build the message
  for record in sheet:
    if not sheet[record]:
      content += """\
%(name)s:
""" % { "name": record }
    else:
      content += """\
%(name)s: summary(%(summary)s) identifier(%(identifier)s)
""" % { "name": record,
        "summary":
          str(sheet[record]["meta"]["summary"]["passed"]) + "/" +
            str(sheet[record]["meta"]["summary"]["total"]),
        "identifier": sheet[record]["meta"]["identifier"] }

  ktp.log.Record(LogStore,
                 "#(monitor) content(" + content + ")") 

  # send the email.
  ktp.miscellaneous.Email("admin@opaak.org",
                          [ EmailAddress ],
                          "'kaneton hook(monitor) suite(" +             \
                            namespace.suite + ")'",
                          content)

#
# this function monitors a suite, building a sheet reporting the students'
# progress.
#
def                     Monitor(namespace):
  sheet = {}
  names = None
  name = None
  match = None
  environment = None
  machine = None
  suite = None
  identifier = None
  report = None
  champion = None

  # retrieve the capability names.
  names = ktp.capability.List(CapabilityStore)

  # treat every capability.
  for name in names:
    ktp.log.Record(LogStore,
                   "#(monitor) name(" + name + ")")

    # apply the pattern onto the name.
    match = re.search(namespace.pattern, name, re.MULTILINE)

    # ignore the name should it fail to match the pattern.
    if not match:
      ktp.log.Record(LogStore,
                     "#(monitor) warning(non-matching pattern)")

      continue

    # load the capability.
    capability = ktp.capability.Load(CapabilityStore + "/" +            \
                                       name +                           \
                                       ktp.capability.Extension)

    ktp.log.Record(LogStore,
                   "#(monitor) capability(" + str(capability) + ")")

    # load the database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   capability["identifier"] +           \
                                   ktp.database.Extension)

    ktp.log.Record(LogStore,
                   "#(monitor) database(" + str(database) + ")")

    # ignore this database if no reports are present.
    if not "reports" in database:
      ktp.log.Record(LogStore,
                     "#(monitor) warning(no reports present)")

      continue

    # initialize the champion.
    champion = None

    # go through the environments.
    for environment in database["reports"]:
      ktp.log.Record(LogStore,
                     "#(monitor) environment(" + environment + ")")

      # go through the machines.
      for machine in database["reports"][environment]:
        ktp.log.Record(LogStore,
                       "#(monitor) machine(" + machine + ")")

        # set the suite.
        suite = namespace.suite

        # ignore this machine if no suite is present.
        if (not suite in database["reports"][environment][machine]) or  \
           (not database["reports"][environment][machine][suite]):
          continue

        # go through the reports associated with the given suite.
        for identifier in database["reports"][environment][machine][suite]:
          ktp.log.Record(LogStore,
                         "#(monitor) report(" + identifier + ")")

          # load the report.
          report = ktp.report.Load(ReportStore + "/" + identifier +     \
                                     ktp.report.Extension)

          # ignore report that are in progress or which failed.
          if (report["meta"]["state"] != ktp.report.StateDone) or       \
             ("error" in report["meta"]):
            ktp.log.Record(LogStore,
                           "#(monitor) warning(unsuccessful report)")

            continue

          # keep the best test so far.
          if (not champion) or                                          \
             (report["meta"]["summary"]["passed"] >                     \
                champion["meta"]["summary"]["passed"]):
            champion = report

    ktp.log.Record(LogStore,
                   "#(monitor) champion(" + str(champion) + ")")

    # store the student's best report.
    sheet[capability["identifier"]] = champion

  return sheet

#
# this function initializes the script.
#
def                     Initialize(namespace):
  ktp.log.Record(LogStore,
                 "#(monitor) message(initializing)")

#
# this function cleans the script.
#
def                     Clean(namespace):
  ktp.log.Record(LogStore,
                 "#(monitor) message(cleaning)")

#
# this is the main function
#
def                     Main():
  global g_parser
  sheet = None

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script "         \
                                       "monitors the test reports "     \
                                       "generated.")

  # set up the authorised arguments.
  g_parser.add_argument("--pattern", '-p',
                        default = "^.*$",
                        help = "the filter pattern to apply to the "    \
                          "names of the students to consider",
                        dest = "pattern")
  g_parser.add_argument("--suite", '-s',
                        required = True,
                        help = "the suite to monitor",
                        dest = "suite")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # monitor the students' reports.
  sheet = Monitor(namespace)

  # email the sheet.
  Email(namespace, sheet)

  # clean the script.
  Clean(namespace)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
