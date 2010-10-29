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
# updated       julien quintard   [wed oct 27 14:50:58 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import yaml
import os

import miscellaneous

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".rpt"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of paths to locally stored reports.
#
def                     List(directory):
  reports = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path):
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
