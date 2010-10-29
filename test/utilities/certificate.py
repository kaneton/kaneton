#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repo...ton.STABLE/test/utilities/certificate.py
#
# created       julien quintard   [sun mar 22 18:05:23 2009]
# updated       julien quintard   [wed oct 27 13:27:30 2010]
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
import ktp

#
# ---------- functions --------------------------------------------------------
#

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
  key = ktp.key.Generate()

  # generate a certificate request.
  request = ktp.certificate.Request(key,
                                    CN = "test.opaak.org",
                                    emailAddress = "admin@opaak.org")

  # finally, build a valid, self-certified certificate
  certificate = ktp.certificate.Create(request,
                                       (request, key),
                                       0,
                                       (0, 60 * 60 * 24 * 365 * 10))

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
  key = ktp.key.Generate()

  # generate a certificate request.
  request = ktp.certificate.Request(key,
                                    CN = "test.opaak.org",
                                    emailAddress = "admin@opaak.org")

  # finally, build a valid, self-certified certificate
  certificate = ktp.certificate.Create(request,
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

  # generate the ca certificate.
  (ca_key, ca_certificate) = ca()

  # generate the server certificate.
  (server_key, server_certificate) = server(ca_key, ca_certificate)

  # save the ca key and certificate
  ktp.key.Store(env._TEST_STORE_KEY_DIR_ + "/ca" + ktp.key.Extension,
                ca_key)

  ktp.certificate.Store(env._TEST_STORE_CERTIFICATE_DIR_ + "/ca" +      \
                          ktp.certificate.Extension,
                        ca_certificate)

  # save the server key and certificate
  ktp.key.Store(env._TEST_STORE_KEY_DIR_ + "/server" + ktp.key.Extension,
                server_key)

  ktp.certificate.Store(env._TEST_STORE_CERTIFICATE_DIR_ + "/server" +  \
                          ktp.certificate.Extension,
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
