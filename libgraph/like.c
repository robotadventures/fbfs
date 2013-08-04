#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

like *create_like (const char *name)
{
        like *l = malloc(sizeof(like));
        if (!l) {
                perror("create_interest malloc");
                return NULL;
        }

        char *n = malloc((size_t) (strlen(name) + 1));
        if (!n) {
                perror("create_interest malloc");
                free(l);
                return NULL;
        }
        strcpy(n, name);
        l->name = n;

        return l;
}

void destroy_like(like *l)
{
        if (l) {
                if (l->name) {
                        free(l->name);
                }
                free(l);
        }
}

void destroy_likes(like **ls)
{
        like **lsr = ls;
        if (!ls) {
                return;
        }
        while (*lsr) {
                destroy_like(*lsr);
                lsr++;
        }
        free(ls);
}

void print_like(like *l)
{
        printf("interest { name: \"%s\" }\n", l->name);
}

void print_likes(like **ls)
{
        while (*ls) {
                print_like(*ls);
                ls++;
        }
}

