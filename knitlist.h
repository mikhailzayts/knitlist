/**
 *  @file   knitlist.h
 *  @brief  One-sided linked list module
 *
 *  @author Mikhail Zaytsev
 *  @date   20230814
 */

#ifndef __SVYAZLIST_H__
#define __SVYAZLIST_H__

/** Includes */

#include <stdint.h>
#include <stddef.h>

/** Structures and types */

/**<  List structure */
typedef struct knitlist_s 
{
    void * p_priv;      /**< Pointer to a private content. You should not touch this pointer */
} knitlist_s;

/**<  Link data structure */
typedef struct knitlist_link_s 
{
    void * p_val;   /**< Pointer to a link value */
    size_t size;    /**< Link value size */
} knitlist_link_s;

/** Public functions prototypes */

knitlist_s * knitlist_new(void);
knitlist_s * knitlist_copy(knitlist_s * p_list);
void knitlist_free(knitlist_s * p_list);

void knitlist_link_add(knitlist_s * p_list, knitlist_link_s * p_link);
knitlist_link_s * knitlist_link_get(knitlist_s * p_list, uint32_t idx);
void knitlist_link_move(knitlist_s * p_list, uint32_t idx_from, uint32_t idx_to);
void knitlist_link_remove(knitlist_s * p_list, uint32_t idx);

int32_t knitlist_len_get(knitlist_s * p_list);

#endif // __SVYAZLIST_H__
