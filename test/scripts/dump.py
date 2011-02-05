#! /usr/bin/python2
#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/scripts/dump.py
#
# created       julien quintard   [mon apr 13 04:06:49 2009]
# updated       julien quintard   [thu feb  3 00:01:11 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this extremely simple script takes a YAML file and displays its inner
# data structure in a hiearchical way.
#

#
# ---------- requirement ------------------------------------------------------
#

import os
import sys
import re

TestDirectory = os.path.abspath(os.path.dirname(                        \
                  os.path.realpath(sys.argv[0])) + "/..")

sys.path.append(TestDirectory + "/packages")

#
# ---------- packages ---------------------------------------------------------
#

import argparse
import yaml

import ktp

#
# ---------- globals ----------------------------------------------------------
#

# the command-line parser.
g_parser = None

#
# ---------- functions --------------------------------------------------------
#

#
# this function dumps any Python data structure in a hierarchical way.
#
def                     Dump(data, margin = "", alignment = 26):
  key = None
  length = None
  element = None

  if isinstance(data, dict):
    for key in data:
      if not isinstance(data[key], dict) and not isinstance(data[key], list):
        length = len(margin) + len(str(key)) + 1

        print(margin + str(key) + ":" +
              (alignment - length) * " " +
              str(data[key]))
      else:
        print(margin + str(key) + ":")

        Dump(data[key], margin + "  ")
  elif isinstance(data, list):
    for element in data:
      Dump(element, margin)
  else:
    length = len(margin)

    print(margin +
          (alignment - length) * " " +
          str(data))

#
# this is the main function
#
def                     Main():
  global g_parser

  content = None

  # create a new parser.
  g_parser = argparse.ArgumentParser(description="This script dumps "   \
                                       "the given YAML file being a "   \
                                       "statement, database, etc.")

  # set up the authorised arguments.
  g_parser.add_argument("--path", '-p',
                        required = True,
                        help = "the path to the file to display",
                        dest = "path")

  # parse the arguments.
  namespace = g_parser.parse_args()

  # load the statement.
  content = yaml.load(file(namespace.path, 'r'))

  # display the content.
  Dump(content)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  Main()
