#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/server/scheduler.py
#
# created       julien quintard   [mon mar 23 12:39:26 2009]
# updated       julien quintard   [fri feb 25 20:17:16 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script takes snapshots from the queue and test them according to
# the information contained in the report's meta data.
#
# once the testing is done, an email is sent to the users associated with
# the report's capability after which they will be able to consult the report.
#
# IMPORTANT! should the server fail to produce relevant reports, the
#   administrator should look for two possible explanations:
#     1) there are too many mounted loop devices.
#        one can detect these temporary mounts through either:
#          $> mount
#        or:
#          $> losetup -a
#     2) there already is a virtual machine running, being Xen or QEMU
#        for QEMU, once can detect such running virtual machine through:
#          $> ps aux | grep -i qemu
#        for Xen, the following command must only return the Domain-0:
#          $> xm list
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

import time

import ktp

#
# ---------- constants --------------------------------------------------------
#

# directories
ScriptsDirectory = TestDirectory + "/scripts"
StoreDirectory = TestDirectory + "/store"

# stores
ReportStore = StoreDirectory + "/report"
QueueStore = StoreDirectory + "/queue"
LogStore = StoreDirectory + "/log"
CapabilityStore = StoreDirectory + "/capability"

# scripts
TestScript = ScriptsDirectory + "/test.py"

#
# ---------- functions --------------------------------------------------------
#

#
# this function walk through the unit and counts the number of successful
# tests.
#
def                     Count(tests):
  passed = 0
  failed = 0
  total = 0

  # explore the tests.
  for test in tests:
    if tests[test]["status"] == True:
      passed += 1
    else:
      failed += 1

    total += 1

  return (passed, failed, total)

#
# this function displays a brief overview of the given report.
#
def                     Overview(report, margin=""):
  content = None
  passed = None
  failed = None
  total = None
  status = None
  length = None
  count = None
  unit = None
  tests = None

  # initialize the content
  content = str()

  # for the units of the report.
  for unit in report["data"]:
    # retrieve the tests.
    tests = report["data"][unit]

    # compute the status of all the tests of the given unit.
    (passed, failed, total) = Count(tests)

    # generate the counter.
    count = " [" + str(passed) + "/" + str(total) + "]"

    # generate the unit's status.
    length = 79 - len(unit) - len(margin) - 4 - len(count)
    status = length * " " + count

    # display the unit name.
    content += margin + unit + status + "\n"

  return content

#
# this function sends an email to the report's owner.
#
def                     Email(identifier):
  capability = None
  report = None
  emails = None
  summary = None
  detail = None

  ktp.log.Record(LogStore,
                 "#(scheduler) message(emailing the users)")

  # load the final report in order to send it back to the client.
  report = ktp.report.Load(ReportStore + "/" + identifier +             \
                             ktp.report.Extension)

  # load the capability in order to retrieve the email.
  capability = ktp.capability.Load(CapabilityStore + "/" +              \
                                     report["meta"]["user"] +           \
                                     ktp.capability.Extension)

  ktp.log.Record(LogStore,
                 "#(scheduler) capability(" + str(capability) + ")")

  # build the message.
  if "error" in report["meta"]:
    message = """\
environment(%(environment)s) :: platform(%(platform)s) :: architecture(%(architecture)s) :: suite(%(suite)s) :: duration(%(duration)ss)

---[ Error

%(error)s
""" % { "environment": report["meta"]["environments"]["stress"],
        "platform": report["meta"]["platform"],
        "architecture": report["meta"]["architecture"],
        "suite": report["meta"]["suite"],
        "duration": report["meta"]["duration"],
        "error": report["meta"]["error"] }
  else:
    message = """\
environment(%(environment)s) :: platform(%(platform)s) :: architecture(%(architecture)s) :: suite(%(suite)s) :: duration(%(duration)ss) :: summary(%(passed)s/%(total)s)

---[ Overview

%(overview)s

---[ Detail

%(detail)s
""" % { "environment": report["meta"]["environments"]["stress"],
        "platform": report["meta"]["platform"],
        "architecture": report["meta"]["architecture"],
        "suite": report["meta"]["suite"],
        "duration": report["meta"]["duration"],
        "passed": report["meta"]["summary"]["passed"],
        "total": report["meta"]["summary"]["total"],
        "overview": Overview(report, "    "),
        "detail": ktp.miscellaneous.Stringify(report, "    ") }

  # initialize the emails variable.
  emails = []

  # compute the email addresses.
  for member in capability["members"]:
    emails += [ member["email"] ]

  # add the admin@opaak.org address to the list of emails.
  emails += [ "admin@opaak.org" ]

  # send the email.
  ktp.miscellaneous.Email("admin@opaak.org",
                          emails,
                          "'kaneton report " + identifier + "'",
                          message)

#
# this function takes the identifier of a report/snapshot and tests
# it.
#
def                     Test(identifier):
  report = None
  stream = None
  output = None

  try:
    # generate a temporary file.
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    ktp.log.Record(LogStore,
                   "#(scheduler) message(launching the test script)")

    # launch the test script.
    status =                                                            \
      ktp.process.Invoke(TestScript,
                         [ "--report", identifier ],
                         stream = stream,
                         option = ktp.process.OptionNone)

    ktp.log.Record(LogStore,
                   "#(scheduler) message(test script completed)");

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    ktp.log.Record(LogStore,
                   "#(scheduler) status(" + str(status) + ") " +        \
                     "output(" + str(output) + ")")

    # load the report.
    report = ktp.report.Load(ReportStore + "/" + identifier +           \
                               ktp.report.Extension)

    # update the report's meta section if the whole process failed.
    if status == ktp.StatusError:
      # include the error message within the report's meta data.
      report["meta"]["error"] = output

      ktp.log.Record(LogStore,
                     "#(scheduler) message(recorded the error in the report)");

    # change the report's state to 'done'.
    report["meta"]["state"] = ktp.report.StateDone

    # store the report.
    ktp.report.Store(report,
                     ReportStore + "/" + identifier + ktp.report.Extension)
  except Exception, exception:
    ktp.log.Record(LogStore,
                   "#(scheduler) exception(" +                          \
                     str(exception) + ") trace(" +                      \
                     ktp.trace.Generate() + ")")
  finally:
    # remove temporary files.
    if stream:
      ktp.miscellaneous.Remove(stream)

#
# this function initializes the script
#
def                     Initialize():
  ktp.log.Record(LogStore,
                 "#(scheduler) message(initializing)")

#
# this function cleans the script
#
def                     Clean():
  ktp.log.Record(LogStore,
                 "#(scheduler) message(cleaning)")

#
# the main function gets a snapshot from the queue and test it.
# if there is nothing, just wait a bit.
#
def                     Main():
  identifier = None

  while True:
    # retrieve the identifier of the next scheduled snapshot
    identifier = ktp.queue.Pop(QueueStore)

    # if there is a snapshot scheduler, test it.
    if identifier:
      ktp.log.Record(LogStore,
                     "#(scheduler) identifier(" + identifier + ")")

      # test the snapshot.
      Test(identifier)

      # email the report's user of the completion.
      Email(identifier)

    ktp.log.Record(LogStore,
                   "#(scheduler) message(sleeping)")

    # sleep for 5 minutes.
    time.sleep(300)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
  # initialize the system.
  Initialize()

  # main function
  Main()

  # clean the system.
  Clean()
