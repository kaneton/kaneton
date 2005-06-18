/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * init.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:11 2005   mycure
 * last update   Sat Jun 18 16:32:43 2005   mycure
 */

#ifndef KANETON_DEBUG_H
#define KANETON_DEBUG_H		1

/*
 * debug
 */

#define KANETON_DEBUG_PARAMS	(1 << 0)
#define KANETON_DEBUG_AS	(1 << 1)
#define KANETON_DEBUG_SEGMENT	(1 << 2)

#define KANETON_DEBUG							\
  (0 | KANETON_DEBUG_PARAMS)

#endif
