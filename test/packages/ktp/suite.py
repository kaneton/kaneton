#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/suite.py
#
# created       julien quintard   [wed oct 27 10:53:58 2010]
# updated       julien quintard   [wed oct 27 13:14:51 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import yaml

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".suite"

#
# ---------- functions --------------------------------------------------------
#

#
# this function loads a suite.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
