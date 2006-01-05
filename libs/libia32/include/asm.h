/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/arch/libia32/asm.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [thu dec 22 18:37:51 2005]
 */

#ifndef IA32_IA32_ASM_H
#define IA32_IA32_ASM_H		1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * asm macros
 */

#define		CLI()							\
  asm volatile("cli\n"							\
	       ::)

#define		STI()							\
  asm volatile("sti\n"							\
	       ::)

#define		LGDT(_var_)						\
  asm volatile("lgdt %0\n"						\
	       :							\
	       : "m" (_var_))

#define		SGDT(_var_)						\
  asm volatile("sgdt %0\n"						\
	       : "=m" (_var_)						\
	       :)

#define		LLDT(_var_)						\
  asm volatile("lldt %0\n"						\
	       :							\
	       : "m" (_var_))

#define		SLDT(_var_)						\
  asm volatile("sldt %0\n"						\
	       : "=m" (_var_)						\
	       :)

#define		LCR3(_var_)						\
  asm volatile("movl %0, %%eax\n"					\
	       "movl %%eax, %%cr3\n"					\
	       :							\
	       : "m" (_var_))

#define		SCR3(_var_)						\
  asm volatile("movl %%cr3, %%eax\n"					\
	       "movl %%eax, %0\n"					\
	       : "=m" (_var_)						\
	       :)

/*
 * pio macros
 */

#define		OUTB(_port_, _data_)					\
  asm volatile("outb %%al, %%dx\n"					\
	       :							\
	       : "d" (_port_), "a" (_data_))

#define		OUTW(_port_, _data_)					\
  asm volatile("outw %%ax, %%dx\n"					\
	       :							\
	       : "d" (_port_), "a" (_data_))

#define		OUTL(_port_, _data_)					\
  asm volatile("outl %%eax, %%dx\n"					\
	       :							\
	       : "d" (_port_), "a" (_data_))

#define		INB(_port_, _data_)					\
  asm volatile("inb %%dx, %%al\n"					\
	       : "=a" (_data_)						\
	       : "d" (_port_))

#define		INW(_port_, _data_)					\
  asm volatile("inw %%dx, %%ax\n"					\
	       : "=a" (_data_)						\
	       : "d" (_port_))

#define		INL(_port_, _data_)					\
  asm volatile("inl %%dx, %%eax\n"					\
	       : "=a" (_data_)						\
	       : "d" (_port_))

#endif
