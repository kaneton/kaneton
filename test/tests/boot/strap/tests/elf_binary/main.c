void video_init(short, short, char);
void video_print_string(char*);
void video_newline(void);
void video_clear_screen(void);

void init_serial(void);
void write_serial(char a);

int kernel_main(int arg1, int arg2)
{
  arg1 = arg1;
  arg2 = arg2;

  video_clear_screen();
  video_init(0, 0, 0x07);
  video_print_string("Moulette !");

  init_serial();
  write_serial('C');
  write_serial('H');
  write_serial('I');
  write_serial('C');
  write_serial('H');
  write_serial('E');

  for (;;)
    asm ("hlt");
}
