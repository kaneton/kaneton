#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/client/client.py
#
# created       julien quintard   [mon mar 23 00:09:51 2009]
# updated       julien quintard   [wed oct 20 12:27:57 2010]
#

#
# ---------- information ------------------------------------------------------
#
# this script connects to the test server and triggers operations.
#
# note that this script depends on the ktc package which relies on
# the pyopenssl package.
#

#
# ---------- packages ---------------------------------------------------------
#

import sys
import os
import re
import time
import yaml

import xmlrpclib

import ktc
import env

#
# ---------- globals ----------------------------------------------------------
#

g_host = None
g_capability = None
g_machine = None
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
  machine = None

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK, "configuration:", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  host:                   " + g_host,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  capability:             " + g_capability + ".cap",
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  machine:                " + str(g_machine),
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  platform:               " + str(g_platform),
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  architecture:           " + str(g_architecture),
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

#
# this function walk through the unit and sub-units and if every test
# was successful, return True.
#
def                     Status(tests):
  success = 0
  total = 0

  # explore the tests.
  for test in tests:
    if tests[test]["status"] == True:
      success += 1

    total += 1

  return (success, total)

#
# this function displays a brief summary of the tests
#
OPTION_NONE = 0
OPTION_DETAIL = 1
def                     Summary(unit, tests, margin, options):
  success = None
  total = None
  header = None
  status = None
  result = None
  length = None
  count = None
  test = None
  color = None

  # compute the status of all the tests and sub-units' tests.
  (success, total) = Status(tests)

  # set the colour according to the status.
  if success == total:
    result = "ok"
    color = env.COLOR_GREEN
  else:
    result = "ko"
    color = env.COLOR_RED

  count = " [" + str(success) + "/" + str(total) + "]"
  length = 79 - len(unit) - len(margin) - 4 - len(result) - len(count)

  status = length * " " +                                               \
           env.colorize(result, color, env.OPTION_BOLD) +               \
           count

  # display the unit name.
  env.display(env.HEADER_OK,
              margin + unit + status,
              env.OPTION_NONE)

  # dump the tests' details if requested.
  if options & OPTION_DETAIL:
    for test in tests:
      # select color.
      if tests[test]["status"] == True:
        color = env.COLOR_GREEN
      else:
        color = env.COLOR_RED

      # display status.
      env.display(env.HEADER_OK,
                  margin + "  status: " +
                  env.colorize(str(tests[test]["status"]),
                               color,
                               env.OPTION_NONE),
                  env.OPTION_NONE)

      # display description.
      env.display(env.HEADER_OK,
                  margin + "  description: " +
                  tests[test]["description"],
                  env.OPTION_NONE)

      # display duration.
      env.display(env.HEADER_OK,
                  margin + "  duration: " +
                  str(tests[test]["duration"]),
                  env.OPTION_NONE)

      # display output.
      env.display(env.HEADER_OK,
                  margin + "  output: " +
                  tests[test]["output"],
                  env.OPTION_NONE)

#
# this function display any Python dictionary in a
# hierarchical way.
#
def                     Display(data, margin):
  key = None
  length = None

  for key in data:
    if isinstance(data[key], dict):
      env.display(env.HEADER_OK,
                  margin + "  " + str(key) + ":",
                  env.OPTION_NONE)

      Display(data[key], margin + "  ")
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
  information = ktc.Call(server.Information(capability))

  # display the information by exploring the tree.
  env.display(env.HEADER_OK,
              "information:",
              env.OPTION_NONE)

  Display(information, "")

#
# this function launches a test.
#
def                     Launch(server, capability, arguments):
  snapshot = None

  # warning
  Warning()

  # check the arguments
  if len(arguments) != 2:
    Usage()
    sys.exit(42)

  # retrieve the suite.
  suite = arguments[1]

  # read the snapshot.
  snapshot = env.pull("/home/mycure/kaneton-submission.tar.bz2",
                      env.OPTION_NONE)

  # launch a test.
  report = ktc.Call(server.Launch(capability,
                                  xmlrpclib.Binary(snapshot),
                                  g_machine,
                                  g_platform,
                                  g_architecture,
                                  suite))

  # display the received report.
  env.display(env.HEADER_OK,
              "report",
              env.OPTION_NONE)

  # store the report in a trace file.
  yaml.dump({ "meta":
                { "time": time.strftime("%Y-%m-%d %H:%M:%S"),
                  "suite": suite,
                  "machine": g_machine,
                  "platform": g_platform,
                  "architecture": g_architecture },
              "data":
                report },
            file(env._TEST_STORE_TRACES_DIR_ +
                 "/" +
                 time.strftime("%Y%m%d-%H%M%S") +
                 ".trc", 'w'))

  # display a summary
  for unit in report:
    Summary(unit, report[unit], "  ", OPTION_NONE)

#
# this function dumps a trace.
#
def                     Dump(server, capability, arguments):
  trace = None
  unit = None

  # check the arguments
  if len(arguments) != 2:
    Usage()
    sys.exit(42)

  # retrieve the trace.
  trace = yaml.load(file(env._TEST_STORE_TRACES_DIR_ +                  \
                           "/" + arguments[1] + ".trc", 'r'))

  # dump the trace.
  env.display(env.HEADER_OK,
              "trace:",
              env.OPTION_NONE)

  # dump the meta section.
  env.display(env.HEADER_OK,
              "  meta:",
              env.OPTION_NONE)
  Display(trace["meta"], "  ")

  # dump the data section
  for unit in trace["data"]:
    Summary(unit, trace["data"][unit], "  ", OPTION_DETAIL)

#
# this function lists the traces.
#
def                     List(server, capability, arguments):
  traces = None
  trace = None

  # check the arguments
  if len(arguments) != 1:
    Usage()
    sys.exit(42)

  # retrieve the list of traces.
  traces = env.list(env._TEST_STORE_TRACES_DIR_, env.OPTION_FILE)

  env.display(env.HEADER_OK,
              "traces:",
              env.OPTION_NONE)

  # dump the trace names.
  for trace in traces:
    env.display(env.HEADER_OK,
                "  " + trace.rstrip(".trc"),
                env.OPTION_NONE)

#
# this is the main function.
#
def                     Main():
  global g_host
  global g_capability
  global g_machine
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
  arguments = sys.argv[1].split(":")

  # set the variables.
  g_host = env._TEST_HOST_
  g_capability = env._TEST_CAPABILITY_
  g_machine = env._TEST_MACHINE_
  g_platform = env._TEST_PLATFORM_
  g_architecture = env._TEST_ARCHITECTURE_

  # connect to the server.
  server = xmlrpclib.Server(g_host,
                            allow_none = True)

  # load the student capability.
  capability = ktc.Load(g_capability)

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
  "launch [environment] [machine] [suite]": Launch,
  "list": List,
  "dump [trace]": Dump
}

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
  Main()

# XXX
#
# o verifier que le certificat recu est le meme que celui en local
#
# o cote serveur, il ne faudrait pas inclure les tests dans la compile car
#   un etudiant pourrait les afficher sur la sortie et faire peter la compile
#   pour recuperer tous les tests.
#   il faudrait donc les compiler a cote en ayant une tarball speciale
#   qui ne contient pas le code du kernel (-kaneton/) mais juste les tests
#   et l'environnement de dev. a la sortie, on recuperer tests.lo et on le
#   passe au truc qui compile le kernel.
#   -> bon finalement, si ils veulent et peu importe ce que l'on fait, ils
#   pourront recuperer le tests.lo soit en le dumpant pendant la compile
#   soit en dumpant le code lorsque les tests tournent. c'est pas vraiment
#   un probleme car bon c'est bcp d'efforts pour se faire chier ensuite a
#   capter les tests en asm. ce qu'il faut surtout eviter c'est qu'ils aient
#   acces aux sources des tests. donc la solution la moins couteuse c'est
#   de les compiler a cote.
#   => via l'outil d'export il faudrait que ca genere une tarball pour
#   compiler les tests mais il faudrait egalement que la tarball etudiante
#   ait un Makefile dans kaneton/core/test modifie pour utilise tests.lo
#   directement.
# o changer le nom de la vm 'svm' en 'svm-linux:ia32'
# o creer/detruire un user a chaque compile
# o matter ce flag: IA32_KERNEL_MAPPED, est-il necessaire pour les tests?
# o les scripts compile/run a mettre dans des rep k0/ k1/ etc.
# o rajouter un try/catch autour de compile/run et clean si ca foire
# o rajouter le check de .svn env.mk env.py .dependency.mk etc. pour savoir
#   si la tarball est clean: .o et .lo egalement.
# o lock le fichier de database pour etre sur que le meme user ne lance
#   pas deux fois le truc.
# o rajouter des try/catch dans/autour des scripts prepare/run qui Clean
#   et afficher traceback
#
# XXX
