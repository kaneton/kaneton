#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/scripts/construct.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [tue nov  2 05:18:35 2010]
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
XenTimeout = 70
QEMUTimeout = 500

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
  # print the message.
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
  ktp.process.Invoke("dd",
                     [ "if=/dev/zero",
                       "of=" + namespace.disk,
                       "bs=1M",
                       "count=" + str(DiskSize) ])

  # create an ext2 file system for this file.
  ktp.process.Invoke("mkfs.ext2",
                     [ "-F",
                       "-b 1024",
                       namespace.disk ])

  # build a temporary directory.
  directory = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionDirectory)

  # mount the disk into the temporary directory.
  ktp.process.Invoke("mount",
                     [ "-o", "loop",
                       namespace.disk,
                       directory ])

  # copy the snapshot into the mounted directory.
  ktp.miscellaneous.Copy(namespace.snapshot, directory + "/kaneton.tar.bz2")

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
  ktp.process.Invoke("umount",
                     [ directory ])

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
  log = None
  monitor = None

  # create a stream file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # set the alarm signal
  signal.signal(signal.SIGALRM, Handler)
  signal.alarm(QEMUTimeout)

  try:
    # launch QEMU active logging in background in order to retrieve
    # the process handle.
    monitor = ktp.process.Invoke("qemu",
                                 [ "-nographic",
                                   "-serial", "null",
                                   "-monitor", "null",
                                   "-cdrom",
                                     TestDirectory + "/images/debian.iso",
                                   "-hdb", namespace.disk,
                                   "-name", namespace.name],
                                 stream = stream,
                                 option = ktp.process.OptionBackground)

    # wait for the process to terminate.
    ktp.process.Wait(monitor)
  except Exception, exception:
    # destroy the virtual machine by terminating
    # the process.
    os.kill(monitor[0].pid, signal.SIGKILL)

    # wait for the process to terminate.
    ktp.process.Wait(monitor)

# XXX[not possible with Python 2.5 since subprocess does not provide this functionality]
#    [when ready remove: os.kill, Wait and Pull]
#    log = ktp.process.Terminate(monitor)

    # retrieve the log.
    log = ktp.miscellaneous.Pull(stream)

    # display the log.
    if log:
      print(log)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

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
  output = None
  stream = None
  log = None

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
boot = 'd'\
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
    # launch the xen virtual machine with active logging.
    ktp.process.Invoke("xm",
                       [ "create",
                         namespace.configuration ],
                       stream = stream,
                       option = ktp.process.OptionNone)

    # wait for the virtual machine to terminate.
    while True:
      (status, output) = ktp.process.Invoke("xm",
                                            [ "list",
                                              namespace.name ],
                                            option = ktp.process.OptionNone)

      # if the virtual machine does not appear in the list, it means
      # that it has terminated.
      if status == ktp.StatusError:
        break

      # sleep for a bit.
      time.sleep(5)
  except Exception, exception:
    # destroy the virtual machine
    ktp.process.Invoke("xm",
                       [ "destroy",
                         namespace.name ])

    # retrieve the log.
    log = ktp.miscellaneous.Pull(stream)

    # display the log.
    if log:
      print(log)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

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
  error = None
  log = None

  # create a temporary directory.
  directory = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionDirectory)

  # mount the disk.
  ktp.process.Invoke("mount",
                     [ "-o" "loop",
                       namespace.disk,
                       directory ])

  # check if an error occured. if so, print the log followed
  # by the error message before existing.
  if os.path.exists(directory + "/.error"):
    error = ktp.miscellaneous.Pull(directory + "/.error")
    if not error:
      error = ""

    # retrieve the log.
    if os.path.exists(directory + "/.log"):
      log = ktp.miscellaneous.Pull(directory + "/.log")
    if not log:
      log = ""

    # umount the snapshot.
    ktp.process.Invoke("umount",
                       [ directory ])

    # remove the temporary directory.
    ktp.miscellaneous.Remove(directory)

    # stop the script.
    Error(namespace,
          log + "\n" + error)
  else:
    # otherwise, no error occured. therefore, copy the image.
    if not os.path.exists(directory + "/kaneton.img"):
      Error(namespace,
            "[error] the kaneton image does not seem to have been generated")

    # copy the generated image to its target.
    ktp.miscellaneous.Copy(directory + "/kaneton.img", namespace.image)

    # umount the snapshot.
    ktp.process.Invoke("umount",
                       [ directory ])

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
                        default = "ia32",
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
  if namespace.environment == XenEnvironment:
    Xen(namespace)
  elif namespace.environment == QEMUEnvironment:
    QEMU(namespace)
  else:
    Error(namespace,
          "unknown environment '" + namespace.environment + "'")

  # retrieve the generated stuff: log, error and image.
  Retrieve(namespace)

  # clean the temporary stuff.
  Clean(namespace)

  # display a message.
  print("the kaneton image has been constructed in '" + namespace.image + "'")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
