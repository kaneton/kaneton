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
# updated       julien quintard   [wed feb  2 20:31:59 2011]
#

#
# ---------- packages ---------------------------------------------------------
#

import os
import time

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

  handle.write(header + message + "\n")

  handle.close()
