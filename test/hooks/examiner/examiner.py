#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/hooks/examiner/examiner.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [sat mar  5 20:42:26 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script explores the submissions related to a given stage.
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
  record = None

  ktp.log.Record(LogStore,
                 "#(examiner) message(emailing)")

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
%(name)s: identifier(%(identifier)s) date(%(date)s)
""" % { "name": record,
        "identifier": sheet[record]["snapshot"],
        "date": sheet[record]["date"] }

  ktp.log.Record(LogStore,
                 "#(examiner) content(" + content + ")") 

  # send the email.
  ktp.miscellaneous.Email("admin@opaak.org",
                          [ EmailAddress ],
                          "'kaneton hook(examiner) stage(" +            \
                            namespace.stage + ")'",
                          content)

#
# this function examines a stage, building a sheet reporting the students'
# progress.
#
def                     Examine(namespace):
  sheet = {}
  names = None
  name = None
  match = None
  stage = None

  # retrieve the capability names.
  names = ktp.capability.List(CapabilityStore)

  # treat every capability.
  for name in names:
    ktp.log.Record(LogStore,
                   "#(examiner) name(" + name + ")")

    # apply the pattern onto the name.
    match = re.search(namespace.pattern, name, re.MULTILINE)

    # ignore the name should it fail to match the pattern.
    if not match:
      ktp.log.Record(LogStore,
                     "#(examiner) warning(non-matching pattern)")

      continue

    # load the capability.
    capability = ktp.capability.Load(CapabilityStore + "/" +            \
                                       name +                           \
                                       ktp.capability.Extension)

    ktp.log.Record(LogStore,
                   "#(examiner) capability(" + str(capability) + ")")

    # load the database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   capability["identifier"] +           \
                                   ktp.database.Extension)

    ktp.log.Record(LogStore,
                   "#(examiner) database(" + str(database) + ")")

    # ignore this database if no deliveries are present.
    if (not "deliveries" in database) or                                \
       (not namespace.stage in database["deliveries"]):
      ktp.log.Record(LogStore,
                     "#(examiner) warning(no delivery)")

      continue

    ktp.log.Record(LogStore,
                   "#(examiner) delivery(" +                            \
                     str(database["deliveries"][namespace.stage]) + ")")

    # store the student's best report.
    sheet[capability["identifier"]] = database["deliveries"][namespace.stage]

  return sheet

#
# this function initializes the script.
#
def                     Initialize(namespace):
  ktp.log.Record(LogStore,
                 "#(examiner) message(initializing)")

#
# this function cleans the script.
#
def                     Clean(namespace):
  ktp.log.Record(LogStore,
                 "#(examiner) message(cleaning)")

#
# this is the main function
#
def                     Main():
  global g_parser
  sheet = None

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script "         \
                                       "examines the snapshots "        \
                                       "submitted by the students.")

  # set up the authorised arguments.
  g_parser.add_argument("--pattern", '-p',
                        default = "^.*$",
                        help = "the filter pattern to apply to the "    \
                          "names of the students to consider",
                        dest = "pattern")
  g_parser.add_argument("--stage", '-s',
                        required = True,
                        help = "the stage to examine",
                        dest = "stage")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # examine the students' submissions.
  sheet = Examine(namespace)

  # email the sheet.
  Email(namespace, sheet)

  # clean the script.
  Clean(namespace)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
