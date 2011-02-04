#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/queue.py
#
# created       julien quintard   [mon oct 25 19:58:10 2010]
# updated       julien quintard   [wed feb  2 15:58:41 2011]
#

#
# ---------- packages ---------------------------------------------------------
#

import os

#
# ---------- functions --------------------------------------------------------
#

#
# this function adds an identifier to the queue.
#
def                     Push(directory,
                             identifier):
  handle = None
  path = None

  path = directory + "/" + identifier

  handle = open(path, "w")

  handle.write(identifier)

  handle.close()
