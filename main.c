#include "svyazlist.h"

#include "stdio.h"

static void print_list_content(svyazlist_s * ptr_list);

int main()
{
    svyazlist_s * ptr_list = svyazlist_new();

    svyazlist_link_add(ptr_list, &(svyazlist_link_s){.ptr_value = "Hello world", .size = 12});
    svyazlist_link_add(ptr_list, &(svyazlist_link_s){.ptr_value = "Some string", .size = 12});
    svyazlist_link_add(ptr_list, &(svyazlist_link_s){.ptr_value = "Another text for linked list", .size = 29});
    print_list_content(ptr_list);

    svyazlist_link_remove(ptr_list, 1);
    print_list_content(ptr_list);

    svyazlist_link_remove(ptr_list, 0);
    print_list_content(ptr_list);

    svyazlist_link_remove(ptr_list, 0);
    print_list_content(ptr_list);

    svyazlist_free(ptr_list);

    return 0;
}

static void print_list_content(svyazlist_s * ptr_list)
{
    svyazlist_link_s * ptr_link = NULL;
    for (uint32_t idx = 0; idx < svyazlist_len_get(ptr_list); idx++)
    {
        ptr_link = svyazlist_link_get(ptr_list, idx);
        printf("Link %d: %d bytes: \"%s\"\n", idx, (uint32_t)ptr_link->size, (char *)ptr_link->ptr_value);
    }

}
