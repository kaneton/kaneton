#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/hooks/cron.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [fri feb 25 20:19:35 2011]
#

#
# ---------- information ------------------------------------------------------
#
# XXX
#

#
# ---------- requirement ------------------------------------------------------
#

import os
import sys
import re

TestDirectory = os.path.abspath(os.path.dirname(                        \
                  os.path.realpath(sys.argv[0])) + "/..")

sys.path.append(TestDirectory + "/packages")

#
# ---------- packages ---------------------------------------------------------
#

import time
import argparse

import ktp

#
# ---------- constants --------------------------------------------------------
#

# directories
StoreDirectory = TestDirectory + "/store"
HooksDirectory = TestDirectory + "/hooks"

# stores
LogStore = StoreDirectory + "/log"

#
# ---------- functions --------------------------------------------------------
#

#
# this is the main function
#
def                     Main():
  hooks = None
  hook = None
  stream = None
  output = None

  ktp.log.Record(LogStore,
                 "#(cron) message(triggering the hooks)")

  # load the dispatcher.
  hooks = ktp.hook.Load(HooksDirectory)

  # go through the hooks.
  for hook in hooks:
    try:
      ktp.log.Record(LogStore,
                     "#(cron) hook(" + hook + ")")

      # generate a temporary file.
      stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

      # launch the test script.
      status =                                                          \
        ktp.process.Invoke(HooksDirectory + "/" +                       \
                             hooks[hook]["path"],
                           [ hooks[hook]["options"] ],
                           stream = stream,
                           option = ktp.process.OptionNone)

      ktp.log.Record(LogStore,
                     "#(cron) message(hook completed)");

      # retrieve the output.
      output = ktp.miscellaneous.Pull(stream)

      ktp.log.Record(LogStore,
                     "#(cron) status(" + str(status) + ") " +           \
                       "output(" + str(output) + ")")
    except Exception, exception:
      ktp.log.Record(LogStore,
                     "#(cron) exception(" +                             \
                       str(exception) + ") trace(" +                    \
                       ktp.trace.Generate() + ")")
    finally:
      # remove temporary files.
      if stream:
        ktp.miscellaneous.Remove(stream)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
