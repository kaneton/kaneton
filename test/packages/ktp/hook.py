#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/hook.py
#
# created       julien quintard   [mon oct 25 19:58:10 2010]
# updated       julien quintard   [thu feb  3 23:56:36 2011]
#

#
# ---------- packages ---------------------------------------------------------
#

import os
import yaml

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".hk"

#
# ---------- functions --------------------------------------------------------
#

#
# this function loads the hooks dispatcher.
#
def                     Load(directory):
  path = directory + "/hooks" + Extension

  return yaml.load(file(path, 'r'))
