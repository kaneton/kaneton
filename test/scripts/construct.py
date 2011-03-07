#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/scripts/construct.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [sun mar  6 17:18:34 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script takes a kaneton snapshot, starts a Debian Live system in an
# emulated environment in order to compile the snapshot into a bootable image.
#
# first, a disk is created in order to receive: (i) the kaneton snapshot,
# (ii) the kaneton bundle i.e the set of pre-compiled tests to include to
# the kernel compilation and (iii) a set of environment variables required
# by the compilation process: the kaneton environment variables.
#
# the emulator is then launched, taking care to set a timer so that the
# process stops should the emulator crash or hang.
#
# finally, the script mounts the disk image which should, theoretically,
# contain the generated kaneton image. should the process have failed, the
# disk image should contain the reason of this failure through the .log and
# .error files.
#
# for more information about the Debian Live system, please refer to the
# images/ directory.
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
LogStore = StoreDirectory + "/log"

# timeout in seconds: qemu/xen.
Timeouts = { ktp.environment.QEMU: 1000,
             ktp.environment.Xen: 100 }

# disk size in mega bytes.
DiskSize = 10

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
  print("[construct]")

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

  ktp.log.Record(LogStore,
                 "#(construct) message(creating the disk image)")

  # initialize the size of the file.
  if ktp.process.Invoke("dd",
                        [ "if=/dev/zero",
                          "of=" + namespace.disk,
                          "bs=1M",
                          "count=" + str(DiskSize) ]) == ktp.StatusError:
    Error(namespace,
          "unable to create a temporary disk image")

  ktp.log.Record(LogStore,
                 "#(construct) message(creating the disk's file system)")

  # create an ext2 file system for this file.
  if ktp.process.Invoke("mkfs.ext2",
                        [ "-F",
                          "-b 1024",
                          namespace.disk ]) == ktp.StatusError:
    Error(namespace,
          "unable to create a file system on the temporary disk image")

  # build a temporary directory.
  directory = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionDirectory)

  ktp.log.Record(LogStore,
                 "#(construct) message(mounting the disk image)")

  # mount the disk into the temporary directory.
  if ktp.process.Invoke("mount",
                        [ "-o", "loop",
                          namespace.disk,
                          directory ]) == ktp.StatusError:
    Error(namespace,
          "unable to mount the temporary disk image")

  ktp.log.Record(LogStore,
                 "#(construct) message(copying the snapshot to the disk)")

  # check if the snapshot exists.
  if not os.path.exists(namespace.snapshot):
    Error(namespace,
          "the snapshot does not seem to exist")

  # check if the snapshot is not too big.
  if os.path.getsize(namespace.snapshot) > (DiskSize * 1024 * 1024 * 2/3):
    Error(namespace,
          "the snapshot is too big to fit on the disk image")

  # copy the snapshot into the mounted directory.
  ktp.miscellaneous.Copy(namespace.snapshot, directory + "/kaneton.tar.bz2")

  ktp.log.Record(LogStore,
                 "#(construct) message(copying the bundle to the disk)")

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

  ktp.log.Record(LogStore,
                 "#(construct) message(generating the environment file)")

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

  ktp.log.Record(LogStore,
                 "#(construct) message(unmounting the disk image)")

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

  ktp.log.Record(LogStore,
                 "#(construct) environment(QEMU)")

  # create a stream file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # create a redirection file.
  redirect = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  ktp.log.Record(LogStore,
                 "#(construct) message(setting the timer)")

  # set the alarm signal
  signal.signal(signal.SIGALRM, Handler)
  signal.alarm(Timeout[ktp.environment.QEMU])

  try:
    ktp.log.Record(LogStore,
                   "#(construct) message(launching QEMU)")

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

    ktp.log.Record(LogStore,
                   "#(construct) message(reading the output)")

    # read the status file.
    content = ktp.miscellaneous.Pull(stream)

    # check if this operation has been successful.
    if not content:
      raise Exception("[error] unable to retrieve QEMU's log file")

    ktp.log.Record(LogStore,
                   "#(construct) message(extracting the serial port)")

    # retrieve the serial port.
    match = re.search("char device redirected to (.*)$", content, re.MULTILINE)

    # check if it was successful.
    if not match:
      raise Exception("[error] unable to retrieve the serial port")

    # set the port.
    port = match.group(1)

    ktp.log.Record(LogStore,
                   "#(construct) message(waiting for the VM to terminate)")

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

    ktp.log.Record(LogStore,
                   "#(construct) message(VM terminated)")

    # remove the files.
    ktp.miscellaneous.Remove(redirect)
    ktp.miscellaneous.Remove(stream)
  except Exception, exception:
    ktp.log.Record(LogStore,
                   "#(construct) message(destorying the VM)")

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

    ktp.log.Record(LogStore,
                   "#(construct) message(wait a bit)")

    # reset the alarm.
    signal.alarm(0)

    # wait for a bit.
    time.sleep(3)

    # retrieve the output.
    namespace.output = ktp.miscellaneous.Pull(redirect)

    ktp.log.Record(LogStore,
                   "#(construct) output(" + str(namespace.output) + ")")

    # remove the files.
    ktp.miscellaneous.Remove(redirect)
    ktp.miscellaneous.Remove(stream)

    # display the output.
    if namespace.output:
      print(namespace.output)

    # stop the script.
    Error(namespace,
          str(exception))

  ktp.log.Record(LogStore,
                 "#(construct) message(resetting the timer)")

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

  ktp.log.Record(LogStore,
                 "#(construct) environment(Xen)")

  ktp.log.Record(LogStore,
                 "#(construct) message(generating the Xen configuration)")

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

  ktp.log.Record(LogStore,
                 "#(construct) message(setting the timer)")

  # set the alarm signal
  signal.signal(signal.SIGALRM, Handler)
  signal.alarm(Timeouts[ktp.environment.Xen])

  try:
    ktp.log.Record(LogStore,
                   "#(construct) message(launching Xen)")

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

    ktp.log.Record(LogStore,
                   "#(construct) message(retrieving the QEMU log file)")

    # read the status file.
    content = ktp.miscellaneous.Pull("/var/log/xen/qemu-dm-" +          \
                                       namespace.name + ".log")

    # check if this operation has been successful.
    if not content:
      raise Exception("[error] unable to retrieve QEMU's log file")

    ktp.log.Record(LogStore,
                   "#(construct) message(extracting the serial port)")

    # retrieve the serial port.
    match = re.search("char device redirected to (.*)$", content, re.MULTILINE)

    # check if it was successful.
    if not match:
      raise Exception("[error] unable to retrieve the serial port")

    # set the port.
    port = match.group(1)

    ktp.log.Record(LogStore,
                   "#(construct) message(waiting for the VM to terminate)")

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

    ktp.log.Record(LogStore,
                   "#(construct) message(VM terminated)")

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)
  except Exception, exception:
    ktp.log.Record(LogStore,
                   "#(construct) message(destroying the VM)")

    # destroy the virtual machine
    ktp.process.Invoke("xm",
                       [ "destroy",
                         namespace.name ])

    # read the serial file.
    namespace.output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    ktp.log.Record(LogStore,
                   "#(construct) output(" + str(namespace.output) + ")")

    # display the output.
    if namespace.output:
      print(namespace.output)

    # stop the script.
    Error(namespace,
          str(exception))

  ktp.log.Record(LogStore,
                 "#(construct) message(resetting the timer)")

  # reset the alarm.
  signal.alarm(0)

#
# this function retrieves both the generated image and the compile log.
#
def                     Retrieve(namespace):
  directory = None

  # create a temporary directory.
  directory = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionDirectory)

  ktp.log.Record(LogStore,
                 "#(construct) message(mounting the disk image)")

  # mount the disk.
  if ktp.process.Invoke("mount",
                        [ "-o" "loop",
                          namespace.disk,
                          directory ]) == ktp.StatusError:
    Error(namespace,
          "unable to mount the temporary disk image")

  ktp.log.Record(LogStore,
                 "#(construct) message(checking the presence of error files)")

  # check if an error occured. if so, print the log followed
  # by the error message before existing.
  if os.path.exists(directory + "/.error"):
    ktp.log.Record(LogStore,
                   "#(construct) message(retrieving the error messages)")

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

    ktp.log.Record(LogStore,
                   "#(construct) message(unmounting the disk image)")

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
    ktp.log.Record(LogStore,
                   "#(construct) message(retrieving the bootable image)")

    # otherwise, no error occured. therefore, copy the image.
    if not os.path.exists(directory + "/kaneton.img"):
      Error(namespace,
            "[error] the kaneton image does not seem to have been generated")

    # copy the generated image to its target.
    ktp.miscellaneous.Copy(directory + "/kaneton.img", namespace.image)

    ktp.log.Record(LogStore,
                   "#(construct) message(unmounting the disk image)")

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
  ktp.log.Record(LogStore,
                 "#(construct) message(initializing)")

  # initialize the variables.
  namespace.disk = None
  namespace.configuration = None

#
# this function cleans what has been created by this script.
#
def                     Clean(namespace):
  ktp.log.Record(LogStore,
                 "#(construct) message(cleaning)")

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
  global g_parser

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script builds "  \
                                       "a bootable image from the "     \
                                       "given kaneton snapshot.")

  # set up the authorised arguments.
  g_parser.add_argument("--name", '-n',
                        default = time.strftime("%Y%m%d-%H%M%S"),
                        help = "a unique name for the operation",
                        dest = "name")
  g_parser.add_argument("--snapshot", '-s',
                        required = True,
                        help = "the path to the kaneton snapshost",
                        dest = "snapshot")
  g_parser.add_argument("--image", '-i',
                        default =
                          ktp.miscellaneous.Temporary(                  \
                            ktp.miscellaneous.OptionFile),
                        help = "the path to the generated kaneton image",
                        dest = "image")
  g_parser.add_argument("--environment", '-e',
                        required = True,
                        help = "the environment in which the image is " \
                          "to be generated",
                        dest = "environment")
  g_parser.add_argument("--host", '-o',
                        default = "linux/ia32",
                        help = "the host in which the image is to be "  \
                          "generated",
                        dest = "host")
  g_parser.add_argument("--platform", '-p',
                        default = "ibm-pc",
                        help = "the platform on which the image is to " \
                          "be generated",
                        dest = "platform")
  g_parser.add_argument("--architecture", '-a',
                        default = "ia32/educational",
                        help = "the architecture on which the image "   \
                          "is to be generated",
                        dest = "architecture")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # create a disk image.
  Disk(namespace)

  # launch the compilation process according to the environment.
  if namespace.environment == ktp.environment.QEMU:
    QEMU(namespace)
  elif namespace.environment == ktp.environment.Xen:
    Xen(namespace)
  else:
    Error(namespace,
          "unknown environment '" + namespace.environment + "'")

  # retrieve the generated stuff: log, error and image.
  Retrieve(namespace)

  # clean the temporary stuff.
  Clean(namespace)

  ktp.log.Record(LogStore,
                 "#(construct) message(image constructed in '" +        \
                   namespace.image + "')")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
