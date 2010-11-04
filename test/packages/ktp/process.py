#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANE...TEST-SYSTEM/test/packages/ktp/process.py
#
# created       julien quintard   [mon oct 25 20:54:05 2010]
# updated       julien quintard   [thu nov  4 14:40:44 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import subprocess
import os

import miscellaneous
import ktp

#
# ---------- definitions ------------------------------------------------------
#

OptionNone = 0
OptionBackground = 1

#
# ---------- functions --------------------------------------------------------
#

#
# this function invokes a sub-process and returns either the status or a
# monitor if the invocation has been performed in background.
#
def                     Invoke(command,
                               arguments,
                               stream = None,
                               option = OptionNone):
  info = None
  directory = None
  file = None
  handle = None
  descriptor = None
  status = None
  output = None
  error = None

  # retrive file information.
  info = os.path.split(command)

  directory = info[0]
  file = info[1]

  # change directory if required.
  if directory:
    wd = os.getcwd()
    os.chdir(directory)

  # set up the stdout and stderr, according to the given stream file.
  if stream:
    # open the stream file.
    descriptor = os.open(stream, os.O_CREAT | os.O_TRUNC | os.O_WRONLY)

    # set the output and error descriptors.
    output = descriptor
    error = subprocess.STDOUT
  else:
    # open the null stream file.
    descriptor = os.open(os.devnull, os.O_WRONLY)

    # set the output and error descriptors so that to ignore everything.
    output = descriptor
    error = subprocess.STDOUT

  # launch the command.
  handle = subprocess.Popen(command + " " + " ".join(arguments),
                            stdout = output,
                            stderr = error,
                            shell = True)

  # come back to the original directory if necessary.
  if directory:
    os.chdir(wd)

  # return information according to the option.
  if option == OptionBackground:
    return (handle, stream, descriptor)
  else:
    # wait for the process to terminate.
    status = handle.wait()

    # close the descriptor.
    if descriptor:
      os.close(descriptor)

    if status != 0:
      return (ktp.StatusError)
    else:
      return (ktp.StatusOk)

#
# this function waits for a process to terminate and return the status.
#
def                     Wait(monitor):
  handle = None
  stream = None
  descriptor = None
  status = None

  # retrieve the information.
  (handle, stream, descriptor) = monitor

  # wait for the process to terminate.
  status = handle.wait()

  # close the descriptor.
  if descriptor:
    os.close(descriptor)

  if status != 0:
    return (ktp.StatusError)
  else:
    return (ktp.StatusOk)

#
# this function probes a process.
#
def                     Probe(monitor):
  handle = None
  stream = None
  descriptor = None
  status = None

  # retrieve the information.
  (handle, stream, descriptor) = monitor

  # probe the process.
  status = handle.poll()

  if isinstance(status, int):
    # close the descriptor.
    if descriptor:
      os.close(descriptor)

    if status != 0:
      return (ktp.StatusError)
    else:
      return (ktp.StatusOk)
  else:
    return (ktp.StatusUnknown)

#
# this function terminates a process by killing it.
#
def                     Terminate(monitor):
  handle = None
  stream = None
  descriptor = None
  status = None

  # retrieve the information.
  (handle, stream, descriptor) = monitor

  # kill the process.
  handle.kill()

  # close the descriptor.
  if descriptor:
    os.close(descriptor)
