#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h> 


/* struct definitions */
#ifndef STRUCT_NODE
#define STRUCT_NODE
struct node {
  char *name;
  char *path;
  long time;
  long nano;
  struct node *next;
};
typedef struct node node_t;
#endif

#ifndef STRUCT_FLAG_S_
#define STRUCT_FLAG_S_
struct flag_s {
    int OPT_a;
    int OPT_t;
}; typedef struct flag_s flag_t;
#endif

#ifndef STRUCT_ARG_S_
#define STRUCT_ARG_S_
struct arg_s {
    int file_count;
    int directory_count;
    node_t *file_head;
    node_t *directory_head;
}; typedef struct arg_s arg_t;
#endif

/* function prototypes in my_ls.c */
void my_ls(arg_t* args, flag_t* flag);
void get_directory_list(arg_t* args, flag_t* flag);
void create_unique_path(char* dst, char *data, char *arg);
void print_directory_list(node_t *head, node_t *sub_head, int count);

/* function prototypes in linklist.c */
node_t *create_node(char* name, char* arg);
void insert_at_head(node_t **head, node_t *node_to_insert);
void swap_nodes(node_t *a, node_t *b);
void print_list(node_t *head);
void free_list(node_t *head);

/* function prototypes in sort.c */
void sort_directory_list(node_t *sub_head, int time);
void sort_list(node_t *head);
void sort_list_by_time(node_t *head);

/* function prototypes in parser.c */
int my_getopt(int argc, char **argv, flag_t *flag);
int check_for_flags(int argc, char *argv[], char *optstring);
int parse_arguments(int argc, char **argv, arg_t* args);
int check_file_type(char* arg);

