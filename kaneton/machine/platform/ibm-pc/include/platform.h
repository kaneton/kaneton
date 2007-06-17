/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...chine/platform/ibm-pc/include/platform.h
 *
 * created       julien quintard   [mon jun 11 05:59:19 2007]
 * updated       matthieu bucchianeri   [sun jun 17 19:09:39 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file is the ibmpc main platform header.
 */

#ifndef PLATFORM_PLATFORM_H
#define PLATFORM_PLATFORM_H		1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <platform/timer.h>
#include <platform/keyboard.h>
#include <platform/irq.h>
#include <platform/cons.h>
#include <platform/vga.h>
#include <platform/serial.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define IBMPC_TIMER_FREQUENCY	200

#endif
