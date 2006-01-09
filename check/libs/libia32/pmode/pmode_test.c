/*
 * test function
 */

void			pmode_test(void)
{
  t_uint16		seg1;
  t_uint16		seg2;
  t_segment		seg1des;
  void			*ptr;
  int			r;
  t_uint16		es, fs;

  gdt_size(NULL, &seg1);

  printf("current gdt size %d\n", seg1);

  seg1des.base = 0;
  seg1des.limit = 1024;
  seg1des.privilege = prvl_supervisor;
  seg1des.is_system = 0;
  seg1des.type.usr = type_code;

  gdt_reserve_segment(NULL, seg1des, &seg1);

  printf("reserved segment : %d\n", seg1);

  seg1des.base = 1456789;
  seg1des.limit = 1024 * 1024 * 3;
  seg1des.privilege = prvl_supervisor;
  seg1des.is_system = 0;
  seg1des.type.usr = type_data;

  gdt_reserve_segment(NULL, seg1des, &seg2);

  printf("reserved segment : %d\n", seg2);

  gdt_dump(NULL);

  gdt_delete_segment(NULL, seg2);

  seg1des.base = 4242;
  seg1des.limit = 256 * sizeof(t_ldte);
  seg1des.privilege = prvl_supervisor;
  seg1des.is_system = 1;
  seg1des.type.sys = type_ldt;

  gdt_add_segment(NULL, 42, seg1des);

  gdt_dump(NULL);

  gdt_delete_segment(NULL, 42);
  gdt_delete_segment(NULL, seg1);
/*
  ptr = malloc(8);

  seg1des.base = (t_paddr)ptr;
  seg1des.limit = 8;
  seg1des.is_system = 0;
  seg1des.type.usr = type_data;

  gdt_reserve_segment(NULL, seg1des, &seg1);

  seg1des.base += 4;
  seg1des.limit = 4;

  gdt_reserve_segment(NULL, seg1des, &seg2);

  gdt_build_selector(seg1, prvl_supervisor, &es);
  gdt_build_selector(seg2, prvl_supervisor, &fs);

  asm volatile("movw %1, %%ax\t\n"
	       "movw %%ax, %%es\t\n"
	       "movl $41424344, %%es:(4)\t\n"
	       "movw %2, %%ax\t\n"
	       "movw %%ax, %%fs\t\n"
	       "movl %%fs:(0), %0\t\n"
	       : "=r" (r)
	       : "g" (es), "g" (fs)
	       : "%eax");

  if (r != 41424344)
    printf("error in segments !\n");
  printf("r = %d\n", r);

  free(ptr);

  gdt_delete_segment(NULL, seg1);
  gdt_delete_segment(NULL, seg2);
*/
}
