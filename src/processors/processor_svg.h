#ifndef PROCESSOR_SVG_H
#define PROCESSOR_SVG_H

/*
 * @file: processor_svg.h
 * @brief: svg exporter implementation

 * This module export forms in svg file, the svg exporter instance is unique
 * This module don't free the forms 
 * @author: Gustavo Freire
*/
#include "processor_dir.h"
#include "../data_structures/list.h"

typedef void *SvgExporter;

/*
 * Retrieve the svg exporter instance
 * @return SvgExporter
*/
SvgExporter svg_exporter_init();

/*
 * Export the svg file
*/
void svg_export_forms(); 

/*
 * Set the directory for svg exportation
 * @param "Dir" the directory to be used
*/
void svg_set_directory(Dir newDir); 

/*
 * Insert forms list to exportation
 * @param "List" the list of forms
*/
void svg_add_forms(List forms); 

/*
 * Clear all forms on the export list 
*/
void svg_reset_forms_list(); 


#endif
