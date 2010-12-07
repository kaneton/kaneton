#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/statement.py
#
# created       julien quintard   [mon oct 25 20:23:05 2010]
# updated       julien quintard   [mon dec  6 07:45:36 2010]
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

Extension = ".stm"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of statement identifiers.
#
def                     List(directory):
  statements = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      statements += [ entry[:-len(Extension)] ]

  return statements

#
# this function stores a statement.
#
def                     Store(statement, path):
  yaml.dump(statement,
            file(path, 'w'))

#
# this function loads a statement.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
