#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/suite.py
#
# created       julien quintard   [wed oct 27 10:53:58 2010]
# updated       julien quintard   [mon nov  1 17:52:26 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import yaml
import os
import re

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".suite"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of suite identifiers.
#
def                     List(directory):
  suites = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      suites += [ entry[:-len(Extension)] ]

  return suites

#
# this function loads a suite.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
