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
# updated       julien quintard   [tue may 12 13:48:21 2009]
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

import xmlrpclib

import ktc
import env

#
# ---------- globals ----------------------------------------------------------
#

g_host = None
g_capability = None
g_suite = None
g_machine = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function prints the usage.
#
def			Usage():
  command = None

  env.display(env.HEADER_ERROR, "usage: client.py [command]", env.OPTION_NONE)

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
              "  suite:                  " + g_suite,
              env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  machine:                " + str(g_machine),
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

#
# this function walk through the unit and sub-units and if every test
# was successful, return True.
#
def                     Status(unit):
  tests = 0
  success = 0
  t = None
  s = None

  # explore the tests.
  if "tests" in unit:
    for test in unit["tests"]:
      if test["status"] == True:
        success += 1

      tests += 1

  # explore the sub-units.
  if "units" in unit:
    for u in unit["units"]:
      (s, t) = Status(u)

      success += s
      tests += t

  return (success, tests)

#
# this function displays a brief summary of the tests
#
def                     Summary(unit, margin):
  success = None
  tests = None
  colour = None
  status = None
  u = None

  # compute the status of all the tests and sub-units' tests.
  (success, tests) = Status(unit)

  # set the color according to the status.
  if success == tests:
    colour = env.COLOR_GREEN

    status = ""
  else:
    colour = env.COLOR_RED

    status = " (" + str(success) + "/" + str(tests) + ")"

  # display the unit name.
  env.display(env.HEADER_NONE,
              margin + env.colorize(unit["component"] + status,
                                    colour,
                                    env.OPTION_NONE),
              env.OPTION_NONE)

  # explore the sub-units.
  if "units" in unit:
    for u in unit["units"]:
      Summary(u, margin + "  ")

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
                  margin + "  " + str(key) + ":" + (40 - length) * " " + str(data[key]),
                  env.OPTION_NONE)

#
# this function asks the server for information related to
# the given capability hence user.
#
# these information are then displayed.
#
def                     Information(server, capability):
  information = None
  item = None

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
def                     Launch(server, capability):
  snapshot = None

  # read the snapshot.
  snapshot = env.pull("/home/mycure/kaneton/test/kaneton-snapshot.tar.bz2", env.OPTION_NONE)

  # launch a test.
  report = ktc.Call(server.Launch(capability,
                                  xmlrpclib.Binary(snapshot),
                                  g_suite,
                                  g_machine))

  print report
  return
# XXX

  # display the received report.
  env.display(env.HEADER_OK,
              "report",
              env.OPTION_NONE)

  for unit in report:
    Summary(unit, "      ")

#
# this is the main function.
#
def                     Main():
  global g_host
  global g_capability
  global g_suite
  global g_machine

  server = None
  capability = None
  command = None

  # retrieve the command.
  if len(sys.argv) != 2:
    Usage()
    sys.exit(42)

  # set the command.
  command = sys.argv[1]

  # set the variables.
  g_host = env._TEST_HOST_
  g_capability = env._TEST_CAPABILITY_
  g_suite = env._TEST_SUITE_
  g_machine = env._TEST_MACHINE_.strip(" \t\n")

  # warning
  Warning()
  
  # connect to the server.
  server = xmlrpclib.Server(g_host,
                            allow_none = True)

  # load the student capability.
  capability = ktc.Load(g_capability)

  # trigger the appropriate command.
  for name in c_commands:
    if command == name:
      c_commands[name](server, capability)
      sys.exit(0)

  # wrong command.
  env.display(env.HEADER_ERROR,
              "unknown command '" + command + "'",
              env.OPTION_NONE)

  # display usage.
  Usage()

#
# ---------- constants --------------------------------------------------------
#

c_commands = {
  "information": Information,
  "launch": Launch
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
# o passer egalement la platform/architecture utilisee pour que le serveur
#   lance les tests sur le bon systeme (QEMU dans le cas de mips par exemple)
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
# o changer le pass root
# o passer le serveur en threade (enfin tester d'abord)
# o changer le nom de la vm 'svm' en 'svm-linux:ia32'
# o creer/detruire un user a chaque compile
# o a chaque compile, si on veut que ca se fasse en parallele, il faut copier
#   l'image VM sinon on va avoir des problemes de FS
# o il faut generer le compile.sh pour qu'il prenne en compte platform/arch
#   car ca differe entre contributor et student
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
