#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANE...EST-SYSTEM/packages/ktp/configuration.py
#
# created       julien quintard   [mon oct 25 20:22:05 2010]
# updated       julien quintard   [wed oct 27 13:11:00 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import yaml

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".conf"

#
# ---------- functions --------------------------------------------------------
#

#
# this function loads a configuration.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
