#ifndef _FBUTIL_H_
#define _FBUTIL_H_

#include <graph/graph.h>
#include "fbfs.h"

int fb_is_friend_path(const char *);
const char *fb_friend_name_from_path(const char *);
int is_valid_friend(const char *);


#endif

