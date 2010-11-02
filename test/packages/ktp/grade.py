#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/grade.py
#
# created       julien quintard   [mon oct 25 20:23:05 2010]
# updated       julien quintard   [mon nov  1 17:51:45 2010]
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

Extension = ".grd"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of grade identifiers.
#
def                     List(directory):
  grades = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      grades += [ entry[:-len(Extension)] ]

  return grades

#
# this function stores a grade.
#
def                     Store(grade, path):
  yaml.dump(grade,
            file(path, 'w'))

#
# this function loads a grade.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
