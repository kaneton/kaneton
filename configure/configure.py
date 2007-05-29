#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/configure/configure.py
#
# created       julien quintard   [wed may 23 10:17:59 2007]
# updated       julien quintard   [fri may 25 13:10:53 2007]
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

import env
import yaml
import sys

import environment
import dialog

#
# ---------- globals ----------------------------------------------------------
#

g_title = "[configure]"

g_frames = []

g_entries = None

#
# ---------- types ------------------------------------------------------------
#

class c_entry:
  def __init__(self):
    self.type = None

    self.frame = None
    self.path = None

    self.variable = None

#
# ---------- options ----------------------------------------------------------
#

TYPE_FRAME = 1
TYPE_VARIABLE = 2

#
# ---------- functions --------------------------------------------------------
#

#
# frame()
#
# this function displays the current frame and waits for the user
# to do something.
#
def			frame(path):
  global g_entries
  object = None
  choice = None
  stream = None
  entry = None
  menu = []
  s = None

  # initialisation of the 'g_entries' menu.
  g_entries = []

  # load the frame file and parse the YAML syntax
  stream = yaml.load(env.file(path, None, env.OPTION_READ))

  # build a list of entries.
  for s in stream:
    # new instance of the c_entry class.
    entry = c_entry()

    # detect the entry type and build the entry object as well as
    # the menu list.
    try:
      entry.type = TYPE_FRAME
      entry.frame = s["frame"]
      entry.path = s["path"]

      menu.append(entry.frame)
    except:
      try:
        entry.type = TYPE_VARIABLE
        entry.variable = s["variable"]

        menu.append(environment.get(entry.variable).name)
      except:
        continue

    g_entries.append(entry)

  # actually display the frame and get the user entry choice.
  choice = dialog.menu(g_title, menu)

  if choice == None:
    return None

  return g_entries[choice]



#
# radio()
#
# this function displays a radiolist in order to ask the user to
# choice a value among a list.
#
def			radio(object):
  default = None
  value = None
  key = None
  menu = []

  for value in object.values:
    key = value.keys()[0]

    if object.value == value[key]:
      default = key

    menu.append(key)

  dialog.radio(g_title, menu, default)



#
# variable()
#
# this function dispatch the function call to an appropriate function
# depending on the variable value type.
#
def			variable(entry):
  object = None
  value = None

  object = environment.get(entry.variable)

  if object.type == environment.TYPE_STATE:
    value = check(object)
  elif object.type == environment.TYPE_SET:
    value = radio(object)
  elif object.type == environment.TYPE_ANY:
    value = input(object)



#
# main()
#
# this function starts by loading the environment and then displays the
# interfactive frames.
#
def			main():
  global g_frames
  entry = None

  # load the environment variables.
  environment.init()

  # initialiase the dialog package.
  dialog.init()

  # initialise the 'g_frames' variable.
  g_frames.append("configure.frm")

  # main loop until there is a frame or variable to display.
  while True:
    # display the current frame stored in 'g_frame'.
    entry = frame(g_frames[len(g_frames) - 1])

    # did the user want to go back?
    if not entry:
      g_frames.pop(len(g_frames) - 1)

      if len(g_frames) == 0:
        sys.exit(0)

      continue

    # if the choice is a subframe, then go to it. otherwise, ask the
    # user to enter a new value for the variable.
    if entry.type == TYPE_FRAME:
      g_frames.append(entry.path)

    elif entry.type == TYPE_VARIABLE:
      variable(entry)

#
# ---------- entry point ------------------------------------------------------
#

main()
