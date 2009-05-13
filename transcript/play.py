#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/transcript/play.py
#
# created       julien quintard   [mon may 28 12:39:21 2007]
# updated       julien quintard   [mon may 11 21:45:25 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this script replays a recorded session.
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

g_directories = ("basic",)

g_store = []
g_transcript = None
g_path = None

#
# ---------- functions --------------------------------------------------------
#

#
# launch()
#
# this function launch the playing.
#
def			launch():
  # a final message.
  env.display(env.HEADER_OK, "playing the session...", env.OPTION_NONE)

  # finally launch the recording.
  env.play(env._TRANSCRIPT_DIR_ + "/" + g_path, env.OPTION_NONE)



#
# usage()
#
# this function prints the transcripts names.
#
def			usage():
  location = None
  store = None

  env.display(env.HEADER_ERROR, "usage: play.py [transcript]", env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  env.display(env.HEADER_ERROR, "transcripts:", env.OPTION_NONE)

  for store in g_store:
    for location in store:
      env.display(env.HEADER_ERROR, "  " + location, env.OPTION_NONE)



#
# lookup()
#
# this function tries to match the given transcript name with transcripts
# previously found.
#
def			lookup():
  global g_transcript
  global g_path
  location = None
  store = None

  for store in g_store:
    for location in store:
      if re.search(g_transcript, location) != None:
        g_transcript = location
        g_path = store[location]
        return



#
# warning()
#
# this function asks the user if everything is right.
#
def			warning():
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)
  env.display(env.HEADER_OK, "configuration:", env.OPTION_NONE)
  env.display(env.HEADER_OK,
              "  transcript:             " + g_transcript,
              env.OPTION_NONE)
  env.display(env.HEADER_NONE, "", env.OPTION_NONE)



#
# locate()
#
# this function tries to locate the transcripts directories.
#
def			locate(directories):
  locations = []
  papers = None
  paper = None

  for directory in directories:
    store = {}

    papers = env.search(directory, "^.*\.ts$",
                        env.OPTION_FILE | env.OPTION_RECURSIVE)

    for paper in papers:
      store[paper.replace("/", "::")] = paper

    locations += [store]

  return locations



#
# main()
#
# this function builds and displays the transcript requested by the user.
#
def			main():
  global g_transcript
  global g_store
  transcript = None

  # find out the locations of the transcripts.
  g_store = locate(g_directories)

  # check the number of arguments.
  if len(sys.argv) != 2:
    usage()
    sys.exit(42)

  # display a message.
  env.display(env.HEADER_OK,
              "looking for the transcript in the transcripts database",
              env.OPTION_NONE)

  # set the transcript name looked for.
  g_transcript = sys.argv[1]

  # look up for the given name in the transcripts database.
  lookup()

  # if no transcript is found, display an error.
  if g_path == None:
    env.display(env.HEADER_ERROR, "no transcript named '" + g_transcript +
                "' found in the transcript database", env.OPTION_NONE)
    usage()
    sys.exit(42)

  # display the transcript name and ask the user to continue.
  warning()

  # launch the playing.
  launch()

  # a final message.
  env.display(env.HEADER_OK, "transcript played successfully",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
