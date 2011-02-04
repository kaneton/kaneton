#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/report.py
#
# created       julien quintard   [tue oct 26 11:12:10 2010]
# updated       julien quintard   [wed feb  2 14:41:38 2011]
#

#
# ---------- packages ---------------------------------------------------------
#

import yaml
import os
import re

import miscellaneous

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".rpt"

StateInProgress = "in progress"
StateDone = "done"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of report identifiers.
#
def                     List(directory):
  reports = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      reports += [ entry[:-len(Extension)] ]

  return reports

#
# this function stores a report.
#
def                     Store(report, path):
  yaml.dump(report,
            file(path, 'w'))

#
# this function loads a report.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
