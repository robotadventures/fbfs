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

typedef struct interest {
        char *name;
} interest;

/* user */
user **get_friends(const graph_session *, int);
user *create_user(unsigned long, const char *);
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

/* interest */
interest **get_interests(const graph_session *, const user *, int);
interest *create_interest(const char *);
void destroy_interest(interest *);
void destroy_interests(interest **);
void print_interest(interest *);
void print_interests(interest **);

#endif

