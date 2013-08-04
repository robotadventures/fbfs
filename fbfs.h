#ifndef _FBFS_H_
#define _FBFS_H_
#include <stdio.h>
#include <limits.h>

#define FBFS_LOG_PATH "/tmp/fbfs.log"

typedef struct fb_state {
	FILE *log_file;
	char *root_dir;
} fb_state;

#define FB_DATA ((fb_state *) fuse_get_context()->private_data)

#endif

