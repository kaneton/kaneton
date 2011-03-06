#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/server/registrar.py
#
# created       julien quintard   [mon mar 23 12:39:26 2009]
# updated       julien quintard   [sat mar  5 20:09:07 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this script, which must be run on the server, waits for XML RPCs.
#
# for every action, the requesting client provides a user capability. this
# capability is first verified.
#
# the 'information' command returns information on the requesting user's
# profile by inspecting the user's database. In addition, this command
# returns the available suites, stages and environments.
#
# the 'test' command takes a snapshot, an environment and a suite to run. the
# report is initially set up, and recorded for the scheduler to test it.
#
# the 'submit' command takes a snapshot and saves it as the final work of the
# student. later on, the administrator will be able to use other scripts
# in order to evaluate such submissions.
#
# finally, the 'retest' command takes an existing report identifier and
# re-queue it in the scheduler.
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
QueueStore = StoreDirectory + "/queue"
LogStore = StoreDirectory + "/log"

# ca
CACertificate = CertificateStore + "/ca" + ktp.certificate.Extension

# server
ServerHost = "test.opaak.org"
ServerPort = 8421
ServerCertificate = CertificateStore + "/server" + ktp.certificate.Extension
ServerKey = KeyStore + "/server" + ktp.key.Extension
ServerCode = CodeStore + "/server" + ktp.code.Extension

# maintenance
MaintenanceLock = ServerDirectory + "/.maintenance"

# test construct environment
TestConstructEnvironment = ktp.environment.Xen

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

  ktp.log.Record(LogStore,
                 "#(registrar) function(Information) message(entering)")

  try:
    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Information) " +            \
                       "error(maintenance detected)")

      return (ktp.StatusError, "the system is currently under "         \
                "maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Information) " +            \
                       "error(invalid capability)")

      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = capability

    ktp.log.Record(LogStore,
                   "#(registrar) function(Information) profile(" +      \
                     str(profile) + ")")

    # extract the user's database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   profile["identifier"] +              \
                                   ktp.database.Extension)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Information) database(" +     \
                     str(database) + ")")

    # build the information.
    information = {}

    # add the profile.
    information["profile"] = { "identifier": profile["identifier"],
                               "type": profile["type"],
                               "attributes": profile["attributes"],
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

    ktp.log.Record(LogStore,
                   "#(registrar) function(Information) information(" +  \
                     str(information) + ")")
  except Exception, exception:
    ktp.log.Record(LogStore,
                   "#(registrar) function(Information) exception(" +    \
                     str(exception) + ") trace(" +                      \
                     ktp.trace.Generate() + ")")

    return (ktp.StatusError, ktp.trace.Generate())

  ktp.log.Record(LogStore,
                 "#(registrar) function(Information) message(leaving)")

  return (ktp.StatusOk, information)

#
# this function triggers a test suite for the given snapshot.
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
  report = None

  ktp.log.Record(LogStore,
                 "#(registrar) function(Test) message(entering)")

  try:
    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Test) error(maintenance detected)")

      return (ktp.StatusError, "the system is currently under "         \
                "maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Test) error(invalid capability)")

      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = capability

    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) profile(" +             \
                     str(profile) + ")")

    # compute the machine.
    machine = platform + "." + architecture

    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) machine(" + machine + ")")

    # retrieve the user's database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   profile["identifier"] +              \
                                   ktp.database.Extension)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) database(" +            \
                     str(database) + ")")

    # verify the database consistency.
    if (not "quotas" in database) or                                    \
       (not environment in database["quotas"]) or                       \
       (not machine in database["quotas"][environment]) or              \
       (not suite in database["quotas"][environment][machine]):
      ktp.log.Record(LogStore,
                     "#(registrar) function(Test) error(unallowed action)")

      return (ktp.StatusError, "the action does not seem to be "        \
                "allowed for your profile")

    # check that the quota has not been reached.
    if (not "reports" in database) or                                   \
       (not environment in database["reports"]) or                      \
       (not machine in database["reports"][environment]) or             \
       (not suite in database["reports"][environment][machine]) or      \
       (len(database["reports"][environment][machine][suite]) ==        \
          database["quotas"][environment][machine][suite]):
      ktp.log.Record(LogStore,
                     "#(registrar) function(Test) error(reached quota)")

      return (ktp.StatusError, "the tests quota seems to have been reached")

    # create a unique identifier for the test context.
    while True:
      identifier = time.strftime("%Y%m%d:%H%M%S")

      if not os.path.exists(SnapshotStore + "/" + identifier +          \
                              ktp.snapshot.Extension):
        break

    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) identifier(" +          \
                     identifier + ")")

    # update the database by recording the test identifier.
    database["reports"][environment][machine][suite] += [ identifier ]

    # store the database.
    ktp.database.Store(database,
                       DatabaseStore + "/" + profile["identifier"] +    \
                         ktp.database.Extension)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) message(database updated)")

    # store the snapshot.
    ktp.miscellaneous.Push(str(snapshot),
                           SnapshotStore + "/" + identifier +           \
                             ktp.snapshot.Extension)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) message(snapshot saved)")

    # build the initial report.
    report = {
               "meta":
                 {
                   "user": profile["identifier"],
                   "identifier": identifier,
                   "date": None,
                   "environments":
                     {
                       "construct": TestConstructEnvironment,
                       "stress": environment
                     },
                   "platform": platform,
                   "architecture": architecture,
                   "suite": suite,
                   "duration": None,
                   "state": ktp.report.StateInProgress
                 },
               "data":
                 None
             }

    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) report(" +              \
                     str(report) + ")")

    # store the initial report.
    ktp.report.Store(report,
                     ReportStore + "/" + identifier + ktp.report.Extension)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) message(initial report saved)")

    # queue the identifier so that the scheduler picked it for testing.
    ktp.queue.Push(QueueStore,
                   identifier)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) message(snapshot queued)")
  except Exception, exception:
    ktp.log.Record(LogStore,
                   "#(registrar) function(Test) exception(" +           \
                     str(exception) + ") trace(" +                      \
                     ktp.trace.Generate() + ")")

    return (ktp.StatusError, ktp.trace.Generate())

  ktp.log.Record(LogStore,
                 "#(registrar) function(Test) message(leaving)")

  return (ktp.StatusOk, identifier)

#
# this function re-triggers a test suite for the given report.
#
def                     Retest(capability,
                               identifier):
  profile = None

  ktp.log.Record(LogStore,
                 "#(registrar) function(Retest) message(entering)")

  try:
    ktp.log.Record(LogStore,
                   "#(registrar) function(Retest) identifier(" +        \
                     identifier + ")")

    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Retest) " +                 \
                       "error(maintenance detected)")

      return (ktp.StatusError, "the system is currently under "         \
                "maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Retest) error(invalid capability)")

      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = capability

    ktp.log.Record(LogStore,
                   "#(registrar) function(Retest) profile(" +           \
                     str(profile) + ")")

    # check that the requesting user is a contributor.
    if profile["type"] != ktp.capability.TypeContributor:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Retest) error(not a contributor)")

      return (ktp.StatusError, "only a contributor can request "        \
                "this operation")

    # try to load the report to make sure the identifier is valid.
    report = ktp.report.Load(ReportStore + "/" + identifier +           \
                               ktp.report.Extension)

    # queue the identifier so that the scheduler picked it for testing.
    ktp.queue.Push(QueueStore,
                   identifier)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Retest) message(snapshot queued)")
  except Exception, exception:
    ktp.log.Record(LogStore,
                   "#(registrar) function(Retest) exception(" +         \
                     str(exception) + ") trace(" +                      \
                     ktp.trace.Generate() + ")")

    return (ktp.StatusError, ktp.trace.Generate())

  ktp.log.Record(LogStore,
                 "#(registrar) function(Retest) message(leaving)")

  return (ktp.StatusOk, None)

#
# this function submits a snapshot.
#
def                     Submit(capability,
                               snapshot,
                               stage):
  profile = None
  database = None
  identifier = None
  date = None

  ktp.log.Record(LogStore,
                 "#(registrar) function(Submit) message(entering)")

  try:
    # verify that the maintenance mode has not been activated.
    if os.path.exists(MaintenanceLock) == True:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Submit) " +                 \
                       "error(maintenance detected)")

      return (ktp.StatusError, "the system is currently under "         \
                "maintenance ... please try again later")

    # verify the given capability.
    if ktp.capability.Validate(g_code, capability) != True:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Submit) error(invalid capability)")

      return (ktp.StatusError, "invalid capability")

    # retrieve the profile inside the capability.
    profile = capability

    ktp.log.Record(LogStore,
                   "#(registrar) function(Submit) profile(" +           \
                     str(profile) + ")")

    # retrieve the user's database.
    database = ktp.database.Load(DatabaseStore + "/" +                  \
                                   profile["identifier"] +              \
                                   ktp.database.Extension)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Submit) database(" +          \
                     str(database) + ")")

    # verify the database consistency.
    if (not "deliveries" in database) or                                \
       (not stage in database["deliveries"]):
      ktp.log.Record(LogStore,
                     "#(registrar) function(Submit) " +                 \
                       "error(unallowed operation)")

      return (ktp.StatusError, "the action does not seem to be "        \
                "allowed for your profile")

    # check that no snapshot has been submitted already.
    if database["deliveries"][stage] != None:
      ktp.log.Record(LogStore,
                     "#(registrar) function(Submit) " +                 \
                       "error(prior submission detected)")

      return (ktp.StatusError, "a snapshot seems to have already "      \
                "been submitted")

    # create a unique identifier for the submission.
    while True:
      identifier = time.strftime("%Y%m%d:%H%M%S")

      if not os.path.exists(SnapshotStore + "/" + identifier +          \
                              ktp.snapshot.Extension):
        break

    # retrieve the current date.
    date = time.strftime("%Y/%m/%d %H:%M:%S")

    ktp.log.Record(LogStore,
                   "#(registrar) function(Submit) identifier(" +        \
                     identifier + ")")

    # store the snapshot.
    ktp.miscellaneous.Push(str(snapshot),
                           SnapshotStore + "/" + identifier +           \
                             ktp.snapshot.Extension)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Submit) message(snapshot saved)")

    # set the snapshot identifier in the database.
    database["deliveries"][stage] = {
                                      "snapshot": identifier,
                                      "date": date
                                    }

    # store the database.
    ktp.database.Store(database, DatabaseStore + "/" +                  \
                         profile["identifier"] + ktp.database.Extension)

    ktp.log.Record(LogStore,
                   "#(registrar) function(Submit) " +                   \
                     "message(submission recorded)");

    ktp.log.Record(LogStore,
                   "#(registrar) function(Submit) database(" +          \
                     str(database) + ")")
  except Exception, exception:
    ktp.log.Record(LogStore,
                   "#(registrar) function(Submit) exception(" +         \
                     str(exception) + ") trace(" +                      \
                     ktp.trace.Generate() + ")")

    return (ktp.StatusError, ktp.trace.Generate())

  ktp.log.Record(LogStore,
                 "#(registrar) function(Submit) message(leaving)")

  return (ktp.StatusOk, None)

#
# ---------- functions --------------------------------------------------------
#

#
# this function initializes the test system.
#
def                     Initialize():
  global g_code

  ktp.log.Record(LogStore,
                 "#(registrar) message(initializing)")

  # load the server's code which is used in order to validate the
  # students' capabilities.
  g_code = ktp.code.Load(ServerCode)

#
# this function cleans the test system.
#
def                     Clean():
  ktp.log.Record(LogStore,
                 "#(registrar) message(cleaning)")

#
# this is the main function.
#
def                     Main():
  server = None
  information = None

  # create the server.
  server = SSLXMLRPCServer((ServerHost, ServerPort),
                           SSLXMLRPCRequestHandler)

  # register the routines.
  server.register_function(Information)
  server.register_function(Test)
  server.register_function(Retest)
  server.register_function(Submit)

  # log the startup.
  information = server.socket.getsockname()

  ktp.log.Record(LogStore,
                 "#(registrar) @("+ information[0] + ":" +              \
                   str(information[1]) + ")")

  # loop endlessly waiting for requests.
  server.serve_forever()

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == '__main__':
  # initialize the system.
  Initialize()

  # call the main function
  Main()

  # clean the system.
  Clean()
