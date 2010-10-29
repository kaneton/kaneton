#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/process.py
#
# created       julien quintard   [mon oct 25 20:54:05 2010]
# updated       julien quintard   [fri oct 29 19:48:17 2010]
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
# this function invokes a sub-process and returns either a tuple
# (status, text) or a monitor if the invocation has been performed
# in background.
#
def                     Invoke(command, arguments,
                               stream = None,
                               option = OptionNone):
  info = None
  directory = None
  file = None
  handle = None
  descriptor = None
  text = None
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

    # retrieve the text and remove the file, if necessary.
    if stream:
      # retrieve the text.
      text = miscellaneous.Pull(stream)

      # remove the stream file.
      miscellaneous.Remove(stream)

    if status != 0:
      return (ktp.StatusError, text)
    else:
      return (ktp.StatusOk, text)

#
# this function waits for a process to terminate and return the status.
#
def                     Wait(monitor):
  handle = None
  stream = None
  descriptor = None
  status = None
  text = None

  # retrieve the information.
  (handle, stream, descriptor) = monitor

  # wait for the process to terminate.
  status = handle.wait()

  # close the descriptor.
  if descriptor:
    os.close(descriptor)

  # retrieve the text and remove the file, if necessary.
  if stream:
    # retrieve the text.
    text = miscellaneous.Pull(stream)

    # remove the stream file.
    miscellaneous.Remove(stream)

  if status != 0:
    return (ktp.StatusError, text)
  else:
    return (ktp.StatusOk, text)

#
# this function terminates a process by killing it.
#
def                     Terminate(monitor):
  handle = None
  stream = None
  descriptor = None
  status = None
  text = None

  # retrieve the information.
  (handle, stream, descriptor) = monitor

  # kill the process.
  handle.kill()

  # close the descriptor.
  if descriptor:
    os.close(descriptor)

  # retrieve the text and remove the file, if necessary.
  if stream:
    # retrieve the text.
    text = miscellaneous.Pull(stream)

    # remove the stream file.
    miscellaneous.Remove(stream)

  return text
