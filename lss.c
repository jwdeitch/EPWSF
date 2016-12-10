#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <memory.h>
#include <limits.h>
#include <unistd.h>

const int list_page_size = 100;

struct stat statbuf;
struct dirent *dir_obj;
struct stat *s;
struct dirent *list_array;

int main(int argc, char *argv[]) {
    char *curr_dir;

    if (argc == 1) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            curr_dir = cwd;
        } else {
            printf("invalid path");
            return 1;
        }
    } else {
        char resolved_path[PATH_MAX];
        if (realpath(argv[1], resolved_path) != NULL) {
            curr_dir = resolved_path;
        } else {
            printf("invalid path");
            return 1;
        }
    }

}

struct dirent* exploreDir(char* curr_dir) {
    DIR *dirp;
    dirp = opendir(curr_dir);

    list_array = (struct dirent *) calloc(list_page_size, sizeof(struct dirent));
    int alloc_count = 0;

    while ((dir_obj = readdir(dirp)) != NULL) {
        stat(dir_obj->d_name, &statbuf);
        char *objName = dir_obj->d_name;

        if (strcmp(objName, ".") == 0 || strcmp(objName, "..") == 0) {
            continue;
        }

        if (alloc_count < list_page_size) {
            list_array[alloc_count] = *dir_obj;
            alloc_count++;
        } else {
            list_array = (struct dirent *) realloc(list_array,
                                                   sizeof(struct dirent) * (alloc_count / list_page_size) * 2);
        }


        printf("%s\n", list_array[0].d_name);
    }
}