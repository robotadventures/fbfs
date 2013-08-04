#ifndef _FBFS_H_
#define _FBFS_H_
#include <stdio.h>
#include <limits.h>
#include <fuse.h>
#include <pthread.h>

#include "libgraph/graph.h"

#define FBFS_LOG_PATH "/tmp/fbfs.log"

typedef struct fb_state {
	FILE *log_file;
	char *root_dir;
        
        /*pthread_mutex_t friends_lock;*/
        graph_session *session;
        user **friends;

        /*pthread_mutex_t init_lock;*/
} fb_state;

#define FB_DATA ((fb_state *) fuse_get_context()->private_data)

#endif

