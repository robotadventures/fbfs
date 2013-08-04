/**
 * Facebook Graph API Library
 * Kaitlin Poskaitis, Joshua Matthews, and Wayne Chang
 */
#include "util.h"

json_object *http_get_request_json(char *url)
{
    CURLcode res;
    CURL *curl = curl_easy_init();
    json_tokener *tok;
    json_object *jobj;

    if(curl)
    {
        /*Set up response string*/
        string response;
        init_string(&response);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, build_string_response);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            fprintf(stderr, "Could not get friends.\n");
        }

        curl_easy_cleanup(curl);

        tok = json_tokener_new();
        jobj = json_tokener_parse_ex(tok, response.ptr, response.len);

        json_tokener_free(tok);

        return jobj;
    }
    fprintf(stderr, "Could not init CURL to get friends.\n");
    return NULL;
}

size_t build_string_response(void *ptr, size_t size, size_t nmemb, struct string *s)
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

char *create_url(char *base, char *access_token)
{
    char *url = calloc(strlen(base) + strlen(access_token) + 1, sizeof(char));
    strcpy(url, base);
    strcat(url, access_token);
    return url;
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
