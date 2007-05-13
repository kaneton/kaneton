#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ofile/environment/behaviour/behaviour.py
#
# created       julien quintard   [tue may  8 13:03:40 2007]
# updated       julien quintard   [sun may 13 11:26:14 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this file contains generic stuff about the python interface including
# options and general functions.
#
# since the python scripting language was designed to provide portable
# operations, the following operations are considered as generic, making
# work easier for behaviour sub-profiles.
#

#
# ---------- imports ----------------------------------------------------------
#

import os
import re
import sys
import tempfile
import shutil
import time

#
# ---------- options ----------------------------------------------------------
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
OPTION_RECURSIVE = 3
OPTION_ALL = OPTION_FILE | OPTION_DIRECTORY

OPTION_DEVICE = 1
OPTION_IMAGE = 2

#
# ---------- functions --------------------------------------------------------
#

#
# display()
#
# this function prints a kaneton message.
#
def			display(header, text, options):
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
def			contents(files, options):
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
def			cwd(options):
  return os.getcwd()



#
# input()
#
# this function waits for an input.
#
def			input(options):
  return raw_input()



#
# copy()
#
# this function copies a file.
#
def			copy(source, destination, options):
  shutil.copyfile(source, destination)



#
# link()
#
# this function builds a link name source to the file destination.
#
def			link(source, destination, options):
  os.symlink(destination, source)



#
# remove()
#
# this function removes the targets.
#
def			remove(target, options):
  entries = None
  entry = None

  if os.path.isfile(target) or os.path.islink(target):
    os.unlink(target)

  if os.path.isdir(target):
    entries = os.listdir(target)
    for entry in entries:
      remove(target + "/" + entry, OPTION_NONE)
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
def			cd(directory, options):
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

      if (options & OPTION_RECURSIVE) and				\
         (not os.path.islink(directory + "/" + entry)):
        elements += search(directory + "/" + entry, pattern, options)

  return elements



#
# mkdir()
#
# this function creates a directory.
#
def			mkdir(directory, options):
  os.mkdir(directory)



#
# stamp()
#
# this function returns the current formatted date.
#
def			stamp(format, options):
  return time.strftime(format)



#
# path()
#
# this function returns information on a path: file, directory etc..
#
def			path(path, options):
  if options == OPTION_FILE:
    return os.path.basename(path)
  if options == OPTION_DIRECTORY:
    return os.path.dirname(path)

  return None
