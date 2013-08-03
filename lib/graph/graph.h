#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef struct user {
        unsigned long id;
        char *name;
} user;

/**
 * access tokens are variable-length
 * https://developers.facebook.com/docs/facebook-login/access-tokens/
 **/
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

/**
 * this returns a NULL-terminated list of pointers to user
 */
user **get_friends(const graph_session *, const user *, int);
user *create_user(unsigned long, const char *);
void destroy_users(user **);
void destroy_user(user *);

graph_session *create_graph_session(const char *);
void destroy_graph_session(graph_session *);

/**
 * this returns a NULL-terminated list of pointers to post
 */
post **get_posts(const graph_session *, const user *, int);
post *create_post(unsigned long, const user *, const user *, const char *);
void destroy_posts(post **);
void destroy_post(post *);

/**
 * this returns a NUL-terminated list of pointers to interest
 */
interest **get_interests(const graph_session *, const user *, int);
interest *create_interest(const char *);
void destroy_interests(interest **);
void destroy_interest(interest *);

#endif

