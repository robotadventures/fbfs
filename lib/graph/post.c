#include <stdlib.h>
#include <string.h>

post *create_post(unsigned long id, const user *to, const user *from,
                  const char *message)
{
        post *p = malloc(sizeof(post));
        if (!p) {
                perror("create_post malloc");
                return NULL;
        }
        p->id = id;

        user *t = create_user(to->id, to->name);
        if (!t) {
                perror("create_post create_user");
                free(p);
                return NULL;
        }
        p->to = t;

        user *f = create_user(from->id, from->name);
        if (!f) {
                perror("create_post create_user");
                destroy_user(t);
                free(p);
                return NULL;
        }
        p->from = f;

        char *m = malloc((size_t) (strlen(message) + 1));
        if (!m) {
                perror("create_post malloc");
                destroy_user(f);
                destroy_user(t);
                free(p);
                return NULL;
        }
        strcpy(m, message);
        p->message = m;

        return p;
}

void destroy_post(post *p)
{
        if (p) {
                destroy_user(p->to);
                destroy_user(p->from);
                if (p->message) {
                        free(p->message);
                }
                free(p);
        }
}

void destroy_posts(post **ps)
{
        post **psr = ps;
        if (!ps) {
                return;
        }
        while (*psr) {
                destroy_post(*psr);
                psr++;
        }
        free(ps);
}


