#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/enguerrand/...t/profile/host/linux/ia32.mips64/util.py
#
# created       enguerrand raymond   [fri apr 17 20:59:04 2009]
# updated       enguerrand raymond   [fri apr 17 21:00:47 2009]
#

#
# ---------- information ------------------------------------------------------
#
# this file implements some useful functions used for mips compilation, build
# and installation.
#



#
# binary_extract()
#
# this function extracts given sections (section names separate by space) 
# from elf to put in binary
#
def			binary_extract(elf, sections, binary):
  section_list = sections.split()
  cmd_option = "-S"

  for section in section_list:
    cmd_option += " -j " + section

  cmd_option += " --output-target binary " + elf + " " + binary

  launch(_OBJCOPY_, cmd_option, "")



#
# concat_file()
#
# this function concatenates file a the result file end
#
def			concat_file(file, result):
  os.system(_CAT_ + " " + file + " >> " + result)
