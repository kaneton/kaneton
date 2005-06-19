/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * asm.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Sun Jun 19 14:09:42 2005   mycure
 */

#ifndef IA32_IA32_ASM_H
#define IA32_IA32_ASM_H		1

/*
 * asm macros
 */

#define		CLI()							\
  asm volatile ("cli\n"							\
		::)

#define		STI()							\
  asm volatile ("sti\n"							\
		::)

#define		LGDT(_var_)						\
  asm volatile ("lgdt %0\n"						\
		:							\
		: "m" (_var_))

#define		SGDT(_var_)						\
  asm volatile ("sgdt %0\n"						\
		: "=m" (_var_)						\
		:)

#define		LCR3(_var_)						\
  asm volatile ("movl %0, %%eax\n"					\
		"movl %%eax, %%cr3\n"					\
		:							\
		: "m" (_var_))

#define		SCR3(_var_)						\
  asm volatile ("movl %%cr3, %%eax\n"					\
		"movl %%eax, %0\n"					\
		: "=m" (_var_)						\
		:)

/*
 * pio macros
 */

#define		OUTB(_port_, _data_)					\
  asm volatile ("outb %%al, %%dx\n"					\
		:							\
		: "d" (_port_), "a" (_data_))

#define		OUTW(_port_, _data_)					\
  asm volatile ("outw %%ax, %%dx\n"					\
		:							\
		: "d" (_port_), "a" (_data_))

#define		OUTL(_port_, _data_)					\
  asm volatile ("outl %%eax, %%dx\n"					\
		:							\
		: "d" (_port_), "a" (_data_))

#define		INB(_port_, _data_)					\
  asm volatile ("inb %%dx, %%al\n"					\
		: "=a" (_data_)						\
		: "d" (_port_))

#define		INW(_port_, _data_)					\
  asm volatile ("inw %%dx, %%ax\n"					\
		: "=a" (_data_)						\
		: "d" (_port_))

#define		INL(_port_, _data_)					\
  asm volatile ("inl %%dx, %%eax\n"					\
		: "=a" (_data_)						\
		: "d" (_port_))

#endif
