#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef struct user {
        unsigned long id;
        char *name;
} user;

typedef struct graph_session {
        user *me;
        char *access_token;
} graph_session;

typedef struct post {
        unsigned long id;
        user *to, *from;
        char *message;
} post;

typedef struct like {
        char *name;
} like;

/* user */
user **get_friends(const graph_session *, int);
user *create_user(unsigned long, const char *);
void post_on_timeline(const graph_session *, char *);
void destroy_user(user *);
void destroy_users(user **);
void print_user(user *);
void print_users(user **);

/* graph_session */
user *retrieve_graph_user(const char *access_token);
graph_session *create_graph_session(const char *);
void destroy_graph_session(graph_session *);
void print_graph_session(graph_session *);

/* post */
post **get_posts(const graph_session *, const user *, int);
post *create_post(unsigned long, const user *, const user *, const char *);
void destroy_post(post *);
void destroy_posts(post **);
void print_post(post *);
void print_posts(post **);

/* like */
like **get_likes(const graph_session *, const user *, int);
like *create_like(const char *);
void destroy_like(like *);
void destroy_likes(like**);
void print_like(like*);
void print_likes(like**);

#endif

