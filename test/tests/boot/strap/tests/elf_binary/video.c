#define NROW 25
#define NCOL 80

struct s_char_attrib
{
  int fg : 3;
  int si : 1;
  int bg : 3;
  int blink : 1;
};

short* const video = (short*) 0xB8000;

struct s_frame
{
  short row;
  short col;
  struct s_char_attrib char_attrib;
} frame;

void video_init(short row, short col, struct s_char_attrib char_attrib)
{
  frame.row = row;
  frame.col = col;
  frame.char_attrib = char_attrib;
}

void video_clear_screen()
{
  short blank = ' ' | (0x7 << 8);
  int i;

  for (i = 0; i < (NCOL * NROW); i++)
  {
    video[i] = blank;
  }
}

void video_write(char c)
{
  char attrib = *(char *)(&(frame.char_attrib));
  video[frame.row * NCOL + frame.col] = c |
    (attrib << 8);
  frame.col++;

  if (frame.col == NCOL)
  {
    frame.col = 0;
    frame.row++;

    if (frame.row == NROW) /* No scrolling */
      frame.row = 0;
  }
}

void video_print_string(char* s)
{
  while (*s)
    video_write(*s++);
}

void video_newline(void)
{
  frame.col = 0;
  frame.row++;

  if (frame.row == NROW)
    frame.row = 0;
}
