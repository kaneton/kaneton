#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/manifest.py
#
# created       julien quintard   [wed oct 27 10:53:58 2010]
# updated       julien quintard   [tue dec  7 13:41:02 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import yaml

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".mnf"

#
# ---------- functions --------------------------------------------------------
#

#
# this function loads a manifest.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
