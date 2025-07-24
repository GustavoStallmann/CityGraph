#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "form.h"
#include "form_circle.h"
#include "form_line.h"
#include "form_rect.h"
#include "form_state.h"
#include "form_animated.h"
#include "form_style.h"
#include "form_text.h"
#include "form_animated.h"

typedef struct {
    FormType form_type; 
    void *form_instance; 
    int id; 
} Form_st; 

FormType form_get_type(Form f) {
    Form_st *form = (Form_st *) f; 
    return form->form_type; 
}

static void* form_get_instance(Form f) {
    Form_st *form = (Form_st *) f; 
    return form->form_instance; 
}

Form new_form(FormType tp, int id, double x, double y, double wr, double h, char *text, FormStyle style) {

    Form_st *form = (Form_st *) malloc(sizeof(Form_st)); 
    if (form == NULL) {
        fprintf(stderr, "[form] Error: insufficient memory to alloc\n");
        exit(EXIT_FAILURE); 
    }

    form->id = id; 
    form->form_type = tp; 
    form->form_instance = NULL; 
    
    switch (tp) {
        case CIRCLE: 
            form->form_instance = new_circle(x, y, wr, style);
            break;
        case RECT: 
            form->form_instance = new_rect(x, y, wr, h, style);
            break;
        case LINE: 
            form->form_instance = new_line(x, y, wr, h, style); 
            break;
        case TEXT: 
            form->form_instance = new_text(x, y, text, style);
            break;
        case ANIMATED:
            fprintf(stderr, "[form] Error: use new_animated_form_wrapper for ANIMATED forms\n"); 
            free(form); 
            return NULL;
        default:
            fprintf(stderr, "[form] Error: got an undefined form type to creation\n"); 
            free(form); 
            return NULL; 
    }

    if (form->form_instance == NULL) {
        fprintf(stderr, "[form] Error: couldn't alloc memory for the form\n"); 
        free(form); 
        return NULL; 
    }
    
    return form; 
}

void form_get_bounding_box(Form form, double *x, double *y, double *w, double *h) {
    assert(form); 

    FormType form_type = form_get_type(form);
    void *form_instance = form_get_instance(form);
    if (form_instance == NULL) return; 

    switch (form_type) {
        case CIRCLE:
            get_circle_bounding_box((Circle) form_instance, x, y, w, h);
            break; 
        case RECT: 
            get_rect_bounding_box((Rect) form_instance, x, y, w, h); 
            break; 
        case TEXT:
            get_text_bounding_box((Text) form_instance, x, y, w, h); 
            break;
        case LINE: 
            get_line_bounding_box((Line) form_instance, x, y, w, h);
            break;
        
        default:
            fprintf(stderr, "[form] Error: invalid form provided to compute the bounding box\n");
            break;
    }
}

FormStyle form_get_style(Form form) {
    assert(form); 
    
    FormType form_type = form_get_type(form);
    void *form_instance = form_get_instance(form);
    if (form_instance == NULL) return NULL; 

    switch (form_type) {
        case CIRCLE:
            return get_circle_style((Circle) form_instance);
        case RECT: 
            return get_rect_style((Rect) form_instance);
        case TEXT:
            return get_text_style((Text) form_instance);
        case LINE: 
            return get_line_style((Line) form_instance);
        case ANIMATED:
            return get_animated_form_style((AnimatedForm) form_instance);
        
        default:
            fprintf(stderr, "[form] Error: invalid form provided to get style\n");
            return NULL;
    }
}

void form_get_coordinates(Form form, double *x, double *y) {
    assert(form); 
    
    FormType form_type = form_get_type(form);
    void *form_instance = form_get_instance(form);
    if (form_instance == NULL) return; 

    switch (form_type) {
        case CIRCLE:
            get_circle_positions((Circle) form_instance, x, y, NULL);
            break; 
        case RECT: 
            get_rect_positions((Rect) form_instance, x, y, NULL, NULL); 
            break; 
        case TEXT:
            get_text_positions((Text) form_instance, x, y); 
            break;
        case LINE: 
            get_line_positions((Line) form_instance, x, y, NULL, NULL);
            break;
        case ANIMATED: 
            get_animated_form_positions((AnimatedForm) form_instance, x, y, NULL);
            break; 
        
        default:
            fprintf(stderr, "[form] Error: invalid form provided to get coordinates\n");
            break;
    }
}

void form_get_dimensions(Form form, double *w, double *h) {
    assert(form); 
    
    FormType form_type = form_get_type(form);
    void *form_instance = form_get_instance(form);
    if (form_instance == NULL) return; 

    switch (form_type) {
        case CIRCLE:
            get_circle_positions((Circle) form_instance, NULL, NULL, w);
            if (h != NULL) *h = *w;
            break; 
        case RECT: 
            get_rect_positions((Rect) form_instance, NULL, NULL, w, h); 
            break; 
        case TEXT:
            get_text_bounding_box((Text) form_instance, NULL, NULL, w, h); 
            break;
        case LINE: 
            get_line_positions((Line) form_instance, NULL, NULL, w, h);
            break;
        case ANIMATED: 
            break;
        
        default:
            fprintf(stderr, "[form] Error: invalid form provided to get dimensions\n");
            break;
    }
}

FormState form_get_state(Form form) {
    assert(form); 
    
    FormType form_type = form_get_type(form);
    void *form_instance = form_get_instance(form);
    if (form_instance == NULL) return NULL; 

    switch (form_type) {
        case CIRCLE:
            return get_circle_state((Circle) form_instance);
        case RECT: 
            return get_rect_state((Rect) form_instance);
        case TEXT:
            return get_text_state((Text) form_instance);
        case LINE: 
            return get_line_state((Line) form_instance);
        
        default:
            fprintf(stderr, "[form] Error: invalid form provided to get state\n");
            return NULL;
    }
}

int form_get_id(Form f) {
    assert(f); 

    Form_st *form = (Form_st *) f; 
    return form->id; 
}

char* form_get_text(Form f) {
    assert(f); 

    Form_st *form = (Form_st *) f;
    if (form->form_type != TEXT) return NULL;
    
    Text text_inst = (Text) form->form_instance;
    if (text_inst == NULL) return "";
    return get_text_string(text_inst);
}

void form_transp(Form form, double x, double y) {
    assert(form); 
    
    FormType form_type = form_get_type(form);
    void *form_instance = form_get_instance(form);
    if (form_instance == NULL) return; 

    switch (form_type) {
        case CIRCLE:
            circle_transp((Circle) form_instance, x, y);
            break;
        case RECT:
            rect_transp((Rect) form_instance, x, y);
            break;
        case TEXT:
            text_transp((Text) form_instance, x, y);
            break;
        case LINE:  
            line_transp((Line) form_instance, x, y);
            break;
        default:
            fprintf(stderr, "[form] Error: invalid form provided to translate\n");
            break;
    }
}

char* form_get_name(Form form) {
    assert(form); 
    
    FormType form_type = form_get_type(form);
    switch (form_type) {
        case CIRCLE: 
            return "Circle";
        case RECT: 
            return "Rectangle";
        case LINE: 
            return "Line"; 
        case TEXT:
            return "Text"; 
        case ANIMATED:
            return "Animated";
        default:
            return "Unknown";
    }
}

void form_free(Form form) {
    assert(form); 
    
    FormType form_type = form_get_type(form);
    void *form_instance = form_get_instance(form);
    if (form_instance == NULL) return; 
    
    switch (form_type) {
        case CIRCLE:
            free_circle((Circle) form_instance);
            break; 
        case RECT: 
            free_rect((Rect) form_instance);
            break; 
        case TEXT:
            free_text((Text) form_instance);
            break;
        case LINE: 
            free_line((Line) form_instance);
            break;
        case ANIMATED:
            free_animated_form((AnimatedForm) form_instance);
            break;
        
        default:
            fprintf(stderr, "[form] Error: invalid form provided to free memory\n");
            break;
    }

    free(form);
}

Form new_animated_form_wrapper(int id, double x, double y, double r, List path_points) {
    Form_st *form = (Form_st *) malloc(sizeof(Form_st)); 
    if (form == NULL) {
        fprintf(stderr, "[form] Error: insufficient memory to alloc\n");
        exit(EXIT_FAILURE); 
    }

    form->id = id; 
    form->form_type = ANIMATED; 
    form->form_instance = new_animated_form(x, y, r, path_points, new_form_style(NULL, NULL, NULL, NULL, NULL, NULL, NULL));

    if (form->form_instance == NULL) {
        fprintf(stderr, "[form] Error: couldn't alloc memory for the animated form\n"); 
        free(form); 
        return NULL; 
    }
    
    return form; 
}

List form_get_path_points(Form form) {
    assert(form);
    
    FormType form_type = form_get_type(form);
    if (form_type != ANIMATED) {
        return NULL;
    }
    
    void *form_instance = form_get_instance(form);
    if (form_instance == NULL) return NULL;
    
    return get_animated_form_path_points((AnimatedForm) form_instance);
}