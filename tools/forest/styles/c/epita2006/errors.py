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

ERRORS_WARNING = 0
ERRORS_STYLE = 1
ERRORS_FATAL = 100

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
    e.pure += error.pure
    e.detail += error.detail
    e.count += error.count



#
# open()
#
# this function initializes the error object 'error'.
#
def		open(e, error):
  code = None

  if e.options.output_detail:
    error.detail += "<node>\n"

    if error.code:
      error.detail += "  <code>\n"
      code = error.code
      code = code.replace("<", "&lt;")
      code = code.replace(">", "&gt;")
      error.detail += code
      error.detail += "  </code>\n"

    error.detail += "  <errors>\n"



#
# add()
#
# this function adds an error to the error object 'error'.
#
ERRORS_COUNT_WARNING = 0
ERRORS_COUNT_STYLE = 1
ERRORS_COUNT_FATAL = 100

def		add(e, error, type, description):
  try:
    fname = error.name
    if fname:
      description = "in function %s : " % fname + description
  except AttributeError:
    pass
  if e.options.output_detail and error:
    if type == ERRORS_FATAL:
      type_str = "fatal"
    elif type == ERRORS_STYLE:
      type_str = "style"
    elif type == ERRORS_WARNING:
      type_str = "warning"
    else:
      type_str = "other"
    error.detail += "    <error type='" + type_str + "'>\n"
    error.detail += description
    error.detail += "    </error>\n"

  if e.options.output_pure:
    error.pure += "[" + e.parser.file + "] " + description

  error.nerrors += 1

  error.count += type
#   if type == ERRORS_WARNING:
#     error.count += ERRORS_COUNT_WARNING
#   elif type == ERRORS_STYLE:
#     error.count += ERRORS_COUNT_STYLE
#   elif type == ERRORS_FATAL:
#     error.count += ERRORS_COUNT_FATAL



#
# close()
#
# this function adds the necessary to the error object 'error'.
#
def		close(e, error):
  if e.options.output_detail:
    error.detail += "  </errors>\n"
    error.detail += "</node>\n"



#
# summary()
#
# this function arranges the summary depending on the output option.
#
def		summary(e):
  x = None

  if e.options.output_detail and e.detail:
    x = e.detail

    e.detail = ""
    e.detail += "<file name='" + e.parser.file + "' style='" +		\
                informations.STYLE + "' version='" +			\
                informations.VERSION + "' author='" +			\
                informations.AUTHOR + "' contributors='" +		\
                informations.CONTRIBUTORS + "'>\n"
    e.detail += x
    e.detail += "  <summary errors='" + str(e.count) + "'/>\n"
    e.detail += "</file>\n"

  if e.options.output_pure and e.pure:
    x = e.pure

    # nothing to do

    e.pure = x
