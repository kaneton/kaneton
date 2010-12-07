#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/KANETON-TEST-SYSTEM/packages/ktp/suite.py
#
# created       julien quintard   [wed oct 27 10:53:58 2010]
# updated       julien quintard   [tue dec  7 15:09:17 2010]
#

#
# ---------- packages ---------------------------------------------------------
#

import yaml
import os
import re
import sys

TestDirectory = os.path.abspath(os.path.dirname(                        \
                  os.path.realpath(sys.argv[0])) + "/..")

sys.path.append(TestDirectory + "/packages")

import ktp

#
# ---------- definitions ------------------------------------------------------
#

Extension = ".suite"

TestsDirectory = TestDirectory + "/tests"

#
# ---------- functions --------------------------------------------------------
#

#
# this function returns a list of suite identifiers.
#
def                     List(directory):
  suites = []
  entries = None
  entry = None
  path = None

  entries = os.listdir(directory)

  for entry in entries:
    path = directory + "/" + entry

    if os.path.isfile(path) and re.search("^.*" + Extension + "$", path):
      suites += [ entry[:-len(Extension)] ]

  return suites

#
# this function loads a suite.
#
def                     Load(path):
  return yaml.load(file(path, 'r'))

#
# this function loads all the manifests belonging to the given suite in
# a dictionary.
#
def                     Manifests(suite):
  manifests = {}
  component = None
  name = None
  location = None
  paths = None
  path = None
  manifest = None
  test = None

  # load the manifests.
  if "components" in suite:
    for component in suite["components"]:
      for location in suite["components"][component]:
        # search for manifest files.
        paths = ktp.miscellaneous.Search(TestsDirectory + "/" +       \
                                           location,
                                         "^.*\.mnf$",
                                         ktp.miscellaneous.OptionFile |
                                         ktp.miscellaneous.OptionRecursive)

        for path in paths:
          # load the manifest.
          manifest = ktp.manifest.Load(path)

          # compute the proper name according to the defined component:
          # for instance, given the "segment" component, the test name
          # segment/permissions/01 will be transformed into permissions/01
          if component == manifest["name"][:len(component)]:
            test = manifest["name"][len(component) + 1:]
          else:
            test = manifest["name"]

          manifests[test] = manifest

  return manifests
