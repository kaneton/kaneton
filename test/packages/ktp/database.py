#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/database.py
#
# created       julien quintard   [mon oct 25 20:23:05 2010]
# updated       julien quintard   [thu feb  3 23:58:14 2011]
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

Extension = ".db"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of database identifiers.
#
def                     List(directory):
  databases = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      databases += [ entry[:-len(Extension)] ]

  return databases

#
# this function generates a database based on the given configuration.
#
def                     Generate(configuration):
  return configuration

#
# this function stores a database.
#
def                     Store(database, path):
  yaml.dump(database,
            file(path, 'w'))

#
# this function loads a database.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
