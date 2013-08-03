#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

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

/**
 * this returns a NULL-terminated list of pointers to const users
 */
const user **get_friends(const graph_session *, const user *, int);
void free_users(const user **);
void free_user(const user *);

/**
 * this returns a NULL-terminated list of pointers to const posts
 */
const post **get_posts(const graph_session *, const user *, int);
void free_posts(const post **);
void free_post(const post *);

