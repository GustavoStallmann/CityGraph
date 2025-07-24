#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processor_svg.h"
#include "processor_dir.h"
#include "../forms/form.h"
#include "../forms/form_animated.h"
#include "../city/point.h"
#include "../data_structures/list.h"
#include "../utils/file.h"

typedef struct SvgExporter_st {
    List forms; 
    Dir export_dir;
    int unique_id_counter; 
} SvgExporter_st; 

static SvgExporter_st *exporter_instance = NULL;

SvgExporter svg_exporter_init( ) {
    if (exporter_instance == NULL) {
        exporter_instance = (SvgExporter_st *) malloc(sizeof(SvgExporter_st));
        if (exporter_instance == NULL) return NULL; 

        exporter_instance->forms = new_list(); 
        exporter_instance->export_dir = new_dir("./output.svg");
        exporter_instance->unique_id_counter = 1;
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
    text = text != NULL? text: "INVALID"; 

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

typedef struct {
    FILE *svg_file;
    int is_first_point;
} path_export_data;

static void build_path_point(void *value, void *call_data) {
    Point point = (Point) value;
    path_export_data *data = (path_export_data *) call_data;
    
    if (point == NULL) return;
    
    double x, y;
    point_get_coordinates(point, &x, &y);
    
    if (data->is_first_point) {
        fprintf(data->svg_file, "M %lf %lf", x, y);
        data->is_first_point = 0;
    } else {
        fprintf(data->svg_file, " L %lf %lf", x, y);
    }
}

static void svg_export_animated_form(FILE *svg_file, int form_id, double x, double y, double r, FormStyle style, List path_points) {
    (void)x; 
    (void)y; 
    fprintf(svg_file, "\t<g id='animated_%d'>\n", form_id);
    
    if (path_points != NULL && list_get_size(path_points) > 1) {
        fprintf(svg_file, "\t\t<path id='path_%d' d='", form_id);
        
        path_export_data data = { .svg_file = svg_file, .is_first_point = 1 };
        list_foreach(path_points, build_path_point, &data);
        
        fprintf(svg_file, "' stroke='%s' stroke-width='5' fill='none'/>\n", 
                get_form_style_border_color(style));
    }
    
    fprintf(svg_file, 
        "\t\t<circle id='%d' r='%lf' fill='%s' stroke='%s' stroke-width='%s' fill-opacity='0.8'>\n",
        form_id, r, get_form_style_fill_color(style), get_form_style_border_color(style), get_form_style_stroke_width(style)
    );
    
    if (path_points != NULL && list_get_size(path_points) > 1) {
        fprintf(svg_file, 
            "\t\t\t<animateMotion dur='5s' repeatCount='indefinite'>\n"
            "\t\t\t\t<mpath href='#path_%d'/>\n"
            "\t\t\t</animateMotion>\n",
            form_id
        );
    }
    
    fprintf(svg_file, "\t\t</circle>\n");
    fprintf(svg_file, "\t</g>\n");
}

static void export_form(void *value, callback_data call_data) {
    Form form = (Form) value; 
    FILE *svg_file = call_data; 
    assert(form); 
    assert(call_data);

    FormType form_type = form_get_type(form); 
    
    // Generate unique ID for SVG to avoid conflicts
    int unique_id = exporter_instance->unique_id_counter++;
    
    double x, y, wr, h; 
    form_get_coordinates(form, &x, &y);
    form_get_dimensions(form, &wr, &h);
    FormStyle form_style = form_get_style(form);
    
    switch (form_type) {
        case CIRCLE: 
            svg_export_circle(svg_file, unique_id, x, y, wr, form_style);
            break; 
        case RECT: 
            svg_export_rectangle(svg_file, unique_id, x, y, wr, h, form_style);
            break; 
        case LINE: 
            svg_export_line(svg_file, unique_id, x, y, wr, h, form_style);
            break; 
        case TEXT: {
            char *form_text = form_get_text(form);
            svg_export_text(svg_file, unique_id, x, y, form_text, form_style);
            break;
        }
        case ANIMATED: {
            List path_points = form_get_path_points(form);
            svg_export_animated_form(svg_file, unique_id, x, y, 10, form_style, path_points);
            break;
        }
        default:
            fprintf(stderr, "[processor_svg] Error: invalid form type, skipping exportation (%d)\n", form_type);
    }
}

static void svg_export_header(FILE *svg_file) {
    assert(svg_file);
    fprintf(svg_file, "<svg viewBox='0 0 10500 3500' xmlns='http://www.w3.org/2000/svg'>\n"); 
}

static void svg_close(FILE *svg_file) {
    assert(svg_file);
    fprintf(svg_file, "</svg>");
}

static void svg_export_forms_list(FILE *svg_file) {
    assert(exporter_instance);
    if (exporter_instance->export_dir == NULL || exporter_instance->forms == NULL) {
        fprintf(stderr, "[processor_svg] Error: export directory or forms list were not defined\n");
        return;
    }
    
    list_foreach(exporter_instance->forms, &export_form, svg_file); 
}

void svg_export_forms() {
    assert(exporter_instance); 
    if (exporter_instance->export_dir == NULL) return; 

    exporter_instance->unique_id_counter = 1;

    FILE *svg_file = file_open_writable(exporter_instance->export_dir); 
    if (svg_file == NULL) return; 

    svg_export_header(svg_file);
    svg_export_forms_list(svg_file);
    svg_close(svg_file);
    file_close(svg_file);
}