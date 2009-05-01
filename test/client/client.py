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
# updated       julien quintard   [fri may  1 21:06:57 2009]
#

#
# ---------- packages ---------------------------------------------------------
#

import sys
import os

import xmlrpclib

import ktc
import env

#
# ---------- functions --------------------------------------------------------
#

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
# this is the main function.
#
def                     Main():
  server = None
  capability = None

  # connect to the server.
  server = xmlrpclib.Server(env._TEST_HOST_,
                            allow_none = True)

  # load the student capability.
  capability = ktc.Load(env._TEST_CAPABILITY_)

  report = ktc.Call(server.Launch(capability,
                                  xmlrpclib.Binary(env.pull("/home/mycure/kaneton/test/kaneton-snapshot.tar.bz2", env.OPTION_NONE)),
                                  "kaneton"))

  # XXX
  suite = "kaneton"

  env.display(env.HEADER_OK,
              "displaying a report summary for '" + report["name"] + "'",
              env.OPTION_NONE)

  for unit in report["suite"]:
    Summary(unit, "      ")

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
#
# XXX
