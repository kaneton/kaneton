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
# updated       julien quintard   [wed feb  2 20:59:25 2011]
#

#
# ---------- packages ---------------------------------------------------------
#

import os

import ktp

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

  ktp.miscellaneous.Push("",
                         path)

#
# this function returns the next queue'd identifier, or None if none.
#
def                     Pop(directory):
  identifier = None
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    identifier = entry

    ktp.miscellaneous.Remove(path)

    return identifier

  return None
