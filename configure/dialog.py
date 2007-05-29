#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/configure/dialog.py
#
# created       julien quintard   [wed may 23 10:36:51 2007]
# updated       julien quintard   [fri may 25 12:25:02 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this module provides a wrapper upon the well-know dialog software.
#

#
# ---------- imports ----------------------------------------------------------
#

import env

#
# ---------- functions --------------------------------------------------------
#

#
# menu()
#
# this function displays a menu.
#
def			menu(title, entries):
  choice = None
  menu = ""

  for i in range(len(entries)):
    menu += "\"" + entries[i] + "\"" + " " + "\"\""

    if (i + 1) < len(entries):
      menu += " "

  env.launch("dialog", "--title \"" + title +				\
             "\" --menu \"\" " + "10 50 0 " + menu +			\
             " 2> " + g_temporary, env.OPTION_NONE)

  choice = env.file(g_temporary, None, env.OPTION_READ)

  if not choice:
    return None

  return entries.index(choice)



#
# radio()
#
# this function displays a radio list.
#
def			radio(title, entries, current):
  choice = None
  menu = ""

  for i in range(len(entries)):
    if current == entries[i]:
      status = "\"on\""
    else:
      status = "\"off\""

    menu += "\"" + entries[i] + "\"" + " " + "\"\"" + " " + status

    if (i + 1) < len(entries):
      menu += " "

  env.launch("dialog", "--title \"" + title +				\
             "\" --radiolist \"\" " + "10 50 0 " + menu +		\
             " 2> " + g_temporary, env.OPTION_NONE)

  choice = env.file(g_temporary, None, env.OPTION_READ)

  if not choice:
    return None

  return entries.index(choice)



#
# init()
#
# this function iniitialises the dialog package by building a temporary
# file.
#
def			init():
  global g_temporary

  g_temporary = env.temporary(env.OPTION_FILE)
