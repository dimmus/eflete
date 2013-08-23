#ifndef UI_PROPERTY_H
#define UI_PROPERTY_H

/**
 * @defgroup Property Property
 * @ingroup Block
 *
 * This widget designed for show group and part details.
 * @note In soon module can be remake.
 */

#include "efl_tet.h"
#include "widget_manager.h"
#include "widget_define.h"

/**
 * Add a new Property widget.
 *
 * @param parent The parent widget.
 * @return A new 'property' widget handler or NULL, on errors
 *
 * @ingroup Property
 */
Evas_Object *
ui_property_view_new(Evas_Object *parent);

/**
 * Set to current 'property' widget a group view which group details.
 * If current property has a group view, then old view will be removed.
 *
 * @param prop_view A 'property' widget.
 * @param group_view A group view.
 *
 * @ingroup Property
 */
void
ui_property_group_view_set(Evas_Object *prop_view, Evas_Object *group_view);

/**
 * Set to current 'property' widget a part view which part details.
 * If current property has a part view, then old view will be removed.
 *
 * @param prop_view A 'property' widget.
 * @param part_view A part view.
 *
 * @ingroup Property
 */
void
ui_property_part_view_set(Evas_Object *prop_view, Evas_Object *part_view);

/**
 * Get from current 'property' widget a group view.
 *
 * @param prop_view A 'property' widget.
 * @return A group view.
 *
 * @ingroup Property
 */
Evas_Object *
ui_property_group_view_get(Evas_Object *prop_view);

/**
 * Get from current 'property' widget a part view.
 *
 * @param prop_view A 'property' widget.
 * @return A group view.
 *
 * @ingroup Property
 */
Evas_Object *
ui_property_part_view_get(Evas_Object *prop_view);

/**
 * Add to parent property view a new group details view.
 *
 * @param prop_view A parent object.
 * @param group A Group object data to set a view.
 * @return A new group details view.
 *
 * @ingroup Property
 */
Evas_Object *
ui_prop_group_info_view_add(Evas_Object *prop_view, Group *group);

/**
 * Update a group view data.
 *
 * @param prop_view A property widget.
 * @param group A Group object to set a data.
 *
 * @ingroup Property
 */
void
ui_prop_group_info_view_update(Evas_Object *prop_view, Group *group);

/**
 * Add to parent property view a new part details view.
 *
 * @param prop_view The parent object.
 * @param group The Group object
 * @param part The Part object data to set a view.
 *
 * @ingroup Property
 */
Evas_Object *
ui_prop_part_info_view_add(Evas_Object *prop_view, Group *group, Part *part);

/**
 * Add to part view a new state view.
 *
 * @param part_view A parent object
 * @param state A Part_State object to add to a part view.
 * @return A new part state view.
 *
 * @ingroup Property
 */
Evas_Object *
ui_prop_part_info_state_view_add(Evas_Object *part_view,
                                 Group *group,
                                 Part *part);

/**
 * Set to part view a state view.
 *
 * @param part_view A part view to set a state view.
 * @param state_view A state view.
 *
 * @ingroup Property
 */
void
ui_prop_part_info_state_set(Evas_Object *part_view, Evas_Object *state_view);

/**
 * Get a state view.
 *
 * @param part_view A part view.
 * @return A state view.
 *
 * @ingroup Property
 */
Evas_Object *
ui_prop_part_info_state_view_get(Evas_Object *part_view);

#endif /* UI_PROPERTY_H */
