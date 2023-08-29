#ifndef WORKSPACE_H
#define WORKSPACE_H

/**
 * @defgroup Workspace Workspace
 * @ingroup EFLETE
 *
 * A Workspace module create and control workspace area. It's contain control
 * for zoom operation with workspace, button to switch separate mode view.
 * Workspace provide highlight element for show and edit current part.
 * Also Workspace contain two rulers vertical and horizontal with functional
 * like most graphics editors. A popup menu can show by mouse click right
 * button on workspace object.
 */

#include "eflete.h"
#include "demo_group.h"
#include "resource_manager2.h"

typedef enum
{
   MODE_NORMAL = 1,
   MODE_CODE,
   /* MODE_SEPARATE, */
   /* MODE_ANIMATOR, */
   MODE_DEMO
} Workspace_Mode;


/**
 * Create a new Workspace object to the parent.
 *
 * @param parent The parent object.
 * @param group The group to be editing.
 *
 * @return The new object or NULL if it cannot be created.
 *
 * @ingroup Workspace
 */
Evas_Object *
workspace_add(Evas_Object *parent, Group2 *group);

/**
 * Update part info in part list
 *
 * @param obj The workspace object.
 * @param part Part to be updated.
 *
 * @ingroup Workspace
 */
void
workspace_group_navigator_update_part(Evas_Object *obj, Part2 *part);

/**
 * Update group_data info in group_data list
 *
 * @param obj The workspace object.
 * @param group_data Resource to be updated.
 *
 * @ingroup Workspace
 */
void
workspace_group_navigator_update_group_data(Evas_Object *obj, Resource2 *group_data);

/**
 * Update the parts attributes, with recalculation.
 *
 * @paramaram obj The workspace object.
 *
 * @ingroup Workspace
 */
void
workspace_groupview_hard_update(Evas_Object *obj);

/**
 * Update the parts attributes, without recalculation.
 *
 * @paramaram obj The workspace object.
 *
 * @ingroup Workspace
 */
void
workspace_groupview_soft_update(Evas_Object *obj);

/**
 * Add new part to group_edit and group_navigator.
 * Part should be already added to edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of added part
 *
 * @ingroup Workspace
 */
void
workspace_part_add(Evas_Object *obj, Eina_Stringshare *part_name);

/**
 * Add new part item to group_edit and group_navigator.
 * Part should be already added to edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of part
 * @param index index of added part_item
 *
 * @ingroup Workspace
 */
void
workspace_part_item_add(Evas_Object *obj,
                        Eina_Stringshare *part_name,
                        unsigned int index);

/**
 * Add new program to group_edit and group_navigator.
 * Part should be already added to edit_object.
 *
 * @param obj The workspace object.
 * @param program_name name of added program
 *
 * @ingroup Workspace
 */
void
workspace_program_add(Evas_Object *obj, Eina_Stringshare *program_name);

/**
 * Add new group_data to group_edit and group_navigator.
 * Part should be already added to edit_object.
 *
 * @param obj The workspace object.
 * @param group_data_name name of added group_data
 *
 * @ingroup Workspace
 */
void
workspace_group_data_add(Evas_Object *obj, Eina_Stringshare *group_data_name);

/**
 * Select state in group_navigator.
 * Part should be already selected.
 *
 * @param obj The workspace object.
 * @param part_name name of part
 * @param state_name name of selected part_state
 * @param state_value value of selected part_state
 *
 * @ingroup Workspace
 */
void
workspace_part_state_select(Evas_Object *obj,
                            Eina_Stringshare *part_name,
                            Eina_Stringshare *state_name,
                            double state_value);
/**
 * Add new part state to group_edit and group_navigator.
 * Part should be already added to edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of part
 * @param state_name name of added part_state
 * @param state_value value of added part_state
 *
 * @ingroup Workspace
 */
void
workspace_part_state_add(Evas_Object *obj,
                         Eina_Stringshare *part_name,
                         Eina_Stringshare *state_name,
                         double state_value);

/**
 * Add new part item to group_edit and group_navigator.
 * Part should be already deleted from edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of part
 * @param index Index of part item that is deleted
 *
 * @ingroup Workspace
 */
void
workspace_part_item_del(Evas_Object *obj,
                        Eina_Stringshare *part_name,
                        unsigned int index);

/**
 * Add new part state to group_edit and group_navigator.
 * Part should be already deleted from edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of part
 * @param state_name name of deleted part_state
 * @param state_value value of deleted part_state
 *
 * @ingroup Workspace
 */
void
workspace_part_state_del(Evas_Object *obj,
                         Eina_Stringshare *part_name,
                         Eina_Stringshare *state_name,
                         double state_value);

/**
 * Del part from group_edit and group_navigator.
 * Part should be already deleted edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of deleted part
 *
 * @ingroup Workspace
 */
void
workspace_part_del(Evas_Object *obj, Eina_Stringshare *part_name);

/**
 * Del program from group_edit and group_navigator.
 * Part should be already deleted edit_object.
 *
 * @param obj The workspace object.
 * @param program_name name of deleted program
 *
 * @ingroup Workspace
 */
void
workspace_program_del(Evas_Object *obj, Eina_Stringshare *program_name);

/**
 * Del group_data from group_edit and group_navigator.
 * Part should be already deleted edit_object.
 *
 * @param obj The workspace object.
 * @param group_data_name name of deleted group_data
 *
 * @ingroup Workspace
 */
void
workspace_group_data_del(Evas_Object *obj, Eina_Stringshare *group_data_name);

/**
 *
 */
void
workspace_part_restack(Evas_Object *obj,
                       Eina_Stringshare *part_name,
                       Eina_Stringshare *relative_part_name);

/**
 *
 */
void
workspace_part_item_restack(Evas_Object *obj,
                            Eina_Stringshare *part_name,
                            unsigned int item_index,
                            Eina_Bool move_up);

/**
 * Set zoom factor for view zoommed style in groupspace.
 *
 * @param obj The Workspace object.
 * @param factor A factor for scale. Where value 1.0 = 100% scale (not zoom
 * object). Minimum is 0.01, maximum is 20. (from 1% to 2000% zoom).
 *
 * @ingroup Workspace
 */
void
workspace_zoom_factor_set(Evas_Object *obj, double factor);

/**
 * Get zoom factor from workspace.
 *
 * @param The workspace object.
 *
 * @return double value of zoom factor, or '0' if fail.
 *
 * @ingroup Workspace
 */
double
workspace_zoom_factor_get(Evas_Object *obj);

/**
 * Resize the container to workspace size
 *
 * @param obj The workspace object.
 *
 * @ingroup Workspace
 */
void
workspace_container_fill(Evas_Object *obj);

/**
 * Fill the workspace
 *
 * @param obj The workspace object.
 *
 * @ingroup Workspace
 */
void
workspace_container_fit(Evas_Object *obj);

/**
 * Switch beetwen show or hide mode of legend in workspace object.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_legend_visible_set(Evas_Object *obj);

/**
 * Unset highlight from previously selected part.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_highlight_unset(Evas_Object *obj);

/**
 * Turn on/off the middle border of highlight (align) visibility.
 *
 * @param obj The workspace object,
 * @param separated ON/OFF (true/false) the middle border (align) visibility,
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_highlight_align_visible_set(Evas_Object *obj, Eina_Bool flag);

/**
 * Returns if middle border of highlight (align) is visible
 *
 * @param obj The workspace object,
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_highlight_align_visible_get(Evas_Object *obj);

/**
 * Turn on/off the object area visibility.
 *
 * @param obj The workspace object,
 * @param visible ON/OFF (true/false) the object area visibility,
 *
 * @ingroup Workspace
 */
void
workspace_object_area_visible_set(Evas_Object *obj, Eina_Bool visible);

/**
 * Returns if the object area is visible
 *
 * @param obj The workspace object,
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_object_area_visible_get(Evas_Object *obj);

/**
 * Turn on/off the part markers visibility.
 *
 * @param obj The workspace object,
 * @param visible ON/OFF (true/false) the markers visibility,
 *
 * @ingroup Workspace
 */
void
workspace_parts_markers_visible_set(Evas_Object *obj, Eina_Bool visible);

/**
 * Returns if the markers is visible
 *
 * @param obj The workspace object,
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_parts_markers_visible_get(Evas_Object *obj);

void
workspace_rulers_visible_set(Evas_Object *obj, Eina_Bool visible);

Eina_Bool
workspace_rulers_visible_get(Evas_Object *obj);

TODO("remove after property refactor!!! HIGH LEVEL");
Eina_Bool
workspace_active_demo_mode_get(Evas_Object *obj);

void
workspace_demo_group_property_update(Evas_Object *obj);

void
workspace_add_part_request(Evas_Object *obj);

void
workspace_add_part_item_request(Evas_Object *obj);

void
workspace_add_state_request(Evas_Object *obj);

void
workspace_add_program_request(Evas_Object *obj);

void
workspace_add_group_data_request(Evas_Object *obj);

void
workspace_delete_request(Evas_Object *obj);

void
workspace_mode_set(Evas_Object *obj, Workspace_Mode mode);

void
workspace_state_next_request(Evas_Object *obj);

void
workspace_part_next_request(Evas_Object *obj);

void
workspace_part_prev_request(Evas_Object *obj);

void
workspace_part_showhide_request(Evas_Object *obj);

void
workspace_all_parts_showhide_request(Evas_Object *obj);

void
workspace_part_unselect_request(Evas_Object *obj);

void
workspace_demo_swallow_set(Evas_Object *obj, Demo_Part *part);

void
workspace_demo_text_set(Evas_Object *obj, Demo_Part *part);

void
workspace_demo_signal_set(Evas_Object *obj, Demo_Signal *sig);

void
workspace_code_reload(Evas_Object *obj);

void
workspace_code_changed(Evas_Object *obj);

Evas_Object *
workspace_group_navigator_get(Evas_Object *obj);

void
workspace_history_undo(Evas_Object *obj);

void
workspace_history_redo(Evas_Object *obj);

void
workspace_history_update(Evas_Object *obj);

#endif /* WORKSPACE_H */
