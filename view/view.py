#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/view.py
#
# created       julien quintard   [sun may 13 11:04:52 2007]
# updated       julien quintard   [mon may 11 21:45:47 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this script is used to build and display kaneton documents.
#

#
# ---------- imports ----------------------------------------------------------
#

import env

import sys
import re

#
# ---------- globals ----------------------------------------------------------
#

g_directories = ("book",
                 "exam",
                 "feedback",
                 "internship",
                 "lecture",
                 "paper",
                 "talk")

g_store = []
g_document = None
g_path = None

#
# ---------- functions --------------------------------------------------------
#

#
# build()
#
# this function builds the document.
#
def			build():
  env.launch(g_path + "/Makefile", "build", env.OPTION_NONE)



#
# view()
#
# this function displays the document.
#
def			view():
  env.launch(g_path + "/Makefile", "view", env.OPTION_NONE)



#
# usage()
#
# this function prints the documents names.
#
def			usage():
  location = None
  store = None

  env.display(env.HEADER_ERROR, "usage: view.py [document]", env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  env.display(env.HEADER_ERROR, "documents:", env.OPTION_NONE)

  for store in g_store:
    for location in store:
      env.display(env.HEADER_ERROR, "  " + location, env.OPTION_NONE)



#
# lookup()
#
# this function tries to match the given document name with documents
# previously found.
#
def			lookup():
  global g_document
  global g_path
  location = None
  store = None

  for store in g_store:
    for location in store:
      if re.search(g_document, location) != None:
        g_document = location
        g_path = store[location]
        return



#
# warning()
#
# this function displays the configuration.
#
def			warning():
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK, "configuration:", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  document:               " + g_document,
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)



#
# locate()
#
# this function tries to locate the documents directories.
#
def			locate(directories):
  locations = []
  papers = None
  paper = None

  for directory in directories:
    store = {}

    papers = env.search(directory, "^.*\.tex$",
                        env.OPTION_FILE | env.OPTION_RECURSIVE)

    for paper in papers:
      paper = env.path(paper, env.OPTION_DIRECTORY)

      store[paper.replace("/", "::")] = paper

    locations += [store]

  return locations



#
# main()
#
# this function builds and displays the document requested by the user.
#
def			main():
  global g_document
  global g_store
  document = None

  # find out the locations of the documents.
  g_store = locate(g_directories)

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # display a message.
  env.display(env.HEADER_OK,
              "looking for the document in the documents database",
              env.OPTION_NONE)

  # set the document name looked for.
  g_document = sys.argv[1]

  # look up for the given name in the document database.
  lookup()

  # if no document is found, display an error.
  if g_path == None:
    env.display(env.HEADER_ERROR, "no document named '" + g_document +
                "' found in the document database", env.OPTION_NONE)
    usage()
    sys.exit(42)

  # display the document name and ask the user to continue.
  warning()

  # build the document.
  build()

  # display the document.
  view()

  # a final message.
  env.display(env.HEADER_OK, "document displayed successfully",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
