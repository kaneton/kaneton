#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/configure/configure.py
#
# created       julien quintard   [wed may 23 10:17:59 2007]
# updated       julien quintard   [sun oct 24 14:52:05 2010]
#

#
# ---------- information ------------------------------------------------------
#
# this script is a user-friendly software allowing users to customize
# their own development environment
#

#
# ---------- imports ----------------------------------------------------------
#

import critical

import yaml
import sys

import environment
import dialog

#
# ---------- types ------------------------------------------------------------
#

class c_entry:
  def __init__(self):
    self.type = None

    self.frame = None
    self.path = None

    self.variable = None

    self.list = []
    self.current = None

class c_frame:
  def __init__(self):
    self.title = None
    self.description = None

    self.entries = []

    # a list of strings rather that a list of objects: self.entries
    self.list = []

#
# ---------- globals ----------------------------------------------------------
#

g_frame = None

g_paths = []
g_path = None

#
# ---------- options ----------------------------------------------------------
#

TYPE_FRAME = 1
TYPE_VARIABLE = 2

DRAW_MENU = 1
DRAW_RADIO = 2

#
# ---------- functions --------------------------------------------------------
#

#
# load()
#
# this function loads the current frame into a instance of the
# frame class.
#
def			load(path):
  global g_frame
  streams = None
  stream = None
  object = None
  entry = None
  value = None
  key = None

  # load the YAML entries.
  streams = yaml.load(file(path, 'r'))

  # create an new frame object.
  g_frame = c_frame()

  # try to fill the g_frame object with the YAML entries.
  for stream in streams:
    try:
      g_frame.title = stream["title"]
      g_frame.description = stream["description"]

      continue
    except:
      pass

    entry = c_entry()

    try:
      entry.frame = stream["frame"]
      entry.path = stream["path"]
      entry.type = TYPE_FRAME

      g_frame.list.append(entry.frame)
      g_frame.entries.append(entry)
    except:
      try:
        entry.type = TYPE_VARIABLE
        entry.variable = environment.get(stream["variable"])

        if not entry.variable:
          continue

        # build the list and current value for each variable type.
        if entry.variable.type == environment.TYPE_SET:
          for key, value in entry.variable.values.items():
            if entry.variable.assignment == value:
              entry.current = key

            entry.list.append(key)
        elif entry.variable.type == environment.TYPE_ANY:
          entry.current = entry.variable.assignment

        g_frame.list.append(entry.variable.string)
        g_frame.entries.append(entry)

      except:
        continue



#
# variable()
#
# this function is launched whenever a user wants to edit a variable.
#
def			variable(choice):
  select = None
  value = None

  # set variable
  if g_frame.entries[choice].variable.type == environment.TYPE_SET:
    select = dialog.radio(g_frame.entries[choice].variable.string,
                          g_frame.entries[choice].variable.description,
                          g_frame.entries[choice].list,
                          g_frame.entries[choice].current)

    if select == None:
      return

    value = g_frame.entries[choice].variable.values[
                g_frame.entries[choice].list[select]
              ]

  # any variable
  elif g_frame.entries[choice].variable.type == environment.TYPE_ANY:
    value = dialog.input(g_frame.entries[choice].variable.string,
                         g_frame.entries[choice].variable.description,
                         g_frame.entries[choice].current)

  # tell the environment to modify the variable's value.
  environment.modify(g_frame.entries[choice].variable.variable, value)



#
# frame()
#
# this function displays the current frame and get the user's choice.
#
def			frame():
  global g_paths
  global g_path
  choice = None

  # display the current frame and get the user's choice.
  choice = dialog.menu(g_frame.title, g_frame.description, g_frame.list)

  # did the user want to go back?
  if choice == None:
    g_paths.pop(len(g_paths) - 1)

    if len(g_paths) == 0:
      sys.exit(0)

    g_path = g_paths[len(g_paths) - 1]

    return

  # if the choice is a subframe, then go to it.
  if g_frame.entries[choice].type == TYPE_FRAME:
    g_path = g_frame.entries[choice].path
    g_paths.append(g_path)
  # otherwise, ask the user to enter a new value for the variable.
  elif g_frame.entries[choice].type == TYPE_VARIABLE:
    variable(choice)



#
# main()
#
# this function starts by loading the environment and then displays the
# interfactive frames.
#
def			main():
  global g_paths
  global g_path

  # load the environment variables.
  environment.init()

  # initialiase the dialog package.
  dialog.init()

  # initialize the 'g_path' variable.
  g_path = "configure.frm"
  g_paths.append(g_path)

  # main loop until there is a frame or variable to display.
  while True:
    # load the information on the current frame.
    load(g_path)

    # handle the current frame: display, get the user choice etc.
    frame()

#
# ---------- entry point ------------------------------------------------------
#

main()
