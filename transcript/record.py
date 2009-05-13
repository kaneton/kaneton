#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/transcript/record.py
#
# created       julien quintard   [mon may 28 01:53:11 2007]
# updated       julien quintard   [mon may 11 21:45:30 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this script captures a shell session in a transcript file
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
# launch()
#
# this function launch the session recording.
#
def			launch():
  # a final message.
  env.display(env.HEADER_OK, "recording the session...", env.OPTION_NONE)

  # finally launch the recording.
  env.record(g_path, env.OPTION_NONE)



#
# usage()
#
# this function prints the transcript names.
#
def			usage():
  location = None
  store = None

  env.display(env.HEADER_ERROR, "usage: record.py [transcript]",
              env.OPTION_NONE)

  env.display(env.HEADER_NONE, "", env.OPTION_NONE)

  env.display(env.HEADER_ERROR, "existing transcripts:", env.OPTION_NONE)

  for store in g_store:
    for location in store:
      env.display(env.HEADER_ERROR, "  " + location, env.OPTION_NONE)



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
# this function tries to locate the transcript locations.
#
def			locate(directories):
  transcripts = None
  transcript = None
  locations = []
  dir = None

  for directory in directories:
    store = {}

    transcripts = env.search(directory, "^.*\.ts$",
                             env.OPTION_FILE | env.OPTION_RECURSIVE)

    for transcript in transcripts:
      store[transcript.replace("/", "::")] = transcript

    locations += [store]

  return locations



#
# main()
#
# this function records a session into a transcript.
#
def			main():
  global g_transcript
  global g_path
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
              "looking for the transcript in the transripts database",
              env.OPTION_NONE)

  # set the transcript name looked for.
  g_transcript = sys.argv[1]
  g_path = g_transcript.replace("::", "/")

  # display the transcript name and ask the user to continue.
  warning()

  # launch the recording.
  launch()

  # a final message.
  env.display(env.HEADER_OK, "session successfully transcripted",
              env.OPTION_NONE)

#
# ---------- entry point ------------------------------------------------------
#

if __name__ == "__main__":
  main()
