#ifndef CHECK_ARRAY_INIT_H_
# define CHECK_ARRAY_INIT_H_

# include <klibc.h>
# include <kaneton.h>

# define TEST_ENTER							\
  t_leaks __memory_leaks;						\
  moulinette_leaks_init(&__memory_leaks)

# define TEST_LEAVE							\
  moulinette_leaks_display(&__memory_leaks)

typedef struct
{

  t_uint32	nb_alloc;
  t_uint32	nb_free;

}		t_leaks;

typedef void (*t_dispdata)(void *data);

void	moulinette_many_add(t_setid setid, int nb, ...);
void	moulinette_many_remove(t_setid setid, int nb, ...);
void	moulinette_display_set(t_setid setid, t_dispdata disp);
void	moulinette_leaks_init(t_leaks *l);
void	moulinette_leaks_display(t_leaks *l);

#endif /* !CHECK_ARRAY_INIT_H_ */
