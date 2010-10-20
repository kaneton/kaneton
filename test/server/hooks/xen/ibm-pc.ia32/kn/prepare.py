#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANE...TEST-SYSTEM/hooks/ibm-pc.ia32/prepare.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [mon oct 18 16:51:22 2010]
#

#
# ---------- imports ----------------------------------------------------------
#

import sys
import md5
import tempfile
import shutil
import os
import signal
import yaml

#
# ---------- constants --------------------------------------------------------
#

# timeout in seconds.
TimeoutCompile = 120

# disk size in mega bytes.
DiskSize = 5

#
# ---------- Timeout ----------------------------------------------------------
#

class TimeoutException(Exception):
  pass

class Timeout:
  def __init__(self, function, timeout):
    self.timeout = timeout
    self.function = function

  def handle(self, signum, frame):
    raise TimeoutException("timeout")

  def __call__(self, *args):
    result = None

    old = signal.signal(signal.SIGALRM, self.handle)
    signal.alarm(self.timeout)

    try:
      result = self.function(*args)
    finally:
      signal.signal(signal.SIGALRM, old)

    signal.alarm(0)
    return result

#
# ---------- functions --------------------------------------------------------
#

#
# this function prints the usage.
#
def             Usage():
  print "usage: prepare.py [context]"
  sys.exit(42)

#
# this function reads a file.
#
def             Pull(file):
  handle = None
  line = None

  if not os.path.exists(file):
    return None

  try:
    handle = open(file, "r")
  except IOError:
    return None

  content = ""

  for line in handle.readlines():
    content += line

  handle.close()

  return content

#
# this function writes a file.
#
def             Push(file, content):
  handle = None

  handle = open(file, "w")

  handle.write(content)

  handle.close()

#
# this function removes the target.
#
def             Remove(target):
  entries = None
  entry = None

  if os.path.isfile(target) or os.path.islink(target):
    os.unlink(target)

  if os.path.isdir(target):
    entries = os.listdir(target)

    for entry in entries:
      Remove(target + "/" + entry)

    os.rmdir(target)

#
# this function creates a temporary file or directory.
#
OptionFile = 1
OptionDirectory = 2

def             Temporary(options):
  location = None

  if options == OptionFile:
    tuple = tempfile.mkstemp()

    os.close(tuple[0])

    location = tuple[1]
  elif options == OptionDirectory:
    location = tempfile.mkdtemp()

  return location

#
# this function creates a disk containing the snapshot that will
# be used by the compile virtual machine as a partition.
#
def             Disk(context):
  directory = None
  environment = None

  # build a temporary file.
  context["disk"] = Temporary(OptionFile)

  # initialize the size of the file.
  os.system("dd if=/dev/zero of=" + context["disk"] + " bs=1M count=" + str(DiskSize) + " >/dev/null 2>&1")

  # create an ext2 file system for this file.
  os.system("mkfs.ext2 -F -b 1024 " + context["disk"] + " >/dev/null 2>&1")

  # build a temporary directory.
  directory = Temporary(OptionDirectory)

  # mount the disk into the temporary directory.
  os.system("mount -o loop " + context["disk"] + " " + directory)

  # copy the snapshot into the mounted directory.
  shutil.copy(context["snapshot"], directory + "/kaneton.tar.bz2")

  # environment
  environment = """
export KANETON_USER='test'
export KANETON_HOST='linux/ia32'
export KANETON_PYTHON='/usr/bin/python'
export KANETON_PLATFORM='%(platform)s'
export KANETON_ARCHITECTURE='%(architecture)s'
""" % { "platform": context["platform"],
        "architecture": context["architecture"] }

  # save the environment in a file.
  Push(directory + "/environment.sh", environment)

  # umount the temporary directory
  os.system("umount " + directory)

  # remove the directory.
  Remove(directory)

#
# this function generates a Xen configuration file for
# the compile virtual machine.
#
def             Configuration(context):
  configuration = None

  # create a temporary file.
  context["configuration"] = Temporary(OptionFile)

  # configuration.
  configuration = """
kernel = '/kaneton/hooks/ibm-pc.ia32/data/vmlinuz-2.6.18-xen'
memory = 128
name = '%(name)s'
disk = ['tap:aio:/kaneton/hooks/ibm-pc.ia32/data/svm:prepare.img,sda1,w', 'tap:aio:%(disk)s,sdb1,w']
root = '/dev/sda1 ro'
extra = 'gentoo=nodevfs'
extra = 'xencons=tty'
""" % { "name": context["name"],
        "disk": context["disk"] }

  # save the configuration in a file.
  Push(context["configuration"], configuration)

#
# this function launches a virtual machine according
# to the given configuration file.
#
def             Compile(context):
  # launch Xen and connect a console to it so that we know when
  # it is finished.
  function = Timeout(os.system, TimeoutCompile)

  try:
    function("xm create " + context["configuration"] + " -c >/dev/null 2>&1")
  except Exception, exception:
    # destroy the vm.
    os.system("xm destroy " + context["name"] + " >/dev/null 2>&1")

    print exception

    Clean(context)

    sys.exit(42)

#
# this function retrieves both the generated image and the compile log.
#
def             Retrieve(context):
  directory = None
  log = None
  error = None
  information = None

  # create a temporary directory.
  directory = Temporary(OptionDirectory)

  # mount the disk.
  os.system("mount -o loop " + context["disk"] + " " + directory)

  # check if the compile script even executed.
  if not os.path.exists(directory + "/log"):
    print "the compilation script has not been executed"

  # read the log.
  log = Pull(directory + "/log")

  # check if an error occured. if so, print the log followed
  # by the error message before existing.
  if (not os.path.exists(directory + "/kaneton.img")) or                \
     (os.path.exists(directory + "/error")):
    # read the error file if it exists.
    if os.path.exists(directory + "/error"):
      error = Pull(directory + "/error")

      information = log + "\n" + error
    else:
      information = log

    # umount the snapshot.
    os.system("umount " + directory)

    # remove the temporary directory.
    Remove(directory)

    print information

    Clean(context)

    sys.exit(42)

  # finally, if everything is ok, copy the image.
  shutil.copy(directory + "/kaneton.img", context["image"])

  # umount the snapshot.
  os.system("umount " + directory)

  # remove the temporary directory.
  Remove(directory)

#
# this function cleans what has been created by this script.
#
def             Clean(context):
  if "disk" in context:
    Remove(context["disk"])
    del context["disk"]

  if "configuration" in context:
    Remove(context["configuration"])
    del context["configuration"]

#
# this function serializes a data structure.
#
def             Serialize(data, path):
  yaml.dump(data, file(path, 'w'))

#
# this function extract a serialized data structure.
#
def             Extract(path):
  return yaml.load(file(path, 'r'))

#
# this is the main function
#
def             Main():
  path = None
  context = None

  # check the number of arguments.
  if len(sys.argv) != 2:
    Usage()
    sys.exit(42)

  # retrieve arguments.
  path = sys.argv[1]

  # load the context.
  context = Extract(path)

  # create a disk image.
  Disk(context)

  # generate a configuration file.
  Configuration(context)

  # launch the compile virtual machine.
  Compile(context)

  # retrieve the generated stuff: log, error and image.
  Retrieve(context)

  # clean the temporary stuff.
  Clean(context)

  # update the context.
  Serialize(context, path)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
