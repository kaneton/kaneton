#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/hooks/ibm-pc.ia32/run.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [mon oct 18 20:39:37 2010]
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
import serial
import struct
import re
import time
import yaml

#
# ---------- constants --------------------------------------------------------
#

# timeout in seconds.
TimeoutRun = 120

# magic number for serial communications.
Magic = 0xF4859632

# directories
DirectorySuites = "../../suites"
DirectoryTests = "../../tests"

# commands.
CommandReady = "[ready]"
CommandCall = "[call]"

# status.
StatusOk = True
StatusError = False

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
  print "usage: run.py [context]"
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
# this function searches for file names matching the given pattern.
#
def             Search(directory, pattern):
  elements = []
  entries = None
  entry = None

  entries = os.listdir(directory)

  for entry in entries:
    if (os.path.isfile(directory + "/" + entry)) and            \
       (re.search(pattern, entry)):
      elements += [ directory + "/" + entry ]

    if (os.path.isdir(directory + "/" + entry)):
      if re.search(pattern, entry):
        elements += [ directory + "/" + entry ]

      if (not os.path.islink(directory + "/" + entry)):
        elements += Search(directory + "/" + entry, pattern)

  return elements

#
# this function computes the checksum.
#
def             Checksum(data):
  sum = 0
  c = None

  # compute the XOR checksum.
  for c in data:
    sum = sum ^ ord(c)

  return sum

#
# this function writes to the serial channel.
#
TypeNone = 0
TypeCommand = 1
TypeText = 2

def             Emit(line, command):
  format = "<IBII" + str(len(command) + 1) + "s"

  packet = struct.pack(format,
                       Magic,
                       TypeCommand,
                       len(command) + 1,
                       Checksum(command),
                       command + "\0")

  line.write(packet)

#
# this function reads on the serial channel.
#
def             Receive(line):
  magic = None
  type = None
  length = None
  crc = None
  message = None

  magic = struct.unpack("<I", line.read(4))[0]
  type = struct.unpack("<B", line.read(1))[0]
  length = struct.unpack("<I", line.read(4))[0]
  crc = struct.unpack("<I", line.read(4))[0]

  if (magic != Magic):
    return (StatusError, (TypeNone, "invalid magic number"))

  message = line.read(length).strip("\r")

  if (crc != Checksum(message)):
    return (StatusError, (TypeNone, "invalid CRC"))

  return (StatusOk, (type, message))

#
# this function performs the handshake by waiting for
# the "[ready]" command message.
#
def             Handshake(line):
  status = None
  type = None
  message = None

  # read the serial line.
  (status, (type, message)) = Receive(line)

  if status != StatusOk:
    return (StatusError, message)

  if (type != TypeCommand) or (message != "[ready]"):
    return (StatusError, "invalid message")

  return (StatusOk, None)

#
# this function calls a test and return the output.
#
def             Call(line, symbol):
  output = None

  Emit(line, "[call] " + symbol)

  start = time.clock()
  while True:
    (status, (type, message)) = Receive(line)

    if status != StatusOk:
      return (StatusError, None, message)

    if (type == TypeCommand) and (message == "[enter]"):
      output = str()
      continue

    if (type == TypeCommand) and (message == "[leave]"):
      break

    if isinstance(output, str) == True:
      output += message
  end = time.clock()

  return (StatusOk, end - start, output)

#
# this function loads the manifests and initialize the
# test states.
#
def             Manifests(context):
  stream = None
  component = None
  
  # read the file.
  stream = yaml.load(file(DirectorySuites + "/" + context["suite"] + ".suite", 'r'))

  # initialize the manifests and tests.
  context["manifests"] = {}

  # load the manifests.
  if "components" in stream:
    for component in stream["components"]:
      # search for manifest files.
      manifests = Search(DirectoryTests + "/" + stream["components"][component], "^.*\.mnf$")

      # initialize the data structure.
      context["manifests"][component] = {}

      for manifest in manifests:
        context["manifests"][component][os.path.splitext(os.path.basename(manifest))[0]] = yaml.load(file(manifest, 'r'))

#
# this function initializes the report according to the manifests.
#
def             Report(context):
  component = None
  manifest = None
  
  context["report"] = {}

  # go through the manifests.
  for component in context["manifests"]:
    context["report"][component] = {}

    for manifest in context["manifests"][component]:
      context["report"][component][manifest] = { "duration": None,
                                                 "output": None,
                                                 "status": None,
                                                 "description": context["manifests"][component][manifest]["description"] }

#
# this function creates a bootable cdrom image from the
# snapshot image.
#
def             ISO(context):
  status = None

  # create a temporary file.
  context["iso"] = Temporary(OptionFile)

  # create an ISO file.
  os.system("mkisofs -pad -b " + os.path.basename(context["image"]) + " -R -o " + context["iso"] + " " + context["image"] + " >/dev/null 2>&1")

#
# this function generates a Xen configuration file for
# the run virtual machine.
#
def             Configuration(context):
  configuration = None

  # create a temporary file.
  context["configuration"] = Temporary(OptionFile)

  # configuration.
  configuration = """
kernel = '/usr/lib/xen/boot/hvmloader'
builder = 'hvm'
memory = 128
name = '%(name)s'
disk = ['tap:aio:%(iso)s,hdc:cdrom,r']
boot = 'd'
vnc = 1
serial = 'pty'
""" % { "name": context["name"],
        "iso": context["iso"] }

  # save the configuration in a file.
  Push(context["configuration"], configuration)

#
# this function launches a virtual machine according
# to the given configuration file.
#
def             Run(context):
  function = None

  # launch Xen and connect a console to it so that we know when
  # it is finished.
  function = Timeout(os.system, TimeoutRun)

  try:
    # launch the Xen virtual machine.
    function("xm create " + context["configuration"] + " >/dev/null 2>&1")

    # call the tests.
    Suite(context)
  except Exception, exception:
    # destroy the vm.
    os.system("xm destroy " + context["name"] + " >/dev/null 2>&1")

    print exception

    Clean(context)

    sys.exit(42)

#
# this function runs the tests suite.
#
def             Suite(context):
  status = None
  message = None
  content = None
  m = None
  port = None
  component = None
  line = None

  # read the status file.
  content = Pull("/var/log/xen/qemu-dm-" + context["name"] + ".log")

  # retrieve the serial port.
  m = re.search("char device redirected to (.*)$", content, re.MULTILINE)

  if not m:
    raise Exception, "unable to retrieve the serial port"

  # set the port.
  port = m.group(1)

  # load the manifests.
  Manifests(context)

  # initialize the report.
  Report(context)

  # initialize the serial line.
  line = serial.Serial(port, 57600)

  # perform the handshake.
  (status, message) = Handshake(line)

  if status == StatusError:
    raise Exception, "[error] handshake failed '" + message + "'"

  # launch the tests.
  for component in context["manifests"]:
    for manifest in context["manifests"][component]:
   
      (context["report"][component][manifest]["status"],
       context["report"][component][manifest]["duration"],
       context["report"][component][manifest]["output"]) = Call(line,
                                                                context["manifests"][component][manifest]["symbol"])

#
# this function cleans what has been created by this script.
#
def             Clean(context):
  # destroy the VM.
  os.system("xm destroy " + context["name"] + " >/dev/null 2>&1")

  # remove the temporary files.
  if "iso" in context:
    Remove(context["iso"])
    del context["iso"]

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
# this function
#
# this is the main function
#
def             Main():
  path = None
  iso = None
  summary = None

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # retrieve arguments.
  path = sys.argv[1]

  # load the context.
  context = Extract(path)

  try:
    # create a bootable disk image.
    ISO(context)

    # generate a configuration file.
    Configuration(context)

    # run the virtual machine.
    Run(context)
  except Exception, exception:
    print exception

    Clean(context)

    sys.exit(42)
    
  # clean the temporary stuff.
  Clean(context)

  # update the context.
  Serialize(context, path)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  try:
    Main()
  except Exception, exception:
    print exception

    sys.exit(42)
