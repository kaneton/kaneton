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
# updated       julien quintard   [wed oct 27 13:11:38 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import yaml

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".db"

#
# ---------- functions --------------------------------------------------------
#

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
