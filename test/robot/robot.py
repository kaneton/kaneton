#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/robot/robot.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [tue dec  7 22:05:28 2010]
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

import signal
import time
import argparse

import ktp

#
# ---------- constants --------------------------------------------------------
#

# directories
StoreDirectory = TestDirectory + "/store"

# stores
BundleStore = StoreDirectory + "/bundle"

# environments
XenEnvironment = "xen"
QEMUEnvironment = "qemu"

# timeout in seconds.
XenTimeout = 100
QEMUTimeout = 1000

# disk size in mega bytes.
DiskSize = 5

#
# ---------- globals ----------------------------------------------------------
#

# the command-line parser.
g_parser = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function displays an error message, cleans the script before
# exiting.
#
def                     Error(namespace, message):
  # display the script name.
  print "[construct]"

  # print the message.
  if message:
    print(message)

  # clean the script.
  Clean(namespace)

  # exit with an error code.
  sys.exit(42)

#
# this function creates a disk containing the snapshot that will
# be used by the compile virtual machine as a partition.
#
def                     Disk(namespace):
  directory = None
  environment = None

  # build a temporary file.
  namespace.disk = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # initialize the size of the file.
  if ktp.process.Invoke("dd",
                        [ "if=/dev/zero",
                          "of=" + namespace.disk,
                          "bs=1M",
                          "count=" + str(DiskSize) ]) == ktp.StatusError:
    Error(namespace,
          "unable to create a temporary disk image")

  # create an ext2 file system for this file.
  if ktp.process.Invoke("mkfs.ext2",
                        [ "-F",
                          "-b 1024",
                          namespace.disk ]) == ktp.StatusError:
    Error(namespace,
          "unable to create a file system on the temporary disk image")

  # build a temporary directory.
  directory = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionDirectory)

  # mount the disk into the temporary directory.
  if ktp.process.Invoke("mount",
                        [ "-o", "loop",
                          namespace.disk,
                          directory ]) == ktp.StatusError:
    Error(namespace,
          "unable to mount the temporary disk image")

  # check if the snapshot exists.
  if not os.path.exists(namespace.snapshot):
    Error(namespace,
          "the snapshot does not seem to exist")

  # copy the snapshot into the mounted directory.
  ktp.miscellaneous.Copy(namespace.snapshot, directory + "/kaneton.tar.bz2")

  # check if the bundle exists.
  if not os.path.exists(BundleStore + "/" +                             \
                           namespace.platform + "." +                   \
                           namespace.architecture + "/kaneton.lo"):
    Error(namespace,
          "the kaneton bundle does not seem to be present")

  # copy the kaneton tests bundle.
  ktp.miscellaneous.Copy(BundleStore + "/" +                            \
                           namespace.platform + "." +                   \
                           namespace.architecture + "/kaneton.lo",
                         directory)

  # generate the environment.
  environment = """\
export KANETON_USER='test'
export KANETON_HOST='%(host)s'
export KANETON_PYTHON='/usr/bin/python'
export KANETON_PLATFORM='%(platform)s'
export KANETON_ARCHITECTURE='%(architecture)s'\
""" % { "host": namespace.host,
        "platform": namespace.platform,
        "architecture": namespace.architecture }

  # save the environment in a file.
  ktp.miscellaneous.Push(environment, directory + "/environment.sh")

  # umount the temporary directory
  if ktp.process.Invoke("umount",
                        [ directory ]) == ktp.StatusError:
    Error(namespace,
          "unable to unmount the temporary disk image")

  # remove the directory.
  ktp.miscellaneous.Remove(directory)

#
# this is the alarm handler.
#
def                     Handler(number, frame):
  raise Exception("[timeout]")

#
# this function triggers the compilation process
# through a QEMU virtual machine.
#
def                     QEMU(namespace):
  stream = None
  monitor = None
  output = None
  content = None
  match = None
  port = None
  redirect = None

  # create a stream file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # create a redirection file.
  redirect = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # set the alarm signal
  signal.signal(signal.SIGALRM, Handler)
  signal.alarm(QEMUTimeout)

  try:
    # launch QEMU active logging in background in order to retrieve
    # the process handle.
    monitor = ktp.process.Invoke("qemu",
                                 [ "-nographic",
                                   "-serial", "pty",
                                   "-monitor", "null",
                                   "-cdrom",
                                     TestDirectory + "/images/debian.iso",
                                   "-hdb", namespace.disk,
                                   "-name", namespace.name],
                                 stream = stream,
                                 option = ktp.process.OptionBackground)

    # wait a few seconds to be sure QEMU has started.
    time.sleep(3)

    # read the status file.
    content = ktp.miscellaneous.Pull(stream)

    # check if this operation has been successful.
    if not content:
      raise Exception("[error] unable to retrieve QEMU's log file")

    # retrieve the serial port.
    match = re.search("char device redirected to (.*)$", content, re.MULTILINE)

    # check if it was successful.
    if not match:
      raise Exception("[error] unable to retrieve the serial port")

    # set the port.
    port = match.group(1)

    # wait for the virtual machine to terminate.
    while True:
      # probe the virtual machine.
      status = ktp.process.Probe(monitor)

      # check if the machine is still running.
      if status != ktp.StatusUnknown:
        break

      # transfer the output content on a regular basis.
      ktp.miscellaneous.Transfer(port, redirect)

    # retrieve the output.
    namespace.output = ktp.miscellaneous.Pull(redirect)

    # remove the files.
    ktp.miscellaneous.Remove(redirect)
    ktp.miscellaneous.Remove(stream)
  except Exception, exception:
    # destroy the virtual machine by terminating
    # the process.
    try:
      os.kill(monitor[0].pid, signal.SIGKILL)
    except:
      pass

    # wait for the process to terminate.
    ktp.process.Wait(monitor)

# XXX[not possible with Python 2.5 since subprocess does not provide this functionality]
#    [when ready remove: os.kill, Wait and Pull]
#    output = ktp.process.Terminate(monitor)

    # reset the alarm.
    signal.alarm(0)

    # wait for a bit.
    time.sleep(3)

    # retrieve the output.
    namespace.output = ktp.miscellaneous.Pull(redirect)

    # remove the files.
    ktp.miscellaneous.Remove(redirect)
    ktp.miscellaneous.Remove(stream)

    # display the output.
    if namespace.output:
      print(namespace.output)

    # stop the script.
    Error(namespace,
          str(exception))

  # reset the alarm.
  signal.alarm(0)

#
# this function generates a Xen configuration file for
# the compile virtual machine and runs it.
#
def                     Xen(namespace):
  configuration = None
  status = None
  content = None
  match = None
  port = None
  stream = None

  # create a temporary file.
  namespace.configuration =                                             \
    ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # configuration.
  configuration = """\
device_model = "/usr/lib/xen/bin/qemu-dm"
kernel = "/usr/lib/xen/boot/hvmloader"
builder = "hvm"
memory = 256
name = "%(name)s"
pae = 1
disk = ["tap:aio:%(root)s/images/debian.iso,hdc:cdrom,r", "tap:aio:%(disk)s,hdb,w"]
boot = 'd'
serial = "pty"\
""" % { "root": TestDirectory,
        "name": namespace.name,
        "disk": namespace.disk }

  # save the configuration in a file.
  ktp.miscellaneous.Push(configuration, namespace.configuration)

  # create a stream file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # set the alarm signal
  signal.signal(signal.SIGALRM, Handler)
  signal.alarm(XenTimeout)

  try:
    # launch the xen virtual machine.
    #
    # note that the 'xm' binary automatically sets the system
    # in background so there is no choice here but to rely
    # on the 'xm list' command to know if the process has
    # finished.
    ktp.process.Invoke("xm",
                       [ "create",
                         namespace.configuration ])

    # wait a few seconds to be sure Xen has started.
    time.sleep(3)

    # read the status file.
    content = ktp.miscellaneous.Pull("/var/log/xen/qemu-dm-" +          \
                                       namespace.name + ".log")

    # check if this operation has been successful.
    if not content:
      raise Exception("[error] unable to retrieve QEMU's log file")

    # retrieve the serial port.
    match = re.search("char device redirected to (.*)$", content, re.MULTILINE)

    # check if it was successful.
    if not match:
      raise Exception("[error] unable to retrieve the serial port")

    # set the port.
    port = match.group(1)

    # wait for the virtual machine to terminate.
    while True:
      # check if the machine exists in the list of active VMs.
      status = ktp.process.Invoke("xm",
                                  [ "list",
                                    namespace.name ])

      # if the virtual machine does not appear in the list, it means
      # that it has terminated.
      if status == ktp.StatusError:
        break

      # transfer the output content on a regular basis.
      ktp.miscellaneous.Transfer(port, stream)

    # read the serial file.
    namespace.output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)
  except Exception, exception:
    # destroy the virtual machine
    ktp.process.Invoke("xm",
                       [ "destroy",
                         namespace.name ])

    # read the serial file.
    namespace.output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    # display the output.
    if namespace.output:
      print(namespace.output)

    # stop the script.
    Error(namespace,
          str(exception))

  # reset the alarm.
  signal.alarm(0)

#
# this function retrieves both the generated image and the compile log.
#
def                     Retrieve(namespace):
  directory = None

  # create a temporary directory.
  directory = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionDirectory)

  # mount the disk.
  if ktp.process.Invoke("mount",
                        [ "-o" "loop",
                          namespace.disk,
                          directory ]) == ktp.StatusError:
    Error(namespace,
          "unable to mount the temporary disk image")

  # check if an error occured. if so, print the log followed
  # by the error message before existing.
  if os.path.exists(directory + "/.error"):
    # print the output.
    if namespace.output:
      print(namespace.output)

    # retrieve the error file.
    error = ktp.miscellaneous.Pull(directory + "/.error")

    # print the error.
    if error:
      print(error)

    # retrieve and print the log.
    if os.path.exists(directory + "/.log"):
      # read the log file.
      log = ktp.miscellaneous.Pull(directory + "/.log")

      # print the log.
      print(log)

    # umount the snapshot.
    if ktp.process.Invoke("umount",
                          [ directory ]) == ktp.StatusError:
      Error(namespace,
            "unable to unmount the temporary disk image")

    # remove the temporary directory.
    ktp.miscellaneous.Remove(directory)

    # stop the script.
    Error(namespace,
          None)
  else:
    # otherwise, no error occured. therefore, copy the image.
    if not os.path.exists(directory + "/kaneton.img"):
      Error(namespace,
            "[error] the kaneton image does not seem to have been generated")

    # copy the generated image to its target.
    ktp.miscellaneous.Copy(directory + "/kaneton.img", namespace.image)

    # umount the snapshot.
    if ktp.process.Invoke("umount",
                          [ directory ]) == ktp.StatusError:
      Error(namespace,
            "unable to unmount the temporary disk image")

    # remove the temporary directory.
    ktp.miscellaneous.Remove(directory)

#
# this function initializes the script.
#
def                     Initialize(namespace):
  # initialize the variables.
  namespace.disk = None
  namespace.configuration = None

#
# this function cleans what has been created by this script.
#
def                     Clean(namespace):
  # remove the disk file.
  if namespace.disk:
    ktp.miscellaneous.Remove(namespace.disk)

  # remove the configuration file.
  if namespace.configuration:
    ktp.miscellaneous.Remove(namespace.configuration)

#
# this is the main function
#
def                     Main():
  # initialize the script.
  Initialize()

  # checkout the kaneton repository.
  Checkout()

  # trigger the tests.
  Test()

  # clean the temporary stuff.
  Clean(namespace)

  # display a message.
  print("the kaneton image has been constructed in '" + namespace.image + "'")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
