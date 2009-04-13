#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/server/server.py
#
# created       julien quintard   [mon mar 23 12:39:26 2009]
# updated       julien quintard   [mon apr 13 19:14:39 2009]
#

#
# ---------- imports ----------------------------------------------------------
#

import socket
import yaml

import SocketServer
import BaseHTTPServer
import SimpleHTTPServer
import SimpleXMLRPCServer

from OpenSSL import SSL, crypto

import env
import ktc

#
# ---------- constants --------------------------------------------------------
#

# XXX[wrong paths :: to fix]

CACertificate = "../store/certificate/ca.crt"

ServerHost = "localhost" # XXX "test.opaak.org"
ServerPort = 8421
ServerCertificate = "../store/certificate/server.crt"
ServerKey = "../store/key/server.key"
ServerCode = "../store/code/server"
ServerDatabaseDirectory = "../store/database"

StatusOk = True
StatusError = False

ScriptCompile = "scripts/compile.py"
ScriptTest = "scripts/test.py"

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
    context.use_privatekey_file(ServerKey)
    context.use_certificate_file(ServerCertificate)

    # load the certification authority.
    context.load_verify_locations(CACertificate)

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
  # this method test a given snapshot against a given tests suite.
  #
  def Launch(self,
             capability,
             snapshot,
             suite):
    object = None
    directory = None
    file = None

    # verify the given capability.
    if Test.Verify(capability) != True:
      return (StatusError, "invalid capability")

    # retrieve the object.
    object = ktc.Extract(capability)

    # retrieve the user profile.
    profile = Test.Load(object["id"])

    if profile:
      if profile[suite] <= 0:
        return (StatusError, "no tests left")

    # create a temporary directory and file.
    directory = env.temporary(env.OPTION_DIRECTORY)
    file = env.temporary(env.OPTION_FILE)

    # save the snapshot in the temporary file.
    env.push(file, str(snapshot), env.OPTION_NONE)

    # extract the snapshot in the temporary directory.
    env.unpack(file, directory, env.OPTION_NONE)

    # launch the compile script that will compile the snapshot, producing
    # a bootable floppy image.
# XXX    env.launch(ScriptCompile, directory + " " + file, env.OPTION_NONE)

    # launch the test script that will run the test suite in a virtual machine.
    env.launch(ScriptTest, file, env.OPTION_NONE)

# XXX
#    ./compile directory file
#    ./test file result

# 0) sauver le snapshot dans un fichier
# 1) extraire le snapshot dans un rep temporaire
# 2) extraire les tests lies a la suite donnee (pas necessaire, il suffit
#    de les avoir quelques part genre ServerSuitesDirectory)
# 3) lancer une vm de compilation
# 4) lancer la compilation et creation d'une image dans la vm compil
# 5) recuperer l'image generee dans la vm compil
# 6) lancer une vm de test avec l'image generee au stade precedent
# 7) lancer la test suite qui communique via le port Serie
# 9) recuperer les resultats
# 10) generer une une trace python i.e convertible en YAML
# 11) mettre a jour le profile etudiant en decrementant le nombre de tests
#     restants

    # remove the temporary file and directory.
    env.remove(file, env.OPTION_NONE)
    env.remove(directory, env.OPTION_NONE)

    return (StatusOk, None)

  #
  # this method returns information regarding the user capabilities.
  #
  def Information(self,
                  capability):
    information = None
    stream = None
    object = None
    profile = None

    # verify the given capability.
    if Test.Verify(capability) != True:
      return (StatusError, "invalid capability")

    # retrieve the object.
    object = ktc.Extract(capability)

    # extract the user profile.
    profile = Test.Load(object["id"])

    # build the information.
    information = object

    if profile:
      information["profile"] = profile

    return (StatusOk, information)

#
# ---------- test -------------------------------------------------------------
#

class Test:
  code = None

  def Initialize():
    # read the server code.
    Test.code = ktc.Read(ServerCode)

  #
  # this method verifies the given capability.
  #
  def Verify(capability):
    # extract and validate the given capability.
    try:
      ktc.Validate(Test.code, capability)
    except:
      return False

    # return True if the capability is valid.
    return True

  #
  # this function return a user profile given its id.
  #
  def Load(id):
    path = None

    # compute the path.
    path = ServerDatabaseDirectory + "/" + id + ".db"

    if not env.path(path, env.OPTION_EXIST):
      return None

    # load the database.
    database = yaml.load(file(ServerDatabaseDirectory + "/" + id, 'r'))

    return database

  #
  # this function stores the new profile of a given user.
  #
  def Store(id, profile):
    path = None

    # compute the path.
    path = ServerDatabaseDirectory + "/" + id + ".db"

    # load the database.
    database = yaml.dump(profile,
                         file(ServerDatabaseDirectory + "/" + id, 'w'))

  #
  # static methods
  #
  Initialize = staticmethod(Initialize)
  Verify = staticmethod(Verify)
  Load = staticmethod(Load)
  Store = staticmethod(Store)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  # create the server.
  server = SSLXMLRPCServer((ServerHost, ServerPort),
                           SSLXMLRPCRequestHandler)

  # register the remote methods.
  server.register_instance(TestServer())

  # print information
  information = server.socket.getsockname()
  print "[information] serving on " + information[0] + ":" + str(information[1])

  # initialize the test class.
  Test.Initialize()

  # loop endlessly waiting for requests.
  server.serve_forever()

# XXX
#
# o passer le serveur en threade
#
# /XXX
