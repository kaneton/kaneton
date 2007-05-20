#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/julian/kaneton/kaneton/test/compile.py
#
# created       julian pidancet   [thu mar 22 20:56:38 2007]
# updated       julian pidancet   [wed mar 21 20:56:38 2007]
#

#
# ---------- imports ----------------------------------------------------------
#

import re
from parse import *

#
# ---------- functions --------------------------------------------------------
#

def			build_obj_list(data):
  source_list = get_source_list(data)
  obj_list = []
  for i in source_list:
    repl = re.subn('.c$', '.o', i)
    if repl[1] == 0:
      raise TestYamlError, 'File is not a C source file'
    obj_list.append(repl[0])
  return obj_list

#
# ---------- code -------------------------------------------------------------
#

output = open('.test.mk', 'w')
test_data = get_test_data()
obj_list = build_obj_list(test_data)
output.write('OBJS = ')
if len(obj_list) > 0:
  output.write(obj_list[0])
  for i in range(1, len(obj_list)):
    output.write('\\\n')
    output.write(obj_list[i])
output.write('\n\n')
output.write('compile: $(OBJS)\n')
output.write('\t$(launch archive,$(_TEST_LO_),$(OBJS),)')

  
