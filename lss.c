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
char *curr_dir;

struct dirent *exploreDir(char *curr_dir);
void listdir(const char *name, int level);

int main(int argc, char *argv[]) {

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


    listdir(curr_dir, 0);

}

int explore(int argc, char *argv[]) {
    exploreDir(curr_dir);

    for(int i = 0; list_array[i].d_ino != 0; i++) {
        if (list_array[i].d_type == '\x04') {

        }
        printf("%s\n", list_array[i].d_name);
    }
}

struct dirent *exploreDir(char *curr_dir) {
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
    }

    return list_array;
}

void listdir(const char *name, int level)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            printf("%*s[%s]\n", level*2, "", entry->d_name);
            listdir(path, level + 1);
        }
        else
            printf("%*s- %s\n", level*2, "", entry->d_name);
    } while (entry = readdir(dir));
    closedir(dir);
} // http://stackoverflow.com/a/8438663/4603498
