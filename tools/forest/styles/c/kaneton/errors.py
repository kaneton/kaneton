##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/errors.py
##
## created       julien quintard   [thu sep  7 04:22:02 2006]
## updated       julien quintard   [sat sep 16 18:51:44 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file contains everything for the errors handling.
#

#
# ----- imports ---------------------------------------------------------------
#

import informations

#
# ----- globals ---------------------------------------------------------------
#

ERRORS_WARNING = "warning"
ERRORS_STYLE = "style"
ERRORS_FATAL = "fatal"

#
# ----- functions -------------------------------------------------------------
#

#
# commit()
#
# this function adds the error 'error' to the current error summary.
#
def		commit(e, error):
  if error.nerrors != 0:
    e.summary += error.summary
    e.count += error.count



#
# open()
#
# this function initializes the error object 'error'.
#
def		open(e, error):
  code = None

  if e.options.output_detail:
    error.summary += "<node>\n"

    if error.code:
      error.summary += "  <code>\n"
      code = error.code
      code = code.replace("<", "&lt;")
      code = code.replace(">", "&gt;")
      error.summary += code
      error.summary += "  </code>\n"

    error.summary += "  <errors>\n"



#
# add()
#
# this function adds an error to the error object 'error'.
#
ERRORS_COUNT_WARNING = 0
ERRORS_COUNT_STYLE = 1
ERRORS_COUNT_FATAL = 3

def		add(e, error, type, description):
  if e.options.output_detail:
    error.summary += "    <error type='" + type + "'>\n"
    error.summary += description
    error.summary += "    </error>\n"

  if e.options.output_pure:
    error.summary += "[" + e.parser.file + "] " + description

  error.nerrors += 1

  if type == ERRORS_WARNING:
    error.count += ERRORS_COUNT_WARNING
  elif type == ERRORS_STYLE:
    error.count += ERRORS_COUNT_STYLE
  elif type == ERRORS_FATAL:
    error.count += ERRORS_COUNT_FATAL



#
# close()
#
# this function adds the necessary to the error object 'error'.
#
def		close(e, error):
  if e.options.output_detail:
    error.summary += "  </errors>\n"
    error.summary += "</node>\n"



#
# summary()
#
# this function arranges the summary depending on the output option.
#
def		summary(e):
  x = None

  if not e.summary:
    return

  if e.options.output_detail:
    x = e.summary

    e.summary = ""
    e.summary += "<file name='" + e.parser.file + "' style='" +		\
                 informations.STYLE + "' version='" +			\
                 informations.VERSION + "' author='" +			\
                 informations.AUTHOR + "' contributors='" +		\
                 informations.CONTRIBUTORS + "'>\n"
    e.summary += x
    e.summary += "  <summary errors='" + str(e.count) + "'/>\n"
    e.summary += "</file>\n"

  if e.options.output_pure:
    x = e.summary

    # nothing to do

    e.summary = x
