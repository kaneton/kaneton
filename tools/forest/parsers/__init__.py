##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/__init__.py
##
## created       julien quintard   [tue sep  5 22:27:41 2006]
## updated       julien quintard   [sat sep 16 11:58:08 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file is called every time the module is imported.
#
# this file is just a wrapper over the differents parsers.
#

#
# ----- imports ---------------------------------------------------------------
#

import fnmatch

import exceptions
import classes

import c

#
# ----- interface -------------------------------------------------------------
#

#
# type()
#
# this function determines the file type.
#
TYPE_UNKNOWN = 0
TYPE_SOURCE = 1
TYPE_HEADER = 2

def		type(p):
  if fnmatch.fnmatch(p.file, "*.c"):
    p.type = TYPE_SOURCE
  elif fnmatch.fnmatch(p.file, "*.h"):
    p.type = TYPE_HEADER
  else:
    p.type = TYPE_UNKNOWN



#
# load()
#
# this function just reads the file's contents and stores it in
# the string 'contents'.
#
def		load(file):
  contents = None
  handle = None
  line = None

  try:
    handle = open(file, "r")
  except IOError:
    raise exceptions.ParserError

  contents = ""

  for line in handle.readlines():
    contents += line

  handle.close()

  return contents



#
# options()
#
# this function just forwards the call to the underlying parser.
#
def		options(parser, optparse):
  if not parser:
    return

  if (parser == "c"):
    return c.options(optparse)



#
# parse()
#
# this function builds an instance of the 'c_parser' class and then forwards
# the call to the underlying parser.
#
def		parse(parser, file, options):
  p = classes.c_parser(file)

  if not parser:
    return p

  type(p)

  p.options = options

  p.contents = load(file)

  if (parser == "c"):
    c.parse(p, options)

  return p
