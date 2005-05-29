/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * pio.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:24:13 2005   mycure
 * last update   Sat May 28 22:22:17 2005   mycure
 */

#ifndef IA32_PIO_H
#define IA32_PIO_H		1

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
