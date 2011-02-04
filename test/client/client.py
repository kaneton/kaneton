#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/client/client.py
#
# created       julien quintard   [mon mar 23 00:09:51 2009]
# updated       julien quintard   [thu feb  3 12:39:36 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script connects to the test server and triggers operations.
#
# note that this script depends on the ktp package which relies on
# the pyopenssl, yaml and other packages.
#

#
# ---------- packages ---------------------------------------------------------
#

import sys

import env
import ktp

#
# ---------- globals ----------------------------------------------------------
#

g_server = None
g_capability = None
g_platform = None
g_architecture = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function prints the usage.
#
def			Usage():
  command = None

  env.display(env.HEADER_ERROR, "usage: client.py [command]",
              env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  env.display(env.HEADER_ERROR, "commands:", env.OPTION_NONE)

  for command in c_commands:
    env.display(env.HEADER_ERROR, "  " + command, env.OPTION_NONE)

#
# this function simply displays the current configuration.
#
def                     Warning():
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK, "configuration:", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  server:                 " + g_server,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  capability:             " + g_capability,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  platform:               " + str(g_platform),
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  architecture:           " + str(g_architecture),
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

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
def                     Summarize(report, margin = ""):
  success = None
  total = None

  status = None
  length = None
  count = None

  color = None

  unit = None
  tests = None

  # for the units of the report.
  for unit in report["data"]:
    # retrieve the tests.
    tests = report["data"][unit]

    # compute the status of all the tests of the given unit.
    (success, total) = Count(tests)

    # set the colour according to the status.
    if success == total:
      color = env.COLOR_GREEN
    else:
      color = env.COLOR_RED

    # generate the count.
    count = " [" + str(success) + "/" + str(total) + "]"

    # generate the unit's status.
    length = 79 - len(unit) - len(margin) - 4 - len(count)
    status = length * " " +                                               \
             env.colorize(count, color, env.OPTION_BOLD)

    # display the unit name.
    env.display(env.HEADER_OK,
                margin + unit + status,
                env.OPTION_NONE)

#
# this function displays a detailed version of the given report.
#
def                     Detail(report, margin = ""):
  success = None
  total = None

  status = None
  length = None
  count = None

  color = None

  unit = None
  tests = None
  test = None

  # for the units of the report.
  for unit in report["data"]:
    # retrieve the tests.
    tests = report["data"][unit]

    # compute the status of all the tests of the given unit.
    (success, total) = Count(tests)

    # set the colour according to the status.
    if success == total:
      color = env.COLOR_GREEN
    else:
      color = env.COLOR_RED

    # generate the count.
    count = " [" + str(success) + "/" + str(total) + "]"

    # generate the unit's status.
    length = 79 - len(unit) - len(margin) - 4 - len(count) - 1
    status = length * " " +                                               \
             env.colorize(count, color, env.OPTION_BOLD)

    # display the unit name.
    env.display(env.HEADER_OK,
                margin + env.colorize(unit,
                                      env.COLOR_NONE,
                                      env.OPTION_BOLD) + ":" + status,
                env.OPTION_NONE)

    for test in tests:
      # select color.
      if tests[test]["status"] == True:
        color = env.COLOR_GREEN
      else:
        color = env.COLOR_RED

      # display name.
      env.display(env.HEADER_OK,
                  margin + "  " + test + ":",
                  env.OPTION_NONE)

      # display status.
      env.display(env.HEADER_OK,
                  margin + "    status: " +
                  env.colorize(str(tests[test]["status"]),
                               color,
                               env.OPTION_NONE),
                  env.OPTION_NONE)

      # display description.
      env.display(env.HEADER_OK,
                  margin + "    description: " +
                  tests[test]["description"],
                  env.OPTION_NONE)

      # display duration.
      env.display(env.HEADER_OK,
                  margin + "    duration: " +
                  str(tests[test]["duration"]),
                  env.OPTION_NONE)

      # display output.
      env.display(env.HEADER_OK,
                  margin + "    output: " +
                  tests[test]["output"],
                  env.OPTION_NONE)

#
# this function dumps any Python data structure in a hierarchical way.
#
def                     Dump(data, margin = "", alignment = 26):
  key = None
  length = None

  if isinstance(data, dict):
    for key in data:
      if not isinstance(data[key], dict) and not isinstance(data[key], list):
        length = len(margin) + len(str(key)) + 1

        env.display(env.HEADER_OK,
                    margin + str(key) + ":" +
                    (alignment - length) * " " +
                    str(data[key]),
                    env.OPTION_NONE)
      else:
        env.display(env.HEADER_OK,
                    margin + str(key) + ":",
                    env.OPTION_NONE)

        Dump(data[key], margin + "  ")
  elif isinstance(data, list):
    for element in data:
      Dump(element, margin)
  else:
    env.display(env.HEADER_OK,
                alignment * " " +
                str(data),
                env.OPTION_NONE)

#
# this function asks the server for information related to
# the given capability hence user.
#
# these information are then displayed.
#
def                     Information(server, capability, arguments):
  information = None
  item = None

  # check the arguments
  if len(arguments) != 0:
    Usage()
    sys.exit(42)

  # warning
  Warning()

  # retrieve the information.
  information = ktp.xmlrpc.Call(server.Information(capability))

  # display a message.
  env.display(env.HEADER_OK,
              "information:",
              env.OPTION_NONE)

  # dump the information dictionary.
  Dump(information, "  ")

#
# this function triggers a test.
#
def                     Test(server, capability, arguments):
  snapshot = None
  suite = None
  environment = None
  identifier = None

  # warning
  Warning()

  # check the arguments
  if len(arguments) != 2:
    Usage()
    sys.exit(42)

  # retrieve the arguments.
  environment = arguments[0]
  suite = arguments[1]

  # display a message.
  env.display(env.HEADER_OK,
              "generating the kaneton snapshot",
              env.OPTION_NONE)

  # export the current kaneton implementation.
  env.launch(env._EXPORT_SCRIPT_,
             "test:" + capability["identifier"],
             env.OPTION_QUIET)

  # display a message.
  env.display(env.HEADER_OK,
              "loading the kaneton snapshot",
              env.OPTION_NONE)

  # read the snapshot.
  snapshot = env.pull(env._EXPORT_DIR_ + "/output/" +                   \
                        "test:" + capability["identifier"] + ".tar.bz2",
                      env.OPTION_NONE)

  # display a message.
  env.display(env.HEADER_OK,
              "requesting the server",
              env.OPTION_NONE)

  # trigger a test.
  identifier = ktp.xmlrpc.Call(server.Test(capability,
                                           ktp.miscellaneous.Binary(snapshot),
                                           g_platform,
                                           g_architecture,
                                           environment,
                                           suite))

  # display the received report.
  env.display(env.HEADER_OK,
              "the snapshot has been scheduled for testing under the "  \
              "identifier: " + identifier,
              env.OPTION_NONE)

#
# this function requests the server for re-testing a snapshot.
#
def                     Retest(server, capability, arguments):
  snapshot = None
  identifier = None

  # warning
  Warning()

  # check the arguments
  if len(arguments) != 1:
    Usage()
    sys.exit(42)

  # retrieve the arguments.
  identifier = arguments[0]

  # display a message.
  env.display(env.HEADER_OK,
              "requesting the server",
              env.OPTION_NONE)

  # trigger a test.
  ktp.xmlrpc.Call(server.Retest(capability,
                                identifier))

  # display a message.
  env.display(env.HEADER_OK,
              "the snapshot has been re-tested successfully",
              env.OPTION_NONE)

#
# this function submits a snapshot.
#
def                     Submit(server, capability, arguments):
  snapshot = None
  stage = None

  # warning
  Warning()

  # check the arguments
  if len(arguments) != 1:
    Usage()
    sys.exit(42)

  # retrieve the arguments.
  stage = arguments[0]

  # display a message.
  env.display(env.HEADER_OK,
              "generating the kaneton snapshot",
              env.OPTION_NONE)

  # export the current kaneton implementation.
  env.launch(env._EXPORT_SCRIPT_,
             "test:" + capability["identifier"],
             env.OPTION_QUIET)

  # display a message.
  env.display(env.HEADER_OK,
              "loading the kaneton snapshot",
              env.OPTION_NONE)

  # read the snapshot.
  snapshot = env.pull(env._EXPORT_DIR_ + "/output/" +                   \
                        "test:" + capability["identifier"] + ".tar.bz2",
                      env.OPTION_NONE)

  # display a message.
  env.display(env.HEADER_OK,
              "requesting the server",
              env.OPTION_NONE)

  # submit the snapshot.
  ktp.xmlrpc.Call(server.Submit(capability,
                                ktp.miscellaneous.Binary(snapshot),
                                stage))

  # display a message.
  env.display(env.HEADER_OK,
              "the snapshot has been submitted successfully",
              env.OPTION_NONE)

#
# this function displays a report.
#
def                     Display(server, capability, arguments):
  report = None
  identifier = None
  unit = None

  # check the arguments
  if len(arguments) != 1:
    Usage()
    sys.exit(42)

  identifier = arguments[0]

  # display a message.
  env.display(env.HEADER_OK,
              "requesting the server",
              env.OPTION_NONE)

  # retrieve the report.
  report = ktp.xmlrpc.Call(server.Fetch(capability, identifier))

  # display a message.
  env.display(env.HEADER_OK,
              "displaying the report",
              env.OPTION_NONE)

  # dump the report.
  env.display(env.HEADER_OK,
              "report:",
              env.OPTION_NONE)

  # dump the meta section.
  env.display(env.HEADER_OK,
              "  meta:",
              env.OPTION_NONE)
  Dump(report["meta"], "    ")

  # dump the data section, if possible.
  if report["meta"]["state"] == ktp.report.StateDone:
    env.display(env.HEADER_OK,
                "  data:",
                env.OPTION_NONE)

    Detail(report, "    ")

#
# this function lists the reports.
#
def                     List(server, capability, arguments):
  list = None
  report = None

  # check the arguments
  if len(arguments) != 0:
    Usage()
    sys.exit(42)

  # display a message.
  env.display(env.HEADER_OK,
              "requesting the server",
              env.OPTION_NONE)

  # retrieve the list.
  list = ktp.xmlrpc.Call(server.List(capability))

  # display a message.
  env.display(env.HEADER_OK,
              "listing the reports",
              env.OPTION_NONE)

  # display a message
  env.display(env.HEADER_OK,
              "reports:",
              env.OPTION_NONE)

  # display the list.
  for report in list:
    # display report's identifier.
    env.display(env.HEADER_OK,
                "  " + report["identifier"] + ":",
                env.OPTION_NONE)

    # display the report's meta.
    env.display(env.HEADER_OK,
                "    " + report["environments"]["stress"] +             \
                  " :: " +                                              \
                  report["platform"] + " :: " +                         \
                  report["architecture"] + " :: " +                     \
                  report["suite"] + " :: " +                            \
                  report["date"] + " :: " +                             \
                  report["state"],
                env.OPTION_NONE)

#
# this is the main function.
#
def                     Main():
  global g_server
  global g_capability
  global g_platform
  global g_architecture

  server = None
  capability = None
  command = None
  arguments = None

  # retrieve the command.
  if len(sys.argv) != 2:
    Usage()
    sys.exit(42)

  # set the command.
  command = sys.argv[1].split("-")[0]

  # set the variables.
  g_server = env._TEST_SERVER_
  g_capability = env._TEST_CAPABILITY_
  g_platform = env._TEST_PLATFORM_
  g_architecture = env._TEST_ARCHITECTURE_

  # connect to the server.
  server = ktp.xmlrpc.Connect(g_server)

  # load the student capability.
  capability = ktp.capability.Load(g_capability)

  # trigger the appropriate command.
  for name in c_commands:
    if command == name.split("-")[0]:
      # set the arguments.
      if len(sys.argv[1].split("-")) > 1:
        arguments = sys.argv[1].split("-")[1].split("::")
      else:
        arguments = []

      c_commands[name](server, capability, arguments)

      sys.exit(0)

  # wrong command.
  env.display(env.HEADER_ERROR,
              "unknown command '" + arguments[0] + "'",
              env.OPTION_NONE)

  # display usage.
  Usage()

#
# ---------- constants --------------------------------------------------------
#

c_commands = {
  "information": Information,
  "list": List,
  "display-[identifier]": Display,
  "test-[environment]::[suite]": Test,
  "retest-[identifier]": Retest,
  "submit-[stage]": Submit
}

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
  Main()
