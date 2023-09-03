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

/* function prototypes */
int parse_arguments(int argc, char **argv, int dot, int time);
void my_ls(node_t *directory_head, node_t *file_head, int dot, int time, int directory_count, int file_count);
int check_file_type(char *arg);
void get_directory_list(node_t *head, int dot, int time, int count);
node_t *create_node(char* name, char* arg);
void create_unique_path(char* dst, char *data, char *arg);
void insert_at_head(node_t **head, node_t *node_to_insert);
void sort_directory_list(node_t *sub_head, int time);
void sort_list(node_t *head);
void sort_list_by_time(node_t *head);
void swap_nodes(node_t *a, node_t *b);
void print_directory_list(node_t *head, node_t *sub_head, int count);
void print_list(node_t *head);
void free_list(node_t *head);
