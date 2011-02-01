/*
** serial.c for elf_binary in elf_binary
**
** Made by pidanc_j
** Login   <pidanc_j@epita.fr>
**
** Started on  Tue 26 Feb 2008 12:31:35 AM CET pidanc_j
** Last update Tue 26 Feb 2008 12:46:27 AM CET pidanc_j
*/

#define PORT 0x3f8   /* COM1 */

#define	  OUTB(_port_, _data_)			\
    asm volatile("outb %%al, %%dx\n"		\
	         :				\
	         : "d" (_port_), "a" (_data_))

#define		INB(_port_, _data_)					\
  asm volatile("inb %%dx, %%al\n"					\
	       : "=a" (_data_)						\
	       : "d" (_port_))


void init_serial(void)
{
   OUTB(PORT + 1, 0x00);    // Disable all interrupts
   OUTB(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   OUTB(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   OUTB(PORT + 1, 0x00);    //                  (hi byte)
   OUTB(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   OUTB(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   OUTB(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

static int is_transmit_empty(void)
{
  int ret;

  INB(PORT + 5, ret);

  return ret & 0x20;
}

void write_serial(char a)
{
   while (is_transmit_empty() == 0);

   OUTB(PORT,a);
}

