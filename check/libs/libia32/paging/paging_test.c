/*
 * test function.
 */

void			paging_test(void)
{
  t_table		pt42;
  t_page		pg0;
  t_page		pg1;
  void			*tmp;
  void			*paddr, *vaddr0, *vaddr1;

  tmp = (void*)MK_BASE(malloc(2 * PT_MAX_ENTRIES * sizeof(t_pte)));

  if (pt_build((t_paddr)tmp, &pt42, 1) != ERROR_NONE)
    printf("error pt_build\n");
  pt42.present = 1;
  pt42.rw = 1;
  pt42.user = 0;

  if (pd_add_table(NULL, 42, pt42) != ERROR_NONE)
    printf("error add_table\n");

  paddr = (void*)0x10000;
  pg0.addr = paddr;
  pg0.present = 1;
  pg0.rw = 1;
  pg0.user = 0;
  if (pt_add_page(&pt42, 0, pg0) != ERROR_NONE)
    printf("error add_page\n");

  pg1.addr = paddr;
  pg1.present = 1;
  pg1.rw = 0;
  pg1.user = 0;
  if (pt_add_page(&pt42, 1, pg1) != ERROR_NONE)
    printf("error add_page\n");

  vaddr0 = ENTRY_ADDR(42, 0);
  vaddr1 = ENTRY_ADDR(42, 1);

  tlb_invalidate((t_vaddr)vaddr0);
  tlb_invalidate((t_vaddr)vaddr1);

  printf("%p mapped to %p\n", vaddr0, paddr);
  printf("%p mapped to %p\n", vaddr1, paddr);

  pt_dump(pt42.entries);

  *((int*)vaddr0) = 0x40414243;
  if (*((int*)vaddr1) != 0x40414243)
    printf("error !\n");
  printf("%x %x\n", *((int*)vaddr0), *((int*)vaddr1));

  pt_dump(pt42.entries);

  pd_delete_table(NULL, 42);

  tlb_flush();

  free(tmp);
}
