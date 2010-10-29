#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANE...EST-SYSTEM/packages/ktp/miscellaneous.py
#
# created       julien quintard   [mon oct 25 19:51:49 2010]
# updated       julien quintard   [wed oct 27 14:41:22 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import xmlrpclib
import tempfile
import os
import shutil
import re

#
# ---------- definitions ------------------------------------------------------
#

OptionNone = 0
OptionFile = 1
OptionDirectory = 2
OptionRecursive = 3

#
# ---------- functions --------------------------------------------------------
#

#
# this function converts some data into its binary form.
#
def                     Binary(data):
  return xmlrpclib.Binary(data)

#
# this function removes the target file system object.
#
def                     Remove(target):
  entries = None
  entry = None

  if os.path.isfile(target) or os.path.islink(target):
    os.unlink(target)

  if os.path.isdir(target):
    entries = os.listdir(target)

    for entry in entries:
      Remove(target + "/" + entry)

    os.rmdir(target)

#
# this function writes data to a file.
#
def                     Push(content, path):
  handle = None

  handle = open(path, "w")

  handle.write(content)

  handle.close()

#
# this function reads data from a file.
#
def                     Pull(path):
  handle = None
  line = None

  if not os.path.exists(path):
    return None

  try:
    handle = open(path, "r")
  except IOError:
    return None

  content = str()

  for line in handle.readlines():
    content += line

  handle.close()

  if len(content) == 0:
    content = None

  return content

#
# this function creates a temporary file or directory.
#
def                     Temporary(option):
  location = None

  if option == OptionFile:
    tuple = tempfile.mkstemp()

    os.close(tuple[0])

    location = tuple[1]
  elif option == OptionDirectory:
    location = tempfile.mkdtemp()

  return location

#
# this function copies one file to another target.
#
def                     Copy(path, target):
  shutil.copy(path, target)

#
# this function searches for file names matching the given pattern.
#
def                     Search(directory, pattern, options):
  elements = []
  entries = None
  entry = None

  entries = os.listdir(directory)

  for entry in entries:
    if (options & OptionFile) and                                       \
       (os.path.isfile(directory + "/" + entry)) and                    \
       (re.search(pattern, entry)):
      elements += [ directory + "/" + entry ]

    if (os.path.isdir(directory + "/" + entry)):
      if (options & OptionDirectory) and                                \
         (re.search(pattern, entry)):
        elements += [ directory + "/" + entry ]

      if (options & OptionRecursive) and                                \
         (not os.path.islink(directory + "/" + entry)):
        elements += Search(directory + "/" + entry, pattern, options)

  return elements
