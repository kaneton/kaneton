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
# updated       julien quintard   [sun feb 20 01:00:57 2011]
#

#
# ---------- packages ---------------------------------------------------------
#

import xmlrpclib
import tempfile
import os
import shutil
import re

import ktp

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

#
# this function creates the directories up to the destination path.
#
def                     Dig(target):
  path = None
  steps = None
  step = None

  if target[0] == "/":
    path = "/"

  steps = os.path.dirname(target).strip("/").split("/")

  for step in steps:
    if not path:
      path = step
    else:
      path = path + "/" + step

    if not os.path.exists(path):
      os.mkdir(path)

#
# this function transfers the content from one file to the other.
#
def                     Transfer(source, destination):
  input = None
  output = None
  line = None

  try:
    input = open(source, "r")
    output = open(destination, "a")
  except IOError:
    return None

  line = input.read(256)
  output.write(line)

  output.close()
  input.close()

def                     Email(source,
                              destination,
                              subject,
                              body,
                              attachment = None):
  capability = None
  content = None
  configuration = None
  message = None
  stream = None
  report = None
  status = None
  output = None
  emails = None
  a = None

  # create a temporary file.
  configuration = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # create a configuration file.
  content = """\
set realname = "%(from)s"
set from = "%(from)s"
set use_from = yes\
""" % { "from": source }

  # store the temporary mutt configuration file.
  ktp.miscellaneous.Push(content, configuration)

  # create a temporary file.
  message = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # prepare the -a option depending on the presence of an attachment.
  if attachment:
    a = [ "-a", attachment ]
  else:
    a = []

  # store the temporary message.
  ktp.miscellaneous.Push(body, message)

  # create a stream file.
  stream = ktp.miscellaneous.Temporary(ktp.miscellaneous.OptionFile)

  # email the capability to the supposed recipient.
  status = ktp.process.Invoke("mutt",
                              a +
                              [ "-F", configuration,
                                "-s", subject,
                                "--",
                                " ".join(destination),
                                "<" + message ])

  # retrieve the output.
  output = ktp.miscellaneous.Pull(stream)

  # remove the stream file.
  ktp.miscellaneous.Remove(stream)

  # remove the temporary files.
  ktp.miscellaneous.Remove(message)
  ktp.miscellaneous.Remove(configuration)

#
# this function transforms a Python data structure into a string
# representation.
#
def                     Stringify(data, margin = "", alignment = 26):
  string = str()
  key = None
  length = None

  if isinstance(data, dict):
    for key in data:
      if not isinstance(data[key], dict) and not isinstance(data[key], list):
        length = len(margin) + len(str(key)) + 1

        string += margin + str(key) + ":" +                             \
            (alignment - length) * " " + str(data[key]) + "\n"
      else:
        string += margin + str(key) + ":" + "\n"

        string += Stringify(data[key], margin + "  ", alignment)
  elif isinstance(data, list):
    for element in data:
      string += Stringify(element, margin, alignment)
  else:
    string += alignment * " " + str(data) + "\n"

  return string
