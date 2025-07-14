#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include "../processors/processor_dir.h" 

/*
 * Open or create a new writable file on the desired path
 * @param dir the file directory
 * @return FILE* the svg file reference
 */
FILE* file_open_writable(Dir d); 

/*
 * Open or create a new readable file on the desired path
 * @param dir the file directory
 * @return FILE* the svg file reference
 */
FILE* file_open_readable(Dir d);


/*
 * Checks if a file exist in the given directory
 * @param dir the file dir to be checked 
 * @return true if exists, false otherwise
 */
bool file_exists(Dir d); 

/*
 * Close the opened svg file
 * @param file the svg file
*/
void file_close(FILE *file); 

#endif