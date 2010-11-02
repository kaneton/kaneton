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
# updated       julien quintard   [sun oct 31 13:45:54 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

from OpenSSL import crypto

import os
import hmac
import pickle
import re

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".cap"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of capability identifiers.
#
def                     List(directory):
  capabilities = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      capabilities += [ entry[:-len(Extension)] ]

  return capabilities

#
# this function creates a capability.
#
def                     Create(code,
                               identifier,
                               community,
                               members):
  h = hmac.new(code, pickle.dumps( (identifier, community, members) ))
  token = h.hexdigest()

  capability = { "identifier": identifier,
                 "community": community,
                 "members": members,
                 "token": token }

  return capability

#
# this function validates the capability according to the given
# key that has been used for issuing this capability.
#
def                     Validate(code,
                                 capability):
  h = hmac.new(code,
               pickle.dumps( (capability["identifier"],
                              capability["community"],
                              capability["members"] )))
  token = h.hexdigest()

  if token != capability["token"]:
    return False

  return True

#
# this function stores a capability on the file system.
#
def                     Store(path,
                              capability):
  open(path, 'w').write(pickle.dumps(capability))

#
# this function loads a capability from the file system.
#
def                     Load(path):
  capability = pickle.loads(open(path, 'r').read())

  return capability
