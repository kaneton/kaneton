
/**
 ** Auto-generated file, dont modify directly
 ** your changes will be lost !
 **
 ** Generated by DSX on 2006-04-12 16:13:48.077966
 ** by unknown@callisto
 **/

#include <klibc.h>

#include <dsx.h>
#define DSX_INMAIN
#include "tg_proto.h"
#include "demux_proto.h"
#include "vld_proto.h"
#include "iqzz_proto.h"
#include "idct_proto.h"
#include "libu_proto.h"
#include "ramdac_proto.h"


int dsx_log_open();
void endianness_init();

extern m_timer *timer;

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA
#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

static unsigned char g_320x200x256[] =
{
	0x63,
	0x03, 0x01, 0x0F, 0x00, 0x0E,
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

static void		write_regs(unsigned char *regs)
{
  unsigned int	i, a;

  OUTB(VGA_MISC_WRITE, *regs);
  regs++;
  for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
      OUTB(VGA_SEQ_INDEX, i);
      OUTB(VGA_SEQ_DATA, *regs);
      regs++;
    }
  OUTB(VGA_CRTC_INDEX, 0x03);
  INB(VGA_CRTC_DATA, a);
  OUTB(VGA_CRTC_DATA, a | 0x80);
  OUTB(VGA_CRTC_INDEX, 0x11);
  INB(VGA_CRTC_DATA, a);
  OUTB(VGA_CRTC_DATA, a & ~0x80);
  regs[0x03] |= 0x80;
  regs[0x11] &= ~0x80;
  for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
      OUTB(VGA_CRTC_INDEX, i);
      OUTB(VGA_CRTC_DATA, *regs);
      regs++;
    }
  for(i = 0; i < VGA_NUM_GC_REGS; i++)
    {
      OUTB(VGA_GC_INDEX, i);
      OUTB(VGA_GC_DATA, *regs);
      regs++;
    }
  for(i = 0; i < VGA_NUM_AC_REGS; i++)
    {
      INB(VGA_INSTAT_READ, a);
      OUTB(VGA_AC_INDEX, i);
      OUTB(VGA_AC_WRITE, *regs);
      regs++;
    }
  INB(VGA_INSTAT_READ, a);
  OUTB(VGA_AC_INDEX, 0x20);
}

static void setup_vga(void)
{
  unsigned int i;

  write_regs(g_320x200x256);

  OUTB(VGA_DAC_WRITE_INDEX, 0);
  for (i = 0; i < 256; i++)
    {
      OUTB(VGA_DAC_DATA, i >> 2);
      OUTB(VGA_DAC_DATA, i >> 2);
      OUTB(VGA_DAC_DATA, i >> 2);
    }

  memset((void *)0xa0000, 0, 320 * 200);

}

i_task mjpeg_task;
i_as mjpeg_asid;
extern i_task ktask;

void mjpeg(void)
{
  static struct _mwmr_t tg_demux;
  static struct _mwmr_t quanti;
  static struct _mwmr_t vld_iqzz;
  static struct _mwmr_t demux_vld;
  static struct _mwmr_t huffman;
  static struct _mwmr_t iqzz_idct;
  static struct _mwmr_t idct_libu;
  static struct _mwmr_t libu_ramdac;
  t_uint32 volatile *tt = &timer->timeref;

  t_uint32 t = timer->timeref;

  if (task_reserve(TASK_CLASS_PROGRAM, TASK_BEHAV_CORE, TASK_HPRIOR_CORE, &mjpeg_task) != ERROR_NONE)
    {
      printf("cannot task_reserve\n");

      while (1)
	;
    }

  if (as_reserve(mjpeg_task, &mjpeg_asid) != ERROR_NONE)
    {
      printf("cannot as_reserve\n");

      while (1)
	;
    }

  task_priority(ktask, 10);

  while (*tt - t < 1500)
    ;

  setup_vga();

  dsx_mwmr_alloc(&tg_demux, 8, 2, "tg_demux");
  dsx_mwmr_alloc(&quanti, 8, 4, "quanti");
  dsx_mwmr_alloc(&vld_iqzz, 32, 2, "vld_iqzz");
  dsx_mwmr_alloc(&demux_vld, 8, 2, "demux_vld");
  dsx_mwmr_alloc(&huffman, 8, 6, "huffman");
  dsx_mwmr_alloc(&iqzz_idct, 64, 2, "iqzz_idct");
  dsx_mwmr_alloc(&idct_libu, 16, 2, "idct_libu");
  dsx_mwmr_alloc(&libu_ramdac, 96, 2, "libu_ramdac");


  static tg_args_t tg_args = {
    .output = &tg_demux,
    .is_unused = 42
  };
  tg_launcher( &tg_args );


  static demux_args_t demux_args = {
    .input = &tg_demux,
    .quanti = &quanti,
    .huffman = &huffman,
    .output = &demux_vld,
    .is_unused = 42
  };
  demux_launcher( &demux_args );


  static vld_args_t vld_args = {
    .input = &demux_vld,
    .huffman = &huffman,
    .output = &vld_iqzz,
    .is_unused = 42
  };
  vld_launcher( &vld_args );


  static iqzz_args_t iqzz_args = {
    .input = &vld_iqzz,
    .quanti = &quanti,
    .output = &iqzz_idct,
    .is_unused = 42
  };
  iqzz_launcher( &iqzz_args );


  static idct_args_t idct_args = {
    .input = &iqzz_idct,
    .output = &idct_libu,
    .is_unused = 42
  };
  idct_launcher( &idct_args );


  static libu_args_t libu_args = {
    .input = &idct_libu,
    .output = &libu_ramdac,
    .is_unused = 42
  };
  libu_launcher( &libu_args );


  static ramdac_args_t ramdac_args = {
    .input = &libu_ramdac,
    .is_unused = 42
  };
  ramdac_launcher( &ramdac_args );

  if (task_state(mjpeg_task, SCHED_STATE_RUN) != ERROR_NONE)
    {
      printf("cannot run mjpeg task\n");
    }

  if (thread_priority(0, THREAD_LPRIOR) != ERROR_NONE)
    printf("bouh\n");

  if (thread_state(0, SCHED_STATE_STOP) != ERROR_NONE)
    printf("bouh\n");


  while (1)
    pthread_yield();
}