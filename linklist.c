#include "header.h"

node_t* create_node(char *name, char* arg)
{
    node_t* new_node = (node_t*)malloc(sizeof(node_t));

    int len = strlen(name);
    new_node->name = malloc(sizeof(char) * (len + 1));
    strcpy(new_node->name, name);

    struct stat sb;

    if (name != arg) {
        char* path_buffer = malloc(100);
        create_unique_path(path_buffer, name, arg);
        lstat(path_buffer, &sb);

        len = strlen(path_buffer);
        new_node->path = (char*)malloc(sizeof(char) * (len + 1));
        strcpy(new_node->path, path_buffer);

        free(path_buffer);
    } else 
    {
        lstat(arg, &sb);
        new_node->path = NULL;
    }

    long *tv_sec = &sb.st_mtimespec.tv_sec;
    long *tv_nsec = &sb.st_mtimespec.tv_nsec;
    long sec = *tv_sec;
    long nsec = *tv_nsec;

    new_node->time = sec;
    new_node->nano = nsec;
    new_node->next = NULL;

    return new_node;
}


void insert_at_head(node_t **head, node_t *node_to_insert) 
{
  node_to_insert->next = *head;
  *head = node_to_insert;
}


void swap_nodes(node_t *x, node_t *y) 
{
    char *temp_name = x->name;
    char *temp_path = x->path;
    time_t temp_time = x->time;
    long temp_nano = x->nano;

    x->name = y->name;
    x->path = y->path;
    x->time = y->time;
    x->nano = y->nano;

    y->name = temp_name;
    y->path = temp_path;
    y->time = temp_time;
    y->nano = temp_nano;
}


void print_list(node_t* head)
{
    node_t* temp = head;

    while (temp != NULL) {
        printf("%s\n", temp->name);
        temp = temp->next;
    }
}


void free_list(node_t *head) 
{
  node_t *tmp = NULL;
  while (head != NULL) {
    tmp = head;
    head = head->next;
    free(tmp->name);
    free(tmp->path);
    free(tmp);
  }
}
