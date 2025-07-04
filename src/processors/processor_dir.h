#ifndef PROCESSOR_DIR 
#define PROCESSOR_DIR

#include <stdbool.h>

typedef void *Dir; 
typedef char *Str; 

/* 
 * Create a new directory based on the given path
 * @param full_dir the path to create the dir
 * @return Dir the created directory
*/
Dir new_dir(char *full_dir);

/*
 * Combines given path and file into a Dir
 * @param path the path for the file
 * @param file the file name with the extension
 * @return Dir the created dir based on the given params
*/
Dir dir_combine_path_and_file(char *path, char *file);

/* 
 * Puts the full Dir path on the given string
 * @param d the directory to ther the path
 * @param path the string to receive the full path
*/
void get_full_dir(Dir d, char *path);

/* 
 * Get the dir path 
 * @param d the directory to get the path
 * @return str (char*) the dir path
*/
char* get_dir_path(Dir d);

/* 
 * Get the file name
 * @param d the directory to get the path
 * @return str (char*) the file name of the dir
*/
char* get_dir_file_name(Dir d);

/* 
 * Get the file extension
 * @param d the directory to get the path
 * @return str (char*) the file extension of the dir
*/
char* get_dir_file_extension(Dir d);

/* 
 * Checks if the dir file exists
 * @return str (char*) the file name of the dir
 * @return bool true if the file exists, false otherwise
*/
bool dir_file_exists(Dir d); 

/* 
 * Free the given directory from memory
 * @param d the directory to be freed 
*/
void dir_free(Dir d); 

#endif