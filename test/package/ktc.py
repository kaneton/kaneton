#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/package/ktc.py
#
# created       julien quintard   [sun mar 22 13:54:03 2009]
# updated       julien quintard   [sun oct 24 12:08:27 2010]
#

#
# ---------- information ------------------------------------------------------
#
# the kaneton test crypto package contains everything necessary for the test
# system to communicate and perform students authentication.
#

#
# ---------- packages ---------------------------------------------------------
#

from OpenSSL import crypto

import string
import random
import hmac
import pickle
import sys

import xmlrpc

#
# ---------- definitions ------------------------------------------------------
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
  type = value[0]
  data = value[1]

  if isinstance(data, xmlrpc.Binary) == True:
    data = str(value[1])

  if type == StatusError:
    print "[error] " + data

    sys.exit(42)

  return data

#
# this function generates a cryptographic key.
#
def                     Key(type = crypto.TYPE_RSA,
                            length = 2048):
  key = crypto.PKey()

  key.generate_key(type, length)

  return key

#
# this function returns a certificate request.
#
def                     Request(key,
                                digest = "md5",
                                **attributes):
  request = crypto.X509Req()

  subject = request.get_subject()

  for (k, v) in attributes.items():
    setattr(subject, k, v)

  request.set_pubkey(key)
  request.sign(key, digest)

  return request

#
# this function builds a certificate according to the given
# certificate request.
#
def                     Certificate(request,
                                    (issuer_certificate, issuer_key),
                                    serial,
                                    (timestamp_notbefore, timestamp_notafter),
                                    digest = "md5"):
  certificate = crypto.X509()

  certificate.set_serial_number(serial)
  certificate.gmtime_adj_notBefore(timestamp_notbefore)
  certificate.gmtime_adj_notAfter(timestamp_notafter)
  certificate.set_issuer(issuer_certificate.get_subject())
  certificate.set_subject(request.get_subject())
  certificate.set_pubkey(request.get_pubkey())

  certificate.sign(issuer_key, digest)

  return certificate

#
# this function stores a key.
#
def                     Put(name,
                            key):
  open(name + ".key",
       'w').write(crypto.dump_privatekey(crypto.FILETYPE_PEM, key))

#
# this function saves the given certificate on the file system.
#
def                     Save(name,
                             certificate):
  open(name + ".crt",
       'w').write(crypto.dump_certificate(crypto.FILETYPE_PEM, certificate))

#
# this function generates a random code.
#
def                     Code():
  random.seed()

  code = "".join([ random.choice(string.letters) for x in xrange(128) ])

  return code

#
# this function builds a capability.
#
def                     Capability(code,
                                   name,
                                   community,
                                   email):
  h = hmac.new(code, pickle.dumps( (name, community, email) ))
  token = h.hexdigest()

  capability = pickle.dumps( { "name": name,
                               "community": community,
                               "email": email,
                               "token": token } )

  return capability

#
# this function validates the capability according to the given
# key that has been used for issuing this capability.
#
def                     Validate(code,
                                 capability):
  object = Extract(capability)

  h = hmac.new(code,
               pickle.dumps( (object["name"],
                              object["community"],
                              object["email"] )))
  token = h.hexdigest()

  if token != object["token"]:
    raise Exception("invalid token")

#
# this function takes a capability and returns the original data structure.
#
def                     Extract(capability):
  object = pickle.loads(capability)

  return object

#
# this function stores a code on the file system.
#
def                     Store(name,
                              code):
  open(name + ".cod", 'w').write(code)

#
# this function reads a code file from the file system
#
def                     Read(name):
  code = open(name + ".cod", 'r').read()

  return code

#
# this function dumps a capability on the file system.
#
def                     Dump(name,
                             capability):
  open(name + ".cap", 'w').write(capability)

#
# this function loads a capability from the file system.
#
def                     Load(name):
  capability = open(name + ".cap", 'r').read()

  return capability
