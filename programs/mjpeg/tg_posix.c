#include <libc.h>
#include "dsx.h"
#include "tg_proto.h"

#include "jpeg.h"

#include "image.h"

FUNC(tg)
{
    dsx_log(TRACE, "TG up and running...\n");
    dsx_mwmr_t output = GET_ARG(output);

    while(1) {
        dsx_mwmr_write(output,image,sizeof(image)/4);
    }
}

