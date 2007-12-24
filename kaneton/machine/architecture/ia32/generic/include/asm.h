/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/misc/asm.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:50:38 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this is the place to put inline assembly macro-functions.
 *
 */

#ifndef ARCHITECTURE_ASM_H
#define ARCHITECTURE_ASM_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * asm macro functions
 */

#define		IRET()							\
  asm volatile("iret");

#define		LEAVE()							\
  asm volatile("leave");

#define		CLI()							\
  asm volatile("cli\n")

#define		STI()							\
  asm volatile("sti\n")

#define		STS()							\
  asm volatile("movl %%cr0, %%eax\n\t"					\
	       "orl $0x8, %%eax\n\t"					\
	       "movl %%eax, %%cr0"					\
	       :							\
	       :							\
	       : "%eax");

#define		CLTS()							\
  asm volatile("clts");


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

#define		LIDT(_var_)						\
  asm volatile("lidt %0\n"						\
	       :							\
	       : "m" (_var_))

#define		SIDT(_var_)						\
  asm volatile("sidt %0\n"						\
	       : "=m" (_var_)						\
	       :)

#define		LCR3(_var_)						\
  asm volatile("movl %0, %%eax\n"					\
	       "movl %%eax, %%cr3\n"					\
	       :							\
	       : "m" (_var_))

#define		SCR2(_var_)						\
  asm volatile("movl %%cr2, %%eax\n"					\
	       "movl %%eax, %0\n"					\
	       : "=m" (_var_)						\
	       :)

#define		SCR3(_var_)						\
  asm volatile("movl %%cr3, %%eax\n"					\
	       "movl %%eax, %0\n"					\
	       : "=m" (_var_)						\
	       :)

#define		LTR(_var_)						\
  asm volatile("ltr %0\n\t"						\
	       :							\
	       : "m" (_var_))

#define		HLT()							\
  asm volatile("hlt")

#define		WBINVD()						\
  asm volatile("wbinvd")

#define		FINIT()							\
  asm volatile("finit")

#define		FSAVE(_var_)						\
  asm volatile("fsave %0"						\
	       :							\
	       : "m" (_var_))

#define		FRSTOR(_var_)						\
  asm volatile("frstor %0"						\
	       :							\
	       : "m" (_var_))

#define		FXSAVE(_var_)						\
  asm volatile("fxsave %0"						\
	       :							\
	       : "m" (_var_))

#define		FXRSTOR(_var_)						\
  asm volatile("fxrstor %0"						\
	       :							\
	       : "m" (_var_))

#define		CPUID(_var_,_eax_,_ebx_,_ecx_,_edx_)			\
  asm volatile("cpuid\n\t"						\
	       : "=a" (_eax_), "=b" (_ebx_), "=c" (_ecx_), "=d" (_edx_)	\
	       : "a" (_var_))

/*
 * pio macro functions
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

/*
 * aliases for platform.
 */

#define ARCHITECTURE_IN_8	INB
#define ARCHITECTURE_IN_16	INW
#define ARCHITECTURE_IN_32	INL
#define ARCHITECTURE_OUT_8	OUTB
#define ARCHITECTURE_OUT_16	OUTW
#define ARCHITECTURE_OUT_32	OUTL

#endif
