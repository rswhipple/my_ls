#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h> 
#include "header.h"

int main(int argc, char *argv[]) // read flags
{
    // flag variables
    int include_dot = 0;
    int include_time = 0;

    // check for flags
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-a") == 0) {
                include_dot = 1;
            } else if (strcmp(argv[i], "-t") == 0) {
                include_time = 1;
            } else if (strcmp(argv[i], "-at") == 0) {
                include_dot = 1;
                include_time = 1;
            } else if (strcmp(argv[i], "-ta") == 0) {
                include_dot = 1;
                include_time = 1;
            } 
        }
    }

    parse_arguments(argc, argv, include_dot, include_time);

    return 0;

}

int parse_arguments(int argc, char **argv, int dot, int time) // parse arguments
{
    node_t *directory_head = NULL;
    node_t *file_head = NULL;
    node_t *new_node;
    int directory_count = 0;
    int file_count = 0;
    int i = 1;
    int type = 0;

    while (i < argc) {
        if (strcmp(argv[i], "-a") != 0 && strcmp(argv[i], "-t") != 0 && strcmp(argv[i], "-at") != 0 && strcmp(argv[i], "-ta") != 0){
            type = check_file_type(argv[i]);
            if (type == -1) {
                return 0;
            }
            else if (type == 0) {
                char *file_name = argv[i];
                new_node = create_node(file_name, file_name);
                insert_at_head(&file_head, new_node);
                file_count += 1;
            } 
            else if (type == 1) {    
                char *dir_name = argv[i];   
                new_node = create_node(dir_name, dir_name);
                insert_at_head(&directory_head, new_node);
                directory_count += 1;
            }
        }
        i += 1;
    }

    my_ls(directory_head, file_head, dot, time, directory_count, file_count);

    free_list(file_head);
    free_list(directory_head);

    return 0;
}

void my_ls(node_t *directory_head, node_t *file_head, int dot, int time, int directory_count, int file_count) 
{
    node_t *new_node;

    if (directory_count == 0 && file_count == 0) {
        char *path = ".";
        new_node = create_node(path, path);
        insert_at_head(&directory_head, new_node);
        get_directory_list(directory_head, dot, time, directory_count);
        return;
    }

    if (file_count > 0) {
        if (time > 0) // -t 
        {
            sort_list_by_time(file_head);
        } else 
        {
         sort_list(file_head);
        }
        print_list(file_head);
        if (directory_count > 0) {
            printf("\n");
        }
    }

    if (directory_count > 1) {
        if (time > 0) // -t 
        {
            sort_list_by_time(directory_head);
        } else 
        {
            sort_list(directory_head);
            // print_list(directory_head);  TESTING ONLY
        }
        get_directory_list(directory_head, dot, time, directory_count);
    }

    if (directory_count == 1) {
        get_directory_list(directory_head, dot, time, directory_count);
    }
    
}

int check_file_type(char* arg) 
{
    struct stat st;

    if (lstat(arg, &st) == -1) {
        printf("my_ls: cannot access '%s': No such file or directory\n", arg);
        return -1;
    }

    switch (st.st_mode & S_IFMT) {
        case S_IFBLK: break;
        case S_IFCHR: break;
        case S_IFDIR: return 1;
        case S_IFIFO: break;
        case S_IFLNK: break;
        case S_IFREG: break;
        case S_IFSOCK: break;
        default: break;
    }

    return 0;
}

void get_directory_list(node_t *head, int dot, int time, int count)
{
    while (head != NULL) {
        DIR *dir;
        struct dirent *entry;
        dir = opendir(head->name);

        node_t *sub_head = NULL;
        node_t *new_node;
        errno = 0;

        if (dot > 0) // if -a
        {
            while ((entry = readdir(dir)) != NULL) {
            new_node = create_node(entry->d_name, head->name);
            insert_at_head(&sub_head, new_node);
            }
        } else
        {
            while ((entry = readdir(dir)) != NULL) {
            char *tmp = entry->d_name;
                if (tmp[0] != '.') {
                    new_node = create_node(tmp, head->name);
                    insert_at_head(&sub_head, new_node);
                }
            }
        } 

        sort_directory_list(sub_head, time);

        print_directory_list(head, sub_head, count);

        free_list(sub_head);
        closedir(dir);

        head = head->next;
    }
    
}

void sort_directory_list(node_t *sub_head, int time) {
    if (time) {
        sort_list_by_time(sub_head);
    } 
    else {
        sort_list(sub_head);
    } 
}


void print_directory_list(node_t *head, node_t *sub_head, int count) 
{
    if (count > 1 && head->next != NULL) {
        printf("%s:\n", head->name);
        print_list(sub_head);
        printf("\n");
    } 
    else if (count > 1 && head != NULL) {
        printf("%s:\n", head->name);
        print_list(sub_head);
    } 
    else {
        print_list(sub_head);
    }
}

void create_unique_path(char* dst, char *data, char *arg) 
{
    int i = 0; 
    int j = 0;

    while (arg[i] != '\0') {
        dst[i] = arg[i];
        i += 1;
    }

    dst[i] = '/';
    i += 1;
    
    while (data[j] != '\0') {
        dst[i] = data[j];
        i += 1;
        j += 1;
    }

    dst[i] = '\0';
}

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

void swap_nodes(node_t *x, node_t *y) {
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

void free_list(node_t *head) {
  node_t *tmp = NULL;
  while (head != NULL) {
    tmp = head;
    head = head->next;
    free(tmp->name);
    free(tmp->path);
    free(tmp);
  }
}
