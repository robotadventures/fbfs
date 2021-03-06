#include <stdlib.h>
#include <stdio.h>
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
        u->name = n;
        
        return u;
}

void destroy_user(user *u)
{
        if (u) {
                if (u->name) {
                        free(u->name);
                }
                free(u);
        }
}

void destroy_users(user **us)
{
        user **usr = us;
        if (!us) {
                return;
        }
        while (*usr) {
                destroy_user(*usr);
                usr++;
        }
        free(us);
}

void print_user(user *u)
{
        printf("user { id: %ld, name: \"%s\" }\n", u->id, u->name);
}

void print_users(user **us)
{
        while (*us) {
                print_user(*us);
                us++;
        }
}

