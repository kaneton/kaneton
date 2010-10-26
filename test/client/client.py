#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repo...ies/kaneton.STABLE/test/client/client.py
#
# created       julien quintard   [mon mar 23 00:09:51 2009]
# updated       julien quintard   [tue oct 26 11:55:44 2010]
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
import os
import re
import time
import yaml

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
              "  capability:             " + g_capability + ".cap",
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
                margin + unit + ":" + status,
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
# this function dumps any Python dictionary in a hierarchical way.
#
def                     Dump(data, margin = ""):
  key = None
  length = None

  for key in data:
    if isinstance(data[key], dict):
      env.display(env.HEADER_OK,
                  margin + "  " + str(key) + ":",
                  env.OPTION_NONE)

      Dump(data[key], margin + "  ")
    else:
      length = len(margin) + 2 + len(str(key)) + 1

      env.display(env.HEADER_OK,
                  margin +
                  "  " +
                  str(key)
                  + ":" +
                  (26 - length) * " " +
                  str(data[key]),
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
  if len(arguments) != 1:
    Usage()
    sys.exit(42)

  # warning
  Warning()

  # retrieve the information.
  information = ktp.xmlrpc.Call(server.Information(capability))

  # display the information by exploring the tree.
  env.display(env.HEADER_OK,
              "information:",
              env.OPTION_NONE)

  # dump the information dictionary.
  Dump(information)

#
# this function launches a test.
#
def                     Launch(server, capability, arguments):
  snapshot = None
  suite = None
  environment = None

  # warning
  Warning()

  # check the arguments
  if len(arguments) != 3:
    Usage()
    sys.exit(42)

  # retrieve the arguments.
  environment = arguments[1]
  suite = arguments[2]

  # XXX
  # 1) make export-student:contributor
  # 2) export/output/test:contributor.tar.bz2
  # XXX

  # read the snapshot.
  snapshot = env.pull("/home/mycure/kaneton.STABLE/export/output/test:contributor.tar.bz2",
                      env.OPTION_NONE)

  # launch a test.
  report = ktp.xmlrpc.Call(server.Launch(capability,
                                         ktp.miscellaneous.Binary(snapshot),
                                         g_platform,
                                         g_architecture,
                                         environment,
                                         suite))

  # display the received report.
  env.display(env.HEADER_OK,
              "report",
              env.OPTION_NONE)

  # store the report.
  yaml.dump({ "meta":
                { "time": time.strftime("%Y-%m-%d %H:%M:%S"),
                  "platform": g_platform,
                  "architecture": g_architecture,
                  "environment": environment,
                  "suite": suite },
              "data":
                report },
            file(env._TEST_STORE_REPORT_DIR_ + "/" +                    \
                   environment + "/" +                                  \
                   g_platform + "." + g_architecture + "/" +            \
                   suite + "/" +                                        \
                   time.strftime("%Y%m%d-%H%M%S") + ".rpt", 'w'))

  # display a summary
  for unit in report:
    Summary(unit, report[unit], "  ", OPTION_NONE)

#
# this function displays a report.
#
def                     Display(server, capability, arguments):
  report = None
  identifier = None
  unit = None

  # check the arguments
  if len(arguments) != 2:
    Usage()
    sys.exit(42)

  identifier = arguments[1]

  # retrieve the report.
  report = ktp.report.Load(env._TEST_STORE_REPORT_DIR_ + "/" +          \
                             identifier)

  # dump the report.
  env.display(env.HEADER_OK,
              "report:",
              env.OPTION_NONE)

  # dump the meta section.
  env.display(env.HEADER_OK,
              "  meta:",
              env.OPTION_NONE)
  Dump(report["meta"], "  ")

  # dump the data section
  env.display(env.HEADER_OK,
              "  data:",
              env.OPTION_NONE)

  Detail(report, "    ")

#
# this function lists the reports.
#
def                     List(server, capability, arguments):
  reports = None
  report = None

  # check the arguments
  if len(arguments) != 1:
    Usage()
    sys.exit(42)

  # display a message
  env.display(env.HEADER_OK,
              "reports:",
              env.OPTION_NONE)

  # retrieve the list of report identifiers.
  reports = ktp.report.List(env._TEST_STORE_REPORT_DIR_)

  # display the identifiers.
  for report in reports:
    # display report's identifier.
    env.display(env.HEADER_OK,
                "  " + report,
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
  arguments = None

  # retrieve the command.
  if len(sys.argv) != 2:
    Usage()
    sys.exit(42)

  # set the arguments.
  arguments = sys.argv[1].split("::")

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
    if arguments[0] == name.split(" ")[0]:
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
  "display [identifier]": Display,
  "launch [environment] [suite]": Launch
}

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
  Main()

# XXX
#
# o verifier que le certificat recu est le meme que celui en local. faire des
#   tests avec des autority ou serv-cert foireux.
#
# o matter ce flag: IA32_KERNEL_MAPPED, est-il necessaire pour les tests?
#
# o creer les repertoires de traces/ proprement.
# o de meme pour les traces a dumper, ne regarder que dans ce repertoire
#
# o le serveur devrait renvoye un report qui contient deja meta + data
#
# XXX
