/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:41 2005   mycure
 * last update   Tue Nov 15 19:37:54 2005   mycure
 */

#ifndef IA32_KANETON_SEGMENT_H
#define IA32_KANETON_SEGMENT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX #include <kaneton/segment.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the segment architecture-dependent interface
 */

typedef struct
{
  t_error			(*segment_clone)(t_asid,
						 t_segid,
						 t_segid*);
  t_error			(*segment_reserve)(t_asid,
						   t_psize,
						   t_perms,
						   t_segid*);
  t_error			(*segment_release)(t_segid);
  t_error			(*segment_catch)(t_asid,
						 t_segid);
  t_error			(*segment_perms)(t_segid,
						 t_perms);
  t_error			(*segment_type)(t_segid,
						t_type);
  t_error			(*segment_flush)(t_asid);
  t_error			(*segment_init)(t_fit);
  t_error			(*segment_clean)(void);
}				i_segment;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define		machdep_include_segment()				\
  extern i_segment		segment_interface

#define		machdep_call_segment(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (segment_interface._function_ != NULL)				\
        _r_ = segment_interface._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_segment()

#define		machdep_data_o_segment()

#endif
