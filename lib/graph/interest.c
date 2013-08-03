#include <stdlib.h>
#include <string.h>

interest *create_interest(const char *name)
{
        interest *i = malloc(sizeof(interest));
        if (!i) {
                perror("create_interest malloc");
                return NULL;
        }

        char *n = malloc((size_t) (strlen(name) + 1));
        if (!n) {
                perror("create_interest malloc");
                free(i);
                return NULL;
        }
        strcpy(n, name);
        i->name = n;

        return i;
}

void destroy_interest(interest *i)
{
        if (i) {
                if (i->name) {
                        free(i->name);
                }
                free(i);
        }
}

void destroy_interests(interests **is)
{
        interest **isr = is;
        if (!is) {
                return;
        }
        while (*isr) {
                destroy(*isr);
                isr++;
        }
        free(is);
}

