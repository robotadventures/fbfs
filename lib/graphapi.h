#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/*Struct definitions*/
struct string
{
    char *ptr;
    size_t len;
};

/*Function definitions*/
char **getFriends(char *access_token);

/*Helpers*/
size_t buildStringResponse(void *ptr, size_t size, size_t nmemb, struct string *s);
void init_string(struct string *s);
void destroy_string(struct string *s);

