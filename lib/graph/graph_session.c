#include <stdlib.h>
#include <string.h>

#include "graph.h"

user *retrieve_graph_user(const char *access_token)
{
        int id = 0;
        char *name = "";
        /* curl magic to get user */

        return create_user(id, name);
}

graph_session *create_graph_session(const char *access_token)
{
        graph_session *gs = malloc(sizeof(graph_session));
        if (!gs) {
                perror("create_graph_session malloc");
                return NULL;
        }

        user *u = retrieve_graph_user(access_token);       
        if (!u) {
                perror("create_graph_session retrieve_graph_user");
                free(gs);
                return NULL;
        }
        gs->me = u;

        char *at = malloc((size_t) (strlen(access_token) + 1));
        if (!at) {
                perror("create_graph_session malloc");
                destroy_user(u);
                free(gs);
                return NULL;
        }
        strcpy(at, access_token);
        gs->access_token = at;

        return gs;
}

void destroy_graph_session(graph_session *gs)
{
        if (gs) {
                if (gs->me) {
                        destroy_user(gs->me);
                }
                if (gs->access_token) {
                        free(gs->access_token);
                }
        }
}

