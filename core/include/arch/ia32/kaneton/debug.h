/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * debug.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Tue Nov 15 19:22:27 2005   mycure
 */

#ifndef IA32_KANETON_DEBUG_H
#define IA32_KANETON_DEBUG_H	1

/*
 * ---------- defines ---------------------------------------------------------
 */

#define IA32_DEBUG_INIT		(1 << 0)
#define IA32_DEBUG_PMODE	(1 << 1)
#define IA32_DEBUG_PAGING	(1 << 2)

#define IA32_DEBUG							\
  (CONF_MACHDEP_DEBUG)

#endif
