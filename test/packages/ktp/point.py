#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/point.py
#
# created       julien quintard   [mon oct 25 20:23:05 2010]
# updated       julien quintard   [mon nov  1 17:51:55 2010]
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

Extension = ".pts"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of point identifiers.
#
def                     List(directory):
  points = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      points += [ entry[:-len(Extension)] ]

  return points

#
# this function stores a point.
#
def                     Store(point, path):
  yaml.dump(point,
            file(path, 'w'))

#
# this function loads a point.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
