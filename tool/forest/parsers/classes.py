##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/parsers/classes.py
##
## created       julien quintard   [tue sep  5 14:56:08 2006]
## updated       julien quintard   [sat sep 16 11:54:49 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file contains the definition of the general parsers classes.
#
# an instance of the class 'c_parser' is returned by the 'parse()'
# function of the __init__.py file.
#

#
# ----- classes ---------------------------------------------------------------
#

#
# parser
#

class c_parser:
  def __init__(self, file):
    self.file = file
    self.type = None
    self.options = None
    self.characters = None
    self.strings = None
    self.comments = None
    self.externs = None
    self.functions = None
    self.prototypes = None
    self.types = None
    self.structures = None
    self.enumerations = None
    self.unions = None
    self.macros = None
    self.globals = None
    self.blanks = None
    self.contents = None
