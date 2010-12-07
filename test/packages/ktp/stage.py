#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/stage.py
#
# created       julien quintard   [mon oct 25 20:23:05 2010]
# updated       julien quintard   [sun dec  5 22:15:02 2010]
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

Extension = ".stg"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of stage identifiers.
#
def                     List(directory):
  stages = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      stages += [ entry[:-len(Extension)] ]

  return stages

#
# this function stores a stage.
#
def                     Store(stage, path):
  yaml.dump(stage,
            file(path, 'w'))

#
# this function loads a stage.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
