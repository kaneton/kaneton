/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/bootloader/02/02.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [mon apr  3 13:39:54 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../../../check/common/common.h"

extern t_init*	init;

#define CHECK_SEG(Id,Addr,Sz,Perms)					\
  if (init->segments[(Id)].address != (Addr) ||				\
      init->segments[(Id)].size != (Sz) ||				\
      init->segments[(Id)].perms != (Perms))				\
    printf("error in segment %d\n", (Id))

#define CHECK_REG(Id,Addr,Offs,Sz)					\
  if (init->regions[(Id)].address != (Addr) ||				\
      init->regions[(Id)].offset != (Offs) ||				\
      init->regions[(Id)].size != (Sz))					\
    printf("error in region %d\n", (Id))

#define PERM_RW								\
  (PERM_READ | PERM_WRITE)
#define PERM_RX								\
  (PERM_READ | PERM_EXEC)

/*
 * segments and regions test
 */

void		check_bootloader_02(void)
{
  TEST_ENTER;

  CHECK_SEG(0, INIT_ISA_ADDR, 0x1000, PERM_READ);
  CHECK_SEG(1, INIT_ISA_ADDR + 0x1000, INIT_ISA_SIZE - 0x1000, PERM_RW);
  CHECK_SEG(2, init->kcode, init->kcodesz, PERM_RX);
  CHECK_SEG(3, init->init, init->initsz, PERM_RW);
  CHECK_SEG(4, (t_paddr)init->modules, init->modulessz, PERM_RX);
  CHECK_SEG(5, (t_paddr)init->segments, init->segmentssz, PERM_RW);
  CHECK_SEG(6, (t_paddr)init->regions, init->regionssz, PERM_RW);
  CHECK_SEG(7, init->kstack, init->kstacksz, PERM_RW);
  CHECK_SEG(8, init->alloc, init->allocsz, PERM_RW);

  CHECK_REG(0, init->segments[1].address, 0, init->segments[1].size);
  CHECK_REG(1, init->segments[2].address, 0, init->segments[2].size);
  CHECK_REG(2, init->segments[3].address, 0, init->segments[3].size);
  CHECK_REG(3, init->segments[7].address, 0, init->segments[7].size);
  CHECK_REG(4, init->segments[8].address, 0, init->segments[8].size);
  CHECK_REG(5, init->segments[9].address, 0, init->segments[9].size);
  CHECK_REG(6, init->segments[10].address, 0, init->segments[10].size);

  TEST_LEAVE;
}
