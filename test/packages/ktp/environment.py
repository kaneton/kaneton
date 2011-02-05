#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/environment.py
#
# created       julien quintard   [mon oct 25 20:23:05 2010]
# updated       julien quintard   [wed feb  2 22:57:01 2011]
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

Extension = ".env"

QEMU = "qemu"
Xen = "xen"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of environment identifiers.
#
def                     List(directory):
  environments = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      environments += [ entry[:-len(Extension)] ]

  return environments

#
# this function stores a environment.
#
def                     Store(environment, path):
  yaml.dump(environment,
            file(path, 'w'))

#
# this function loads a environment.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))
