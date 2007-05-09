#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ile/environment/behaviour/linux/linux.py
#
# created       julien quintard   [tue may  8 13:20:21 2007]
# updated       julien quintard   [wed may  9 15:08:39 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this file implements the entire kaneton python interface.
#

#
# ---------- imports ----------------------------------------------------------
#

import os
import re
import sys
import tempfile
import shutil

#
# ---------- definitions ------------------------------------------------------
#

HEADER_NONE = 0
HEADER_OK = 1
HEADER_ERROR = 2
HEADER_INTERACTIVE = 4

COLOR_NONE = 0
COLOR_RED = 1
COLOR_GREEN = 2
COLOR_YELLOW = 3
COLOR_BLUE = 4
COLOR_WHITE = 5

OPTION_NONE = 0
OPTION_NO_NEWLINE = 1
OPTION_FLICKERING = 2

OPTION_FILE = 1
OPTION_DIRECTORY = 2
OPTION_ALL = OPTION_FILE | OPTION_DIRECTORY

OPTION_DEVICE = 1
OPTION_IMAGE = 2

#
# ---------- functions --------------------------------------------------------
#

#
# colorize()
#
# this function returns a colorized text if the environment is configured
# to or simply the original text.
#
# note that this function implementation is based on UNIX escape sequences.
#
def			colorize(text, color, options):
  if _DISPLAY_ == _DISPLAY_UNCOLORED_:
    return text

  if options & OPTION_FLICKERING:
    text = "[01;05m" + text + "[39;49;00m"

  if color == COLOR_RED:
    text = "[31;01m" + text + "[39;49;00m"
  elif color == COLOR_GREEN:
    text = "[32;01m" + text + "[39;49;00m"
  elif color == COLOR_YELLOW:
    text = "[33;01m" + text + "[39;49;00m"
  elif color == COLOR_BLUE:
    text = "[34;01m" + text + "[39;49;00m"
  elif color == COLOR_WHITE:
    text = "[37;01m" + text + "[39;49;00m"

  return text



#
# display()
#
# this function prints a kaneton message.
#
def			display(header, text):
  if header == HEADER_NONE:
    sys.stdout.write(colorize(text, COLOR_NONE, OPTION_NONE))
  elif header == HEADER_OK:
    sys.stdout.write(colorize("[", COLOR_BLUE, OPTION_NONE))
    sys.stdout.write(colorize("+", COLOR_GREEN, OPTION_NONE))
    sys.stdout.write(colorize("]", COLOR_BLUE, OPTION_NONE))
    sys.stdout.write(" ")
    sys.stdout.write(colorize(text, COLOR_WHITE, OPTION_NONE))
  elif header == HEADER_ERROR:
    sys.stdout.write(colorize("[", COLOR_BLUE, OPTION_NONE))
    sys.stdout.write(colorize("!", COLOR_RED, OPTION_NONE))
    sys.stdout.write(colorize("]", COLOR_BLUE, OPTION_NONE))
    sys.stdout.write(" ")
    sys.stdout.write(colorize(text, COLOR_WHITE, OPTION_NONE))
  elif header == HEADER_INTERACTIVE:
    sys.stdout.write(colorize("[", COLOR_BLUE, OPTION_NONE))
    sys.stdout.write(colorize("?", COLOR_YELLOW, OPTION_FLICKERING))
    sys.stdout.write(colorize("]", COLOR_BLUE, OPTION_NONE))
    sys.stdout.write(" ")
    sys.stdout.write(colorize(text, COLOR_WHITE, OPTION_NONE))

  sys.stdout.write("\n")



#
# contents()
#
# this functions returns the contents of one or more files.
#
def			contents(files):
  contents = None
  handle = None
  file = None
  line = None

  contents = ""

  for file in files:
    if not os.path.exists(file):
      continue

    try:
      handle = open(file, "r")
    except IOError:
      continue

    for line in handle.readlines():
      contents += line

    handle.close()

  return contents



#
# temporary()
#
# this function provides an easy way to create a temporary file or directory.
#
def			temporary(options):
  location = None

  if options == OPTION_FILE:
    location = tempfile.mkstemp()[1]
  elif options == OPTION_DIRECTORY:
    location = tempfile.mkdtemp()

  return location



#
# cwd()
#
# this function returns the current working directory.
#
def			cwd():
  return os.getcwd()



#
# input()
#
# this function waits for an input.
#
def			input():
  return raw_input()



#
# launch()
#
# this function launch a new program/script/make etc..
#
def			launch(file, arguments):
  directory = None
  info = None
  status = 0
  wd = None
  pp = None

  info = os.path.split(file)

  directory = info[0]
  file = info[1]

  wd = cwd()

  cd(directory)

  if re.match("^.*\.sh$", file):
    status = os.system(_SHELL_ + " " + file + " " + arguments)
  elif re.match("^.*\.py$", file):
    pp = os.getenv("PYTHONPATH")
    os.putenv("PYTHONPATH", pp + ":" + wd)
    status = os.system(_PYTHON_ + " " + file + " " + arguments)
    os.putenv("PYTHONPATH", pp)
  elif re.match("^.*\.pl$", file):
    status = os.system(_PERL_ + " " + file + " " + arguments)
  elif re.match("^Makefile$", file):
    status = os.system(_MAKE_ + " -f " + file + " " + arguments)
  else:
    status = os.system(file + " " + arguments)

  cd(wd)

  return status



#
# copy()
#
# this function copies a file.
#
def			copy(source, destination):
  shutil.copyfile(source, destination)



#
# link()
#
# this function builds a link name source to the file destination.
#
def			link(source, destination):
  os.symlink(destination, source)



#
# remove()
#
# this function removes the targets.
#
def			remove(target):
  entries = None
  entry = None

  if os.path.isfile(target) or os.path.islink(target):
    os.unlink(target)

  if os.path.isdir(target):
    entries = os.listdir(target)
    for entry in entries:
      remove(target + "/" + entry)
    os.rmdir(target)



#
# list()
#
# this function lists the entries of a directory.
#
def			list(directory, options):
  elements = []
  entries = None
  entry = None

  entries = os.listdir(directory)

  for entry in entries:
    if (options & OPTION_FILE) and					\
       (os.path.isfile(directory + "/" + entry)):
      elements += [ entry ]
    if (options & OPTION_DIRECTORY) and					\
       (os.path.isdir(directory + "/" + entry)):
      elements += [ entry ]

  return elements



#
# cd()
#
# this function changes the current working directory.
#
def			cd(directory):
  os.chdir(directory)



#
# search()
#
# this function searches for file names matching the given pattern.
#
def			search(directory, pattern, options):
  elements = []
  entries = None
  entry = None

  entries = os.listdir(directory)

  for entry in entries:
    if (options & OPTION_FILE) and					\
       (os.path.isfile(directory + "/" + entry)) and			\
       (re.search(pattern, entry)):
      elements += [ directory + "/" + entry ]

    if (os.path.isdir(directory + "/" + entry)):
      if (options & OPTION_DIRECTORY) and				\
         (re.search(pattern, entry)):
        elements += [ directory + "/" + entry ]

      elements += search(directory + "/" + entry, pattern, options)

  return elements



#
# pack()
#
# this function creates an archive of the given directory.
#
def			pack(directory, file):
  launch(_TAR_, "-czf " + file + " " + directory)



#
# unpack()
#
# this function unpackages an archive into the given (optional) directory.
#
def			unpack(file, directory):
  if directory:
    launch(_TAR_, "-xzf " + file + " -C " + directory)
  else:
    launch(_TAR_, "-xzf " + file)



#
# mkdir()
#
# this function creates a directory.
#
def			mkdir(directory):
  os.mkdir(directory)



#
# load()
#
# this function copies a file on a device, this device can be virtual:
# an image.
#
def			load(file, device, path, options):
  if options == OPTION_DEVICE:
    launch(_MCOPY_, "-o -n " + file + " " + device + path)

  if options == OPTION_IMAGE:
    launch(_MCOPY_, "-o -n " + "-i" + device + " " + file + " ::/" + path)



#
# stamp()
#
# this function returns the current formatted date.
#
def			stamp(format):
  return strftime(format)



#
# record()
#
# this function runs the program recording a session.
#
def			record(log, time):
  launch(_SCRIPT_, "-q -t " + log + " -c " + _TRANSCRIPTS_CMD_ + " 2> " + time)



#
# play()
#
# this function runs the program replaying a session.
#
def			play(log, time):
  launch(_SCRIPTREPLAY_TOOL_, time + " " + log)



#
# locate()
#
# this function tries to locate a program on the system.
#
def			locate(file):
  path = None
  directory = None

  path = os.getenv("PATH")

  for directory in path.split(os.pathsep):
    if os.path.exists(directory + "/" + file):
      return directory + "/" + file

  return None
