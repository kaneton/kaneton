/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:32:13 2007]
 */

#ifndef CHECK_AS_COMMON_H_
# define CHECK_AS_COMMON_H_

# include <libc/libc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../read_write_01/read_write_01.c
 *	../copy_01/copy_01.c
 *	../copy_02/copy_02.c
 *	../as_with_segments_01/as_with_segments_01.c
 *	../as_with_regions_01/as_with_regions_01.c
 *	../as_with_map_01/as_with_map_01.c
 *	../as_switching_01/as_switching_01.c
 *	../clone_01/clone_01.c
 *	../clone_02/clone_02.c
 *	../give_01/give_01.c
 *	../mapping_01/mapping_01.c
 *	../paddr_vaddr_01/paddr_vaddr_01.c
 */

/*
 * common.c
 */

void	check_as_common(void);

void	check_as_tests(void);


/*
 * ../read_write_01/read_write_01.c
 */

void		check_as_read_write_01(void);


/*
 * ../copy_01/copy_01.c
 */

void		check_as_copy_01(void);


/*
 * ../copy_02/copy_02.c
 */

void		check_as_copy_02(void);


/*
 * ../as_with_segments_01/as_with_segments_01.c
 */

void		check_as_as_with_segments_01(void);


/*
 * ../as_with_regions_01/as_with_regions_01.c
 */

void		check_as_as_with_regions_01(void);


/*
 * ../as_with_map_01/as_with_map_01.c
 */

void		check_as_as_with_map_01(void);


/*
 * ../as_switching_01/as_switching_01.c
 */

void		check_as_as_switching_01(void);


/*
 * ../clone_01/clone_01.c
 */

void		check_as_clone_01(void);


/*
 * ../clone_02/clone_02.c
 */

void		check_as_clone_02(void);


/*
 * ../give_01/give_01.c
 */

void		check_as_give_01(void);


/*
 * ../mapping_01/mapping_01.c
 */

void		check_as_mapping_01(void);


/*
 * ../paddr_vaddr_01/paddr_vaddr_01.c
 */

void		check_as_paddr_vaddr_01(void);


/*
 * eop
 */

#endif /* !CHECK_AS_COMMON_H_ */
