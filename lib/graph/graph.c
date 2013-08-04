/**
 * Facebook Graph API Library
 * Kaitlin Poskaitis, Joshua Matthews, and Wayne Chang
 */

#include "util.h"
#include "graph.h"

user **getFriends(char *access_token)
{
    int i;
    int friends_len;
    json_object *jobj;
    json_object *friend;
    json_object *friends_data;
    json_object *friend_id_obj;
    json_object *friend_name_obj;

    user **friends_arr = NULL;

    char *friend_name;
    unsigned long friend_id;

    char *api_base = "https://graph.facebook.com/me?fields=friends&access_token=";
    char *url = create_url(api_base, access_token);

    jobj = http_get_request_json(url);

    json_object_object_get_ex(jobj, "friends", friends_obj);
    json_object_object_get_ex(friends_obj, "data", friends_data);

    friends_len = json_object_array_length(friends_data, "data");
    if(friends_len)
    {
        /*NULL terminated array*/
        friends_arr = calloc(friends_len + 1, sizeof(user*));
        for (i = 0; i < friends_len; ++i)
        {
            friend = json_object_array_get_idx(i);

            json_object_object_get_ex(friend, "id", friend_id_obj);
            json_object_object_get_ex(friend, "name", friend_name_obj);

            friend_id = atoi(json_object_get_string(friend_id_obj));
            friend_name = json_object_get_string(friend_name_obj);

            friend_arr[i] = create_user(friend_id, friend_name);
        }
    }

    json_object_put(jobj);
    free(url);
    free(friend_name);

    return friend_arr;
}
