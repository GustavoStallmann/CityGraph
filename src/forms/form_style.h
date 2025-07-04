#ifndef FORM_STYLE_H
#define FORM_STYLE_H

typedef void* FormStyle; 

/*
 * Creates a new form style with the specified properties
 * @param borderColor the border color of the form
 * @param fillColor the fill color of the form
 * @param fontFamily the font family for text
 * @param fontWeight the font weight for text
 * @param textAnchor the text anchor position
 * @param fontSize the font size for text
 * @param strokeWidth the stroke width for the form
 * @return a new FormStyle object
 */
FormStyle new_form_style(char *borderColor, char *fillColor, char *fontFamily, char *fontWeight, char *textAnchor, char *fontSize, char *strokeWidth);

/*
 * Gets the border color of the form style
 * @param style the form style to get the border color from
 * @return the border color as a string
 */
char* get_form_style_border_color(FormStyle style);

/*
 * Gets the fill color of the form style
 * @param style the form style to get the fill color from
 * @return the fill color as a string
 */
char* get_form_style_fill_color(FormStyle style);

/*
 * Gets the font family of the form style
 * @param style the form style to get the font family from
 * @return the font family as a string
 */
char* get_form_style_font_family(FormStyle style);

/*
 * Gets the text anchor of the form style
 * @param style the form style to get the text anchor from
 * @return the text anchor as a string
 */
char* get_form_style_text_anchor(FormStyle style);

/*
 * Gets the font weight of the form style
 * @param style the form style to get the font weight from
 * @return the font weight as a string
 */
char* get_form_style_font_weight(FormStyle style);

/*
 * Gets the font size of the form style
 * @param style the form style to get the font size from
 * @return the font size as a string
 */
char* get_form_style_font_size(FormStyle style);

/*
 * Gets the stroke width of the form style
 * @param style the form style to get the stroke width from
 * @return the stroke width as a string
 */
char* get_form_style_stroke_width(FormStyle style);

/*
 * Sets the fill color of the form style
 * @param style the form style to modify
 * @param fillColor the new fill color to set
 */
void set_form_fill_color(FormStyle style, char *fillColor);

/*
 * Sets the border color of the form style
 * @param style the form style to modify
 * @param borderColor the new border color to set
 */
void set_form_border_color(FormStyle style, char *borderColor);

/*
 * Sets the stroke width of the form style
 * @param style the form style to modify
 * @param strokeWidth the new stroke width to set
 */
void set_form_style_stroke_width(FormStyle style, char *strokeWidth);

/*
 * Frees the memory of the form style
 * @param style the form style to free
 */
void free_form_style(FormStyle style);

#endif