##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/forest.py
##
## created       julien quintard   [sun jul 30 15:35:02 2006]
## updated       julien quintard   [sat sep 16 13:17:51 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file contains the main routines of the forest program.
#
# notice that the 'options()' function calls the 'options()' function
# of the parsers module and styles module. this action is performed
# to allow the parser and the style to use additional options.
#
# the 'f' variable represents the main forest variable containing everything
# necessary for the program.
#

#
# ----- informations ----------------------------------------------------------
#

VERSION = "1.0"
AUTHOR = "mycure {julien.quintard@gmail.com}"
CONTRIBUTORS = "Pwipwi {christophe.eymard@gmail.com}"

#
# ----- imports ---------------------------------------------------------------
#

from optparse import OptionParser

import sys
import re

import parsers
import styles

import traceback

#
# ----- classes ---------------------------------------------------------------
#

#
# forest
#

class c_forest:
  def __init__(self):
    self.program = None
    self.arguments = None
    self.options = None
    self.parser = None
    self.style = None
    self.files = None
    self.pure = ""
    self.detail = ""
    self.count = 0

#
# ----- functions -------------------------------------------------------------
#

#
# error()
#
def		error(f, string):
  sys.stderr.write("usage: forest.py [options]\n"
                   "\n"
                   "forest.py: " + string + "\n")
  sys.exit(42)



#
# summary()
#
def		summary(f):
  x = None

  if f.options.output_detail:
    x = f.detail

    f.detail = ""

    f.detail += "<forest version='" + VERSION + "' author='" +		\
                AUTHOR + "' contributors='" + CONTRIBUTORS + "'>\n"

    if x:
      f.detail += "  <files>\n"
      f.detail += x
      f.detail += "  </files>\n"

    f.detail += "  <summary errors='" + str(f.count) + "'/>\n"
    f.detail += "</forest>\n"

  if f.options.output_pure:
    x = f.pure

    f.pure = ""

    if x:
      f.pure += x
    f.pure += "[summary] " + str(f.count) + " errors.\n"



#
# output()
#
def		output(f):
  handle = None

  sys.stdout.write(f.pure)

  if f.options.output:
    try:
      handle = open(f.options.output, "w")
    except IOError:
      error(f, "error: unable to open the file \"" + f.options.output + "\".")

    handle.write(f.detail)
  else:
    sys.stdout.write(f.detail)



#
# process()
#
def		process(f, file):
  detail = None
  pure = None
  count = None
  p = None

  try:
    p = parsers.parse(f.parser, file, f.options)
  except parsers.exceptions.ParserError:
    error(f, "error: unable to parse the file \"" + file + "\".")

  try:
    pure, detail, count = styles.check(f.parser, f.style, p, f.options)
  except:
    traceback.print_exc()
    error(f, "error: FATAL !!")

  if pure or detail:
    f.pure += pure
    f.detail += detail
    f.count += count



#
# options()
#
s_parser = "--parser(?:=| +)([a-zA-Z0-9_]*)"
e_parser = re.compile(s_parser)

s_style = "--style(?:=| +)([a-zA-Z0-9_]*)"
e_style = re.compile(s_style)

def		options(f, arguments):
  optparse = OptionParser()
  options = None
  match = None
  o = None

  match = e_parser.search(" ".join(arguments))
  if match:
    f.parser = match.group(1)

  match = e_style.search(" ".join(arguments))
  if match:
    f.style = match.group(1)

  optparse.add_option("--parser",
                      action="store",
                      dest="style",
                      metavar="PARSER",
                      type="string",
                      help="parser")

  optparse.add_option("--style",
                      action="store",
                      dest="style",
                      metavar="STYLE",
                      type="string",
                      help="checking style")

  optparse.add_option("--output",
                      action="store",
                      dest="output",
                       metavar="FILE",
                      type="string",
                      help="output the detailed summary in the file FILE")

  optparse.add_option("--output-pure",
                      action="store_true",
                      dest="output_pure",
                      help="output a pure summary")

  optparse.add_option("--output-detail",
                      action="store_true",
                      dest="output_detail",
                      help="output a detailed summary")

  o = parsers.options(f.parser, optparse)
  if o:
    optparse = o

  o = styles.options(f.parser, f.style, optparse)
  if o:
    optparse = o

  options, arguments = optparse.parse_args(arguments)

  return options, arguments



#
# main()
#
def		main():
  f = c_forest()
  optparse = None
  file = None

  f.program = sys.argv[0]
  f.arguments = sys.argv[1:]

  f.options, f.files = options(f, f.arguments)

  if not f.parser:
    error(f, "error: please specify a parser.")

  if not f.files:
    error(f, "error: please specify at least one file.")

  if f.style and (not f.options.output_pure and not f.options.output_detail):
    error(f, "error: please specify an output format: pure or detailed.")

  if not f.style and (f.options.output_pure or f.options.output_detail):
    error(f, "error: please specify a style.")

  for file in f.files:
    process(f, file)

  summary(f)

  output(f)



#
# banner()
#
def		banner():
  sys.stderr.write("\n" +						\
                   "-------[" +						\
                   " forest " +						\
                   "]" +						\
                   "-------------------------------" +			\
                   "-------------------------------" +			\
                   "\n\n")



#
# entry point
#
if __name__ == "__main__":
#   banner()
  main()
