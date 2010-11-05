#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/scripts/stress.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [fri nov  5 22:08:34 2010]
#

#
# ---------- information ------------------------------------------------------
#
# this script takes a bootable kaneton image and launches it in an emulated
# environment.
#
# the script then establishes a connection with the emulated environment
# through a serial port. this connection is used to both trigger tests
# and retrieving the output of those tests.
#
# upon completion, a list of results is gathered containing the output, status
# and duration of the test. these results are finally saved in a bulletin
# file.
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
# ---------- imports ----------------------------------------------------------
#

import sys
import os
import signal
import serial
import struct
import re
import argparse
import time

import ktp

#
# ---------- constants --------------------------------------------------------
#

# directories
SuitesDirectory = TestDirectory + "/suites"
TestsDirectory = TestDirectory + "/tests"

# environments
XenEnvironment = "xen"
QEMUEnvironment = "qemu"

# timeout in seconds.
XenTimeout = 150 # XXX
QEMUTimeout = 150 # XXX

# magic number for serial communications.
Magic = 0xF4859632

# tokens.
ReadyToken = "[ready]"
CallToken = "[call]"
EnterToken = "[enter]"
LeaveToken = "[leave]"

# types.
TypeNone = 0
TypeCommand = 1
TypeText = 2

# statuses.
StatusOk = True
StatusError = False

#
# ---------- globals ----------------------------------------------------------
#

# the command-line parser.
g_parser = None

#
# ---------- serial -----------------------------------------------------------
#

#
# this function computes the checksum.
#
def                     Checksum(data):
  sum = 0
  c = None

  # compute the XOR checksum.
  for c in data:
    sum = sum ^ ord(c)

  return sum

#
# this function writes to the serial channel.
#
def                     Emit(line, command):
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
def                     Receive(line):
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
# the ready token.
#
def                     Handshake(line):
  status = None
  type = None
  message = None

  # read the serial line.
  (status, (type, message)) = Receive(line)

  if status != StatusOk:
    return (StatusError, message)

  if (type != TypeCommand) or (message != ReadyToken):
    return (StatusError, "invalid message")

  return (StatusOk, None)

#
# this function calls a test and return the output.
#
def                     Call(line, symbol):
  output = None
  start = None
  end = None

  Emit(line, CallToken + " " + symbol)

  start = time.time()
  while True:
    (status, (type, message)) = Receive(line)

    if status != StatusOk:
      return (StatusError, None, message)

    if (type == TypeCommand) and (message == EnterToken):
      output = str()
      continue

    if (type == TypeCommand) and (message == LeaveToken):
      break

    if isinstance(output, str) == True:
      output += message
  end = time.time()

  return (StatusOk, "%.3f" % (end - start), output)

#
# ---------- functions --------------------------------------------------------
#

#
# this function displays an error message, cleans the script before
# exiting.
#
def                     Error(namespace, message):
  # display the script name.
  print "[stress]"

  # print the message.
  if message:
    print(message)

  # clean the script.
  Clean(namespace)

  # exit with an error code.
  sys.exit(42)

#
# this function creates a bootable cdrom image from the
# snapshot image.
#
def                     ISO(namespace):
  # create a temporary file.
  namespace.iso = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # create an ISO file.
  if ktp.process.Invoke("mkisofs",
                        [ "-pad",
                          "-b", os.path.basename(namespace.image),
                          "-R",
                          "-o", namespace.iso,
                          namespace.image ] ) == ktp.StatusError:
    Error(namespace,
          "unable to create the ISO file system")

#
# this function loads the manifests and initialize the
# test states.
#
def                     Manifests(namespace):
  stream = None
  component = None
  test = None
  name = None

  # read the file.
  stream = ktp.suite.Load(SuitesDirectory + "/" + namespace.suite + ktp.suite.Extension)

  # initialize the manifests and tests.
  namespace.manifests = {}

  # load the manifests.
  if "components" in stream:
    for component in stream["components"]:
      # search for manifest files.
      manifests = ktp.miscellaneous.Search(TestsDirectory + "/" +       \
                                             stream["components"][component],
                                           "^.*\.mnf$",
                                           ktp.miscellaneous.OptionFile |
                                           ktp.miscellaneous.OptionRecursive)

      # initialize the data structure.
      namespace.manifests[component] = {}

      for manifest in manifests:
        # load the manifest.
        test = ktp.manifest.Load(manifest)

        # compute the proper name according to the defined component:
        # for instance, given the "segment" component, the test name
        # segment/permissions/01 will be transformed into permissions/01
        if component == test["name"][:len(component)]:
          name = test["name"][len(component) + 1:]
        else:
          name = test["name"]

        # set the test.
        namespace.manifests[component][name] = test

#
# this function initializes the results according to the manifests.
#
def                     Results(namespace):
  component = None
  test = None

  # initialize the results
  namespace.results = {}

  # go through the manifests.
  for component in namespace.manifests:
    namespace.results[component] = {}

    # for every test.
    for test in namespace.manifests[component]:
      namespace.results[component][test] = { "duration": None,
                                             "output": None,
                                             "status": None,
                                             "description": namespace.manifests[component][test]["description"] }

#
# this function stresses the microkernel with the tests suite.
#
def                     Suite(namespace,
                              port):
  line = None
  status = None
  message = None
  component = None
  test = None

  # load the manifests.
  Manifests(namespace)

  # initialize the results.
  Results(namespace)

  # initialize the serial line.
  line = serial.Serial(port, 57600)

  # perform the handshake.
  (status, message) = Handshake(line)

  if status == StatusError:
    raise Exception("[error] handshake failed '" + message + "'")

  # launch the tests.
  for component in namespace.manifests:
    # verbose messaging.
    if namespace.verbose:
      print(component)

    for test in namespace.manifests[component]:
      # verbose.
      if namespace.verbose:
        # XXX[change for python 2.7 ... 3 with python("  " + test, end="")]
        print "  " + test,

      (namespace.results[component][test]["status"],
       namespace.results[component][test]["duration"],
       namespace.results[component][test]["output"]) =                  \
         Call(line,
              namespace.manifests[component][test]["symbol"])

      # verbose messaging.
      if namespace.verbose:
        print(":: " +                                                   \
                str(namespace.results[component][test]["status"]).lower())

#
# this is the alarm handler.
#
def                     Handler(signum, frame):
  raise Exception("[timeout]")

#
# this function runs QEMU.
#
def                     QEMU(namespace):
  stream = None
  monitor = None
  output = None
  content = None
  match = None
  port = None

  # build a stream file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # set the alarm signal.
  signal.signal(signal.SIGALRM, Handler)
  signal.alarm(QEMUTimeout)

  try:
    # invoke the qemu virtual machine.
    monitor = ktp.process.Invoke("qemu",
                                 [ "-nographic",
                                   "-serial", "pty",
                                   "-monitor", "null",
                                   "-cdrom", namespace.iso ],
                                 stream = stream,
                                 option = ktp.process.OptionBackground)

    # wait a few seconds to be sure Xen has started.
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

    # launch the test suite.
    Suite(namespace, port)

    # destroy the virtual machine instance.
    try:
      os.kill(monitor[0].pid, signal.SIGKILL)
    except:
      pass

    # wait for the process to terminate.
    ktp.process.Wait(monitor)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)
  except Exception, exception:
    # destroy the virtual machine by terminating
    # the process.
    os.kill(monitor[0].pid, signal.SIGKILL)

    # wait for the process to terminate.
    ktp.process.Wait(monitor)

# XXX[not possible with Python 2.5 since subprocess does not provide this functionality]
#    [when ready remove: os.kill, Wait and Pull]
#    output = ktp.process.Terminate(monitor)

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    # display the output.
    if output:
      print(output)

    # stop the script.
    Error(namespace,
          str(exception))

  # reset the alarm.
  signal.alarm(0)

#
# this function generates a Xen configuration file for
# the run virtual machine and runs it.
#
def                     Xen(namespace):
  configuration = None
  stream = None
  monitor = None
  output = None
  content = None
  match = None
  port = None

  # create a temporary file.
  namespace.configuration = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # configuration.
  configuration = """
kernel = "/usr/lib/xen/boot/hvmloader"
builder = "hvm"
memory = 256
name = "%(name)s"
disk = ["tap:aio:%(iso)s,hdc:cdrom,r"]
boot = 'd'
serial = "pty"
""" % { "name": namespace.name,
        "iso": namespace.iso }

  # save the configuration in a file.
  ktp.miscellaneous.Push(configuration, namespace.configuration)

  # build a stream file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # set the alarm signal.
  signal.signal(signal.SIGALRM, Handler)
  signal.alarm(XenTimeout)

  try:
    # invoke the xen virtual machine.
    monitor = ktp.process.Invoke("xm",
                                 [ "create",
                                   namespace.configuration ],
                                 stream = stream,
                                 option = ktp.process.OptionBackground)

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

    # launch the test suite.
    Suite(namespace, port)

    # destroy the virtual machine instance.
    ktp.process.Invoke("xm",
                       [ "destroy",
                         namespace.name ])

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)
  except Exception, exception:
    # destroy the virtual machine instance.
    ktp.process.Invoke("xm",
                       [ "destroy",
                         namespace.name ])

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    # display the output.
    if output:
      print(output)

    # stop the script.
    Error(namespace,
          str(exception))

  # reset the alarm.
  signal.alarm(0)

#
# this function initializes the script.
#
def                     Initialize(namespace):
  # initialize the local variables.
  namespace.iso = None
  namespace.configuration = None

#
# this function cleans what has been created by this script.
#
def                     Clean(namespace):
  if namespace.iso:
    ktp.miscellaneous.Remove(namespace.iso)

  if namespace.configuration:
    ktp.miscellaneous.Remove(namespace.configuration)

#
# this is the main function
#
def                     Main():
  global g_parser

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script runs "    \
                                       "tests against the given "       \
                                       "kaneton image.")

  # set up the authorised arguments.
  g_parser.add_argument("--name", '-n',
                        default = time.strftime("%Y%m%d-%H%M%S"),
                        help = "a unique name for the operation",
                        dest = "name")
  g_parser.add_argument("--image", '-i',
                        required = True,
                        help = "the path to the kaneton image to stress",
                        dest = "image")
  g_parser.add_argument("--environment", '-e',
                        required = True,
                        help = "the environment in which the image "    \
                          "is to be tested",
                        dest = "environment")
  g_parser.add_argument("--bulletin", '-b',
                        default = None,
                        help = "specify the path to the bulletin "      \
                          "file to generate",
                        dest = "bulletin")
  g_parser.add_argument("--suite", '-s',
                        required = True,
                        help = "the test suite to be used to stress "   \
                          "the kaneton implementation",
                        dest = "suite")
  g_parser.add_argument("--verbose", '-v',
                        default = False,
                        action = "store_true",
                        help = "activate the verbose messaging",
                        dest = "verbose")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # create a bootable disk image.
  ISO(namespace)

  # launch the testing process according to the environment.
  if namespace.environment == XenEnvironment:
    Xen(namespace)
  elif namespace.environment == QEMUEnvironment:
    QEMU(namespace)
  else:
    Error(namespace,
          "unknown environment '" + namespace.environment + "'")

  # clean the temporary stuff.
  Clean(namespace)

  # save the results if necessary.
  if namespace.bulletin:
    ktp.bulletin.Store(namespace.results, namespace.bulletin)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
