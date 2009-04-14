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
# updated       julien quintard   [tue apr 14 06:36:47 2009]
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
# this is the main function.
#
def                     Main():
  server = None
  capability = None

  # connect to the server.
  server = xmlrpclib.Server(env._TEST_SERVER_,
                            allow_none = True)

  # load the student capability.
  capability = ktc.Load(env._TEST_CAPABILITY_)

  print ktc.Call(server.Information(capability))

  print ktc.Call(server.Launch(capability,
                               xmlrpclib.Binary(env.pull("/home/mycure/kaneton/kaneton-snapshot.tar.bz2", env.OPTION_NONE)),
                               "k1"))

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
  Main()

# XXX
#
# o ajouter un proxy au client pour que les etudiants, a EPITA puisse passer
#   le proxy: quoiqu'ils peuvent se demerder avec le socks
# o verifier que le certificat recu est le meme que celui en local (chaud ca :()
#
# o mettre les choses en commun dans un fichier: StatusError etc.
#
# XXX
