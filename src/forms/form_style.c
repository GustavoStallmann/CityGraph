#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include  "form_style.h"

typedef struct {
    char *borderColor;
    char *fillColor; 
    char *fontFamily; 
    char *fontWeight;
    char *fontSize; 
    char *textAnchor;
    char *strokeWidth;
} FormStyle_st; 

static char* alloc_str(char *str) {
    if (str == NULL) return NULL; 

    size_t size = strlen(str) + 1; 
    char *new_str = (char *) malloc(sizeof(char) * size);
    if (new_str == NULL) {
        fprintf(stderr, "(ERROR) form_style: insufficient memory to alloc string");
        return NULL;
    } 

    strcpy(new_str, str);

    return new_str; 
}

static char* process_text_anchor(char *textAnchor) {
    char *anchor; 
    if (textAnchor != NULL && strcmp(textAnchor, "m") == 0) {
        anchor = alloc_str("middle");
    } else if (textAnchor != NULL && strcmp(textAnchor, "f") == 0) {
        anchor = alloc_str("end");
    } else {
        anchor = alloc_str("start");
    }

    return anchor; 
}

static char* process_font_weight(char *fontWeight) {
    char *weight; 
    if (fontWeight != NULL && strcmp(fontWeight, "b") == 0) {
        weight =  alloc_str("bold"); 
    } else if (fontWeight != NULL && strcmp(fontWeight, "i") == 0) {
        weight = alloc_str("italic"); 
    } else {
        weight = alloc_str("normal"); 
    }

    return weight;
}

FormStyle new_form_style(char *borderColor, char *fillColor, char *fontFamily, char *fontWeight, char *textAnchor, char *fontSize, char *strokeWidth) {
    FormStyle_st *style = (FormStyle_st *) malloc(sizeof(FormStyle_st)); 
    if (style == NULL) {
        fprintf(stderr, "(ERROR) form_style: insufficient memory to alloc formStyle_st");
        return NULL;
    }

    style->borderColor = alloc_str(borderColor); 
    style->fillColor = alloc_str(fillColor); 
    style->fontFamily = alloc_str(fontFamily); 
    style->textAnchor = process_text_anchor(textAnchor);
    style->fontWeight = process_font_weight(fontWeight); 
    style->fontSize = alloc_str(fontSize);
    style->strokeWidth = alloc_str(strokeWidth);

    return style; 
}

char* get_form_style_border_color(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->borderColor == NULL) return "#000000";

    return style_st->borderColor; 
}

char* get_form_style_fill_color(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->fillColor == NULL) return "#000000";

    return style_st->fillColor;  
}

char* get_form_style_font_family(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->fontFamily == NULL) return "Arial";

    return style_st->fontFamily; 
}

char* get_form_style_font_weight(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->fontWeight == NULL) return "normal";

    return style_st->fontWeight; 
}

char* get_form_style_text_anchor(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->textAnchor == NULL) return "start";

    return style_st->textAnchor; 
}

char* get_form_style_font_size(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return "12px";

    return style_st->fontSize; 
}

char* get_form_style_stroke_width(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL || style_st->strokeWidth == NULL) return "2";

    return style_st->strokeWidth; 
}

void set_form_fill_color(FormStyle style, char *fillColor) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return;

    free(style_st->fillColor);
    style_st->fillColor = alloc_str(fillColor);
}

void set_form_border_color(FormStyle style, char *borderColor) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return;

    free(style_st->borderColor);
    style_st->borderColor = alloc_str(borderColor);
}

void set_form_style_stroke_width(FormStyle style, char *strokeWidth) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return;

    free(style_st->strokeWidth);
    style_st->strokeWidth = alloc_str(strokeWidth);
}

void free_form_style(FormStyle style) {
    FormStyle_st *style_st = (FormStyle_st *) style;
    if (style_st == NULL) return;
    
    free(style_st->borderColor);
    free(style_st->fillColor);
    free(style_st->fontFamily);
    free(style_st->fontWeight);
    free(style_st->textAnchor);
    free(style_st->fontSize);
    free(style_st->strokeWidth);
    free(style_st);
}