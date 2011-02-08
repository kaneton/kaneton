#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/packages/ktp/xmlrpc.py
#
# created       julien quintard   [mon oct 25 19:44:10 2010]
# updated       julien quintard   [tue feb  8 17:39:08 2011]
#

#
# ---------- packages ---------------------------------------------------------
#

import xmlrpclib
import sys

import ktp

#
# ---------- functions --------------------------------------------------------
#

#
# this method wraps a call to a remote method.
#
def                     Call(value):
  type = None
  data = None

  # retrieve the type.
  type = value[0]

  # transform the data should it be in binary form.
  if isinstance(value[1], xmlrpclib.Binary) == True:
    data = str(value[1])
  else:
    data = value[1]

  # display an error if the type indicates so.
  if type == ktp.StatusError:
    print("[error] " + data)
    sys.exit(42)

  return data

#
# this method connects to an XMLRPC server.
#
def                   Connect(server):
  handle = None

  # issue the connection request.
  handle = xmlrpclib.Server(server,
                            allow_none = True)

  return handle
