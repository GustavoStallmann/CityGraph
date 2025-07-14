#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "file.h"
#include "processor_dir.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

bool file_exists(Dir d) {
    if (d == NULL) return false; 

    char full_dir[PATH_MAX]; 
    get_full_dir(d, full_dir); 

    FILE *file = fopen(full_dir, "r");
    if (file == NULL) return false; 
    
    fclose(file);
    return true; 
}

FILE* file_open_writable(Dir d) {
    assert(d); 

    char full_dir[PATH_MAX]; 
    get_full_dir(d, full_dir); 
    
    FILE *file = fopen(full_dir, "w"); 
    assert(file); 

    if (file == NULL) {
        fprintf(stderr, "file: couldn't open the file\n"); 
        return NULL; 
    }

    return file; 
}

FILE* file_open_readable(Dir d) {
    assert(d); 

    char full_dir[PATH_MAX]; 
    get_full_dir(d, full_dir); 
 
    FILE *file = fopen(full_dir, "r"); 

    if (file == NULL) {
        fprintf(stderr, "file: couldn't open the file\n"); 
        return NULL; 
    }

    return file; 
}

void file_close(FILE *file) {
    assert(file);
    if (file == NULL) return; 

    fclose(file);
}