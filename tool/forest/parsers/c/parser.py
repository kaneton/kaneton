##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/c/parser.py
##
## created       julien quintard   [tue sep  5 14:33:36 2006]
## updated       julien quintard   [sat sep 16 12:55:44 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file is the main of the 'c' parser.
#
# this file calls the different subparts of the parser that aim in detecting
# specific syntaxic elements.
#

#
# ----- import ----------------------------------------------------------------
#

import sys
import re

import parsers

import exceptions

import tabulations
import strings
import characters
import comments
import macros
import functions
import prototypes
import types
import structures
import enumerations
import unions
import globals
import blanks

s_newlines = '( \n)*'
e_newlines = re.compile(s_newlines)


#
# ----- functions -------------------------------------------------------------
#

def parse_matching(str, begin_op, end_op):
  balance = 1
  i = 0

  try:
    while balance != 0 :
      if str[i] == begin_op:
        balance += 1
      elif str[i] == end_op:
        balance -= 1
      i += 1
  except IndexError:
    i -= 1

  return str[0:i]

#
# forge()
#
# this function forges a string of the length 'length'.
#
# the forged string's format depends on the type 'type' which can be
# a simple bare stream, a character, a string or a comment.
#
FORGE_STREAM = 0
FORGE_CHARACTER = 1
FORGE_STRING = 2
FORGE_COMMENT = 3

def		forge(type, original, length):
  string = None
  start = None
  end = None
  i = None

  string = ""

  if type == FORGE_CHARACTER:
    string += "\'"
    start = 1
    end = length - 1
  elif type == FORGE_STRING:
    string += "\""
    start = 1
    end = length - 1
  elif type == FORGE_COMMENT:
    string += "/*"
    start = 2
    end = length - 2
  elif type == FORGE_STREAM:
    start = 0
    end = length

  for i in range(start, end):
    if original:
      if original[i] == "\n":
        string += "\n"
      else:
        string += " "
    else:
      string += " "

  if type == FORGE_CHARACTER:
    string += "\'"
  elif type == FORGE_STRING:
    string += "\""
  elif type == FORGE_COMMENT:
    string += "*/"

  return string



#
# debug()
#
# this function dumps information on a specific debugging step depending
# on the current step and the forest's parser specific options.
#
DEBUG_NONE = 0
DEBUG_STRINGS = 1
DEBUG_CHARACTERS = 2
DEBUG_COMMENTS = 4
DEBUG_MACROS = 8
DEBUG_FUNCTIONS = 16
DEBUG_PROTOTYPES = 32
DEBUG_TYPES = 64
DEBUG_STRUCTURES = 128
DEBUG_ENUMERATIONS = 256
DEBUG_UNIONS = 1024
DEBUG_GLOBALS = 2048
DEBUG_BLANKS = 4096
DEBUG_CONTENTS = 8192

def		debug(p, step):
  title = None
  list = None
  l = None

  if step == DEBUG_STRINGS and p.options.parser_strings:
    title = "\n\t\t\t-----[ strings ]-----\n"
    list = p.strings
  elif step == DEBUG_CHARACTERS and p.options.parser_characters:
    title = "\n\t\t\t-----[ characters ]-----\n"
    list = p.characters
  elif step == DEBUG_COMMENTS and p.options.parser_comments:
    title = "\n\t\t\t-----[ comments ]-----\n"
    list = p.comments
  elif step == DEBUG_MACROS and p.options.parser_macros:
    title = "\n\t\t\t-----[ macros ]-----\n"
    list = p.macros
  elif step == DEBUG_FUNCTIONS and p.options.parser_functions:
    title = "\n\t\t\t-----[ functions ]-----\n"
    list = p.functions
  elif step == DEBUG_PROTOTYPES and p.options.parser_prototypes:
    title = "\n\t\t\t-----[ prototypes ]-----\n"
    list = p.prototypes
  elif step == DEBUG_TYPES and p.options.parser_types:
    title = "\n\t\t\t-----[ types ]-----\n"
    list = p.types
  elif step == DEBUG_STRUCTURES and p.options.parser_structures:
    title = "\n\t\t\t-----[ structures ]-----\n"
    list = p.structures
  elif step == DEBUG_ENUMERATIONS and p.options.parser_enumerations:
    title = "\n\t\t\t-----[ enumerations ]-----\n"
    list = p.enumerations
  elif step == DEBUG_UNIONS and p.options.parser_unions:
    title = "\n\t\t\t-----[ unions ]-----\n"
    list = p.unions
  elif step == DEBUG_GLOBALS and p.options.parser_globals:
    title = "\n\t\t\t-----[ globals ]-----\n"
    list = p.globals
  elif step == DEBUG_BLANKS and p.options.parser_blanks:
    title = "\n\t\t\t-----[ blanks ]-----\n"
    list = p.blanks

  if not list:
    return

  sys.stdout.write(title)

  sys.stdout.write("----------------------------------------"
                   "---------------------------------------\n")

  for l in list:
    sys.stdout.write(l + "\n")
    sys.stdout.write("----------------------------------------"
                     "---------------------------------------\n")

  if p.options.parser_contents:
    sys.stdout.write("\n\t\t\t-----[ contents ]-----\n")

    sys.stdout.write("########################################"
                     "#######################################\n")

    sys.stdout.write(p.contents + "\n")

    sys.stdout.write("########################################"
                     "#######################################\n")

  sys.stdout.write("\n\n\n\n\n")



#
# parse()
#
# this is the main function which calls the subparts of the parser.
#

import re

s_bothering = '\r'
e_bothering = re.compile(s_bothering)

def		parse(p):
  needless = None

  p.contents = tabulations.parse(p.contents)

  p.windows_style = None
#  if p.contents.find('\r'):
  if e_bothering.search(p.contents):
    p.contents = p.contents.replace('\r', '')
    p.windows_style = True

  p.old_contents = p.contents

  p.characters, p.contents = characters.parse(p.contents)

  debug(p, DEBUG_CHARACTERS)

  p.comments, p.contents = comments.parse(p.contents)

  debug(p, DEBUG_COMMENTS)

  p.strings, p.contents = strings.parse(p.contents)

  debug(p, DEBUG_STRINGS)

  p.macros, p.contents = macros.parse(p.contents)

  debug(p, DEBUG_MACROS)

  p.blanks, p.contents = blanks.parse(p.contents)

  debug(p, DEBUG_BLANKS)

  p.functions, p.contents = functions.parse(p.contents)

  debug(p, DEBUG_FUNCTIONS)

  p.prototypes, p.contents = prototypes.parse(p.contents)

  debug(p, DEBUG_PROTOTYPES)

  p.types, p.contents = types.parse(p.contents)

  debug(p, DEBUG_TYPES)

  p.globals, p.contents = globals.parse(p.contents)

  debug(p, DEBUG_GLOBALS)
