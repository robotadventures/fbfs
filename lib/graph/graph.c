/**
 * Facebook Graph API Library
 * Kaitlin Poskaitis, Joshua Matthews, and Wayne Chang
 */

#include "graph.h"

char **getFriends(char *access_token)
{
    CURLcode res;
    CURL *curl = curl_easy_init();
    if(curl)
    {
        /*Set up response string*/
        struct string response;
        init_string(&response);

        curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/me?fields=friends");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, buildStringResponse);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            fprintf(stderr, "Could not get friends.\n");
        }

        curl_easy_cleanup(curl);

    }
    fprintf(stderr, "Could not init CURL to get friends.\n");
    return NULL;
}

//Helpers

size_t buildStringResponse(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

void init_string(struct string *s)
{
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if(s->ptr == NULL)
    {
        fprintf(stderr, "Could not malloc string\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

void destroy_string(struct string *s)
{
    free(s->ptr);
}
