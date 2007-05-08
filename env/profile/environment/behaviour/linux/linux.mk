#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ile/environment/behaviour/linux/linux.mk
#
# created       julien quintard   [tue may  8 13:03:34 2007]
# updated       julien quintard   [tue may  8 13:20:14 2007]
#

#
# ---------- traps ------------------------------------------------------------
#

%.o:			%.asm
	$(call assemble-asm,$@,$<,"--output-format elf")

%.o:			%.S
	$(call assemble-S,$@,$<,)

%.o:			%.c
	$(call compile-c,$@,$<,)

%.c:			%.l
	$(call lex-l,$@,$<,)
