#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef struct user {
        unsigned long id;
        const char *name;
} user;

/**
 * access tokens are variable-length
 * https://developers.facebook.com/docs/facebook-login/access-tokens/
 **/
typedef struct graph_session {
        const user *me;
        const char *access_token;
} graph_session;

typedef struct post {
        unsigned long id;
        const user *to, *from;
        const char *message;
} post;

typedef struct interest {
        const char *name;
} interest;

/**
 * this returns a NULL-terminated list of pointers to const user
 */
const user **get_friends(const graph_session *, const user *, int);
user *create_user(unsigned long, const char *);
void destroy_users(const user **);
void destroy_user(const user *);

/**
 * this returns a NULL-terminated list of pointers to const post
 */
const post **get_posts(const graph_session *, const user *, int);
post *create_post(unsigned long, const user *, const user *, const char *);
void destroy_posts(const post **);
void destroy_post(const post *);

/**
 * this returns a NUL-terminated list of pointers to const interest
 */
const interest **get_interests(const graph_session *, const user *, int);
interest *create_interest(const char *);
void destroy_interests(const interest **);
void destroy_interest(const interest *);

#endif

