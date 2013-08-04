#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fuse.h>

#include "fbfs.h"
#include "log.h"

FILE *log_open()
{
    FILE *fh = fopen(FBFS_LOG_PATH, "w");
    if (fh == NULL) {
        perror("log_open");
        exit(EXIT_FAILURE);
    }

    /* flush on newline */
    setvbuf(fh, NULL, _IOLBF, 0);
    return fh;
}

void log_msg(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    vfprintf(FB_DATA->log_file, format, ap);
}

