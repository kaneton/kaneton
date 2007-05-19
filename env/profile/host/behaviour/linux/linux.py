#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ile/environment/behaviour/linux/linux.py
#
# created       julien quintard   [tue may  8 13:20:21 2007]
# updated       julien quintard   [sun may 13 13:49:08 2007]
#

#
# ---------- information ------------------------------------------------------
#
# this file implements the remaining functions of the kaneton python interface.
#
# note that the behaviour profile already provides many functions. these
# functions can be overriden but you will probably just use them.
#
# in addition, the behaviour profile already imports some packages.
#

#
# ---------- functions --------------------------------------------------------
#

#
# colorize()
#
# this function returns a colorized text if the environment is configured
# to or simply the original text.
#
# note that this function implementation is based on UNIX escape sequences.
#
def			colorize(text, color, options):
  if _DISPLAY_ == _DISPLAY_UNCOLORED_:
    return text

  if options & OPTION_FLICKERING:
    text = "[01;05m" + text + "[39;49;00m"

  if color == COLOR_RED:
    text = "[31;01m" + text + "[39;49;00m"
  elif color == COLOR_GREEN:
    text = "[32;01m" + text + "[39;49;00m"
  elif color == COLOR_YELLOW:
    text = "[33;01m" + text + "[39;49;00m"
  elif color == COLOR_BLUE:
    text = "[34;01m" + text + "[39;49;00m"
  elif color == COLOR_WHITE:
    text = "[37;01m" + text + "[39;49;00m"

  return text



#
# launch()
#
# this function launch a new program/script/make etc..
#
def			launch(file, arguments, options):
  directory = None
  info = None
  status = 0
  wd = None

  info = os.path.split(file)

  directory = info[0]
  file = info[1]

  if directory:
    wd = cwd(OPTION_NONE)
    cd(directory, OPTION_NONE)

  if re.match("^.*\.sh$", file):
    status = os.system(_SHELL_ + " " + file + " " + arguments)
  elif re.match("^.*\.py$", file):
    os.putenv("PYTHONPATH", _PYTHON_INCLUDE_DIR_)
    status = os.system(_PYTHON_ + " " + file + " " + arguments)
  elif re.match("^.*\.pl$", file):
    status = os.system(_PERL_ + " " + file + " " + arguments)
  elif re.match("^Makefile$", file):
    status = os.system(_MAKE_ + " -f " + file + " " + arguments)
  else:
    status = os.system(file + " " + arguments)

  if directory:
    cd(wd, OPTION_NONE)

  return status



#
# pack()
#
# this function creates an archive of the given directory.
#
def			pack(directory, file):
  launch(_TAR_, "-czf " + file + " " + directory, OPTION_NONE)



#
# unpack()
#
# this function unpackages an archive into the given (optional) directory.
#
def			unpack(file, directory):
  if directory:
    launch(_TAR_, "-xzf " + file + " -C " + directory, OPTION_NONE)
  else:
    launch(_TAR_, "-xzf " + file, OPTION_NONE)



#
# load()
#
# this function copies a file on a device, this device can be virtual:
# an image.
#
def			load(file, device, path, options):
  if options == OPTION_DEVICE:
    launch(_MCOPY_, "-o -n " + file + " " + device + path, OPTION_NONE)

  if options == OPTION_IMAGE:
    launch(_MCOPY_, "-o -n " + "-i" + device + " " +
           file + " ::/" + path, OPTION_NONE)



#
# record()
#
# this function runs the program recording a session.
#
def			record(log, time):
  launch(_SCRIPT_, "-q -t " + log + " -c " +
         _TRANSCRIPTS_CMD_ + " 2> " + time, OPTION_NONE)



#
# play()
#
# this function runs the program replaying a session.
#
def			play(log, time):
  launch(_SCRIPTREPLAY_TOOL_, time + " " + log, OPTION_NONE)



#
# locate()
#
# this function tries to locate a program on the system.
#
def			locate(file, options):
  return launch(_WHICH_, file + " 1>/dev/null 2>/dev/null", OPTION_NONE)