#include <stdio.h>
#include <crt.h>
#include <libkaneton.h>
#include "../../drivers/ide/ide-driver.h"

int		main(void)
{
  char		buf[IDE_DRIVER_MAX];

  
  if (!(ide_read(0, 0, 1, 1, buf) != ERROR_NONE))
    {
      buf[100] = '\0';
      printf(" -- test_ide: %s\n", buf); //Attention cela fonction car le disk contien des 0
    } 
  else 
    printf(" -- test_ide: reading reply KO\n"); 
  
  
  if (ide_write(0, 0, 0, 1, buf) != ERROR_NONE)
    printf(" -- test_ide: writing reply KO\n");
  else
    printf("WRITE is OK\n");
  
  while (1)
    ;

  return (0);
}
