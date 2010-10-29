
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/capability.py
#
# created       julien quintard   [mon oct 25 19:58:10 2010]
# updated       julien quintard   [wed oct 27 13:09:41 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

from OpenSSL import crypto

import hmac
import pickle

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".cap"

#
# ---------- functions --------------------------------------------------------
#

#
# this function creates a capability.
#
def                     Create(code,
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
    return False

  return True

#
# this function takes a capability and returns the original data structure.
#
def                     Extract(capability):
  object = pickle.loads(capability)

  return object

#
# this function stores a capability on the file system.
#
def                     Store(path,
                              capability):
  open(path, 'w').write(capability)

#
# this function loads a capability from the file system.
#
def                     Load(path):
  capability = open(path, 'r').read()

  return capability
