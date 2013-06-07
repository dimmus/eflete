#ifndef UI_RULER_HEADER_H
#define UI_RULER_HEADER_H

#include <Elementary.h>
#include "logger.h"

/* Key for take data from ruler object */
#define RULERDATAKEY "ruler_data_key"

/**
 * Flags for ruler orintation.
 */
enum _Orient
{
	HORIZONTAL = 0,
	VERTICAL
};
typedef enum _Orient Orient;

/**
 * Add new ruler to the given parent Elementary (container) object
 * in: parent The parent object.
 * out: a new ruler object, or NULL, on errors.
 */
Evas_Object *
ui_ruler_add(Evas_Object *parent);

/**
 * Change the ruler orientation to horizontal or vertical.
 * in: obj Ruler object.
 * in: orient Orientation ruler HORIZONTAL or VERTICAL
 */
void
ui_ruler_orient_set(Evas_Object *obj, Orient orient);

/**
 * Get value of current ruler object orientation
 * in: obj Ruler object.
 * out: HORIZONTAL or VERTICAL.
 */
Orient
ui_ruler_orient_get(Evas_Object *obj);

/**
 * Set position value the pointer.
 * in: obj Ruler object.
 * in: pos New position of pointer in absolute coordinates.
 */
void
ui_ruler_pointer_pos_set(Evas_Object *obj, int pos);

/**
 * Get position value the pointer.
 * in: obj Ruler object.
 * out: position of pointer in absolute coordinates.
 */
int
ui_ruler_pointer_pos_get(Evas_Object *obj);

/**
 * Set ruler pointer visible.
 * in: obj Ruler object.
 * in: visible EINA_TRUE for show pointer, EINA_FALSE for hide pointer.
 */
void
ui_ruler_pointer_visible_set(Evas_Object *obj, Eina_Bool visible);

/**
 * Get visible status of pointer.
 * in: obj Ruler object.
 * out: EINA_TRUE, if pointer visible, EINA_FALSE, if pointer invisible.
 */
Eina_Bool
ui_ruler_pointer_visible_get(Evas_Object *obj);

/**
 * Show ruler.
 * in: obj Ruler object.
 */
void
ui_ruler_show(Evas_Object *obj);

/**
 * Hide ruler.
 * in: obj Ruler object.
 */
void
ui_ruler_hide(Evas_Object *obj);

/**
 * Set relative scale position on ruler. It's need to link relative coordinates
 * to another object.
 * in: obj Ruler object.
 * in: start Start offset of the absolute scale.
 * in: end End for raltive scale. end value must be bigger than start value.
 */
void
ui_ruler_scale_relative_position_set(Evas_Object *obj, int start, int end);

/**
 * Get relative scale offset start and end positions on ruller.
 * in: obj Ruler object.
 * in: start Pointer to an integer in which store start offset relative scale.
 * in: end Pointer to an integer in wich store end offset relative scale.
 */
void
ui_ruler_scale_relative_position_get(Evas_Object *obj, int *start, int *end);

/**
 * Set absolute scale zero position in canvas coordinate.
 * in: obj The ruler object.
 * in: pos Coordinate from canvas.
 */
void
ui_ruler_scale_absolute_position_zero_set(Evas_Object *obj, int pos);

/**
 * Get absolute scale zero position in canvas coordinate.
 * in: obj The ruler object.
 * out: Coordinate in canvas.
 */
int
ui_ruler_scale_absolute_position_zero_get(Evas_Object *obj);

/**
 * Set relative scale step.
 * in: obj Ruler object.
 * in: step Step size for marks on relative scale. Value must be in range
 * from 0.1 to 1.0.
 */
void
ui_ruler_step_relative_set(Evas_Object *obj, double step);

/**
 * Get relative scale step size.
 * in: obj Ruler object.
 * out: Step sizef value in range from 0.1 to 1.0.
 */
double
ui_ruler_step_relative_get(Evas_Object *obj);

/**
 * Set reltive scale ruller visible.
 * in: obj Ruler object.
 * in: enable EINA_TRUE for set visible relative scale, EINA_FALE for
 * invisible mode.
 */
void
ui_ruler_scale_relative_visible_set(Evas_Object *obj, Eina_Bool enable);

/**
 * Get visible mode status for relative scale of ruler.
 * in: obj Ruler object.
 * out: EINA_TRUE if relative scale visible, or EINA_FALSE if scale invisible.
 */
Eina_Bool
ui_ruler_scale_relative_visible_get(Evas_Object *obj);

/**
 * Set absolute scale ruller visible.
 * in: obj Ruler object.
 * in: enable EINA_TRUE for set visible absolute scale, EINA_FALE for
 * invisible mode.
 */
void
ui_ruler_scale_absolute_visible_set(Evas_Object *obj, Eina_Bool enable);

/**
 * Get visible mode status for absolute scale of ruler.
 * in: obj Ruler object.
 * out: EINA_TRUE if absolute scale visible, or EINA_FALSE if scale invisible.
 */
Eina_Bool
ui_ruler_scale_absolute_visible_get(Evas_Object *obj);

#endif /* UI_RULER_HEADER_H */
