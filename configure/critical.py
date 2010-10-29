#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/configure/critical.py
#
# created       julien quintard   [wed may 23 10:31:42 2007]
# updated       julien quintard   [sun oct 24 14:53:16 2010]
#

#
# ---------- information ------------------------------------------------------
#
# this file starts by checking if the user is a newcomer and if so, the
# script creates a new user profile, step by step.
#
# this script is critical as it does not rely on the kaneton environment
# system.
#

#
# ---------- imports ----------------------------------------------------------
#

import sys
import os
import re

#
# ---------- globals ----------------------------------------------------------
#

g_directory = None

g_user = None
g_host = None
g_platform = None
g_architecture = None
g_python = None

#
# ---------- options ----------------------------------------------------------
#

OPTION_FILE = 1
OPTION_DIRECTORY = 2
OPTION_SKIP = 4

#
# ---------- functions --------------------------------------------------------
#

#
# list()
#
# this function returns the given directory entries.
#
def			list(directory, options):
  elements = []
  entries = None
  entry = None

  entries = os.listdir(directory)

  for entry in entries:
    if (options & OPTION_SKIP):
      if re.match("^\..*$", entry):
        continue

    if (options & OPTION_FILE) and					\
       (os.path.isfile(directory + "/" + entry)):
      elements += [ entry ]

    if (options & OPTION_DIRECTORY) and					\
       (os.path.isdir(directory + "/" + entry)):
      elements += [ entry ]

  return elements



#
# build()
#
# this function warns the user before actually creating the user profile.
# then, the function explains the user what to do to next.
#
def			build():
  print("[+] your profile is about the be created at:")
  print("[+]   " + g_directory + "/environment/profile/user/" + g_user)
  print("")

  print("[+] are you sure you want to build the '" +                    \
          g_user +                                                      \
          "' user profile:")
  print("[+]   [y] ", end="")

  key = input()

  if not key:
    key = "y"

  if key != "y":
    print("[!] aborting...")
    sys.exit(42)

  print("")

  if os.path.isdir(g_directory + "/environment/profile/user/" + g_user):
    print("[!] the user profile '" + g_user + "' already exists")
    sys.exit(42)

  os.mkdir(g_directory + "/environment/profile/user/" + g_user)

  print("[+] the user profile was created sucessfully")
  print("")

  print("[+] you now need to modify your shell configuration with the")
  print("[+] following lines:")
  print("[+]")
  print("[+]   export KANETON_USER              '" + g_user + "'")
  print("[+]   export KANETON_HOST              '" + g_host + "'")
  print("[+]   export KANETON_PYTHON            '" + g_python + "'")
  print("[+]   export KANETON_PLATFORM          '" + g_platform + "'")
  print("[+]   export KANETON_ARCHITECTURE      '" + g_architecture + "'")



#
# python()
#
# this function asks the user the location of the python binary file.
#
def			python():
  global g_python
  python = None
  paths = None
  p = None

  paths = os.environ.get("PATH").split(os.pathsep)

  for p in paths:
    if os.path.isfile(p + "/python"):
      python = p + "/python"

  # ask the user the location of the python binary program.
  print("[?] please specify the path of the python binary program:")
  print("[?]   [" + python + "] ", end="")

  # read the user input.
  g_python = input()

  if not g_python:
    g_python = python

  if not g_python or not os.path.isfile(g_python):
    print("[!] the provided python path is incorrect")
    print("[!] aborting...")
    sys.exit(0)

  print("")



#
# architecture()
#
# this function asks the user to choose a architecture.
#
def			architecture():
  global g_architecture
  implementations = []
  architectures = []
  processors = []
  i = None
  p = None
  a = None

  # get the processors entries.
  processors = list(g_directory + "/environment/profile/kaneton/machine/architecture",
                    OPTION_DIRECTORY | OPTION_SKIP)

  # get the implementations to form the complete architecture entries.
  for p in processors:
    implementations = list(g_directory +
                           "/environment/profile/kaneton/machine/architecture/" +
                           p, OPTION_DIRECTORY | OPTION_SKIP)

    for i in implementations:
      architectures.append(p + "/" + i)

  # display a message and list the architectures actually available.
  print("[+] the target architecture represents the microprocessor")
  print("[+] which the kaneton microkernel will be built for.")
  print("")
  print("[+] available target architectures:")
  for a in architectures:
    print("[+]   o " + a)
  print("")

  # ask the user which architecture to use.
  print("[?] please specify the targeted architecture:")
  print("[?]   [] ", end="")

  # read the user input.
  g_architecture = input()

  if not g_architecture in architectures:
    print("[!] unknown target architecture")
    print("[!] aborting...")
    sys.exit(42)

  print("")



#
# platform()
#
# this function asks the user to choose a platform.
#
def			platform():
  global g_platform
  plateforms = []
  p = None

  # get the platform entries.
  platforms = list(g_directory +                                        \
                     "/environment/profile/kaneton/machine/platform",
                   OPTION_DIRECTORY | OPTION_SKIP)

  # display a message and list the platforms actually available.
  print("[+] the target platform represents the hardware board supporting")
  print("[+] the devices: microprocessor, memory, peripherals etc.")
  print("")
  print("[+] available target platforms:")
  for p in platforms:
    print("[+]   o " + p)

  print("")

  # ask the user which platform to use.
  print("[?] please specify the targeted platform:")
  print("[?]   [] ", end="")

  # read the user input.
  g_platform = input()

  if not g_platform in platforms:
    print("[!] unknown target platform")
    print("[!] aborting...")
    sys.exit(42)

  print("")



#
# host()
#
# this function asks the user to choose a host profile name.
#
def			host():
  global g_host
  processors = []
  systems = []
  hosts = {}
  s = None
  p = None
  h = None

  # get the host system entries.
  systems = list(g_directory + "/environment/profile/host",
                 OPTION_DIRECTORY | OPTION_SKIP)

  # for each system, append the host processor building a complete host
  # profile name.
  for s in systems:
    processors = list(g_directory + "/environment/profile/host/" + s,
                      OPTION_DIRECTORY | OPTION_SKIP)

    for p in processors:
      hosts[s + "/" + p.split(".")[0]] = None

  # display a message and list the hosts actually available.
  print("[+] the host profile represents the host you are currently using")
  print("[+] for developing kaneton.")
  print("")
  print("[+] a host profile is composed of an operating system and")
  print("[+] a microprocessor, on which the operating system is running.")
  print("")

  print("[+] available host profiles:")
  for h in hosts:
    print("[+]   o " + h)

  print("")

  # ask the user which host profile to use.
  print("[?] please specify the host profile you want to use:")
  print("[?]   [] ", end="")

  # read the user input.
  g_host = input()

  if not g_host in hosts:
    print("[!] unknown host profile")
    print("[!] aborting...")
    sys.exit(42)

  print("")



#
# user()
#
# this function requests the user profile name.
#
def			user():
  global g_user
  firstname = None
  lastname = None

  # ask the user to specify its firstname.
  print("[?] please specify your firstname without spaces in lowercase:")
  print("[?]   [] ", end="")

  firstname = input().lower()

  print("")

  # ask the user to specify its lastname.
  print("[?] please specify your lastname without spaces and in lowercase:")
  print("[?]   [] ", end="")

  lastname = input().lower()

  print("")

  # if one is missing, abort.
  if not firstname or not lastname:
    print("[!] the firstname and/or the lastname is missing")
    print("[!] aborting...")
    sys.exit(42)

  # set the global variable: g_user
  g_user = firstname + "." + lastname

  # display a little message.
  print("[+] your user profile name will be '" + g_user + "'")
  print("")



#
# system()
#
# this function requests information to the user.
#
def			system():
  global g_directory
  swcd = None
  cwd = None
  key = None

  # get the most likely kaneton root directory: current-directory/..
  scwd = os.getcwd()
  os.chdir(os.pardir)
  cwd = os.getcwd()
  os.chdir(scwd)

  # ask the user to enter the kaneton root directory.
  print("[?] please enter the absolute path to the kaneton root directory:")
  print("[?]   [" + cwd + "] ", end="")

  # read the user's input.
  g_directory = input()

  if not g_directory:
    g_directory = cwd

  # if the path entered is not correct, quit.
  if not os.path.isdir(g_directory):
    print("[!] the entered path is not a directory")
    print("[!] aborting...")
    sys.exit(42)

  print("")



#
# warning()
#
# this function asks the user if he wants to continue in order to create
# a new kaneton user profile.
#
def			warning():
  key = None

  # display a stating message.
  print("[+] it looks like you are a new kaneton user")
  print("")

  # ask the user to continue.
  print("[?] do you want to continue in order to create a new user profile?")
  print("[?]   [y] ", end="")

  # read the user's input and check if it is correct
  key = input().lower()

  if not key:
    key = "y"

  # if not, display a message and quit.
  if key != "y":
    print("[!] aborting...")
    sys.exit(42)

  print("")



#
# main()
#
# this function checks if the shell environment variables are set. if not,
# this means that the user is a new kaneton user. therefore, the function
# calls other functions asking information about the user.
#
def			main():
  a = None
  y = None
  p = None
  h = None
  u = None

  # get the shell environment variables.
  u = os.getenv("KANETON_USER")
  h = os.getenv("KANETON_HOST")
  y = os.getenv("KANETON_PYTHON")
  p = os.getenv("KANETON_PLATFORM")
  a = os.getenv("KANETON_ARCHITECTURE")

  # check the presence of the shell environment variables.
  if u != None and h != None and y != None and p != None and a != None:
    print("[+] it looks like you already set up your environment variables")
    return

  # ask the user to continue.
  warning()

  # request information on the system.
  system()

  # request the user profile name.
  user()

  # ask the user to pick a host.
  host()

  # ask the user to pic a target platform.
  platform()

  # ask the user to specify the target architecture.
  architecture()

  # ask the user about the location of the python binary.
  python()

  # finally build the user profile and tell the user what to do next.
  build()

  # a message.
  print("[+] thank you! you can relaunch the configure script")

  sys.exit(0)

#
# ---------- entry point ------------------------------------------------------
#

main()
