/**
 * Facebook Graph API Library
 * Kaitlin Poskaitis, Joshua Matthews, and Wayne Chang
 */

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <json/json.h>

#include "util.h"
#include "graph.h"

user *retrieve_graph_user(const char *access_token)
{
        int id;
        const char *name;
        char *url = create_url("https://graph.facebook.com/me?fields=name&access_token=", access_token);
        json_object *jobj;
        json_object *name_obj;
        json_object *id_obj;
        user *u;

        jobj = http_get_request_json(url);

        json_object_object_get_ex(jobj, "name", &name_obj);
        json_object_object_get_ex(jobj, "id", &id_obj);

        name = json_object_get_string(name_obj);
        id = (unsigned long) atoi(json_object_get_string(id_obj));

        user *me = create_user(id, name);

        json_object_put(jobj);
        free(url);

        return me;
}

user **get_friends(const graph_session *gs, int limit)
{
    int i;
    int friends_len;
    json_object *jobj;
    json_object *friend;
    json_object *friends_obj;
    json_object *friends_data;
    json_object *friend_id_obj;
    json_object *friend_name_obj;

    user **friends_arr = NULL;

    char *friend_name;
    unsigned long friend_id;

    char *api_base = "https://graph.facebook.com/me?fields=friends&access_token=";
    char *url = create_url(api_base, gs->access_token);

    jobj = http_get_request_json(url);

    json_object_object_get_ex(jobj, "friends", &friends_obj);
    json_object_object_get_ex(friends_obj, "data", &friends_data);

    friends_len = json_object_array_length(friends_data);
    if(friends_len)
    {
        /*NULL terminated array*/
        friends_arr = calloc(friends_len + 1, sizeof(user*));
        for (i = 0; i < friends_len; ++i)
        {
            friend = json_object_array_get_idx(friends_data, i);

            json_object_object_get_ex(friend, "id", &friend_id_obj);
            json_object_object_get_ex(friend, "name", &friend_name_obj);

            friend_id = atoi(json_object_get_string(friend_id_obj));
            friend_name = (char*)json_object_get_string(friend_name_obj);

            friends_arr[i] = create_user(friend_id, friend_name);
        }
    }

    json_object_put(jobj);
    free(url);

    return friends_arr;
}

post **get_posts(const graph_session *session, const user *target_user, int limit)
{
    int i, j=0;
    char user_id[128];
    char url[512];
    int posts_len;
    unsigned long int to_id, from_id, post_id;
    char *to_name, *from_name, *post_message;

    json_object *jobj;
    json_object *post_obj;
    json_object *posts_data;
    json_object *post_id_obj;
    json_object *post_from_obj;
    json_object *from_name_obj;
    json_object *from_id_obj;
    json_object *post_message_obj;

    post **posts_arr;

    user *from_user;

    strcpy(url, "https://graph.facebook.com/");
    sprintf(user_id, "%lu", target_user->id);
    strcat(url, user_id);
    strcat(url, "/posts?fields=id,message,from&limit=50");
    strcat(url, "&access_token=");
    strcat(url, session->access_token);

    jobj = http_get_request_json(url);

    json_object_object_get_ex(jobj, "data", &posts_data);

    posts_len = json_object_array_length(posts_data);

    if(posts_len)
    {
        posts_arr = calloc(posts_len + 1, sizeof(post*));
        for (i = 0; i < posts_len; ++i)
        {
            post_obj = json_object_array_get_idx(posts_data, i);

            if(json_object_object_get_ex(post_obj, "message", &post_message_obj))
            {
                ++j;
                json_object_object_get_ex(post_obj, "id", &post_id_obj);
                json_object_object_get_ex(post_obj, "from", &post_from_obj);

                json_object_object_get_ex(post_from_obj, "id", &from_id_obj);
                json_object_object_get_ex(post_from_obj, "name", &from_name_obj);
                from_id = atoi(json_object_get_string(from_id_obj));
                from_name = (char*)json_object_get_string(from_name_obj);
                from_user = create_user(from_id, from_name);

                json_object_object_get_ex(post_obj, "id", &post_id_obj);
                post_id = atoi(json_object_get_string(post_id_obj));

                json_object_object_get_ex(post_obj, "message", &post_message_obj);
                post_message = (char*)json_object_get_string(post_message_obj);

                posts_arr[j] = create_post(post_id, target_user, from_user, post_message);
            }
        }
    }

    json_object_put(jobj);

    return posts_arr;
}

like **get_likes(const graph_session *session, const user *target_user, int limit)
{
    like **likes_arr = NULL;
    like *l;

    char url[512];
    char user_id[128];
    int likes_len, i;

    json_object *jobj;
    json_object *likes_obj;
    json_object *data_obj;
    json_object *like_obj;
    json_object *name_obj;

    strcpy(url, "https://graph.facebook.com/");
    sprintf(user_id, "%lu", target_user->id);
    strcat(url, user_id);
    strcat(url, "?fields=likes");
    strcat(url, "&access_token=");
    strcat(url, session->access_token);

    jobj = http_get_request_json(url);
    json_object_object_get_ex(jobj, "likes", &likes_obj);
    json_object_object_get_ex(likes_obj, "data", &data_obj);

    likes_len = json_object_array_length(data_obj);

    if(likes_len)
    {
        likes_arr = calloc(likes_len + 1, sizeof(like*));
        for(i = 0; i < likes_len; ++i)
        {
            like_obj = json_object_array_get_idx(data_obj, i);
            json_object_object_get_ex(like_obj, "name", &name_obj);
            l = create_like(json_object_get_string(name_obj));
            likes_arr[i] = l;
        }
    }
    json_object_put(jobj);

    return likes_arr;
}

void post_on_timeline(const graph_session *session, char *message)
{
    char url[512];
    char user_id[128];
    char *access_post = "&access_token=";
    char *message_post = "message=";
    char *postfields = calloc(strlen(access_post) + strlen(session->access_token) + strlen(message_post) + strlen(message) + 1, sizeof(char));

    /*Get url*/
    strcpy(url, "https://graph.facebook.com/");
    sprintf(user_id, "%lu", session->me->id);
    strcat(url, user_id);
    strcat(url, "/feed");

    /*get postfields*/
    strcpy(postfields, message_post);
    strcat(postfields, message);
    strcat(postfields, access_post);
    strcat(postfields, session->access_token);

    http_post_request(url, postfields);
}
