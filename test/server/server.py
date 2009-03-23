#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repositories/kaneton/test/server/server.py
#
# created       julien quintard   [mon mar 23 12:39:26 2009]
# updated       julien quintard   [mon mar 23 15:48:06 2009]
#

#
# ---------- imports ----------------------------------------------------------
#

import socket
import os

import SocketServer
import BaseHTTPServer
import SimpleHTTPServer
import SimpleXMLRPCServer

from OpenSSL import SSL, crypto

import ktc

#
# ---------- globals ----------------------------------------------------------
#

SERVER_HOST = "localhost" # XXX "test.opaak.org"
SERVER_PORT = 8421

CA_CERTIFICATE = "../store/certificate/ca.crt"
SERVER_CERTIFICATE = "../store/certificate/server.crt"
SERVER_KEY = "../store/key/server.key"

SERVER_CODE = "../store/code/server"

#
# ---------- SSLXMLRPCServer --------------------------------------------------
#

class SSLXMLRPCServer(BaseHTTPServer.HTTPServer,
                      SimpleXMLRPCServer.SimpleXMLRPCDispatcher):
  def __init__(self,
               address,
               handler):
    # deactivate log.
    self.logRequests = False

    # initialize the dispatcher.
    SimpleXMLRPCServer.SimpleXMLRPCDispatcher.__init__(self, True, None)

    # initialize the socket server.
    SocketServer.BaseServer.__init__(self, address, handler)

    # build the SSL context.
    context = SSL.Context(SSL.SSLv23_METHOD)

    # load the key and certificate.
    context.use_privatekey_file(SERVER_KEY)
    context.use_certificate_file(SERVER_CERTIFICATE)

    # load the certification authority.
    context.load_verify_locations(CA_CERTIFICATE)

    # build the SSL connection.
    self.socket = SSL.Connection(context,
                                 socket.socket(self.address_family,
                                               self.socket_type))

    # bind the socket and activate the server.
    self.server_bind()
    self.server_activate()

#
# ---------- SSLXMLRPCRequestHandler ------------------------------------------
#

class SSLXMLRPCRequestHandler(SimpleXMLRPCServer.SimpleXMLRPCRequestHandler):
  def setup(self):
    self.connection = self.request

    self.rfile = socket._fileobject(self.request, "rb", self.rbufsize)
    self.wfile = socket._fileobject(self.request, "wb", self.wbufsize)

  def do_POST(self):
    try:
      # get arguments
      data = self.rfile.read(int(self.headers["content-length"]))

      # in previous versions of SimpleXMLRPCServer, _dispatch
      # could be overridden in this class, instead of in
      # SimpleXMLRPCDispatcher. to maintain backwards compatibility,
      # check to see if a subclass implements _dispatch and dispatch
      # using that method if present.
      response = self.server._marshaled_dispatch(data,
                                                 getattr(self, '_dispatch', None))
    except: # this should only happen if the module is buggy
            # internal error, report as HTTP server error.
      self.send_response(500)
      self.end_headers()
    else:
      # got a valid XML RPC response.
      self.send_response(200)
      self.send_header("Content-type", "text/xml")
      self.send_header("Content-length", str(len(response)))
      self.end_headers()
      self.wfile.write(response)

      # shut down the connection.
      self.wfile.flush()
      self.connection.shutdown()

#
# ---------- TestServer -------------------------------------------------------
#

class TestServer:
  #
  # this method initializes the class.
  #
  def __init__(self):
    # read the server code.
    self.code = ktc.Read(SERVER_CODE)

  #
  # this method verifies the given capability.
  #
  def Verify(self,
             capability):
    # extract and validate the given capability.
    try:
      ktc.Validate(self.code, capability)
    except:
      return False

    # return True if the capability is valid.
    return True

  #
  # this method test a given snapshot against a given tests suite.
  #
  def Launch(self,
             capability,
             snapshot,
             suite):
    object = None

    # verify the given capability.
    if self.Verify(capability) != True:
      return False

    # retrieve the object.
    object = ktc.Extract(capability)

    # XXX[add whatever necessary to compile the snapshot and run
    #     it in a VM]

    return True

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
  # create the server.
  server = SSLXMLRPCServer((SERVER_HOST, SERVER_PORT),
                           SSLXMLRPCRequestHandler)

  # register the remote methods.
  server.register_instance(TestServer())

  # print information
  information = server.socket.getsockname()
  print "[information] serving on " + information[0] + ":" + str(information[1])

  # loop endlessly waiting for requests.
  server.serve_forever()

# XXX
#
# o passer le serveur en threade
#
# /XXX
