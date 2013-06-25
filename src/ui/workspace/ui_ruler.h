#ifndef UI_RULER_HEADER_H
#define UI_RULER_HEADER_H

/**
 * @defgroup Ruler Ruler
 * @ingroup Workspace
 *
 * A Ruler module contain API for manipulate ruler parametrs.
 * A ruler can be horizontal or vertical. Ruler object contain an absolute and
 * relarive scales with dashes and mark, pointer.
 * Scales can position from custom coordinats. For relative scale accept state
 * end point.
 * A Ruler is "flexible"" object. When resize parent object, ruler resize and
 * recalculate itself.
 */

#include <Elementary.h>
#include "logger.h"

/* Key for take data from ruler object */
#define RULERDATAKEY "ruler_data_key"

/**
 * @enum _Orient
 * @typedef Orient
 *
 * Used to set ruler in horizontal or vertical state
 *
 * @ingroup Ruler
 */
enum _Orient
{
	HORIZONTAL = 0, /**< Horizontal orientation ruler. */
	VERTICAL /**< Vertical orientation ruler. */
};
typedef enum _Orient Orient;

/**
 * Add new ruler to the given parent Elementary (container) object
 *
 * @param parent The parent object.
 * @return a new ruler object, or NULL, on errors.
 *
 * @ingroup Ruler
 */
Evas_Object *
ui_ruler_add(Evas_Object *parent);

/**
 * Update ruler object scales.
 *
 * @param parent The ruler object.
 *
 * @ingroup Ruler
 */
void
ui_ruler_update(Evas_Object *parent);

/**
 * Change the ruler orientation to horizontal or vertical.
 *
 * @param obj Ruler object.
 * @param orient Orientation ruler HORIZONTAL or VERTICAL
 *
 * @ingroup Ruler
 */
void
ui_ruler_orient_set(Evas_Object *obj, Orient orient);

/**
 * Get value of current ruler object orientation
 *
 * @param obj Ruler object.
 * @return HORIZONTAL or VERTICAL.
 *
 * @ingroup Ruler
 */
Orient
ui_ruler_orient_get(Evas_Object *obj);

/**
 * Set position value the pointer.
 *
 * @param obj Ruler object.
 * @param pos New position of pointer in absolute coordinates.
 *
 * @ingroup Ruler
 */
void
ui_ruler_pointer_pos_set(Evas_Object *obj, int pos);

/**
 * Get position value the pointer.
 *
 * @param obj Ruler object.
 * @return position of pointer in absolute coordinates.
 *
 * @ingroup Ruler
 */
int
ui_ruler_pointer_pos_get(Evas_Object *obj);

/**
 * Set ruler pointer visible.
 *
 * @param obj Ruler object.
 * @param visible EINA_TRUE for show pointer, EINA_FALSE for hide pointer.
 *
 * @ingroup Ruler
 */
void
ui_ruler_pointer_visible_set(Evas_Object *obj, Eina_Bool visible);

/**
 * Get visible status of pointer.
 *
 * @param obj Ruler object.
 * @return EINA_TRUE, if pointer visible, EINA_FALSE, if pointer invisible.
 *
 * @ingroup Ruler
 */
Eina_Bool
ui_ruler_pointer_visible_get(Evas_Object *obj);

/**
 * Show ruler.
 *
 * @param obj Ruler object.
 *
 * @ingroup Ruler
 */
void
ui_ruler_show(Evas_Object *obj);

/**
 * Hide ruler.
 *
 * @param obj Ruler object.
 *
 * @ingroup Ruler
 */
void
ui_ruler_hide(Evas_Object *obj);

/**
 * Get visible status ruler object.
 *
 * @param obj Ruler object.
 * @return EINA_TRUE if ruler visible, EINA_FALSE, if ruler hided.
 *
 * @ingroup Ruler
 */
Eina_Bool
ui_ruler_visible_get(Evas_Object *obj);

/**
 * Set relative scale position on ruler. It's need to link relative coordinates
 * to another object.
 *
 * @param obj Ruler object.
 * @param start Start offset of the absolute scale.
 * @param end End for raltive scale. end value must be bigger than start value.
 *
 * @ingroup Ruler
 */
void
ui_ruler_scale_relative_position_set(Evas_Object *obj, int start, int end);

/**
 * Get relative scale offset start and end positions on ruller.
 *
 * @param obj Ruler object.
 * @param start Pointer to an integer in which store start offset relative scale.
 * @param end Pointer to an integer in wich store end offset relative scale.
 *
 * @ingroup Ruler
 */
void
ui_ruler_scale_relative_position_get(Evas_Object *obj, int *start, int *end);

/**
 * Set absolute scale zero position in canvas coordinate.
 *
 * @param obj The ruler object.
 * @param pos Coordinate from canvas.
 *
 * @ingroup Ruler
 */
void
ui_ruler_scale_absolute_position_zero_set(Evas_Object *obj, int pos);

/**
 * Get absolute scale zero position in canvas coordinate.
 *
 * @param obj The ruler object.
 * @return Coordinate in canvas.
 *
 * @ingroup Ruler
 */
int
ui_ruler_scale_absolute_position_zero_get(Evas_Object *obj);

/**
 * Set relative scale step.
 *
 * @param obj Ruler object.
 * @param step Step size for marks on relative scale. Value must be in range
 * from 0.1 to 1.0.
 *
 * @ingroup Ruler
 */
void
ui_ruler_step_relative_set(Evas_Object *obj, double step);

/**
 * Get relative scale step size.
 *
 * @param obj Ruler object.
 * @return Step sizef value in range from 0.1 to 1.0.
 *
 * @ingroup Ruler
 */
double
ui_ruler_step_relative_get(Evas_Object *obj);

/**
 * Set reltive scale ruller visible.
 *
 * @param obj Ruler object.
 * @param enable EINA_TRUE for set visible relative scale, EINA_FALE for
 * invisible mode.
 *
 * @ingroup Ruler
 */
void
ui_ruler_scale_relative_visible_set(Evas_Object *obj, Eina_Bool enable);

/**
 * Get visible mode status for relative scale of ruler.
 *
 * @param obj Ruler object.
 * @return EINA_TRUE if relative scale visible, or EINA_FALSE if scale invisible.
 *
 * @ingroup Ruler
 */
Eina_Bool
ui_ruler_scale_relative_visible_get(Evas_Object *obj);

/**
 * Set absolute scale ruller visible.
 *
 * @param obj Ruler object.
 * @param enable EINA_TRUE for set visible absolute scale, EINA_FALE for
 * invisible mode.
 *
 * @ingroup Ruler
 */
void
ui_ruler_scale_absolute_visible_set(Evas_Object *obj, Eina_Bool enable);

/**
 * Get visible mode status for absolute scale of ruler.
 *
 * @param obj Ruler object.
 * @return EINA_TRUE if absolute scale visible, or EINA_FALSE if scale invisible.
 *
 * @ingroup Ruler
 */
Eina_Bool
ui_ruler_scale_absolute_visible_get(Evas_Object *obj);

#endif /* UI_RULER_HEADER_H */
