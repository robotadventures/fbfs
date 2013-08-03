#include <stdlib.h>
#include <string.h>

#include "graph.h"

user *create_user(unsigned long id, const char *name)
{
        user *u = malloc(sizeof(user));
        if (!u) {
                perror("create_user malloc");
                return NULL;
        }
        u->id = id;

        char *n = malloc((size_t) (strlen(name) + 1));
        if (!n) {
                perror("create_user malloc");
                free(u);
                return NULL;
        }
        strcpy(n, name);
        u->name = (const char *)n;
        
        return u;
}

void destroy_user(user *u)
{
        if (u) {
                free(u->name);
                free(u);
        }
}

void destroy_users(users **us)
{
        while (us) {
                destroy_user(*us);
                us++;
        }
}


