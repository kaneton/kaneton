#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/server/server.py
#
# created       julien quintard   [mon mar 23 12:39:26 2009]
# updated       julien quintard   [tue oct 26 22:44:01 2010]
#

#
# ---------- requirement ------------------------------------------------------
#

import os
import sys

TestDirectory = os.path.abspath("../")

sys.path.append(TestDirectory + "/packages")

#
# ---------- packages ---------------------------------------------------------
#

import socket
import time

import SocketServer
import BaseHTTPServer
import SimpleHTTPServer
import SimpleXMLRPCServer

from OpenSSL import SSL, crypto

import ktp

#
# ---------- constants --------------------------------------------------------
#

HooksDirectory = TestDirectory + "/hooks"
ScriptsDirectory = TestDirectory + "/scripts"
ServerDirectory = TestDirectory + "/server"
StoreDirectory = TestDirectory + "/store"
SuitesDirectory = TestDirectory + "/suites"
TestsDirectory = TestDirectory + "/tests"

DatabaseStore = StoreDirectory + "/database"
KeyStore = StoreDirectory + "/key"
CodeStore = StoreDirectory + "/code"
CertificateStore = StoreDirectory + "/certificate"
BundleStore = StoreDirectory + "/bundle"
ReportStore = StoreDirectory + "/report"

CACertificate = CertificateStore + "/ca"

ServerHost = "test.opaak.org"
ServerPort = 8421
ServerCertificate = CertificateStore + "/server"
ServerKey = KeyStore + "/server"
ServerCode = CodeStore + "/server"

ConstructHook = "construct.py"
StressHook = "stress.py"

MaintenanceLock = TestDirectory + "/.maintenance"

#
# ---------- globals ----------------------------------------------------------
#

g_code = None

#
# ---------- SSLXMLRPCServer --------------------------------------------------
#

class SSLXMLRPCServer(BaseHTTPServer.HTTPServer,
                      SimpleXMLRPCServer.SimpleXMLRPCDispatcher):
  def                   __init__(self,
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
    context.use_privatekey_file(ServerKey + ".key")
    context.use_certificate_file(ServerCertificate + ".crt")

    # load the certification authority.
    context.load_verify_locations(CACertificate + ".crt")

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
  def                   setup(self):
    self.connection = self.request

    self.rfile = socket._fileobject(self.request, "rb", self.rbufsize)
    self.wfile = socket._fileobject(self.request, "wb", self.wbufsize)

  def                   do_POST(self):
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
# ---------- functions --------------------------------------------------------
#

#
# this function initializes the test system.
#
def                     Initialize():
  global g_code

  g_code = ktp.code.Load(ServerCode)

#
# this function cleans the test system.
#
def                     Clean():
  pass

#
# ---------- routines ---------------------------------------------------------
#

#
# this routine returns information regarding the user's test profile.
#
def                     Information(capability):
  information = None
  stream = None
  profile = None
  database = None

  try:
    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      return (ktp.StatusError, "the system is currently under maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = ktp.capability.Extract(capability)

    # extract the user's database.
    database = ktp.database.Load(DatabaseStore + "/" + profile["name"])

    # build the information.
    information = {}

    # add the profile.
    information["profile"] = profile

    # add the database.
    information["database"] = database
  except Exception, exception:
    return (ktp.StatusError, ktp.trace.Generate())

  return (ktp.StatusOk, information)

#
# this method triggers a test suite for the given snapshot.
#
def                     Trigger(capability,
                                content,
                                platform,
                                architecture,
                                environment,
                                suite):
  context = {}
  profile = None
  machine = None
  database = None
  identifier = None
  snapshot = None
  image = None
  report = None
  stream = None
  status = None
  text = None

  try:
    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      return (ktp.StatusError, "the system is currently under maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = ktp.capability.Extract(capability)

    # compute the machine.
    machine = platform + "." + architecture

    # retrieve the user's database.
    database = ktp.database.Load(DatabaseStore + "/" + profile["name"])

    # verify the database consistency along with the tests quota for this
    # environment/suite couple.
    if (not "settings" in                                                     \
         database) or                                                         \
       (not environment in                                                    \
         database["settings"]) or                                             \
       (not machine in                                                        \
         database["settings"][environment]) or                                \
       (not suite in                                                          \
         database["settings"][environment][machine]) or                       \
       (database["settings"][environment][machine][suite]["requests"] ==      \
         database["settings"][environment][machine][suite]["quota"]):
      return (ktp.StatusError, "the tests quota seems to have been reached")

    # create a unique identifier for the test context.
    identifier = time.strftime("%Y%m%d-%H%M%S")

    # create a temporary files for the received snapshot, the about-to-be
    # generated image and the future report.
    snapshot = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)
    image = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)
    report = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # store the snapshot in the temporary file.
    ktp.miscellaneous.Push(str(content), snapshot)

    # update the user's database by incrementing the number of requests.
    database["settings"][environment][machine][suite]["requests"] += 1

    # launch the build hook which generates a bootable image.
    (status, text) = ktp.process.Invoke(HooksDirectory + "/" +                  \
                                          environment + "/" +                   \
                                          machine + "/" +                       \
                                          suite + "/" +                         \
                                          ConstructHook,
                                        [ "--name", identifier,
                                          "--snapshot", snapshot,
                                          "--image", image,
                                          "--environment", environment,
                                          "--platform", platform,
                                          "--architecture", architecture,
                                          "--suite", suite ],
                                        stream = stream,
                                        option = ktp.process.OptionNone)

    # test the success of the construct hook invocation.
    if status == ktp.StatusError:
      return (ktp.StatusError, ktp.miscellaneous.Binary(text))

  except Exception, exception:
    return (ktp.StatusError, ktp.trace.Generate())
  finally:
    # store the database.
    if database:
      ktp.database.Store(database, DatabaseStore + "/" + profile["name"])

    # remove the temporary files.
    if report:
      ktp.miscellaneous.Remove(report)
    if image:
      ktp.miscellaneous.Remove(image)
    if snapshot:
      ktp.miscellaneous.Remove(snapshot)
    if stream:
      ktp.miscellaneous.Remove(stream)

  return (ktp.StatusOk, report)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
  # create the server.
  server = SSLXMLRPCServer((ServerHost, ServerPort),
                           SSLXMLRPCRequestHandler)

  # register the routines.
  server.register_function(Information)
  server.register_function(Trigger)

  # print information
  information = server.socket.getsockname()
  print("[information] serving on " + information[0] + ":" + str(information[1]))

  # initialize the system.
  Initialize()

  # loop endlessly waiting for requests.
  server.serve_forever()

  # clean the system.
  Clean()
