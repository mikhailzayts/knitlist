#include "knitlist.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef KNITLIST_LOG
#define KNITLIST_LOG(...) \
    do \
    { \
        char message[128] = {'\0'}; \
        uint32_t pref_pos = snprintf(message, sizeof(message), \
                "%s():%d: ", __FUNCTION__, __LINE__); \
        snprintf(&message[pref_pos], sizeof(message) - pref_pos, __VA_ARGS__); \
        printf("%s\n", message); \
    } while (0) 
#endif // KNITLIST_LOG

#define KNITLIST_ASSERT(expr, message, ...) \
    do \
    { \
        if (expr) \
        { \
            KNITLIST_LOG(message); \
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
    knitlist_link_priv_s * p_anchor;
} knitlist_priv_s;

/** Private functions prototypes */

static knitlist_link_priv_s * knitlist_link_mem_alloc(knitlist_link_s * p_link);
static knitlist_link_priv_s * knitlist_link_mem_free(knitlist_link_priv_s * p_link_priv);
static void knitlist_len_inc(knitlist_s * p_list);
static void knitlist_len_dec(knitlist_s * p_list);
static knitlist_link_priv_s * knitlist_anchorlink_get(knitlist_s * p_list);
static void knitlist_anchorlink_set(knitlist_s * p_list, knitlist_link_priv_s * p_anchor);

/** Public functions */

knitlist_s * knitlist_new(void)
{
    knitlist_s * p_list = malloc(sizeof(knitlist_s));
    KNITLIST_ASSERT(NULL == p_list, "Allocation error", return NULL);

    p_list->p_priv = malloc(sizeof(knitlist_priv_s));
    KNITLIST_ASSERT(NULL == p_list->p_priv, "Allocation error", free(p_list); return NULL);

    return p_list;
}

knitlist_s * knitlist_copy(knitlist_s * p_list)
{
    KNITLIST_ASSERT(NULL == p_list, 
            "Null pointer argument", 
            return NULL);
    KNITLIST_ASSERT(NULL == p_list->p_priv, 
            "Corrupted argument", 
            return NULL);

    knitlist_s * p_copy = knitlist_new();

    uint32_t len = knitlist_len_get(p_list);
    for (uint32_t temp = 0; temp < len; temp++)
    {
        knitlist_link_add(p_copy, knitlist_link_get(p_list, temp));
    }

    return p_copy;
}

void knitlist_free(knitlist_s * p_list)
{
    KNITLIST_ASSERT(NULL == p_list, 
            "Null pointer argument", 
            return);
    KNITLIST_ASSERT(NULL == p_list->p_priv, 
            "Corrupted argument", 
            return);

    uint32_t len = knitlist_len_get(p_list);
    for (uint32_t temp = 0; temp < len; temp++)
    {
        knitlist_link_remove(p_list, 0);
    }

    free(p_list->p_priv);
    free(p_list);
}


void knitlist_link_add(knitlist_s * p_list, knitlist_link_s * p_link)
{
    KNITLIST_ASSERT((NULL == p_list) || (NULL == p_link), 
            "Null pointer argument", 
            return);
    KNITLIST_ASSERT((NULL == p_list->p_priv) || (NULL == p_link->p_val), 
            "Corrupted argument", 
            return);
    
    uint32_t len = knitlist_len_get(p_list);
    if (0 == len)
    {
        knitlist_anchorlink_set(p_list, knitlist_link_mem_alloc(p_link));
        knitlist_len_inc(p_list);
        return;
    }

    knitlist_link_priv_s * p_last = knitlist_anchorlink_get(p_list);
    for (uint32_t temp = 1; temp < len; temp++)
    {
        p_last = p_last->p_next;
    }

    p_last->p_next = knitlist_link_mem_alloc(p_link);
    knitlist_len_inc(p_list);
}

knitlist_link_s * knitlist_link_get(knitlist_s * p_list, uint32_t idx)
{
    KNITLIST_ASSERT(NULL == p_list, 
            "Null pointer argument", 
            return NULL);
    KNITLIST_ASSERT(NULL == p_list->p_priv, 
            "Corrupted argument", 
            return NULL);
    uint32_t len = knitlist_len_get(p_list);
    KNITLIST_ASSERT((len - 1) < idx, 
            "Invalid index", 
            return NULL);
    KNITLIST_ASSERT(0 == len, 
            "Invalid index", 
            return NULL);
    
    knitlist_link_priv_s * p_required = knitlist_anchorlink_get(p_list);
    for (uint32_t temp = 0; temp < idx; temp++)
    {
        p_required = p_required->p_next;
    }

    return &p_required->data;
}

void knitlist_link_move(knitlist_s * p_list, uint32_t idx_from, uint32_t idx_to)
{
    KNITLIST_ASSERT(NULL == p_list, 
            "Null pointer argument", 
            return);
    KNITLIST_ASSERT(NULL == p_list->p_priv, 
            "Corrupted argument", 
            return);
    uint32_t len = knitlist_len_get(p_list);
    KNITLIST_ASSERT((len - 1) < idx_from, 
            "Invalid index", 
            return);
    KNITLIST_ASSERT((len - 1) < idx_to, 
            "Invalid index", 
            return);

    /* Removing link from original position */

    knitlist_link_priv_s * p_moving = knitlist_anchorlink_get(p_list);
    knitlist_link_priv_s * p_prev = knitlist_anchorlink_get(p_list);
    if (0 == idx_from)
    {
        knitlist_anchorlink_set(p_list, p_moving->p_next);
    }
    else
    {
        for (uint32_t temp = 0; temp < idx_from; temp++)
        {
            p_prev = p_moving;
            p_moving = p_moving->p_next;
        }
        p_prev->p_next = p_moving->p_next;
    }

    if (0 == idx_to)
    {
        p_moving->p_next = knitlist_anchorlink_get(p_list);
        knitlist_anchorlink_set(p_list, p_moving);
    }
    else
    {
        knitlist_link_priv_s * p_new_next = knitlist_anchorlink_get(p_list);
        p_prev = knitlist_anchorlink_get(p_list);
        for (uint32_t temp = 0; temp < idx_to; temp++)
        {
            p_prev = p_new_next;
            p_new_next = p_new_next->p_next;
        }
        p_prev->p_next = p_moving;
        p_moving->p_next = p_new_next;
    }
}

void knitlist_link_remove(knitlist_s * p_list, uint32_t idx)
{
    KNITLIST_ASSERT(NULL == p_list, 
            "Null pointer argument", 
            return);
    KNITLIST_ASSERT(NULL == p_list->p_priv, 
            "Corrupted argument", 
            return);
    uint32_t len = knitlist_len_get(p_list);
    KNITLIST_ASSERT((len - 1) < idx, 
            "Invalid index", 
            return);
    KNITLIST_ASSERT(0 == len, 
            "Invalid index", 
            return);
    
    if (0 == idx)
    {
        knitlist_anchorlink_set(p_list, knitlist_link_mem_free(knitlist_anchorlink_get(p_list))),
        knitlist_len_dec(p_list);
        return;
    }

    knitlist_link_priv_s * p_required = knitlist_anchorlink_get(p_list);
    knitlist_link_priv_s * p_prev = p_required;
    for (uint32_t temp = 0; temp < idx; temp++)
    {
        p_prev = p_required;
        p_required = p_required->p_next;
    }

    p_prev->p_next = knitlist_link_mem_free(p_required);
    knitlist_len_dec(p_list);
}


int32_t knitlist_len_get(knitlist_s * p_list)
{
    KNITLIST_ASSERT(NULL == p_list, "Null pointer argument", return -1);

    knitlist_priv_s * p_list_priv = (knitlist_priv_s *) p_list->p_priv;
    KNITLIST_ASSERT(NULL == p_list_priv, "Corrupted argument", return -1);

    return p_list_priv->len;
}

/** Private functions */

static knitlist_link_priv_s * knitlist_link_mem_alloc(knitlist_link_s * p_link)
{
    KNITLIST_ASSERT(NULL == p_link, "Null pointer argument", return NULL);

    knitlist_link_priv_s * p_link_priv = malloc(sizeof(knitlist_link_priv_s));
    KNITLIST_ASSERT(NULL == p_link_priv, "Allocation error", return NULL);

    p_link_priv->data.p_val = malloc(p_link->size);
    KNITLIST_ASSERT(NULL == p_link_priv->data.p_val, "Allocation error", free(p_link_priv); return NULL);

    memcpy(p_link_priv->data.p_val, p_link->p_val, p_link->size);
    p_link_priv->data.size = p_link->size;

    return p_link_priv;
}

static knitlist_link_priv_s * knitlist_link_mem_free(knitlist_link_priv_s * p_link_priv)
{
    KNITLIST_ASSERT(NULL == p_link_priv, "Null pointer argument", return NULL);
    KNITLIST_ASSERT(NULL == p_link_priv->data.p_val, "Corrupted argument", return NULL);

    knitlist_link_priv_s * p_next = p_link_priv->p_next;
    free(p_link_priv->data.p_val);
    free(p_link_priv);
    return p_next;
}

static void knitlist_len_inc(knitlist_s * p_list)
{
    KNITLIST_ASSERT(NULL == p_list, "Null pointer argument", return);

    knitlist_priv_s * p_list_priv = (knitlist_priv_s *) p_list->p_priv;
    KNITLIST_ASSERT(NULL == p_list_priv, "Corrupted argument", return);

    p_list_priv->len++;
}

static void knitlist_len_dec(knitlist_s * p_list)
{
    KNITLIST_ASSERT(NULL == p_list, "Null pointer argument", return);

    knitlist_priv_s * p_list_priv = (knitlist_priv_s *) p_list->p_priv;
    KNITLIST_ASSERT(NULL == p_list_priv, "Corrupted argument", return);

    p_list_priv->len--;
}

static knitlist_link_priv_s * knitlist_anchorlink_get(knitlist_s * p_list)
{
    KNITLIST_ASSERT(NULL == p_list, 
            "Null pointer argument", 
            return NULL);
    KNITLIST_ASSERT(NULL == p_list->p_priv, 
            "Corrupted argument", 
            return NULL);
    knitlist_priv_s * p_list_priv = (knitlist_priv_s *) p_list->p_priv;

    return p_list_priv->p_anchor;
}

static void knitlist_anchorlink_set(knitlist_s * p_list, knitlist_link_priv_s * p_anchor)
{
    KNITLIST_ASSERT(NULL == p_list, 
            "Null pointer argument", 
            return);
    KNITLIST_ASSERT(NULL == p_list->p_priv, 
            "Corrupted argument", 
            return);
    knitlist_priv_s * p_list_priv = (knitlist_priv_s *) p_list->p_priv;

    p_list_priv->p_anchor = p_anchor;
}
