#include <stdlib.h>
#include <string.h>

#include "graph.h"

user *create_user(unsigned long id, const char *name)
{
        user *u = malloc(sizeof(user));
        if (u == NULL) {
                perror("create_user malloc");
        }
        char *n = malloc((size_t) strlen(name));
        u->name = (const char *)n;
        
        return u;
}

void destroy_user(const user *u)
{
        if (u) {
                free(u->name);
                free(u);
        }
}

