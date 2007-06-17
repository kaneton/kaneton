/*
 licence       kaneton licence

 project       kaneton

 file          /home/buckman/kaneton/libs/libsun4u/ofw/openfw.S

 created       matthieu bucchianeri   [wed aug  2 19:18:45 2006]
 updated       matthieu bucchianeri   [thu aug 10 17:25:50 2006]
 */

	.text

/*
 * this function attempt a call to OpenFirmware.
 *
 * steps:
 *
 * 1) prepare a new register window.
 * 2) save the global registers.
 * 3) call OpenFirmware.
 * 4) restore the global registers.
 * 5) restore old window and return.
 */

	.align 8
	.globl	openfw
openfw:
	/*
	 * 1)
	 */

	set	prom, %o2
	ldx	[%o2], %o1

	save	%sp, -192, %sp

	/*
	 * 2)
	 */

	mov	%g1, %l1
	mov	%g2, %l2
	mov	%g3, %l3
	mov	%g4, %l4
	mov	%g5, %l5
	mov	%g6, %l6
	mov	%g7, %l7
	rdpr	%pstate, %l0

	/*
	 * 3)
	 */

	mov	%i0, %o0
	call	%i1
	wrpr	%g0, 6, %pstate

	/*
	 * 4)
	 */

	wrpr	%l0, %g0, %pstate
	mov	%l7, %g7
	mov	%l6, %g6
	mov	%l5, %g5
	mov	%l4, %g4
	mov	%l3, %g3
	mov	%l2, %g2
	mov	%l1, %g1

	/*
	 * 5)
	 */

	ret
	restore	%o0, %g0, %o0
