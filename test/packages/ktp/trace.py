#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/server/ktp/trace.py
#
# created       julien quintard   [mon oct 25 20:46:33 2010]
# updated       julien quintard   [tue oct 26 15:42:17 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import traceback

import miscellaneous

#
# ---------- functions --------------------------------------------------------
#

#
# this function generates a stack trace.
#
def                     Generate():
  temporary = None
  trace = None

  # create a temporary file.
  temporary = miscellaneous.Temporary(miscellaneous.OptionFile)

  # put the trace in the temporary file.
  traceback.print_exc(None, file(temporary, 'w'))

  # read the file.
  trace = miscellaneous.Pull(temporary)

  # remove the temporary file.
  miscellaneous.Remove(temporary)

  # return the trace.
  return trace
