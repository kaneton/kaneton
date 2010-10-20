#! /usr/bin/python
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/server.py
#
# created       julien quintard   [mon mar 23 12:39:26 2009]
# updated       julien quintard   [tue oct 19 12:30:42 2010]
#

#
# ---------- imports ----------------------------------------------------------
#

import socket
import yaml
import os
import tempfile
import re
import traceback

import xmlrpclib

import SocketServer
import BaseHTTPServer
import SimpleHTTPServer
import SimpleXMLRPCServer

from OpenSSL import SSL, crypto

import ktc

#
# ---------- constants --------------------------------------------------------
#

# XXX[wrong paths :: to fix]

CACertificate = "store/certificate/ca.crt"

ServerHost = "test.opaak.org"
ServerPort = 8421
ServerCertificate = "store/certificate/server.crt"
ServerKey = "store/key/server.key"
ServerCode = "store/code/server"

DirectoryDatabase = "store/database"
DirectoryHooks = "hooks"

HookPrepare = "prepare.py"
HookRun = "run.py"

#
# ---------- SSLXMLRPCServer --------------------------------------------------
#

class SSLXMLRPCServer(BaseHTTPServer.HTTPServer,
                      SimpleXMLRPCServer.SimpleXMLRPCDispatcher):
  def           __init__(self,
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
  def           setup(self):
    self.connection = self.request

    self.rfile = socket._fileobject(self.request, "rb", self.rbufsize)
    self.wfile = socket._fileobject(self.request, "wb", self.wbufsize)

  def           do_POST(self):
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
  def           Launch(self,
                       capability,
                       snapshot,
                       machine,
                       platform,
                       architecture,
                       suite):
    context = {}

    try:
      # verify the given capability.
      if Test.Verify(capability) != True:
        return (ktc.StatusError, "invalid capability")

      # retrieve the object.
      context["profile"] = ktc.Extract(capability)

      # retrieve the user profile.
      context["database"] = Test.Load(context["profile"]["id"])

      # verify the rights.
      if (not context["database"]) or                                    \
         (not suite in context["database"]) or                           \
         (context["database"][suite] == 0):
        return (ktc.StatusError, "no tests left")

      # create a temporary directory and file.
      context["snapshot"] = Test.Temporary(Test.OptionFile)
      context["image"] = Test.Temporary(Test.OptionFile)

      # create a unique name fo the test context.
      context["name"] = os.path.basename(context["image"])

      # set other context variables.
      context["capability"] = capability
      context["suite"] = suite
      context["machine"] = machine
      context["platform"] = platform
      context["architecture"] = architecture

      # save the snapshot in the temporary file.
      Test.Push(context["snapshot"], str(snapshot))

      # launch the prepare script that generates a bootable image
      # referenced by context["image"].
      output = Test.Launch(DirectoryHooks + "/" + context["machine"] + "/" + HookPrepare, context)

      if len(output):
        return (ktc.StatusError, xmlrpclib.Binary("an error occured during the compiling process:\n---\n" + output.strip(" \n") + "\n---"))

      # launch the run script that will run the test suite in a virtual machine.
      output = Test.Launch(DirectoryHooks + "/" + context["machine"] + "/" + HookRun, context)

      if len(output):
        return (ktc.StatusError, xmlrpclib.Binary("an error occured while running the test suites:\n---\n" + output.strip(" \n") + "\n---"))

      # remove the temporary file and directory.
      Test.Remove(context["snapshot"])
      Test.Remove(context["image"])

      # XXX[save the report somewhere, remove a test from the database by increasing state]
    except Exception, exception:
      return (ktc.StatusError, Test.Trace())
      
    return (ktc.StatusOk, context["report"])

  #
  # this method returns information regarding the user capabilities.
  #
  def           Information(self,
                            capability):
    information = None
    stream = None
    object = None
    profile = None

    # verify the given capability.
    if Test.Verify(capability) != True:
      return (ktc.StatusError, "invalid capability")

    # retrieve the object.
    object = ktc.Extract(capability)

    # extract the user profile.
    profile = Test.Load(object["id"])

    # build the information.
    information = object

    # add the profile.
    information["profile"] = profile

    return (ktc.StatusOk, information)

#
# ---------- test -------------------------------------------------------------
#

class Test:
  OptionNone = 0
  OptionFile = 1
  OptionDirectory = 2
  OptionQuiet = 1

  code = None

  #
  # this method initializes the Test static class.
  #
  def           Initialize():
    # read the server code.
    Test.code = ktc.Read(ServerCode)

  #
  # this method verifies the given capability.
  #
  def           Verify(capability):
    # extract and validate the given capability.
    try:
      ktc.Validate(Test.code, capability)
    except:
      return False

    # return True if the capability is valid.
    return True

  #
  # this function returns a user profile given its id.
  #
  def           Load(id):
    path = None

    # compute the path.
    path = DirectoryDatabase + "/" + id + ".db"

    if not os.path.exists(path):
      return None

    # load the database.
    database = yaml.load(file(path, 'r'))

    return database

  #
  # this function stores the new profile of a given user.
  #
  def           Store(id, profile):
    # load the database.
    yaml.dump(profile,
              file(DirectoryDatabase + "/" + id, 'w'))

  #
  # this function creates a temporary file or directory.
  #
  def           Temporary(options):
    location = None

    if options == Test.OptionFile:
      tuple = tempfile.mkstemp()

      os.close(tuple[0])

      location = tuple[1]
    elif options == Test.OptionDirectory:
      location = tempfile.mkdtemp()

    return location

  #
  # this function writes a file.
  #
  def           Push(file, content):
    handle = None

    handle = open(file, "w")

    handle.write(content)

    handle.close()

  #
  # this function reads a file.
  #
  def           Pull(file):
    handle = None
    line = None

    if not os.path.exists(file):
      return None

    try:
      handle = open(file, "r")
    except IOError:
      return None

    content = ""
    for line in handle.readlines():
      content += line

    handle.close()

    return content

  #
  # this function executes something.
  #
  def           Launch(path, context, options = OptionNone):
    directory = None
    info = None
    output = None
    wd = None
    temporary = None

    # generate a temporary file for the context.
    temporary = Test.Temporary(Test.OptionFile)

    # serialize the context.
    yaml.dump(context, file(temporary, 'w'))

    # handle options.
    if options & Test.OptionQuiet:
      output = " >/dev/null 2>&1"
    else:
      output = ""

    # retrive file information.
    info = os.path.split(path)

    directory = info[0]
    path = info[1]

    # change directory if required.
    if directory:
      wd = os.getcwd()
      os.chdir(directory)

    # launch the script.
    if re.match("^.*\.py$", path):
      output = os.popen("python " + path + " " + temporary + output).read()
    else:
      if directory:
        path = "./" + path

      output = os.popen(path + " " + arguments + output).read()

    # come back to the original directory if necessary.
    if directory:
      os.chdir(wd)

    # extract the potentially modified context by first
    # deleting the previous content and updating it.
    context.clear()
    context.update(yaml.load(file(temporary, 'r')))

    # delete the temporary file.
    os.unlink(temporary)

    return output

  #
  # this function removes the target.
  #
  def           Remove(target):
    entries = None
    entry = None

    if os.path.isfile(target) or os.path.islink(target):
      os.unlink(target)

    if os.path.isdir(target):
      entries = os.listdir(target)

      for entry in entries:
        Test.Remove(target + "/" + entry)

      os.rmdir(target)

  #
  # this function returns the current errors trace.
  #
  def           Trace():
    temporary = None
    trace = None

    # create a temporary file.
    temporary = Test.Temporary(Test.OptionFile)

    # put the trace in the temporary file.
    traceback.print_exc(None, file(temporary, 'w'))

    # read the file.
    trace = Test.Pull(temporary)

    # remove the temporary file.
    Test.Remove(temporary)

    # return the trace.
    return trace

  #
  # static methods
  #
  Initialize = staticmethod(Initialize)
  Verify = staticmethod(Verify)
  Load = staticmethod(Load)
  Store = staticmethod(Store)
  Temporary = staticmethod(Temporary)
  Push = staticmethod(Push)
  Pull = staticmethod(Pull)
  Launch = staticmethod(Launch)
  Remove = staticmethod(Remove)
  Trace = staticmethod(Trace)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
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
