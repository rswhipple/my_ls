#include "header.h"

int main(int argc, char *argv[]) // read flags
{
    int error = 0;
    flag_t *flag = (flag_t *)malloc(sizeof(flag_t));

    // check for flags
    error = my_getopt(argc, argv, flag);
    if (error) {
        free(flag);
        return EXIT_FAILURE;
    } 

    arg_t *args = (arg_t *)malloc(sizeof(arg_t));

    error = parse_arguments(argc, argv, args);
    if (error) {
        free(flag);
        free(args);
        return EXIT_FAILURE;
    } 

    my_ls(args, flag);

    free_list(args->file_head);
    free_list(args->directory_head);

    return 0;

}


void my_ls(arg_t* args, flag_t* flag) 
{
    node_t *new_node;

    // If no args are provided, list current directory
    if (args->directory_count == 0 && args->file_count == 0) {
        printf("no path provided\n");
        char *path = ".";
        new_node = create_node(path, path);
        insert_at_head(&args->directory_head, new_node);
        get_directory_list(args, flag);
    }

    // If files are provided, sort them lexicographically or in descending time order
    if (args->file_count > 0) {
        if (flag->OPT_t) // -t 
        {
            sort_list_by_time(args->file_head);
        } else 
        {
         sort_list(args->file_head);
        }
        // Print list to stdout
        print_list(args->file_head);
        // If there are directories, print a newline
        if (args->directory_count > 0) {
            printf("\n");
        }
    }

    // If more than 1 directory is provided, sort them lexicographically or in descending time order
    if (args->directory_count > 1) {
        if (flag->OPT_t) // -t 
        {
            sort_list_by_time(args->directory_head);
        } else 
        {
            sort_list(args->directory_head);
        }
        // 
        get_directory_list(args, flag);
    }

    if (args->directory_count == 1) {
        get_directory_list(args, flag);
    }
    
}


void get_directory_list(arg_t* args, flag_t* flag)
{
    node_t *head = args->directory_head;

    while (head != NULL) {
        DIR *dir;
        struct dirent *entry;
        dir = opendir(head->name);

        node_t *sub_head = NULL;
        node_t *new_node;
        errno = 0;

        if (flag->OPT_a > 0)
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

        sort_directory_list(sub_head, flag->OPT_t);

        print_directory_list(head, sub_head, args->directory_count);

        free_list(sub_head);
        closedir(dir);

        head = head->next;
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
