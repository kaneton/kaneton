#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repositories/kaneton/test/client/client.py
#
# created       julien quintard   [mon mar 23 00:09:51 2009]
# updated       julien quintard   [mon mar 23 14:20:47 2009]
#

#
# ---------- packages ---------------------------------------------------------
#

import env
import ktc

import xmlrpclib

#
# ---------- functions --------------------------------------------------------
#

#
# main()
#
# this is the main function.
#
def                     main():
  server = None
  capability = None

  # connect to the server.
# XXX  server = xmlrpclib.Server(env._TEST_SERVER_)
  server = xmlrpclib.ServerProxy("https://localhost:8421",
                                 allow_none = True)

  # load the student capability.
  capability = ktc.Load(env._TEST_CAPABILITY_)

  print server.Launch(capability, None, None)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()

# XXX
#
# o ajouter un proxy au client pour que les etudiants, a EPITA puisse passer
#   le proxy.
# o verifier que le certificat recu est le meme que celui en local (chaud ca :()
#
# XXX
