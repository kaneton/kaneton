#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/bulletin.py
#
# created       julien quintard   [mon oct 25 20:23:05 2010]
# updated       julien quintard   [wed oct 27 13:08:57 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import yaml

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".blt"

#
# ---------- functions --------------------------------------------------------
#

#
# this function stores a bulletin.
#
def                     Store(bulletin, path):
  yaml.dump(bulletin,
            file(path, 'w'))

#
# this function loads a bulletin.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
