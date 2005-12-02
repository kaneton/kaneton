/*
 * copyright quintard julien
 *
 * kaneton
 *
 * conf.h
 *
 * path          /home/mycure/kaneton
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Sun Aug 28 16:23:31 2005   mycure
** Last update Thu Dec  1 17:31:25 2005 matthieu bucchianeri
 */

#ifndef KANETON_CONF_H
#define KANETON_CONF_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton/debug.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * the title of your kernel which will be displayed by the grub
 * bootloader and by your kernel during its bootstrap.
 */

#define CONF_TITLE		"kaneton"

/*
 * the current release version.
 */

#define CONF_VERSION		"0.0.1"

/*
 * the current kernel mode, release, development.
 *
 * the development mode means that the current kernel was not
 * entirely tested while the release mode indicates that the kernel can
 * be used.
 */

#define CONF_MODE_DEVELOPMENT	(1 << 0)
#define CONF_MODE_RELEASE	(1 << 1)

#define CONF_MODE		MODE_DEVELOPMENT

/*
 * this define activates the statistics
 */

#define CONF_STATS

/*
 * the configuration debug
 */

#define CONF_DEBUG							\
  (DEBUG_SET)

/*
 * the machine-dependent configuration debug
 */

#define CONF_MACHDEP_DEBUG						\
  (0)

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/conf/conf.c
 */

/*
 * ../../kaneton/conf/conf.c
 */


/*
 * eop
 */

#endif
