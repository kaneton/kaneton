/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/educational/include/asm.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sun jan 16 08:49:45 2011]
 */

#ifndef ARCHITECTURE_ASM_H
#define ARCHITECTURE_ASM_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-functions provide an easy way to perform some hardware
 * operations.
 */

#define		ARCHITECTURE_IRET()					\
  asm volatile ("iret");

#define		ARCHITECTURE_LEAVE()					\
  asm volatile ("leave");

#define		ARCHITECTURE_CLI()					\
  asm volatile ("cli")

#define		ARCHITECTURE_STI()					\
  asm volatile ("sti")

#define		ARCHITECTURE_LGDT(_var_)				\
  asm volatile ("lgdt %0"						\
	       :							\
	       : "m" (_var_))

#define		ARCHITECTURE_SGDT(_var_)				\
  asm volatile ("sgdt %0"						\
	       : "=m" (_var_)						\
	       :)

#define		ARCHITECTURE_LIDT(_var_)				\
  asm volatile ("lidt %0"						\
	       :							\
	       : "m" (_var_))

#define		ARCHITECTURE_SIDT(_var_)				\
  asm volatile ("sidt %0"						\
	       : "=m" (_var_)						\
	       :)

#define		ARCHITECTURE_LCR0(_var_)				\
  asm volatile ("movl %0, %%eax\n"					\
		"movl %%eax, %%cr0"					\
	       :							\
	       : "m" (_var_))

#define		ARCHITECTURE_SCR0(_var_)				\
  asm volatile ("movl %%cr0, %%eax\n"					\
		"movl %%eax, %0"					\
	       : "=m" (_var_)						\
	       :)

#define		ARCHITECTURE_LCR1(_var_)				\
  asm volatile ("movl %0, %%eax\n"					\
		"movl %%eax, %%cr1"					\
	       :							\
	       : "m" (_var_))

#define		ARCHITECTURE_SCR1(_var_)				\
  asm volatile ("movl %%cr1, %%eax\n"					\
		"movl %%eax, %0"					\
	       : "=m" (_var_)						\
	       :)

#define		ARCHITECTURE_LCR2(_var_)				\
  asm volatile ("movl %0, %%eax\n"					\
		"movl %%eax, %%cr2"					\
	       :							\
	       : "m" (_var_))

#define		ARCHITECTURE_SCR2(_var_)				\
  asm volatile ("movl %%cr2, %%eax\n"					\
		"movl %%eax, %0"					\
	       : "=m" (_var_)						\
	       :)

#define		ARCHITECTURE_LCR3(_var_)				\
  asm volatile ("movl %0, %%eax\n"					\
		"movl %%eax, %%cr3"					\
	       :							\
	       : "m" (_var_))

#define		ARCHITECTURE_SCR3(_var_)				\
  asm volatile ("movl %%cr3, %%eax\n"					\
		"movl %%eax, %0"					\
	       : "=m" (_var_)						\
	       :)

#define		ARCHITECTURE_LTR(_var_)					\
  asm volatile ("ltr %0"						\
	       :							\
	       : "m" (_var_))

#define		ARCHITECTURE_HLT()					\
  asm volatile ("hlt")

#define		ARCHITECTURE_OUTB(_port_, _data_)			\
  asm volatile ("outb %%al, %%dx"					\
	       :							\
	       : "d" ((t_uint16)(_port_)), "a" (_data_))

#define		ARCHITECTURE_OUTW(_port_, _data_)			\
  asm volatile ("outw %%ax, %%dx"					\
	       :							\
	       : "d" ((t_uint16)(_port_)), "a" (_data_))

#define		ARCHITECTURE_OUTL(_port_, _data_)			\
  asm volatile ("outl %%eax, %%dx"					\
	       :							\
	       : "d" ((t_uint16)(_port_)), "a" (_data_))

#define		ARCHITECTURE_INB(_port_, _data_)			\
  asm volatile ("inb %%dx, %%al"					\
	       : "=a" (_data_)						\
	       : "d" ((t_uint16)(_port_)))

#define		ARCHITECTURE_INW(_port_, _data_)			\
  asm volatile ("inw %%dx, %%ax"					\
	       : "=a" (_data_)						\
	       : "d" ((t_uint16)(_port_)))

#define		ARCHITECTURE_INL(_port_, _data_)			\
  asm volatile ("inl %%dx, %%eax"					\
	       : "=a" (_data_)						\
	       : "d" ((t_uint16)(_port_)))

#define		ARCHITECTURE_INT(_int_)					\
  asm volatile ("int %0"						\
	       :							\
	       : "i" (_int_))

#define		ARCHITECTURE_INT3()					\
  asm volatile ("int3")

#endif
