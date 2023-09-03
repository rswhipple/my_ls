#include "header.h"

int my_getopt(int argc, char **argv, flag_t *flag) 
{
    flag->OPT_a = 0;
    flag->OPT_t = 0;

    int c;

    while ((c = check_for_flags(argc, argv, "at")) != -1) {
        switch (c) {
            case 'a':
                flag->OPT_a = 1;
                break;
            case 't':
                flag->OPT_t = 1;
                break;
            case '?':
                return EXIT_FAILURE;
            case '\0':
                break; // Move to next argument, no options
        }
    }

    return EXIT_SUCCESS;
}

int check_for_flags(int argc, char *argv[], char *optstring) {
    static int optind = 1;  // Index of the next argument to process
    static int optpos = 1;  // Position within the current argument

    if (optind >= argc) {
        return -1;  // No more options
    }

    if (argv[optind][0] != '-' || argv[optind][1] == '\0') {
        optind += 1;
        return '\0';  // Non-option argument
    }

    // Get the current option character
    char current_opt = argv[optind][optpos++];

    // Find if the current option character is in the optstring
    const char *optchar = strchr(optstring, current_opt);
    if (optchar == NULL) {
        fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], current_opt);
        optind++;
        optpos = 1;
        return '?';
    }

    if (argv[optind][optpos] == '\0') {
        optind++;
        optpos = 1;
    }

    return current_opt;
}

int parse_arguments(int argc, char **argv, arg_t* args) // parse arguments
{
    args->file_head = NULL;
    args->directory_head = NULL;
    args->file_count = 0;
    args->directory_count = 0;
    node_t *new_node;

    int i = 1;
    int type = 0;

    while (i < argc) {
        if (argv[i][0] != '-') {
            type = check_file_type(argv[i]);
            if (type == -1) {
                printf("my_ls: cannot access '%s': No such file or directory\n", argv[i]);
                return EXIT_FAILURE;
            }
            else if (type == 0) {   // If the file is NOT a directory
                new_node = create_node(argv[i], argv[i]);
                insert_at_head(&args->file_head, new_node);
                args->file_count += 1;
            } 
            else if (type == 1) {       // If the file IS a directory  
                new_node = create_node(argv[i], argv[i]);
                insert_at_head(&args->directory_head, new_node);
                args->directory_count += 1;
            }
        }
        i += 1;
    }

    return 0;
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
