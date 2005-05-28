/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * pio.h
 * 
 * path          /mnt/goinfre/projects/kaneton/src/include/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:24:13 2005   mycure
 * last update   Fri Feb 11 02:24:18 2005   mycure
 */

#ifndef IA32_PIO_H
#define IA32_PIO_H

#define outb(port, data)						\
  asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (data))

#define outw(port, data)						\
  asm volatile ("outw %%ax, %%dx" :: "d" (port), "a" (data))

#define outl(port, data)						\
  asm volatile ("outl %%eax, %%dx" :: "d" (port), "a" (data))

#define inb(port, data)							\
  asm volatile ("inb %%dx, %%al" : "=a" (data) : "d" (port))

#define inw(port, data)							\
  asm volatile ("inw %%dx, %%ax" : "=a" (data) : "d" (port))

#define inl(port, data)							\
  asm volatile ("inl %%dx, %%eax" : "=a" (data) : "d" (port))

#endif
