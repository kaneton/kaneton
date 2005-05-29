/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * asm.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Sun May 29 12:45:01 2005   mycure
 */

#ifndef IA32_ASM_H
#define IA32_ASM_H		1

/*
 * asm defines
 */

#define		cli()							\
  asm("cli")

#define		sti()							\
  asm("sti")

/*
 * gcc defines
 */

#define		__PACKED__						\
  __attribute__ ((packed))

#define		__ALIGNED__(_value_)					\
  __attribute__ ((aligned (_value_)))

/*
 * pio macros
 */

#define		outb(port, data)					\
  asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (data))

#define		outw(port, data)					\
  asm volatile ("outw %%ax, %%dx" :: "d" (port), "a" (data))

#define		outl(port, data)					\
  asm volatile ("outl %%eax, %%dx" :: "d" (port), "a" (data))

#define		inb(port, data)						\
  asm volatile ("inb %%dx, %%al" : "=a" (data) : "d" (port))

#define		inw(port, data)						\
  asm volatile ("inw %%dx, %%ax" : "=a" (data) : "d" (port))

#define		inl(port, data)						\
  asm volatile ("inl %%dx, %%eax" : "=a" (data) : "d" (port))

#endif
