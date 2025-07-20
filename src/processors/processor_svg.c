#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "processor_svg.h"
#include "processor_dir.h"
#include "../forms/form.h"
#include "../utils/file.h"

typedef struct SvgExporter_st {
    List forms; 
    Dir export_dir; 
} SvgExporter_st; 

static SvgExporter_st *exporter_instance = NULL;

SvgExporter svg_exporter_init( ) {
    if (exporter_instance == NULL) {
        exporter_instance = (SvgExporter_st *) malloc(sizeof(SvgExporter_st));
        if (exporter_instance == NULL) return NULL; 

        exporter_instance->forms = new_list(); 
        exporter_instance->export_dir = new_dir("./output.svg");
    }

    return exporter_instance;
}

void svg_set_directory(Dir newDir) {
    assert(newDir);
    assert(exporter_instance);

    if (exporter_instance == NULL) return; 

    if (exporter_instance->export_dir != NULL) {
        dir_free(exporter_instance->export_dir); 
    }

    exporter_instance->export_dir = newDir; 
}

void svg_add_forms(List forms) {
    assert(forms);
    assert(exporter_instance);
    assert(exporter_instance->forms);

    list_copy(forms, exporter_instance->forms);
}

void svg_reset_forms_list() {
    assert(exporter_instance);

    if (exporter_instance->forms == NULL) return; 
    list_free(exporter_instance->forms, NULL); 
}

static void svg_export_circle(FILE *svg_file, int form_id, double x, double y, double r, FormStyle style) {
    fprintf(svg_file, 
        "\t<circle id='%d' r='%lf' cx='%lf' cy='%lf' fill='%s' stroke='%s' stroke-width='%s' fill-opacity='0.5'/>\n",
        form_id, r, x, y, get_form_style_fill_color(style), get_form_style_border_color(style), get_form_style_stroke_width(style)
    );     
}

static void svg_export_rectangle(FILE *svg_file, int form_id, double x, double y, double w, double h, FormStyle style) {
    fprintf(svg_file, 
        "\t<rect id='%d' width='%lf' height='%lf' x='%lf' y='%lf' fill='%s' stroke='%s' stroke-width='%s' fill-opacity='0.5'/>\n",
        form_id, w, h, x, y, get_form_style_fill_color(style), get_form_style_border_color(style), get_form_style_stroke_width(style)
    ); 
}

static void svg_export_text(FILE *svg_file, int form_id, double x, double y, char *text, FormStyle style) {
    fprintf(svg_file, 
        "\t<text id='%d' x='%lf' y='%lf' fill='%s' stroke='%s' stroke-width='%s' text-anchor='%s' font-weight='%s' font-size='%s' font-family='%s'><![CDATA[ %s ]]></text>\n",
        form_id, x, y, get_form_style_fill_color(style), get_form_style_border_color(style), get_form_style_stroke_width(style), 
        get_form_style_text_anchor(style), get_form_style_font_weight(style), get_form_style_font_size(style), get_form_style_font_family(style), text
    ); 
}

static void svg_export_line(FILE *svg_file, int form_id, double x1, double y1, double x2, double y2, FormStyle style) {
    fprintf(svg_file, 
        "\t<line id='%d' x1='%lf' y1='%lf' x2='%lf' y2='%lf' stroke='%s' stroke-width='%s'/>\n",
        form_id, x1, y1, x2, y2, get_form_style_border_color(style), get_form_style_stroke_width(style)
    );
}

static void export_form(void *value, callback_data call_data) {
    Form form = (Form) value; 
    FILE *svg_file = call_data; 
    assert(form); 
    assert(call_data);

    FormType form_type = form_get_type(form); 
    FormStyle form_style = form_get_style(form);
    int form_id = form_get_id(form); 
    char *form_text = form_get_text(form);

    double x, y, wr, h; 
    form_get_coordinates(form, &x, &y);
    form_get_dimensions(form, &wr, &h);
    
    switch (form_type) {
        case CIRCLE: 
            svg_export_circle(svg_file, form_id, x, y, wr, form_style);
            break; 
        case RECT: 
            svg_export_rectangle(svg_file, form_id, x, y, wr, h, form_style);
            break; 
        case LINE: 
            svg_export_line(svg_file, form_id, x, y, wr, h, form_style);
            break; 
        case TEXT: 
            svg_export_text(svg_file, form_id, x, y, form_text, form_style);
            break;
        default:
            fprintf(stderr, "[processor_svg] Error: invalid form type, skipping exportation (%d)\n", form_type);
    }
}

static void svg_export_header(FILE *svg_file) {
    assert(svg_file);
    fprintf(svg_file, "<svg xmlns='http://www.w3.org/2000/svg'>\n"); 
}

static void svg_close(FILE *svg_file) {
    assert(svg_file);
    fprintf(svg_file, "</svg>");
}

static void svg_export_forms_list(FILE *svg_file) {
    assert(exporter_instance);
    if (exporter_instance->export_dir == NULL || exporter_instance->forms == NULL) {
        fprintf(stderr, "[processor_svg] Error: export directory or forms list were not defined"); 
    }
    
    list_foreach(exporter_instance->forms, &export_form, svg_file); 
}

void svg_export_forms() {
    assert(exporter_instance); 
    if (exporter_instance->export_dir == NULL) return; 

    FILE *svg_file = file_open_writable(exporter_instance->export_dir); 
    if (svg_file == NULL) return; 

    svg_export_header(svg_file);
    svg_export_forms_list(svg_file);
    svg_close(svg_file);
}