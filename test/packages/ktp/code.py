#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/packages/ktp/code.py
#
# created       julien quintard   [mon oct 25 20:03:52 2010]
# updated       julien quintard   [sat oct 30 11:17:56 2010]
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

  code = "".join([ random.choice(string.ascii_letters + string.punctuation)
                   for x in range(128) ])

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
