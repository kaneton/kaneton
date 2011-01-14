/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...itecture/ia32/educational/segmentation.c
 *
 * created       julien quintard   [fri jan 14 13:51:10 2011]
 * updated       julien quintard   [fri jan 14 13:52:43 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

// XXX
extern t_uint16		ia32_interrupt_ds;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX
 */

t_error			architecture_segmentation_setup(void)
{
  t_uint16		code;
  t_uint16		data;

  /*
   * 1)
   */

  if (architecture_gdt_insert(ARCHITECTURE_GDT_INDEX_KERNEL_CODE,
			      0x0,
			      0xffffffff,
			      ARCHITECTURE_PRIVILEGE_KERNEL,
			      ARCHITECTURE_GDT_CLASS_SEGMENT,
			      ARCHITECTURE_GDT_TYPE_CODE) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the kernel code GDT segment");

  if (architecture_gdt_insert(ARCHITECTURE_GDT_INDEX_KERNEL_DATA,
			      0x0,
			      0xffffffff,
			      ARCHITECTURE_PRIVILEGE_KERNEL,
			      ARCHITECTURE_GDT_CLASS_SEGMENT,
			      ARCHITECTURE_GDT_TYPE_DATA) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the kernel data GDT segment");

  /*
   * 2)
   */

  if (architecture_gdt_insert(ARCHITECTURE_GDT_INDEX_DRIVER_CODE,
			      0x0,
			      0xffffffff,
			      ARCHITECTURE_PRIVILEGE_DRIVER,
			      ARCHITECTURE_GDT_CLASS_SEGMENT,
			      ARCHITECTURE_GDT_TYPE_CODE) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the driver code GDT segment");

  if (architecture_gdt_insert(ARCHITECTURE_GDT_INDEX_DRIVER_DATA,
			      0x0,
			      0xffffffff,
			      ARCHITECTURE_PRIVILEGE_DRIVER,
			      ARCHITECTURE_GDT_CLASS_SEGMENT,
			      ARCHITECTURE_GDT_TYPE_DATA) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the driver data GDT segment");

  /*
   * 3)
   */

  if (architecture_gdt_insert(ARCHITECTURE_GDT_INDEX_SERVICE_CODE,
			      0x0,
			      0xffffffff,
			      ARCHITECTURE_PRIVILEGE_SERVICE,
			      ARCHITECTURE_GDT_CLASS_SEGMENT,
			      ARCHITECTURE_GDT_TYPE_CODE) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the service code GDT segment");

  if (architecture_gdt_insert(ARCHITECTURE_GDT_INDEX_SERVICE_DATA,
			      0x0,
			      0xffffffff,
			      ARCHITECTURE_PRIVILEGE_SERVICE,
			      ARCHITECTURE_GDT_CLASS_SEGMENT,
			      ARCHITECTURE_GDT_TYPE_DATA) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the service data GDT segment");

  /*
   * 4)
   */

  if (architecture_gdt_insert(ARCHITECTURE_GDT_INDEX_GUEST_CODE,
			      0x0,
			      0xffffffff,
			      ARCHITECTURE_PRIVILEGE_GUEST,
			      ARCHITECTURE_GDT_CLASS_SEGMENT,
			      ARCHITECTURE_GDT_TYPE_CODE) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the guest code GDT segment");

  if (architecture_gdt_insert(ARCHITECTURE_GDT_INDEX_GUEST_DATA,
			      0x0,
			      0xffffffff,
			      ARCHITECTURE_PRIVILEGE_GUEST,
			      ARCHITECTURE_GDT_CLASS_SEGMENT,
			      ARCHITECTURE_GDT_TYPE_DATA) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the guest data GDT segment");

  /*
   * 5)
   */

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_CODE,
				ARCHITECTURE_PRIVILEGE_KERNEL,
				&code) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the kernel code segment selector");

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_DATA,
				ARCHITECTURE_PRIVILEGE_KERNEL,
				&data) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the kernel data segment selector");

  /*
   * 6)
   */

  if (architecture_pmode_registers(code, data) != ERROR_OK)
    MACHINE_ESCAPE("unable to update the segment's registers");

  /*
   * 7)
   */

  ia32_interrupt_ds = data;

  MACHINE_LEAVE();
}
