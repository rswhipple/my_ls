#include "header.h"


void sort_directory_list(node_t *sub_head, int time) 
{
    if (time) {
        sort_list_by_time(sub_head);
    } 
    else {
        sort_list(sub_head);
    } 
}


void sort_list(node_t *head) // lexicographical bubble sort
{
    node_t* curr = head;
    node_t* index = NULL;

    if (head == NULL) {
        return;
    } else {
        while (curr != NULL) {
            index = curr->next;

            while(index != NULL) {  
                    if(strcmp(curr->name, index->name) > 0) {  
                        swap_nodes(curr, index); 
                    }  
                    index = index->next;  
                }  
            curr = curr->next;
        }
    }
}


void sort_list_by_time(node_t *head) // time descending bubble sort
{
    node_t* curr = head;
    node_t* index = NULL;

    if (curr == NULL) {
        return;
    } 
    
    while (curr->next != NULL) {
        index = curr->next;
        while (index != NULL) {
            if ((curr->time < index->time) || (curr->time == index->time && curr->nano < index->nano)) {
                swap_nodes(curr, index);
            }
            if (curr->time == index->time && curr->nano == index->nano) {
                if(strcmp(curr->name, index->name) > 0) {  
                    swap_nodes(curr, index);
                }  
            }
            index = index->next;
        }
        curr = curr->next;
    }
}
