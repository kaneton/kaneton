#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/environment/profile/host/linux/linux.py
#
# created       julien quintard   [tue may  8 13:20:21 2007]
# updated       julien quintard   [sat mar  5 09:13:35 2011]
#

#
# ---------- information ------------------------------------------------------
#
# this file implements the remaining functions of the kaneton python interface.
#
# note that the host profile already provides many functions. these
# functions can be overriden but you will probably just use them.
#
# in addition, the host profile already imports some packages.
#

#
# ---------- lang -------------------------------------------------------------
#
# incorrect locales may prevent the system from compiling properly.
#
# the following forces the LANG environment variable to US.
#
os.putenv("LANG", "US")

#
# ---------- python path ------------------------------------------------------
#

_pythonpath_ = os.getenv("PYTHONPATH")

if not _pythonpath_:
  os.putenv("PYTHONPATH", _PYTHON_INCLUDE_DIR_)
else:
  os.putenv("PYTHONPATH", _pythonpath_ + ":" + _PYTHON_INCLUDE_DIR_)

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
    text = "[05m" + text
  if options & OPTION_BOLD:
    text = "[01m" + text

  if color == COLOR_BLACK:
    text = "[30m" + text
  elif color == COLOR_RED:
    text = "[31m" + text
  elif color == COLOR_GREEN:
    text = "[32m" + text
  elif color == COLOR_YELLOW:
    text = "[33m" + text
  elif color == COLOR_BLUE:
    text = "[34m" + text
  elif color == COLOR_MAGENTA:
    text = "[35m" + text
  elif color == COLOR_CYAN:
    text = "[36m" + text
  elif color == COLOR_WHITE:
    text = "[37m" + text

  return text + "[39;49;00m"



#
# launch()
#
# this function launch a new program/script/make etc.
#
def			launch(file, arguments, options):
  directory = None
  info = None
  status = 0
  wd = None

  if options & OPTION_QUIET:
    output = " >/dev/null 2>&1"
  else:
    output = ""

  info = os.path.split(file)

  directory = info[0]
  file = info[1]

  if directory:
    wd = cwd(OPTION_NONE)
    cd(directory, OPTION_NONE)

  if re.match("^.*\.sh$", file):
    status = os.system(_SHELL_ + " " + file + " " + arguments + output)
  elif re.match("^.*\.py$", file):
    status = os.system(_PYTHON_ + " " + file + " " + arguments + output)
  elif re.match("^.*\.pl$", file):
    status = os.system(_PERL_ + " " + file + " " + arguments + output)
  elif re.match("^Makefile$", file):
    status = os.system(_MAKE_ + " -f " + file + " " + arguments + output)
  else:
    if directory:
      file = "./" + file

    status = os.system(file + " " + arguments + output)

  if directory:
    cd(wd, OPTION_NONE)

  return status



#
# pack()
#
# this function creates an archive of the given directory.
#
def			pack(directory, file, options):
  launch(_TAR_, "-cjf " + file + " " + directory, OPTION_NONE)



#
# unpack()
#
# this function unpackages an archive into the given (optional) directory.
#
def			unpack(file, directory, options):
  if directory:
    launch(_TAR_, "-xjf " + file + " -C " + directory, OPTION_NONE)
  else:
    launch(_TAR_, "-xjf " + file, OPTION_NONE)



#
# load()
#
# this function copies a file on a device, this device can be virtual:
# an image.
#
def			load(file, device, path, options):
  if options == OPTION_DEVICE:
    return launch(_MCOPY_, "-o -n " + file + " " + device + path,
                  OPTION_NONE)
  elif options == OPTION_IMAGE:
    return launch(_MCOPY_, "-o -n " + "-i" + device + " " +
                  file + " ::" + path, OPTION_NONE)



#
# record()
#
# this function runs the program recording a session.
#
def			record(transcript, options):
  directory = None
  time = None
  log = None
  tmp = None
  wd = None

  tmp = temporary(OPTION_DIRECTORY)

  directory = tmp + "/" + "transcript"

  log = directory + "/" + "log"
  time = directory + "/" + "time"

  mkdir(directory, OPTION_NONE)

  launch(_SCRIPT_, "-q -t " + log + " -c " +
         _TRANSCRIPT_CMD_ + " 2> " + time, OPTION_NONE)

  wd = cwd(OPTION_NONE)

  cd(tmp, OPTION_NONE)

  pack("transcript", wd + "/" + transcript, OPTION_NONE)

  cd(wd, OPTION_NONE)

  remove(tmp, OPTION_NONE)



#
# play()
#
# this function runs the program replaying a session.
#
def			play(transcript, options):
  directory = None
  time = None
  log = None
  tmp = None
  wd = None

  tmp = temporary(OPTION_DIRECTORY)

  log = tmp + "/" + "transcript/log"
  time = tmp + "/" + "transcript/time"

  unpack(transcript, tmp, OPTION_NONE)

  launch(_REPLAY_TOOL_, time + " " + log, OPTION_NONE)

  remove(tmp, OPTION_NONE)



#
# locate()
#
# this function tries to locate a program on the system.
#
def			locate(file, options):
  return launch(_WHICH_, file + " 1>/dev/null 2>/dev/null", OPTION_NONE)
