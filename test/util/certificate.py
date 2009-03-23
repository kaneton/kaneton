#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repositories/kaneton/test/util/certificate.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [mon mar 23 00:10:21 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this program generates a certifcate for the test.opaak.org certification
# authority but also a certificate for the test server.
#

#
# ---------- packages ---------------------------------------------------------
#

import env
import ktc

#
# ---------- functions --------------------------------------------------------
#

#
# warning()
#
# this function asks the user the permission to continue.
#
def			warning():
  env.display(env.HEADER_INTERACTIVE,
              "to cancel press CTRL^C, otherwise press enter",
              env.OPTION_NONE)

  env.input(env.OPTION_NONE)

#
# ca()
#
# this function generates the key and certificate for the CA.
#
def                     ca():
  key = None
  request = None
  certificate = None

  # generate a cryptographic key.
  key = ktc.Key()

  # generate a certificate request.
  request = ktc.Request(key,
                        CN = "test.opaak.org",
                        emailAddress = "admin@opaak.org")

  # finally, build a valid, self-certified certificate
  certificate = ktc.Certificate(request,
                                (request, key),
                                0,
                                (0, 60 * 60 * 24 * 365 * 5))

  # display
  env.display(env.HEADER_OK,
              "CA key/certificate generated",
              env.OPTION_NONE)

  return (key, certificate)

#
# server()
#
# this function generates the key and certificate for the
# test server.
#
def                     server(ca_key, ca_certificate):
  key = None
  request = None
  certificate = None

  # generate a cryptographic key.
  key = ktc.Key()

  # generate a certificate request.
  request = ktc.Request(key,
                        CN = "test.opaak.org",
                        emailAddress = "admin@opaak.org")

  # finally, build a valid, self-certified certificate
  certificate = ktc.Certificate(request,
                                (ca_certificate, ca_key),
                                0,
                                (0, 60 * 60 * 24 * 365 * 5))

  # display
  env.display(env.HEADER_OK,
              "server key/certificate generated",
              env.OPTION_NONE)

  return (key, certificate)

#
# main()
#
# this is the main function.
#
def                     main():
  ca_key = None
  ca_certificate = None
  server_key = None
  server_certificate = None

  # display.
  env.display(env.HEADER_OK,
              "generating the CA and server's key/certificate pair",
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  # display a warning.
  warning()

  # generate the ca certificate.
  (ca_key, ca_certificate) = ca()

  # generate the server certificate.
  (server_key, server_certificate) = server(ca_key, ca_certificate)

  # save the ca key and certificate
  ktc.Put(env._TEST_STORE_KEY_DIR_ + "/ca",
          ca_key)

  ktc.Save(env._TEST_STORE_CERTIFICATE_DIR_ + "/ca",
           ca_certificate)

  # save the server key and certificate
  ktc.Put(env._TEST_STORE_KEY_DIR_ + "/server",
          server_key)

  ktc.Save(env._TEST_STORE_CERTIFICATE_DIR_ + "/server",
           server_certificate)

  # display.
  env.display(env.HEADER_OK,
              "CA and server's key/certificate pair generated and stored",
              env.OPTION_NONE)
  
#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
