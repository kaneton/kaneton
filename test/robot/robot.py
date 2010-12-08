#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/robot/robot.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [wed dec  8 22:10:18 2010]
#

#
# ---------- information ------------------------------------------------------
#
# this script is called on a regular basis in order to test the current
# kaneton research implementation.
#
# the script starts by retrieving the kaneton implementation by checking out
# the repository. then, several test suites are requested to the server.
#
# finally a report is build and sent to the kaneton contributors mailing list.
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

import time

import ktp

#
# ---------- constants --------------------------------------------------------
#

# the subversion address to the kaneton repository.
Repository = "svn+ssh://subversion@repositories.passeism.org/kaneton"

# the email address to which the reports must be sent.
Email = "contributors@kaneton.opaak.org"

# environments.
Environments = [ "qemu", "xen" ]

#
# ---------- globals ----------------------------------------------------------
#

# the temporary directory.
g_directory = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function displays an error message, cleans the script before
# exiting.
#
def                     Error(message):
  # print the message.
  if message:
    print(message)

  # clean the script.
  Clean()

  # exit with an error code.
  sys.exit(42)

#
# this function walk through the unit and counts the number of successful
# tests.
#
def                     Count(tests):
  success = 0
  total = 0

  # explore the tests.
  for test in tests:
    if tests[test]["status"] == True:
      success += 1

    total += 1

  return (success, total)

#
# this function displays a brief summary of the given report.
#
def                     Summarize(report, margin=""):
  content = None

  success = None
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
    (success, total) = Count(tests)

    # generate the count.
    count = " [" + str(success) + "/" + str(total) + "]"

    # generate the unit's status.
    length = 79 - len(unit) - len(margin) - 4 - len(count)
    status = length * " " + count

    # display the unit name.
    content += margin + unit + status + "\n"

  return content

#
# this function displays a detailed version of the given report.
#
def                     Detail(report, margin = ""):
  content = None

  success = None
  total = None

  status = None
  length = None
  count = None

  unit = None
  tests = None
  test = None

  # initialize the content.
  content = str()

  # for the units of the report.
  for unit in report["data"]:
    # retrieve the tests.
    tests = report["data"][unit]

    # compute the status of all the tests of the given unit.
    (success, total) = Count(tests)

    # generate the count.
    count = " [" + str(success) + "/" + str(total) + "]"

    # generate the unit's status.
    length = 79 - len(unit) - len(margin) - 4 - len(count) - 1
    status = length * " " + count

    # display the unit name.
    content += """\
%(margin)s%(unit)s:%(status)s
""" % { "margin": margin,
        "unit": unit,
        "status": status }

    for test in tests:
      # display name.
      content += """\
%(margin)s  %(test)s:
""" % { "margin": margin,
        "test": test }

      # display status.
      content += """\
%(margin)s    status: %(status)s
""" % { "margin": margin,
        "status": tests[test]["status"] }

      # display description.
      content += """\
%(margin)s    description: %(description)s
""" % { "margin": margin,
        "description": tests[test]["description"] }

      # display duration.
      content += """\
%(margin)s    duration: %(duration)s
""" % { "margin": margin,
        "duration": tests[test]["duration"] }

      # display output.
      content += """\
%(margin)s    output: %(output)s
""" % { "margin": margin,
        "output": tests[test]["output"] }

  return content

#
# this function retrieves the latest kaneton snapshot through
# Subversion.
#
def                     Checkout():
  # launch Subversion.
  if ktp.process.Invoke("svn",
                        [ "co",
                          Repository,
                          g_directory]) == ktp.StatusError:
    Error("unable to check out the kaneton repository")

#
# this function triggers some test through the kaneton
# test client.
#
def                     Test():
  report = None
  summary = None
  detail = None
  reports = None
  report = None
  stream = None
  status = None
  output = None

  # set the kaneton environment variables.
  os.putenv("KANETON_PYTHON", "/usr/bin/python")
  os.putenv("KANETON_HOST", "linux/ia32")
  os.putenv("KANETON_USER", "robot")
  os.putenv("KANETON_PLATFORM", "ibm-pc")
  os.putenv("KANETON_ARCHITECTURE", "ia32/educational")

  # initialize the summary and detail strings.
  summary = str()
  detail = str()

  # create a temporary file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # request a test suite through the test client
  status =                                                              \
    ktp.process.Invoke("make",
                       [ "-C", g_directory,
                         "initialize" ],
                       stream = stream,
                       option = ktp.process.OptionNone)

  # retrieve the output.
  output = ktp.miscellaneous.Pull(stream)

  # remove the stream file.
  ktp.miscellaneous.Remove(stream)

  # if the construction was successful, exit the loop.
  if status == ktp.StatusError:
    Error(output + "\n" +
          "unable to initialize the kaneton environment")

  # for every environment to test...
  for environment in Environments:
    # create a temporary file.
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # request a test suite through the test client
    status =                                                            \
      ktp.process.Invoke("make",
                         [ "-C", g_directory + "/test/client",
                           "test-" + environment + "::kaneton" ],
                         stream = stream,
                         option = ktp.process.OptionNone)

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    # if the construction was successful, exit the loop.
    if status == ktp.StatusError:
      Error(output + "\n" +
            "unable to test the kaneton research implementation")

    # look for the generated report.
    paths = ktp.miscellaneous.Search(g_directory + "/test/store/report",
                                     "^.*\.rpt$",
                                     ktp.miscellaneous.OptionFile)

    # check the reports.
    if len(paths) != 1:
      Error("it seems that more reports than expected are present in the store")

    path = paths[0]

    # load the report.
    report = ktp.report.Load(path)

    # add to the summary.
    summary += """\
environment(%(environment)s) :: platform(%(platform)s) :: architecture(%(architecture)s) :: suite(%(suite)s) :: duration(%(duration)ss)
%(summary)s
""" % { "environment": environment,
        "platform": report["meta"]["platform"],
        "architecture": report["meta"]["architecture"],
        "suite": report["meta"]["suite"],
        "duration": report["meta"]["duration"],
        "summary": Summarize(report, "    ") }

    # add to the detail.
    detail += """\
environment(%(environment)s) :: platform(%(platform)s) :: architecture(%(architecture)s) :: suite(%(suite)s) :: duration(%(duration)ss)
%(detail)s
""" % { "environment": environment,
        "platform": report["meta"]["platform"],
        "architecture": report["meta"]["architecture"],
        "suite": report["meta"]["suite"],
        "duration": report["meta"]["duration"],
        "detail": Detail(report, "    ") }

    # remove the report.
    ktp.miscellaneous.Remove(path)

  # build the final report.
  report = """
---[ Summary

%(summary)s

---[ Detail

%(detail)s
""" % { "summary": summary,
        "detail": detail }

  return report

#
# this function emails the given report.
#
def                     Send(report):
  configuration = None
  content = None
  message = None
  stream = None
  date = None

  # create a temporary file.
  configuration = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # create a configuration file.
  content = """\
set realname = "opaak admin"
set from = "admin@opaak.org"
set use_from = yes\
"""

  # store the temporary mutt configuration file.
  ktp.miscellaneous.Push(content, configuration)

  # create a temporary file.
  message = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # store the temporary message.
  ktp.miscellaneous.Push(report, message)

  # create a stream file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # retrieve the current date.
  date = time.strftime("%Y/%m/%d %H:%M:%S")

  # email the capability to the supposed recipient.
  status = ktp.process.Invoke("mutt",
                              [ "-F", configuration,
                                "-s", "'kaneton robot :: " + date + "'",
                                Email,
                                "<" + message ])

  # retrieve the output.
  output = ktp.miscellaneous.Pull(stream)

  # remove the stream file.
  ktp.miscellaneous.Remove(stream)

  # check the status.
  if status == ktp.StatusError:
    Error(output + "\n" +
          "an error occured while sending the report")

  # remove the temporary files.
  ktp.miscellaneous.Remove(message)
  ktp.miscellaneous.Remove(configuration)

#
# this function initializes the script.
#
def                     Initialize():
  global g_directory

  # create a temporary directory.
  g_directory = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionDirectory)

#
# this function cleans what has been created by this script.
#
def                     Clean():
  # remove the disk directory.
  if g_directory:
    ktp.miscellaneous.Remove(g_directory)

#
# this is the main function
#
def                     Main():
  report = None

  # initialize the script.
  Initialize()

  # checkout the kaneton repository.
  Checkout()

  # trigger the tests.
  report = Test()

  # email the report.
  Send(report)

  # clean the temporary stuff.
  Clean()

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
