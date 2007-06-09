##
## comments.py for forest in /home/chris
##
## Made by Christophe Eymard
## Login   <eymard_c@epita.fr>
##
## Started on  Mon Oct 16 21:32:00 2006 Christophe Eymard
## Last update Sat Nov 11 00:06:14 2006 Christophe Eymard
##


## Sorry mycure, but the std-file-header was messing with this file
## somehow :)
##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/comments.py
##
## created       julien quintard   [tue sep  5 23:13:37 2006]
## updated       julien quintard   [sat sep 16 17:22:00 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file checks the correctness of the comments.
#
# the 'e' variable represents the 'c_epita' checker main variable, the 'z'
# variable contains everything necessary for the comments checking while
# the 'p' variable describes the current comment checking process.
#

#
# ----- imports ---------------------------------------------------------------
#

import re

import checker
import classes
import errors

#
# ----- classes ---------------------------------------------------------------
#

#
# comments
#

class c_comments:
  def __init__(self):
    self.needless = None

#
# comment
#

class c_comment:
  def __init__(self):
    self.needless = None

#
# ----- regexp ----------------------------------------------------------------
#

#
# header
#

s_header = "^"								\
           "/\* *\n"							\
           "\*\* .* for .+ in .+\n"		\
           "\*\* *\n"						\
           "\*\* Made by .+\n"					\
           "\*\* Login .+\n"					\
           "\*\* *\n"						\
           "\*\* Started on .+\n"				\
           "\*\* Last update .+\n"                              \
           "\*/"

e_header = re.compile("(" + s_header + ")",
                      re.MULTILINE)

#
# comment
#

# s_comment = " *"							\
#             "(?:"							\
#               "(?:"							\
#                 "/\*(|\*|!)\n"							\
#                 "(?:"							\
#                   "\*\*(?:.*)\n"					\
#                 ")+"							\
#                 "\*/"							\
#               ")"							\
#                 "|"							\
#               "(?:"							\
#                 "/\*(?:[^*\n]|(?:\*+(?:[^/\n])))*\*/"			\
#               ")"							\
#             ")"

s_comment = ".*?" \
            "(" \
            "/\*(!|\*)? *\n" \
            "(\*\*.*?\n)*" \
            "\*/" \
            "|" \
            "/\*[^\n]*?\*/" \
            ")"

e_comment = re.compile(s_comment, re.MULTILINE)

#
# ----- functions -------------------------------------------------------------
#

#
# options()
#
# this function adds comments checking specific options.
#
def		options(group):
  group.add_option("--epita-comments-exclude",
                   action="append",
                   dest="epita_comments_exclude",
                   metavar="PATTERN",
                   help="exclude comments matching PATTERN from the "
                        "style checking")



#
# header()
#
# this function checks the correctness of the EPITA header.
#
def		header(e, z, i):
  error = classes.c_error(i)
  match = None

  errors.open(e, error)

  if not error.code:
    errors.add(e, error, errors.ERRORS_STYLE,
               "the file does not contains any header.\n")
  else:
    match = e_header.search(error.code)
    if not match:
      errors.add(e, error, errors.ERRORS_STYLE,
                 "the file header is not properly formatted.\n")

    checker.columns(e, error)

  errors.close(e, error)

  errors.commit(e, error)



#
# parse()
#
# this function verify the correctness of the comment format.
#
def		parse(e, z, error):
  p = c_comment()
  match = None

  match = e_comment.search(error.code)

  if not match:
    errors.add(e, error, errors.ERRORS_STYLE,
               "the comment is not properly formatted.\n")
    return None

  return p



#
# comment()
#
# this function launches every test on a given comment.
#
def		comment(e, z, i):
  error = classes.c_error(i)
  p = None

  errors.open(e, error)

  p = parse(e, z, error)
  if not p:
    errors.close(e, error)
    errors.commit(e, error)
    return

  errors.close(e, error)

  errors.commit(e, error)



#
# check()
#
# the main function which checks each comment of the list.
#
def		check(e):
  z = c_comments()
  launch = None
  i = None
  x = None

  if e.parser.comments:
    header(e, z, e.parser.comments[0])
  else:
    header(e, z, None)

  for i in range(1, len(e.parser.comments)):
    launch = 1

    if e.options.epita_comments_exclude:
      for x in e.options.epita_comments_exclude:
        if re.search(x, e.parser.comments[i], re.MULTILINE):
          launch = 0
          break

    if launch:
      comment(e, z, e.parser.comments[i])
