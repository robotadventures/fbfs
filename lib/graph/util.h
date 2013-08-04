#ifndef _UTIL_H_
#define _UTIL_H_

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <json/json.h>

/*simple string struct to make the libcurl -> json transition easier*/
typedef struct string {
    char *ptr;
    int len;
} string;

/*Utility functions*/
json_object *http_get_request_json(char *url);
size_t build_string_response(void *ptr, size_t size, size_t nmemb, struct string *s);
char *create_url(const char *base, const char *access_token);
void init_string(struct string *s);
void destroy_string(struct string *s);
#endif
