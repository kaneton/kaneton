#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/code.py
#
# created       julien quintard   [mon oct 25 20:03:52 2010]
# updated       julien quintard   [wed oct 27 13:11:07 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import random
import string

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".code"

#
# ---------- functions --------------------------------------------------------
#

#
# this function generates a random code.
#
def                     Generate():
  random.seed()

  code = "".join([ random.choice(string.ascii_lowercase) for x in range(128) ])

  return code

#
# this function stores a code on the file system.
#
def                     Store(path,
                              code):
  open(path, 'w').write(code)

#
# this function reads a code file from the file system
#
def                     Load(path):
  code = open(path, 'r').read()

  return code
