#include <stdio.h>
#include <crt.h>
#include <libkaneton.h>
#include "../../drivers/ide/ide-driver.h"

int		main(void)
{
 
  char		buf[IDE_SERVICE_MAX];

  if (!(ide_write("chiche donne nous tout", buf, IDE_SERVICE_MAX) != ERROR_NONE))
    printf(" -- test_ide: %s\n", buf);
  else
    printf(" -- test_ide: reply KO\n");

  while (1)
    ;

  return (0);
}
