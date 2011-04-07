#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/log.py
#
# created       julien quintard   [mon oct 25 19:58:10 2010]
# updated       julien quintard   [tue mar  8 11:49:26 2011]
#

#
# ---------- packages ---------------------------------------------------------
#

import os
import time
import fcntl

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".log"

#
# ---------- functions --------------------------------------------------------
#

#
# this function records the given message within the current log file.
#
def                     Record(directory, message):
  handle = None
  header = None
  path = None

  path = directory + "/" + time.strftime("%Y%m%d") + Extension

  header = time.strftime("[%Y/%m/%d %H:%M:%S] ")

  handle = open(path, "a")

  fcntl.flock(handle.fileno(), fcntl.LOCK_EX)

  handle.write(header + message + "\n")

  handle.close()
