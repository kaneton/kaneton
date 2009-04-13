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
# updated       julien quintard   [mon apr 13 02:24:39 2009]
#

#
# ---------- packages ---------------------------------------------------------
#

import sys
import env
import ktc

import xmlrpclib

#
# ---------- globals ----------------------------------------------------------
#

StatusOk = True
StatusError = False

#
# ---------- functions --------------------------------------------------------
#

#
# this function wraps a call to a remote method.
#
def                     Call(value):
  if value[0] == StatusError:
    print "[error] " + value[1]
    sys.exit(42)
  else:
    return value[1]

# XXX
import md5
# XXX

#
# this is the main function.
#
def                     Main():
  server = None
  capability = None

  # connect to the server.
# XXX  server = xmlrpclib.Server(env._TEST_SERVER_)
  server = xmlrpclib.ServerProxy("https://localhost:8421",
                                 allow_none = True)

  # load the student capability.
  capability = ktc.Load(env._TEST_CAPABILITY_)

  print Call(server.Information(capability))

  print Call(server.Launch(capability,
                           xmlrpclib.Binary(env.pull("/home/mycure/kaneton/kaneton-snapshot.tar.bz2", env.OPTION_NONE)),
                           "k1")

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
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
