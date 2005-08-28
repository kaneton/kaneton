/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * init.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:11 2005   mycure
 * last update   Sun Aug 28 18:20:46 2005   mycure
 */

#ifndef KANETON_DEBUG_H
#define KANETON_DEBUG_H		1

/*
 * ---------- defines ---------------------------------------------------------
 */

#define DEBUG_PARAMS		(1 << 0)
#define DEBUG_AS		(1 << 1)
#define DEBUG_SEGMENT		(1 << 2)
#define DEBUG_SET		(1 << 3)

#define DEBUG								\
  (CONF_DEBUG)

#endif
