#include "knitlist.h"

#include "stdio.h"

static void print_list_content(knitlist_s * p_list);

int main()
{
    knitlist_s * p_list = knitlist_new();

    knitlist_link_add(p_list, &(knitlist_link_s){.p_val = "Hello world", .size = 12});
    knitlist_link_add(p_list, &(knitlist_link_s){.p_val = "Some string", .size = 12});
    knitlist_link_add(p_list, &(knitlist_link_s){.p_val = "Another text for linked list", .size = 29});
    print_list_content(p_list);

    knitlist_link_remove(p_list, 1);
    print_list_content(p_list);

    knitlist_link_remove(p_list, 0);
    print_list_content(p_list);

    knitlist_link_remove(p_list, 0);
    print_list_content(p_list);

    knitlist_free(p_list);

    return 0;
}

static void print_list_content(knitlist_s * p_list)
{
    knitlist_link_s * p_link = NULL;
    for (uint32_t idx = 0; idx < knitlist_len_get(p_list); idx++)
    {
        p_link = knitlist_link_get(p_list, idx);
        printf("Link %d: %d bytes: \"%s\"\n", idx, (uint32_t)p_link->size, (char *)p_link->p_val);
    }

}
