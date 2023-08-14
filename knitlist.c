#include "knitlist.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef SVYAZLIST_LOG(...)
#define SVYAZLIST_LOG(...) \
    do \
    { \
        char message[128] = {'\0'}; \
        uint32_t pref_pos = snprintf(message, sizeof(message), \
                "%s():%d: ", __FUNCTION__, __LINE__); \
        snprintf(&message[pref_pos], sizeof(message) - pref_pos, __VA_ARGS__); \
        printf("%s\n", message); \
    } while (0) 
#endif // SVYAZLIST_LOG

#define SVYAZLIST_ASSERT(expr, message, ...) \
    do \
    { \
        if (expr) \
        { \
            SVYAZLIST_LOG(message); \
            __VA_ARGS__; \
        } \
    } while (0)

typedef struct knitlist_link_priv_s 
{
    knitlist_link_s data;
    struct knitlist_link_priv_s * p_next;
} knitlist_link_priv_s;

typedef struct knitlist_priv_s 
{
    int32_t len;
    knitlist_link_priv_s * p_first;
} knitlist_priv_s;

/** Public functions */

knitlist_s * knitlist_new(void)
{
    knitlist_s * p_list = malloc(sizeof(knitlist_s));
    SVYAZLIST_ASSERT(NULL == p_list, "Allocation error", return NULL);

    p_list->p_priv = malloc(sizeof(knitlist_priv_s));
    SVYAZLIST_ASSERT(NULL == p_list->p_priv, "Allocation error", free(p_list); return NULL);

    return p_list;
}

knitlist_s * knitlist_copy(knitlist_s * p_list)
{
    return NULL;
}

void knitlist_free(knitlist_s * p_list)
{

}


void knitlist_link_add(knitlist_s * p_list, knitlist_link_s * p_link)
{
    SVYAZLIST_ASSERT((NULL == p_list) || (NULL == p_link), 
            "Null pointer argument", 
            return);
    SVYAZLIST_ASSERT((NULL == p_list->p_priv) || (NULL == p_link->p_val), 
            "Corrupted argument", 
            return);


}

knitlist_link_s * knitlist_link_get(knitlist_s * p_list, uint32_t idx)
{

}

void knitlist_link_remove(knitlist_s * p_list, uint32_t idx)
{


}


int32_t knitlist_len_get(knitlist_s * p_list)
{
    SVYAZLIST_ASSERT(NULL == p_list, "Null pointer argument", return -1);

    knitlist_priv_s * p_list_priv = (knitlist_priv_s *) p_list->p_priv;
    SVYAZLIST_ASSERT(NULL == p_list_priv, "Corrupted argument", return -1);

    return p_list_priv->len;
}

/** Private functions */

static knitlist_link_priv_s * knitlist_link_mem_alloc(knitlist_link_s * p_link)
{
    SVYAZLIST_ASSERT(NULL == p_link, "Null pointer argument", return NULL);

    knitlist_link_priv_s * p_link_priv = malloc(sizeof(knitlist_link_priv_s));
    SVYAZLIST_ASSERT(NULL == p_link_priv, "Allocation error", return NULL);

    p_link_priv->data.p_val = malloc(p_link_priv->data.size);
    SVYAZLIST_ASSERT(NULL == p_link_priv->data.p_val, "Allocation error", free(p_link_priv); return NULL);

    return p_link_priv;
}

static void knitlist_link_mem_free(knitlist_link_priv_s * p_link_priv)
{
    SVYAZLIST_ASSERT(NULL == p_link_priv, "Null pointer argument", return);
    SVYAZLIST_ASSERT(NULL == p_link_priv->data.p_val, "Corrupted argument", return);

    free(p_link_priv->data.p_val);
    free(p_link_priv);
}
