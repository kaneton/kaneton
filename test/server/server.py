#! /usr/bin/python2
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
# updated       julien quintard   [thu dec 16 22:50:49 2010]
#

#
# ---------- information ------------------------------------------------------
#
# this script must be run on the server-side. the server waits for XML RPCs
# which trigger one of the following actions: information, test or submit.
#
# for every action, the requesting client provides a user capability. this
# capability is first verified. then, the user's database is loaded.
#
# the information command returns information on the requesting user's profile
# by inspecting the user's database. In addition, this command returns the
# available suites, stages and environments.
#
# the test command takes a snapshot, an environment and a suite to run. first,
# the construct script is called in order to turn the snapshot into a
# bootable image. then, the stress script is called in order to trigger
# the tests belonging to the given test suite. finally, a report is build
# and saved but also sent back to the client.
#
# the submit command takes a snapshot and saves it as the final work of the
# student. later on, the administrator will be able to use the
# 'scripts/evaluate.py' script in order to generate the students' grades for
# a specific stage.
#
# for more information about the scripts called from the server's internals,
# please refer to the scripts/ directory.
#
# IMPORTANT! should the server fail to produce relevant reports, the
#   administrator should look for two possible explanations:
#     1) there are too many mounted loop devices.
#        one can detect these temporary mounts through either:
#          $> mount
#        or:
#          $> losetup -a
#     2) there already is a virtual machine running, being Xen or QEMU
#        for QEMU, once can detect such running virtual machine through:
#          $> ps aux | grep -i qemu
#        for Xen, the following command must only return the Domain-0:
#          $> xm list
#

#
# ---------- requirement ------------------------------------------------------
#

import os
import sys

TestDirectory = os.path.abspath(os.path.dirname(                        \
                  os.path.realpath(sys.argv[0])) + "/..")

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

# directories
ScriptsDirectory = TestDirectory + "/scripts"
ServerDirectory = TestDirectory + "/server"
StoreDirectory = TestDirectory + "/store"
SuitesDirectory = TestDirectory + "/suites"
StagesDirectory = TestDirectory + "/stages"
EnvironmentsDirectory = TestDirectory + "/environments"

# stores
DatabaseStore = StoreDirectory + "/database"
KeyStore = StoreDirectory + "/key"
CodeStore = StoreDirectory + "/code"
CertificateStore = StoreDirectory + "/certificate"
ReportStore = StoreDirectory + "/report"
SnapshotStore = StoreDirectory + "/snapshot"

# ca
CACertificate = CertificateStore + "/ca" + ktp.certificate.Extension

# server
ServerHost = "test.opaak.org"
ServerPort = 8421
ServerCertificate = CertificateStore + "/server" + ktp.certificate.Extension
ServerKey = KeyStore + "/server" + ktp.key.Extension
ServerCode = CodeStore + "/server" + ktp.code.Extension

# scripts
TestScript = ScriptsDirectory + "/test.py"

# maintenance
MaintenanceLock = TestDirectory + "/.maintenance"

# indexes
QEMUIndex = 0
XenIndex = 1

# environments: qemu/xen.
Environments = [ "qemu", "xen" ]

# test construct environment
TestConstructEnvironment = Environments[QEMUIndex]

# the contributors community.
ContributorsCommunity = "contributors"

#
# ---------- globals ----------------------------------------------------------
#

# this variable contains the server's actual code used for validating
# received capabilities.
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
                                                 getattr(self,
                                                         '_dispatch',
                                                         None))
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

  # load the server's code which is used in order to validate the
  # students' capabilities.
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
  suites = None
  suite = None
  stages = None
  stage = None
  environments = None
  environment = None

  print("[routine] Information()")

  try:
    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      return (ktp.StatusError, "the system is currently under "         \
                "maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = capability

    # extract the user's database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   profile["identifier"] +              \
                                   ktp.database.Extension)

    # build the information.
    information = {}

    # add the profile.
    information["profile"] = { "identifier": profile["identifier"],
                               "community": profile["community"],
                               "members": profile["members"] }

    # add the database.
    information["database"] = database

    # add the set of of suites.
    information["suites"] = {}

    suites = ktp.suite.List(SuitesDirectory)
    for suite in suites:
      stream = ktp.suite.Load(SuitesDirectory + "/" +                   \
                                suite + ktp.suite.Extension)
      information["suites"][stream["name"]] = stream["description"]

    # add the set of stages.
    information["stages"] = {}

    stages = ktp.stage.List(StagesDirectory)
    for stage in stages:
      stream = ktp.stage.Load(StagesDirectory + "/" +                   \
                                stage + ktp.stage.Extension)

      information["stages"][stream["name"]] = stream["description"]

    # add the set of environments.
    information["environments"] = {}

    environments = ktp.environment.List(EnvironmentsDirectory)
    for environment in environments:
      stream = ktp.environment.Load(EnvironmentsDirectory + "/" +       \
                                      environment + ktp.environment.Extension)

      information["environments"][stream["name"]] = stream["description"]
  except Exception, exception:
    return (ktp.StatusError, ktp.trace.Generate())

  print("[/routine] Information()")

  return (ktp.StatusOk, information)

#
# this method triggers a test suite for the given snapshot.
#
def                     Test(capability,
                             snapshot,
                             platform,
                             architecture,
                             environment,
                             suite):
  profile = None
  machine = None
  database = None
  identifier = None
  date = None
  report = None
  status = None
  output = None
  stream = None

  print("[routine] Test()")

  try:
    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      return (ktp.StatusError, "the system is currently under "         \
                "maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = capability

    # compute the machine.
    machine = platform + "." + architecture

    # retrieve the user's database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   profile["identifier"] +              \
                                   ktp.database.Extension)

    # verify the database consistency.
    if (not "quotas" in database) or                                    \
       (not environment in database["quotas"]) or                       \
       (not machine in database["quotas"][environment]) or              \
       (not suite in database["quotas"][environment][machine]):
      return (ktp.StatusError, "the action does not seem to be "        \
                "allowed for your profile")

    # check that the quota has not been reached.
    if (len(database["reports"][environment][machine][suite]) ==        \
          database["quotas"][environment][machine][suite]):
      return (ktp.StatusError, "the tests quota seems to have been reached")

    # create a unique identifier for the test context.
    identifier = time.strftime("%Y%m%d:%H%M%S")

    # retrieve the current date.
    date = time.strftime("%Y/%m/%d %H:%M:%S")

    # update the database by recording the test identifier.
    database["reports"][environment][machine][suite] += [ identifier ]

    # store the database.
    ktp.database.Store(database,
                       DatabaseStore + "/" + profile["identifier"] +    \
                         ktp.database.Extension)

    # store the snapshot.
    ktp.miscellaneous.Push(str(snapshot),
                           SnapshotStore + "/" + identifier +           \
                             ktp.snapshot.Extension)

    # build the initial report.
    report = {
               "meta":
                 {
                   "user": profile["identifier"],
                   "identifier": identifier,
                   "date": date,
                   "environments":
                     {
                       "construct": TestConstructEnvironment,
                       "stress": environment
                     },
                   "platform": platform,
                   "architecture": architecture,
                   "suite": suite,
                   "duration": None
                 },
               "data":
                 None
             }

    # store the initial report.
    ktp.report.Store(report,
                     ReportStore + "/" + identifier + ktp.report.Extension)

    # generate a temporary file.
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # launch the test script.
    status =                                                            \
      ktp.process.Invoke(TestScript,
                         [ "--report", identifier ],
                         stream = stream,
                         option = ktp.process.OptionNone)

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    # test the success of the stress script invocation.
    if status == ktp.StatusError:
      return (ktp.StatusError, ktp.miscellaneous.Binary(output))
  except Exception, exception:
    return (ktp.StatusError, ktp.trace.Generate())
  finally:
    # remove temporary files.
    if stream:
      ktp.miscellaneous.Remove(stream)

  # load the final report in order to send it back to the client.
  report = ktp.report.Load(ReportStore + "/" + identifier +             \
                             ktp.report.Extension)

  print("[/routine] Test()")

  return (ktp.StatusOk, report)

#
# this method re-triggers a test suite for the given report.
#
def                     Retest(capability,
                               identifier):
  profile = None
  database = None
  date = None
  report = None
  status = None
  output = None
  stream = None

  print("[routine] Retest()")

  try:
    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      return (ktp.StatusError, "the system is currently under "         \
                "maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = capability

    # check that the requesting user is a contributor.
    if profile["community"] != ContributorsCommunity:
      return (ktp.StatusError, "only a contributor can request "        \
                "this operation")

    # load the final report in order to send it back to the client.
    report = ktp.report.Load(ReportStore + "/" + identifier +           \
                               ktp.report.Extension)

    # retrieve the current date.
    date = time.strftime("%Y/%m/%d %H:%M:%S")

    # reset some of the report attributes.
    report["meta"]["date"] = date
    report["meta"]["duration"] = None
    report["data"] = None

    # store the initial report.
    ktp.report.Store(report,
                     ReportStore + "/" + identifier + ktp.report.Extension)

    # generate a temporary file.
    stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

    # launch the test script.
    status =                                                            \
      ktp.process.Invoke(TestScript,
                         [ "--report", identifier,
                           "--email" ],
                         stream = stream,
                         option = ktp.process.OptionNone)

    # retrieve the output.
    output = ktp.miscellaneous.Pull(stream)

    # remove the stream file.
    ktp.miscellaneous.Remove(stream)

    # test the success of the stress script invocation.
    if status == ktp.StatusError:
      return (ktp.StatusError, ktp.miscellaneous.Binary(output))
  except Exception, exception:
    return (ktp.StatusError, ktp.trace.Generate())
  finally:
    # remove temporary files.
    if stream:
      ktp.miscellaneous.Remove(stream)

  print("[/routine] Retest()")

  return (ktp.StatusOk, None)

#
# this method submits a snapshot.
#
def                     Submit(capability,
                               snapshot,
                               stage):
  profile = None
  database = None
  identifier = None
  date = None

  print("[routine] Submit()")

  try:
    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      return (ktp.StatusError, "the system is currently under "         \
                "maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = capability

    # retrieve the user's database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   profile["identifier"] +              \
                                   ktp.database.Extension)

    # verify the database consistency.
    if (not "deliveries" in database) or                                \
       (not stage in database["deliveries"]):
      return (ktp.StatusError, "the action does not seem to be "        \
                "allowed for your profile")

    # check that no snapshot has been submitted already.
    if database["deliveries"][stage] != None:
      return (ktp.StatusError, "a snapshot seems to have already "      \
                "been submitted")

    # create a unique identifier for the submission.
    identifier = time.strftime("%Y%m%d:%H%M%S")

    # retrieve the current date.
    date = time.strftime("%Y/%m/%d %H:%M:%S")

    # store the snapshot.
    ktp.miscellaneous.Push(str(snapshot),
                           SnapshotStore + "/" + identifier +           \
                             ktp.snapshot.Extension)

    # set the snapshot identifier in the database.
    database["deliveries"][stage] = {
                                      "snapshot": identifier,
                                      "date": date
                                    }

    # store the database.
    ktp.database.Store(database, DatabaseStore + "/" +                  \
                         profile["identifier"] + ktp.database.Extension)
  except Exception, exception:
    return (ktp.StatusError, ktp.trace.Generate())

  print("[/routine] Submit()")

  return (ktp.StatusOk, None)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
  # create the server.
  server = SSLXMLRPCServer((ServerHost, ServerPort),
                           SSLXMLRPCRequestHandler)

  # register the routines.
  server.register_function(Information)
  server.register_function(Test)
  server.register_function(Retest)
  server.register_function(Submit)

  # print information
  information = server.socket.getsockname()
  print("[meta] serving on " +                                          \
          information[0] + ":" + str(information[1]))

  # initialize the system.
  Initialize()

  # loop endlessly waiting for requests.
  server.serve_forever()

  # clean the system.
  Clean()
