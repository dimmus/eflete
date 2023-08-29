#include "eflete.h"

/**
 * @defgroup Demo Demo
 *
 * A Demo is responsible for showing widgets with contexts.
 * After user manipulations with parts, states or programs and saving
 * into a file, this space will show the resulted widget
 * by using user's theme (already saved one).
 */

/**
 * Create a object accordingly to given group.
 *
 * @param parent The parent object
 * @param group The group for create widget
 * @return The widget or layout with contexts
 *
 * @ingroup Demo
 */
Evas_Object *
demo_add(Evas_Object *parent, Group2 *group);
