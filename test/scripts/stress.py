#! /usr/bin/python2
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
# updated       julien quintard   [sun mar  6 17:19:01 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script takes a bootable kaneton image and launches every test in an
# emulated environment.
#
# the script then establishes a connection with the emulated environment
# through a serial port. this connection is used to both trigger the test
# and retrieve the output and signature.
#
# upon completion, a list of results is gathered containing the output, status
# and duration of the test. these results are finally saved in a bulletin
# file.
#
# note that every test is triggered in a separate emulated environment in order
# to ensure that previous tests did not alter the kernel state, hence the
# outcome of other tests.
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
StoreDirectory = TestDirectory + "/store"

# stores
LogStore = StoreDirectory + "/log"

# magic number for serial communications.
Magic = 0xF4859632

# tokens.
ReadyToken = "[ready]"
CallToken = "[call]"
EnterToken = "[enter]"
SignatureToken = "[signature]"
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
  ktp.log.Record(LogStore,
                 "#(stress) emit(" + command + ")")

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

  ktp.log.Record(LogStore,
                 "#(stress) receive(" + message + ")")

  if (crc != Checksum(message)):
    return (StatusError, (TypeNone, "invalid CRC"))

  return (StatusOk, (type, message))

#
# this function calls a test and return the result.
#
def                     Call(namespace,
                             line,
                             symbol):
  start = None
  end = None

  ktp.log.Record(LogStore,
                 "#(stress) call(" + symbol + ")")

  # send the command.
  Emit(line, CallToken + " " + symbol)

  # start the timer.
  start = time.time()

  # initialize the signature.
  namespace.signature = str()

  # read the result.
  while True:
    # wait for the response.
    (status, (type, message)) = Receive(line)

    # if an error occured, signal it.
    if status != StatusOk:
      return (StatusError, "%.3f" % (end - start))

    # handle and ignore the ready command.
    if (type == TypeCommand) and (message == ReadyToken):
      continue

    # handle the enter command.
    if (type == TypeCommand) and (message == EnterToken):
      namespace.text = str()
      continue

    # handle the signature command.
    if (type == TypeCommand) and                                        \
       (message[:len(SignatureToken)] == SignatureToken):
      namespace.signature += message[len(SignatureToken) + 1:]
      continue

    # handle the leave command.
    if (type == TypeCommand) and (message == LeaveToken):
      break

    # treat the test content.
    if isinstance(namespace.text, str) == True:
      namespace.text += message

  # stop the timer.
  end = time.time()

  # transform the text accordingly.
  if not namespace.text:
    namespace.text = str()

  return (StatusOk, "%.3f" % (end - start))

#
# ---------- functions --------------------------------------------------------
#

#
# this function displays an error message, cleans the script before
# exiting.
#
def                     Error(namespace, message):
  # display the script name.
  print("[stress]")

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

  ktp.log.Record(LogStore,
                 "#(stress) message(creating the ISO)")

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

  ktp.log.Record(LogStore,
                 "#(stress) message(loading the manifests)")

  # read the file.
  stream = ktp.suite.Load(SuitesDirectory + "/" +                       \
                            namespace.suite + ktp.suite.Extension)

  # initialize the manifests and tests.
  namespace.manifests = {}

  # load the manifests.
  if "components" in stream:
    for component in stream["components"]:
      # initialize the data structure.
      namespace.manifests[component] = {}

      for path in stream["components"][component]:
        # search for manifest files.
        manifests = ktp.miscellaneous.Search(TestsDirectory + "/" +     \
                                               path,
                                             "^.*\.mnf$",
                                             ktp.miscellaneous.OptionFile |
                                             ktp.miscellaneous.OptionRecursive)

        for manifest in manifests:
          # load the manifest.
          test = ktp.manifest.Load(manifest)

          # check if the test includes a name.
          if not "name" in test:
            raise Exception("the manifest '" +                          \
                              manifest +                                \
                              "' does not embed a test name")

          # compute the proper name according to the defined component:
          # for instance, given the "segment" component, the test name
          # segment/permissions/01 will be transformed into permissions/01
          if component == test["name"][:len(component)]:
            name = test["name"][len(component) + 1:]
          else:
            name = test["name"]

          # transform the output accordingly.
          if not test["output"]:
            test["output"] = str()

          # set the test.
          namespace.manifests[component][name] = test

#
# this function initializes the results according to the manifests.
#
def                     Results(namespace):
  component = None
  test = None

  ktp.log.Record(LogStore,
                 "#(stress) message(initializing the results)")

  # initialize the results
  namespace.results = {}

  # go through the manifests.
  for component in namespace.manifests:
    namespace.results[component] = {}

    # for every test.
    for test in namespace.manifests[component]:
      namespace.results[component][test] =                              \
        {
          "duration": None,
          "output": None,
          "status": None,
          "description": namespace.manifests[component][test]["description"]
        }

#
# this is the alarm handler.
#
def                     Handler(signum, frame):
  raise Exception("[timeout] expired")

#
# this function runs QEMU.
#
def                     QEMU(namespace,
                             symbol,
                             timeout):
  stream = None
  monitor = None
  output = None
  content = None
  match = None
  port = None
  line = None
  status = None
  message = None
  duration = None

  ktp.log.Record(LogStore,
                 "#(stress) environment(QEMU)")

  # build a stream file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  ktp.log.Record(LogStore,
                 "#(stress) message(setting up the timer)")

  # set the alarm signal.
  signal.signal(signal.SIGALRM, Handler)
  signal.alarm(6 + timeout)

  try:
    ktp.log.Record(LogStore,
                   "#(stress) message(launching QEMU)")

    # invoke the qemu virtual machine.
    monitor = ktp.process.Invoke("qemu",
                                 [ "-nographic",
                                   "-serial", "pty",
                                   "-monitor", "null",
                                   "-cdrom", namespace.iso ],
                                 stream = stream,
                                 option = ktp.process.OptionBackground)

    # wait a few seconds to be sure QEMU has started.
    time.sleep(3)

    ktp.log.Record(LogStore,
                   "#(stress) message(reading the output)")

    # read the status file.
    content = ktp.miscellaneous.Pull(stream)

    # check if this operation has been successful.
    if not content:
      raise Exception("[error] unable to retrieve QEMU's log file")

    ktp.log.Record(LogStore,
                   "#(stress) message(extracting the serial port)")

    # retrieve the serial port.
    match = re.search("char device redirected to (.*)$", content, re.MULTILINE)

    # check if it was successful.
    if not match:
      raise Exception("[error] unable to retrieve the serial port")

    # set the port.
    port = match.group(1)

    ktp.log.Record(LogStore,
                   "#(stress) message(initiating the serial communication)")

    # initialize the serial line.
    line = serial.Serial(port, 57600)

    # wait for the serial handshake to proceed.
    time.sleep(3)

    ktp.log.Record(LogStore,
                   "#(stress) message(triggering the test)")

    # trigger the test.
    (status, duration) =                                                \
       Call(namespace, line, symbol)

    ktp.log.Record(LogStore,
                   "#(stress) message(retrieving the test output)")

    # set the output
    output = namespace.text

    ktp.log.Record(LogStore,
                   "#(stress) message(terminating the VM)")

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
    ktp.log.Record(LogStore,
                   "#(stress) message(destroying the VM)")

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

    # set the status.
    status = ktp.StatusError

    ktp.log.Record(LogStore,
                   "#(stress) message(generating the error output)")

    # retrieve the emulator's output.
    output = """
%(emulator)s
%(test)s
%(exception)s
""" % { "emulator": ktp.miscellaneous.Pull(stream),
        "test": namespace.text,
        "exception": str(exception) }

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

  ktp.log.Record(LogStore,
                 "#(stress) message(resetting the timer)")

  # reset the alarm.
  signal.alarm(0)

  return (status, duration, output)

#
# this function generates a Xen configuration file for
# the run virtual machine and runs it.
#
def                     Xen(namespace,
                            symbol,
                            timeout):
  configuration = None
  stream = None
  monitor = None
  output = None
  content = None
  match = None
  line = None
  status = None
  message = None
  port = None
  duration = None

  ktp.log.Record(LogStore,
                 "#(stress) environment(Xen)")

  # create a temporary file.
  namespace.configuration = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  ktp.log.Record(LogStore,
                 "#(stress) message(generating the Xen configuration)")

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

  ktp.log.Record(LogStore,
                 "#(stress) message(setting up the timer)")

  # set the alarm signal.
  signal.signal(signal.SIGALRM, Handler)
  signal.alarm(6 + timeout)

  try:
    ktp.log.Record(LogStore,
                   "#(stress) message(launching Xen)")

    # invoke the xen virtual machine.
    monitor = ktp.process.Invoke("xm",
                                 [ "create",
                                   namespace.configuration ],
                                 stream = stream,
                                 option = ktp.process.OptionBackground)

    # wait a few seconds to be sure Xen has started.
    time.sleep(3)

    ktp.log.Record(LogStore,
                   "#(stress) message(retrieving the QEMU log file)")

    # read the status file.
    content = ktp.miscellaneous.Pull("/var/log/xen/qemu-dm-" +          \
                                       namespace.name + ".log")

    # check if this operation has been successful.
    if not content:
      raise Exception("[error] unable to retrieve QEMU's log file")

    ktp.log.Record(LogStore,
                   "#(stress) message(extracting the serial port)")

    # retrieve the serial port.
    match = re.search("char device redirected to (.*)$", content, re.MULTILINE)

    # check if it was successful.
    if not match:
      raise Exception("[error] unable to retrieve the serial port")

    # set the port.
    port = match.group(1)

    ktp.log.Record(LogStore,
                   "#(stress) message(initiating the serial communication)")

    # initialize the serial line.
    line = serial.Serial(port, 57600)

    # wait for the serial handshake to proceed.
    time.sleep(3)

    ktp.log.Record(LogStore,
                   "#(stress) message(triggering the test)")

    # trigger the test.
    (status, duration) =                                                \
       Call(namespace, line, symbol)

    ktp.log.Record(LogStore,
                   "#(stress) message(retrieving the test output)")

    # set the output
    output = namespace.text

    ktp.log.Record(LogStore,
                   "#(stress) message(terminating the VM)")

    # destroy the virtual machine instance.
    ktp.process.Invoke("xm",
                       [ "destroy",
                         namespace.name ])

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)
  except Exception, exception:
    ktp.log.Record(LogStore,
                   "#(stress) message(destroying the VM)")

    # destroy the virtual machine instance.
    ktp.process.Invoke("xm",
                       [ "destroy",
                         namespace.name ])

    # set the status.
    status = ktp.StatusError

    ktp.log.Record(LogStore,
                   "#(stress) message(generating the error output)")

    # retrieve the emulator's output.
    output = """
%(emulator)s
%(test)s
%(exception)s
""" % { "emulator": ktp.miscellaneous.Pull(stream),
        "test": namespace.text,
        "exception": str(exception) }

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

  ktp.log.Record(LogStore,
                 "#(stress) message(resetting the timer)")

  # reset the alarm.
  signal.alarm(0)

  return (status, duration, output)

#
# this function stresses the microkernel with the tests suite.
#
def                     Suite(namespace):
  status = None
  component = None
  test = None
  duration = None
  output = None

  # load the manifests.
  Manifests(namespace)

  # initialize the results.
  Results(namespace)

  ktp.log.Record(LogStore,
                 "#(stress) message(going through the test suite)")

  # launch the tests.
  for component in namespace.manifests:
    for test in namespace.manifests[component]:
      ktp.log.Record(LogStore,
                     "#(stress) test(" +                                \
                       namespace.manifests[component][test]["symbol"] + \
                       ")")

      # launch the testing process according to the environment.
      if namespace.environment == ktp.environment.QEMU:
        (status, duration, output) =                                    \
            QEMU(namespace,
                 namespace.manifests[component][test]["symbol"],
                 namespace.manifests[component][test]["timeout"]["qemu"])
      elif namespace.environment == ktp.environment.Xen:
        (status, duration, output) =                                    \
            Xen(namespace,
                namespace.manifests[component][test]["symbol"],
                 namespace.manifests[component][test]["timeout"]["xen"])
      else:
        Error(namespace,
              "unknown environment '" + namespace.environment + "'")

      # fill the results.
      namespace.results[component][test]["duration"] = duration
      namespace.results[component][test]["output"] = output

      if (status == StatusError) or                                     \
         (output != namespace.manifests[component][test]["output"]):
        namespace.results[component][test]["status"] = False
      else:
        namespace.results[component][test]["status"] = True

        if namespace.signature !=                                       \
           namespace.manifests[component][test]["signature"]:
          namespace.results[component][test]["status"] = False
          namespace.results[component][test]["output"] += """
[signature] invalid
"""

      ktp.log.Record(LogStore,
                     "#(stress) status(" + str(status) + ") " +         \
                       "duration(" + str(duration) + ") " +             \
                       "output(" + str(output) + ")")

#
# this function initializes the script.
#
def                     Initialize(namespace):
  ktp.log.Record(LogStore,
                 "#(stress) message(initializing)")

  # initialize the local variables.
  namespace.iso = None
  namespace.configuration = None
  namespace.text = None
  namespace.signature = None

#
# this function cleans what has been created by this script.
#
def                     Clean(namespace):
  ktp.log.Record(LogStore,
                 "#(stress) message(cleaning)")

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

  # parse the arguments.
  namespace = g_parser.parse_args()

  # initialize the script.
  Initialize(namespace)

  # create a bootable disk image.
  ISO(namespace)

  # launch the test suite.
  Suite(namespace)

  # clean the temporary stuff.
  Clean(namespace)

  # save the results if necessary.
  if namespace.bulletin:
    ktp.bulletin.Store(namespace.results, namespace.bulletin)

    ktp.log.Record(LogStore,
                   "#(stress) message(bulleting saved in '" +           \
                     namespace.bulletin + "')")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
