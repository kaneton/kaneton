##
## licence       kaneton licence
##
## project       forest
##
## file          /home/mycure/norme/forest/styles/c/epita2006/classes.py
##
## created       julien quintard   [tue sep  5 22:39:34 2006]
## updated       julien quintard   [sat sep 16 14:43:49 2006]
##

#
# ----- description -----------------------------------------------------------
#
# this file contains the main checker's class definitions.
#

#
# ----- classes ---------------------------------------------------------------
#

#
# epita
#

class c_epita:
  def __init__(self): # e
    self.parser = None
    self.options = None
    self.pure = ""
    self.detail = ""
    self.count = 0



#
# error
#

class c_error:
  def __init__(self, c): # error
    self.code = c
    self.nerrors = 0
    self.pure = ""
    self.detail = ""
    self.count = 0
