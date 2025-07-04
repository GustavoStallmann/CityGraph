#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "processor_dir.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef NAME_MAX
#define NAME_MAX 256
#endif

typedef struct {
    char *path, *fileName, *fileExt; 
} Dir_st; 

static void alloc_error() {
    fprintf(stderr, "[processor_dir] Error: insufficient memory for allocation\n");
    exit(EXIT_FAILURE);     
}

static char* alloc_str(char *str) {
    assert(str);
    if (str == NULL) return NULL; 

    size_t size = strlen(str) + 1; 
    char *new_str = (char *) malloc(sizeof(char) * size); //alloc a new str 
    if (new_str == NULL) alloc_error(); 

    strcpy(new_str, str);

    return new_str; 
}

static bool extract_path(char *full_dir, char *path) {
    assert(full_dir); 

    size_t full_dir_len = strlen(full_dir);
    if (full_dir_len <= 0) return false;
    
    int last_slash = -1;
    for (int i = full_dir_len - 1; i >= 0; i--) {
        if (full_dir[i] == '/') {
            last_slash = i;
            break;
        }
    }
    
    if (last_slash == -1) {
        strcpy(path, "");
    } else {
        strncpy(path, full_dir, last_slash + 1);
        path[last_slash + 1] = '\0';
    }
    
    return true;
}

static bool extract_filename(char *full_dir, char *fileName) {
    assert(full_dir);

    size_t full_dir_len = strlen(full_dir);
    if (full_dir_len <= 0) return false;
    
    int last_slash = -1;
    int last_dot = -1;
    
    for (int i = full_dir_len - 1; i >= 0; i--) {
        if (last_slash == -1 && full_dir[i] == '/') {
            last_slash = i;
        }
        if (last_dot == -1 && full_dir[i] == '.') {
            if (last_slash == -1 || i > last_slash) {
                last_dot = i;
            }
        }

        if (last_slash != -1 && last_dot != -1) break;
    }
    
    int filename_start = (last_slash != -1) ? last_slash + 1 : 0;
    int filename_end = (last_dot != -1) ? last_dot : (int) full_dir_len;
    
    if (filename_end > filename_start) {
        strncpy(fileName, full_dir + filename_start, filename_end - filename_start);
        fileName[filename_end - filename_start] = '\0';
    } else {
        strcpy(fileName, "");
    }
    
    return true;
}

static bool extract_extension(char *full_dir, char *fileExt) {
    assert(full_dir);
    assert(fileExt);

    size_t full_dir_len = strlen(full_dir);
    if (full_dir_len <= 0) return false;
    
    int last_slash = -1;
    int last_dot = -1;
    
    for (int i = full_dir_len - 1; i >= 0; i--) {
        if (last_slash == -1 && full_dir[i] == '/') {
            last_slash = i;
        }
        if (last_dot == -1 && full_dir[i] == '.') {
            if (last_slash == -1 || i > last_slash) {
                last_dot = i;
                break; 
            }
        }
    }
    
    if (last_dot != -1) {
        strcpy(fileExt, full_dir + last_dot + 1);
    } else {
        strcpy(fileExt, "");
    }
    
    return true;
}

static bool split_path(char *full_dir, char *path, char *fileName, char *fileExt) {
    if (!extract_path(full_dir, path)) return false;
    if (!extract_filename(full_dir, fileName)) return false;
    if (!extract_extension(full_dir, fileExt)) return false;
    
    return true;
}

Dir new_dir(char *full_dir) {
    assert(full_dir); 

    char path[PATH_MAX], fileName[NAME_MAX], fileExt[NAME_MAX]; 

    bool split_res = split_path(full_dir, path, fileName, fileExt);
    if (!split_res) return NULL; 

    Dir_st *dir = (Dir_st *) malloc(sizeof(Dir_st));
    if (dir == NULL) alloc_error();

    dir->path = alloc_str(path); 
    dir->fileExt = alloc_str(fileExt); 
    dir->fileName = alloc_str(fileName); 

    return (Dir) dir; 
}

Dir dir_combine_path_and_file(char *path, char *file) {
    assert(path);
    assert(file);
    char full_dir[PATH_MAX];

    size_t path_len = strlen(path);
    if (path_len == 0) {
        snprintf(full_dir, sizeof(full_dir), "%s", file);
    } else {
        char path_last_char = path[path_len - 1];
        if (path_last_char == '/') {
            snprintf(full_dir, sizeof(full_dir), "%s%s", path, file);
        } else {
            snprintf(full_dir, sizeof(full_dir), "%s/%s", path, file);
        }
    }

    return new_dir(full_dir);    
}

void get_full_dir(Dir d, char *path) {
    assert(d); 
    assert(path);
    
    Dir_st *dir = (Dir_st *) d; 
    int str_size = strlen(dir->path) + strlen(dir->fileName) + strlen(dir->fileExt) + 3;
    char *full_dir = (char *) malloc(sizeof(char) * str_size);

    char path_last_char = dir->path[strlen(dir->path) - 1];
    if (path_last_char == '/') {
        snprintf(full_dir, str_size, "%s%s.%s", dir->path, dir->fileName, dir->fileExt);
    } else {
        snprintf(full_dir, str_size, "%s/%s.%s", dir->path, dir->fileName, dir->fileExt);
    }

    strcpy(path, full_dir);
    free(full_dir);
}

char* get_dir_file_name(Dir d) {
    Dir_st *dir = (Dir_st *) d; 

    return dir->fileName; 
}

char* get_dir_file_extension(Dir d) {
    Dir_st *dir = (Dir_st *) d; 

    return dir->fileExt; 
}

char* get_dir_path(Dir d) {
    Dir_st *dir = (Dir_st *) d; 

    return dir->path; 
}

char* get_file_extension(Dir d) {
    Dir_st *dir = (Dir_st *) d; 

    return dir->fileExt; 
}

void dir_free(Dir d) {
    Dir_st *directory = (Dir_st *) d; 
    if (directory == NULL) return; 

    free(directory->path);
    free(directory->fileName);
    free(directory->fileExt);
    free(directory);
}