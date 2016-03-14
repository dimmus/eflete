/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

/*
#ifdef HAVE_CONFIG_H
   #include "eflete_config.h"
#endif */
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "property_private.h"
#include "property_macros.h"
#include "main_window.h"
#include "history.h"
#include "validator.h"
#include "group_navigator.h"
#include "change.h"
#include "project_manager.h"
#include "colorsel.h"

#define GROUP_PROP_DATA "group_prop_data"

#define GROUP_PROP_DATA_GET() \
   assert(property != NULL); \
   Group_Prop_Data *pd = evas_object_data_get(property, GROUP_PROP_DATA); \
   assert(pd != NULL);

struct _Group_Prop_Data
{
   Group *group;
   Part *part;
   Change *change;
   int old_int_val, old_int_val2, old_int_val3, old_int_val4;
   double old_double_val;
   Evas_Object *layout;
   Evas_Object *scroller;
   Eina_Stringshare *item_name;
   struct {
        struct {
             Evas_Object *frame;
             Evas_Object *info;
             Evas_Object *shared_check;
             Evas_Object *ctxpopup;
             Evas_Object *name;
             Evas_Object *min_w, *min_h, *min_item;
             Evas_Object *max_w, *max_h, *max_item;
             Evas_Object *current;
        } group;
        struct {
             Evas_Object *frame;
             Evas_Object *name;
             Resource_Name_Validator *validator;
             Evas_Object *type;
             Evas_Object *scale, *scale_item;
             Evas_Object *mouse_events, *mouse_events_item;
             Evas_Object *repeat_events, *repeat_events_item;
             Evas_Object *clip_to, *clip_to_item;
             Evas_Object *ignore_flags, *ignore_flags_item;
             Evas_Object *source, *source_item;
             unsigned int previous_source;
        } part;
        struct {
             Evas_Object *frame;
             Evas_Object *x, *step_x;
             Evas_Object *y, *step_y;
             Evas_Object *confine;
             Evas_Object *threshold;
             Evas_Object *event;
        } part_drag;
        struct {
             Evas_Object *frame;
             Evas_Object *state;
             Evas_Object *proxy_source, *proxy_source_item;
             Evas_Object *visible;
             Evas_Object *min_w, *min_h;
             Evas_Object *max_w, *max_h;
             Evas_Object *fixed_w, *fixed_h;
             Evas_Object *align_x, *align_y;
             Evas_Object *aspect_min, *aspect_max;
             Evas_Object *aspect_pref;
             Evas_Object *color_class, *color1, *color2, *color3, *color_class_item;
             Evas_Object *color, *color_obj, *color_item;
             Evas_Object *minmul_w, *minmul_h;
             Evas_Object *box_layout, *box_layout_item;
             Evas_Object *table_homogeneous, *table_homogeneous_item;
        } state;
        struct {
             Evas_Object *frame;
             Evas_Object *rel1_to_x, *rel1_to_y;
             Evas_Object *rel1_relative_x, *rel1_relative_y;
             Evas_Object *rel1_offset_x, *rel1_offset_y;
             Evas_Object *rel2_to_x, *rel2_to_y;
             Evas_Object *rel2_relative_x, *rel2_relative_y;
             Evas_Object *rel2_offset_x, *rel2_offset_y;
        } state_object_area;
        struct {
             Evas_Object *frame;
             Evas_Object *text;
             Evas_Object *font;
             Elm_Validator_Regexp *validator;
             Evas_Object *size;
             Evas_Object *align_x, *align_y;
             Evas_Object *source;
             Evas_Object *min_x, *min_y;
             Evas_Object *max_x, *max_y;
             Evas_Object *fit_x, *fit_y;
             Evas_Object *text_source;
             Evas_Object *elipsis_toggle, *elipsis;
             Evas_Object *effect, *effect_direction;
             Evas_Object *color2, *color2_obj;
             Evas_Object *color3, *color3_obj;
        } state_text;
        struct {
             Evas_Object *frame;
             Evas_Object *text;
             Evas_Object *style;
             Evas_Object *align_x, *align_y;
             Evas_Object *min_x, *min_y;
             Evas_Object *max_x, *max_y;
             Evas_Object *select_mode;
             Evas_Object *entry_mode;
             Evas_Object *pointer_mode;
             Evas_Object *cursor_mode;
             Evas_Object *multiline;
             Evas_Object *source;
             Evas_Object *source2;
             Evas_Object *source3;
             Evas_Object *source4;
             Evas_Object *source5;
             Evas_Object *source6;
        } state_textblock;
        struct {
             Evas_Object *frame;
             Evas_Object *image;
             Evas_Object *border_r;
             Evas_Object *border_l;
             Evas_Object *border_t;
             Evas_Object *border_b;
             Elm_Validator_Regexp *validator;
             Evas_Object *border_fill;
             Evas_Object *tween;
        } state_image;
        struct {
             Evas_Object *frame;
             Evas_Object *type;
             Evas_Object *smooth;
             Evas_Object *origin_relative_x, *origin_relative_y;
             Evas_Object *origin_offset_x, *origin_offset_y;
             Evas_Object *size_relative_x, *size_relative_y;
             Evas_Object *size_offset_x, *size_offset_y;
        } state_fill;
        struct {
             Evas_Object *frame;
             Evas_Object *align_x, *align_y;
             Evas_Object *padding_x, *padding_y;
             Evas_Object *min_v, *min_h;
        } state_container;
        struct {
             Evas_Object *frame;
             Evas_Object *name;
             Evas_Object *source, *source_item;
             Evas_Object *min_w, *min_h;
             Evas_Object *max_w, *max_h;
             Evas_Object *spread_w, *spread_h;
             Evas_Object *prefer_w, *prefer_h;
             Evas_Object *padding_l, *padding_r, *padding_t, *padding_b;
             Evas_Object *align_x, *align_y;
             Evas_Object *weight_x, *weight_y;
             Evas_Object *aspect_w, *aspect_h;
             Evas_Object *aspect_mode;
             Evas_Object *position_col, *position_row, *position_item; /* Only for items in part TABLE */
             Evas_Object *span_col, *span_row; /* Only for items in part TABLE */
        } part_item;
        struct {
             Evas_Object *frame;
             Evas_Object *name;
             const char *program;
             Evas_Object *signal;
             Evas_Object *source;
             Evas_Object *action;
             Evas_Object *action_params; /* it's a frame */
             Evas_Object *state;
             Evas_Object *state2;
             Evas_Object *value;
             Evas_Object *value2;
             Evas_Object *sample_name;
             Evas_Object *sample_speed;
             Evas_Object *channel;
             Evas_Object *tone_name;
             Evas_Object *tone_duration;
             Evas_Object *transition;
             Evas_Object *transition_params;
             Evas_Object *transition_from_current;
             Evas_Object *transition_time;
             Evas_Object *transition_value1;
             Evas_Object *transition_value2;
             Evas_Object *transition_value3;
             Evas_Object *transition_value4;
             Evas_Object *target;
             Evas_Object *target_box;
             Evas_Object *targets_frame; /* it's a frame */
             Evas_Object *after_box;
             Evas_Object *afters_frame; /* it's a frame */
             Evas_Object *in_from, *in_range;
             Evas_Object *filter_part, *filter_state;
        } program;
   } attributes;
};
typedef struct _Group_Prop_Data Group_Prop_Data;

static const char *edje_aspect_pref[] = { N_("None"),
                                          N_("Vertical"),
                                          N_("Horizontal"),
                                          N_("Both"),
                                          N_("Source"),
                                          NULL};

static const char *edje_item_aspect_pref[] = { N_("None"),
                                               N_("Neither"),
                                               N_("Vertical"),
                                               N_("Horizontal"),
                                               N_("Both"),
                                               NULL};

static const char *edje_select_mode[] = { N_("Default"),
                                          N_("Explicit"),
                                          NULL};

static const char *edje_entry_mode[] = { N_("None"),
                                         N_("Plain"),
                                         N_("Editable"),
                                         N_("Password"),
                                         NULL};

static const char *edje_pointer_mode[] = { N_("Autograb"),
                                           N_("Nograb"),
                                           NULL};

static const char *edje_cursor_mode[] = { N_("Under"),
                                          N_("Before"),
                                          NULL};

static const char *edje_fill_type[] = { N_("Scale"),
                                        N_("Tile"),
                                        NULL};

static const char *edje_ignore_flags[] = { N_("None"),
                                           N_("On hold"),
                                           NULL};

static const char *edje_text_effect_type[] = { N_("None"),
                                               N_("Plain"),
                                               N_("Outline"),
                                               N_("Soft Outline"),
                                               N_("Shadow"),
                                               N_("Soft Shadow"),
                                               N_("Outline Shadow"),
                                               N_("Outline Soft Shadow"),
                                               N_("Far Shadow"),
                                               N_("Far Soft Shadow"),
                                               N_("Glow"),
                                               NULL};

static const char *edje_text_effect_direction[] = { N_("Bottom Right"),
                                                    N_("Bottom"),
                                                    N_("Bottom Left"),
                                                    N_("Left"),
                                                    N_("Top Left"),
                                                    N_("Top"),
                                                    N_("Top Right"),
                                                    N_("Right"),
                                                    NULL};

static const char *edje_middle_type[] = { N_("None"),
                                          N_("Default"),
                                          N_("Solid"),
                                          NULL};

static const char *edje_homogeneous[] = { N_("None"),
                                          N_("Table"),
                                          N_("Item"),
                                          NULL};

static const char *edje_box_layouts[] = { N_("horizontal"),
                                          N_("vertical"),
                                          N_("horizontal_homogeneous"),
                                          N_("vertical_homogeneous"),
                                          N_("horizontal_max"),
                                          N_("vertical_max"),
                                          N_("horizontal_flow"),
                                          N_("vertical_flow"),
                                          N_("stack"),
                                          // N_("Custom Layout"), not implemented yet
                                          NULL};
static const char *
edje_program_actions[] = { N_("None"),
                           N_("state set"),
                           N_("signal emit"),
                           N_("drag value"),
                           N_("drag value step"),
                           N_("drag value page"),
                           N_("play sample"),
                           N_("play tone"),
                           N_("action stop"),
                           NULL};

static const char *
sound_channel[] = { N_("effect"),
                    N_("background"),
                    N_("music"),
                    N_("foreground"),
                    N_("interface"),
                    N_("input"),
                    N_("alert"),
                    N_("all"),
                    NULL };

static const char *
edje_program_transitions[] = { N_("None"),
                               N_("linear"),
                               N_("accelerate"),
                               N_("decelerate"),
                               N_("sinusoidal"),
                               N_("devisior interpretation"),
                               N_("bounce"),
                               N_("spring"),
                               N_("cubic bezier"),
                               NULL};

static void
_ui_property_part_unset(Evas_Object *property);

static void
_ui_property_part_state_unset(Evas_Object *property);

static void
_ui_property_part_item_set(Evas_Object *property, Part *part);

static void
_ui_property_part_item_unset(Evas_Object *property);

static void
_ui_property_program_set(Evas_Object *property, const char *program);

static void
_ui_property_program_unset(Evas_Object *property);

static void
prop_program_signal_update(Group_Prop_Data *pd);

static void
prop_program_source_update(Group_Prop_Data *pd);

static void
prop_program_action_update(Group_Prop_Data *pd);

static void
prop_program_transition_update(Group_Prop_Data *pd);

static void
prop_program_state_update(Group_Prop_Data *pd);

static void
prop_program_state2_update(Group_Prop_Data *pd);

static void
prop_program_sample_name_update(Group_Prop_Data *pd);

static void
prop_program_tone_name_update(Group_Prop_Data *pd);

static void
prop_program_targets_update(Group_Prop_Data *pd);

static void
prop_program_afters_update(Group_Prop_Data *pd);

static void
prop_program_filter_part_update(Group_Prop_Data *pd);

static void
prop_program_filter_state_update(Group_Prop_Data *pd);

static Eina_Bool
ui_property_state_obj_area_set(Evas_Object *property);

static void
ui_property_state_obj_area_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_text_set(Evas_Object *property);

static void
ui_property_state_text_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_textblock_set(Evas_Object *property);

static void
ui_property_state_textblock_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_image_set(Evas_Object *property);

static void
ui_property_state_image_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_fill_set(Evas_Object *property);

static void
ui_property_state_fill_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_container_set(Evas_Object *property);

static void
ui_property_state_container_unset(Evas_Object *property);

static void
prop_state_text_elipsis_update(Group_Prop_Data *pd);

static void
prop_state_image_update(Group_Prop_Data *pd);

static void
prop_state_color_class_update(Group_Prop_Data *pd);

static void
prop_state_object_area_rel1_to_x_update(Group_Prop_Data *pd);

static void
prop_state_object_area_rel1_to_y_update(Group_Prop_Data *pd);

static void
prop_state_object_area_rel2_to_x_update(Group_Prop_Data *pd);

static void
prop_state_object_area_rel2_to_y_update(Group_Prop_Data *pd);

static void
prop_state_text_source_update(Group_Prop_Data *pd);

static void
prop_state_text_text_source_update(Group_Prop_Data *pd);

static void
prop_state_text_update(Group_Prop_Data *pd);

static void
prop_state_font_update(Group_Prop_Data *pd);

static void
prop_state_textblock_style_update(Group_Prop_Data *pd);

static void
prop_state_color_update(Group_Prop_Data *pd);

static void
prop_state_color2_update(Group_Prop_Data *pd);

static void
prop_state_color3_update(Group_Prop_Data *pd);

static void
prop_state_image_border_update(Group_Prop_Data *pd);

static void
prop_part_text_effect_update(Group_Prop_Data *pd);

static void
prop_part_drag_x_step_x_update(Group_Prop_Data *pd);

static void
prop_part_drag_y_step_y_update(Group_Prop_Data *pd);

static void
prop_part_clip_to_update(Group_Prop_Data *pd);

static void
prop_part_drag_confine_update(Group_Prop_Data *pd);

static void
prop_part_drag_threshold_update(Group_Prop_Data *pd);

static void
prop_part_drag_event_update(Group_Prop_Data *pd);

static void
prop_part_source_update(Group_Prop_Data *pd);

static void
prop_state_textblock_source_update(Group_Prop_Data *pd);

static void
prop_state_textblock_source2_update(Group_Prop_Data *pd);

static void
prop_state_textblock_source3_update(Group_Prop_Data *pd);

static void
prop_state_textblock_source4_update(Group_Prop_Data *pd);

static void
prop_state_textblock_source5_update(Group_Prop_Data *pd);

static void
prop_state_textblock_source6_update(Group_Prop_Data *pd);

static void
prop_part_name_update(Group_Prop_Data *pd);

static void
prop_group_name_update(Group_Prop_Data *pd);

static void
prop_part_item_source_update(Group_Prop_Data *pd);

static void
prop_part_item_padding_update(Group_Prop_Data *pd);

static void
prop_state_proxy_source_update(Group_Prop_Data *pd);

static void
prop_item_state_image_tween_update(Evas_Object *tween, Group_Prop_Data *pd);

static Elm_Gengrid_Item_Class *_itc_tween = NULL;

static void
_on_spinner_mouse_wheel(void *data __UNUSED__,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info)
{
   Evas_Event_Mouse_Wheel *mev = event_info;
   mev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
}

static Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    Evas_Object **label,
                    const char *lab_text,
                    const char *text)
{
   assert(parent != NULL);
   assert(label != NULL);

   PROPERTY_ITEM_ADD(parent, lab_text, "1swallow")
   LABEL_ADD(item, *label, text)
   elm_object_part_content_set(item, "elm.swallow.content", *label);
   return item;
}

#define prop_part_type_add(PARENT, NAME, TEXT) prop_item_label_add(PARENT, &pd->attributes.part.type, NAME, TEXT)
#define prop_state_state_add(PARENT, NAME, TEXT) prop_item_label_add(PARENT, &pd->attributes.state.state, NAME, TEXT)
#define prop_part_item_name_add(PARENT, NAME, TEXT) prop_item_label_add(PARENT, &pd->attributes.part_item.name, NAME, TEXT)

#define prop_part_type_update(TEXT) elm_object_text_set(pd->attributes.part.type, TEXT)
#define prop_state_state_update(TEXT) elm_object_text_set(pd->attributes.state.state, TEXT)
#define prop_part_item_name_update(TEXT) elm_object_text_set(pd->attributes.part_item.name, TEXT)


static void
_on_part_selected(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info)
{
   Evas_Object *property = data;
   GROUP_PROP_DATA_GET()
   Part *part = event_info;

   if (!part)
     {
        _ui_property_part_unset(property);
        return;
     }
   ui_property_part_set(property, part);
   ui_property_part_state_set(property, part);
   if (part->current_item_name)
     _ui_property_part_item_set(property, part);
}

static void
_on_part_state_selected(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info)
{
   Evas_Object *property = data;
   GROUP_PROP_DATA_GET()
   Part *part = event_info;

   if (!part)
     {
        TODO("is this unset needed?");
        _ui_property_part_state_unset(property);
        return;
     }
   ui_property_part_state_set(property, part);
}

static void
_on_program_selected(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Evas_Object *property = data;
   GROUP_PROP_DATA_GET()
   const char *name = event_info;

   _on_part_selected(data, obj, NULL);
   if (!name)
     {
        _ui_property_program_unset(property);
        return;
     }
   _ui_property_program_set(property, name);
}
static void
_on_program_unselected(void *data,
                    Evas_Object *obj,
                    void *event_info __UNUSED__)
{
   _on_program_selected(data, obj, NULL);
}

#define PROGRAM_ATTR_1CHECK_UPDATE(SUB, VALUE, MEMBER) \
   elm_check_state_set(pd->attributes.program.transition_from_current, \
                       editor_program_transition_from_current_get(pd->group->edit_object, pd->attributes.program.program));

static void
_on_editor_attribute_changed(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info)
{
   Evas_Object *property = data;
   GROUP_PROP_DATA_GET()
   Attribute *attribute = event_info;

   TODO("Add check that field is shown at the moment");
   switch(*attribute)
     {
      case ATTRIBUTE_GROUP_MAX_H:
         COMMON_1SPINNER_UPDATE(group, max_h, group, STUB_STUB_STUB, 1, GROUP_ARGS);
         break;
      case ATTRIBUTE_GROUP_MIN_H:
         COMMON_1SPINNER_UPDATE(group, min_h, group, STUB_STUB_STUB, 1, GROUP_ARGS);
         break;
      case ATTRIBUTE_GROUP_MAX_W:
         COMMON_1SPINNER_UPDATE(group, max_w, group, STUB_STUB_STUB, 1, GROUP_ARGS);
         break;
      case ATTRIBUTE_GROUP_MIN_W:
         COMMON_1SPINNER_UPDATE(group, min_w, group, STUB_STUB_STUB, 1, GROUP_ARGS);
         break;
      case ATTRIBUTE_STATE_MAX_W:
         COMMON_1SPINNER_UPDATE(state, max_w, state, int, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_MAX_H:
         COMMON_1SPINNER_UPDATE(state, max_h, state, int, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_MIN_W:
         COMMON_1SPINNER_UPDATE(state, min_w, state, int, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_MIN_H:
         COMMON_1SPINNER_UPDATE(state, min_h, state, int, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_ALIGN_X:
         COMMON_1SPINNER_UPDATE(state, align_x, state, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_ALIGN_Y:
         COMMON_1SPINNER_UPDATE(state, align_y, state, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_REL1_RELATIVE_X:
         COMMON_1SPINNER_UPDATE(state, rel1_relative_x, state_object_area, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_REL1_RELATIVE_Y:
         COMMON_1SPINNER_UPDATE(state, rel1_relative_y, state_object_area, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_REL2_RELATIVE_X:
         COMMON_1SPINNER_UPDATE(state, rel2_relative_x, state_object_area, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_REL2_RELATIVE_Y:
         COMMON_1SPINNER_UPDATE(state, rel2_relative_y, state_object_area, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_REL1_OFFSET_X:
         COMMON_1SPINNER_UPDATE(state, rel1_offset_x, state_object_area, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_REL1_OFFSET_Y:
         COMMON_1SPINNER_UPDATE(state, rel1_offset_y, state_object_area, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_REL2_OFFSET_X:
         COMMON_1SPINNER_UPDATE(state, rel2_offset_x, state_object_area, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_REL2_OFFSET_Y:
         COMMON_1SPINNER_UPDATE(state, rel2_offset_y, state_object_area, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_ASPECT_MIN:
         COMMON_1SPINNER_UPDATE(state, aspect_min, state, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_ASPECT_MAX:
         COMMON_1SPINNER_UPDATE(state, aspect_max, state, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_X:
         COMMON_1SPINNER_UPDATE(state_fill, origin_relative_x, state_fill, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_Y:
         COMMON_1SPINNER_UPDATE(state_fill, origin_relative_y, state_fill, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_X:
         COMMON_1SPINNER_UPDATE(state_fill, origin_offset_x, state_fill, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_Y:
         COMMON_1SPINNER_UPDATE(state_fill, origin_offset_y, state_fill, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_X:
         COMMON_1SPINNER_UPDATE(state_fill, size_relative_x, state_fill, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_Y:
         COMMON_1SPINNER_UPDATE(state_fill, size_relative_y, state_fill, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FILL_SIZE_OFFSET_X:
         COMMON_1SPINNER_UPDATE(state_fill, size_offset_x, state_fill, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FILL_SIZE_OFFSET_Y:
         COMMON_1SPINNER_UPDATE(state_fill, size_offset_y, state_fill, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_TEXT_ALIGN_X:
         COMMON_1SPINNER_UPDATE(state_text, align_x, state_text, double, 100, STATE_ARGS);
         COMMON_1SPINNER_UPDATE(state_text, align_x, state_textblock, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_TEXT_ALIGN_Y:
         COMMON_1SPINNER_UPDATE(state_text, align_y, state_text, double, 100, STATE_ARGS);
         COMMON_1SPINNER_UPDATE(state_text, align_y, state_textblock, double, 100, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_TEXT_ELIPSIS:
         prop_state_text_elipsis_update(pd);
         break;
      case ATTRIBUTE_STATE_TEXT_SIZE:
         COMMON_1SPINNER_UPDATE(state_text, size, state_text, int,  1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_TEXT_FIT_X:
         COMMON_CHECK_UPDATE(state_text, fit_x, state_text, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_TEXT_FIT_Y:
         COMMON_CHECK_UPDATE(state_text, fit_y, state_text, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_TEXT_MAX_X:
         COMMON_CHECK_UPDATE(state_text, max_x, state_text, STATE_ARGS);
         COMMON_CHECK_UPDATE(state_text, max_x, state_textblock, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_TEXT_MAX_Y:
         COMMON_CHECK_UPDATE(state_text, max_y, state_text, STATE_ARGS);
         COMMON_CHECK_UPDATE(state_text, max_y, state_textblock, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_TEXT_MIN_X:
         COMMON_CHECK_UPDATE(state_text, min_x, state_text, STATE_ARGS);
         COMMON_CHECK_UPDATE(state_text, min_x, state_textblock, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_TEXT_MIN_Y:
         COMMON_CHECK_UPDATE(state_text, min_y, state_text, STATE_ARGS);
         COMMON_CHECK_UPDATE(state_text, min_y, state_textblock, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FIXED_H:
         COMMON_CHECK_UPDATE(state, fixed_h, state, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FIXED_W:
         COMMON_CHECK_UPDATE(state, fixed_w, state, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_FILL_SMOOTH:
         COMMON_CHECK_UPDATE(state_fill, smooth, state_fill, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_VISIBLE:
         COMMON_CHECK_UPDATE(state, visible, state, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_IMAGE:
         prop_state_image_update(pd);
         break;
      case ATTRIBUTE_STATE_COLOR_CLASS:
         prop_state_color_class_update(pd);
         break;
      case ATTRIBUTE_STATE_REL1_TO_X:
         prop_state_object_area_rel1_to_x_update(pd);
         break;
      case ATTRIBUTE_STATE_REL1_TO_Y:
         prop_state_object_area_rel1_to_y_update(pd);
         break;
      case ATTRIBUTE_STATE_REL2_TO_X:
         prop_state_object_area_rel2_to_x_update(pd);
         break;
      case ATTRIBUTE_STATE_REL2_TO_Y:
         prop_state_object_area_rel2_to_y_update(pd);
         break;
      case ATTRIBUTE_STATE_TEXT_SOURCE:
         prop_state_text_source_update(pd);
         break;
      case ATTRIBUTE_STATE_TEXT_TEXT_SOURCE:
         prop_state_text_text_source_update(pd);
         break;
      case ATTRIBUTE_STATE_TEXT:
         prop_state_text_update(pd);
         break;
      case ATTRIBUTE_STATE_FONT:
         prop_state_font_update(pd);
         break;
      case ATTRIBUTE_STATE_TEXT_STYLE:
         prop_state_textblock_style_update(pd);
         break;
      case ATTRIBUTE_STATE_COLOR:
         prop_state_color_update(pd);
         break;
      case ATTRIBUTE_STATE_COLOR2:
         prop_state_color2_update(pd);
         break;
      case ATTRIBUTE_STATE_COLOR3:
         prop_state_color3_update(pd);
         break;
      case ATTRIBUTE_STATE_IMAGE_BORDER:
         prop_state_image_border_update(pd);
         break;
      case ATTRIBUTE_STATE_IMAGE_BORDER_FILL:
         STATE_ATTR_1COMBOBOX_LIST_UPDATE(state_image, border_fill, state_image);
         break;
      case ATTRIBUTE_STATE_ASPECT_PREF:
         STATE_ATTR_1COMBOBOX_LIST_UPDATE(state, aspect_pref, state);
         break;
      case ATTRIBUTE_STATE_PROXY_SOURCE:
         prop_state_proxy_source_update(pd);
         break;
      case ATTRIBUTE_PART_EFFECT:
         prop_part_text_effect_update(pd);
         break;
      case ATTRIBUTE_PART_IGNORE_FLAGS:
         PART_ATTR_1COMBOBOX_LIST_UPDATE(part, ignore_flags, part);
         break;
      case ATTRIBUTE_PART_MOUSE_EVENTS:
         PART_ATTR_1CHECK_UPDATE(part, mouse_events, part);
         break;
      case ATTRIBUTE_PART_REPEAT_EVENTS:
         PART_ATTR_1CHECK_UPDATE(part, repeat_events, part);
         break;
      case ATTRIBUTE_PART_SCALE:
         PART_ATTR_1CHECK_UPDATE(part, scale, part);
         break;
      case ATTRIBUTE_PART_MULTILINE:
         PART_ATTR_1CHECK_UPDATE(part, multiline, state_textblock);
         break;
      case ATTRIBUTE_PART_DRAG_COUNT_X:
      case ATTRIBUTE_PART_DRAG_COUNT_Y:
         TODO("Implement")
         break;
      case ATTRIBUTE_PART_DRAG_X:
      case ATTRIBUTE_PART_DRAG_STEP_X:
         prop_part_drag_x_step_x_update(pd);
         break;
      case ATTRIBUTE_PART_DRAG_Y:
      case ATTRIBUTE_PART_DRAG_STEP_Y:
         prop_part_drag_y_step_y_update(pd);
         break;
      case ATTRIBUTE_PART_CLIP_TO:
         prop_part_clip_to_update(pd);
         break;
      case ATTRIBUTE_PART_DRAG_CONFINE:
         prop_part_drag_confine_update(pd);
         break;
      case ATTRIBUTE_PART_DRAG_THRESHOLD:
         prop_part_drag_threshold_update(pd);
         break;
      case ATTRIBUTE_PART_DRAG_EVENT:
         prop_part_drag_event_update(pd);
         break;
      case ATTRIBUTE_PART_NAME:
         prop_part_name_update(pd);
         break;
      case ATTRIBUTE_PART_SOURCE:
         prop_part_source_update(pd);
         prop_state_textblock_source_update(pd);
         break;
      case ATTRIBUTE_PART_SOURCE2:
         prop_state_textblock_source2_update(pd);
         break;
      case ATTRIBUTE_PART_SOURCE3:
         prop_state_textblock_source3_update(pd);
         break;
      case ATTRIBUTE_PART_SOURCE4:
         prop_state_textblock_source4_update(pd);
         break;
      case ATTRIBUTE_PART_SOURCE5:
         prop_state_textblock_source5_update(pd);
         break;
      case ATTRIBUTE_PART_SOURCE6:
         prop_state_textblock_source6_update(pd);
         break;
      case ATTRIBUTE_GROUP_NAME:
        prop_group_name_update(pd);
        break;
      case ATTRIBUTE_PART_ITEM_ASPECT_MODE:
        PART_ITEM_ATTR_1COMBOBOX_LIST_UPDATE(part_item, aspect_mode, part_item);
        break;
      case ATTRIBUTE_PART_ITEM_ALIGN_X:
        COMMON_1SPINNER_UPDATE(part_item, align_x, part_item, double,  100, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_ALIGN_Y:
        COMMON_1SPINNER_UPDATE(part_item, align_y, part_item, double,  100, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_WEIGHT_X:
        COMMON_1SPINNER_UPDATE(part_item, align_x, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_WEIGHT_Y:
        COMMON_1SPINNER_UPDATE(part_item, align_y, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_ASPECT_H:
        COMMON_1SPINNER_UPDATE(part_item, aspect_h, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_ASPECT_W:
        COMMON_1SPINNER_UPDATE(part_item, aspect_w, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_MAX_H:
        COMMON_1SPINNER_UPDATE(part_item, max_h, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_MAX_W:
        COMMON_1SPINNER_UPDATE(part_item, max_w, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_MIN_H:
        COMMON_1SPINNER_UPDATE(part_item, min_h, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_MIN_W:
        COMMON_1SPINNER_UPDATE(part_item, min_w, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_PREFER_H:
        COMMON_1SPINNER_UPDATE(part_item, prefer_h, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_PREFER_W:
        COMMON_1SPINNER_UPDATE(part_item, prefer_w, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_SPREAD_H:
        COMMON_1SPINNER_UPDATE(part_item, spread_h, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_SPREAD_W:
        COMMON_1SPINNER_UPDATE(part_item, spread_w, part_item, int,  1, PART_ITEM_ARGS);
        break;
      case ATTRIBUTE_PART_ITEM_SPAN_COL:
      case ATTRIBUTE_PART_ITEM_SPAN_ROW:
      case ATTRIBUTE_PART_ITEM_POSITION_COL:
      case ATTRIBUTE_PART_ITEM_POSITION_ROW:
         TODO("implement, old update macroses are broken atm");
        /*PART_ITEM_DOUBLEVAL_ATTR_2SPINNER_UPDATE(part_item, span_col, span_row, part_item, unsigned char, 1);
        PART_ITEM_DOUBLEVAL_ATTR_2SPINNER_UPDATE(part_item, position_col, position_row, part_item, unsigned short, 1);*/
         break;
      case ATTRIBUTE_PART_ITEM_SOURCE:
         prop_part_item_source_update(pd);
         break;
      case ATTRIBUTE_PART_ITEM_PADDING:
         prop_part_item_padding_update(pd);
         break;
      case ATTRIBUTE_STATE_CONTAINER_ALIGN_X:
      case ATTRIBUTE_STATE_CONTAINER_ALIGN_Y:
         TODO("implement");
         //STATE_DOUBLEVAL_ATTR_2SPINNER_UPDATE(state_container, align_x, align_y, state_container, double, 100)
         break;
      case ATTRIBUTE_STATE_CONTAINER_MIN_V:
      case ATTRIBUTE_STATE_CONTAINER_MIN_H:
         TODO("implement");
         //STATE_DOUBLEVAL_ATTR_2CHECK_UPDATE(state_container, min_v, min_h, state_container)
         break;
      case ATTRIBUTE_STATE_TABLE_HOMOGENEOUS:
         STATE_ATTR_1COMBOBOX_LIST_UPDATE(state, table_homogeneous, state);
         break;
      case ATTRIBUTE_STATE_CONTAINER_PADING_X:
      case ATTRIBUTE_STATE_CONTAINER_PADING_Y:
         TODO("implement");
         //STATE_DOUBLEVAL_ATTR_2SPINNER_UPDATE(state_container, padding_x, padding_y, state_container, int, 1)
         break;
      case ATTRIBUTE_STATE_MINMUL_H:
         COMMON_1SPINNER_UPDATE(state, minmul_h, state, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_STATE_MINMUL_W:
         COMMON_1SPINNER_UPDATE(state, minmul_w, state, double, 1, STATE_ARGS);
         break;
      case ATTRIBUTE_PART_SELECT_MODE:
         PART_ATTR_1COMBOBOX_LIST_UPDATE(part, select_mode, state_textblock);
         break;
      case ATTRIBUTE_PART_ENTRY_MODE:
         PART_ATTR_1COMBOBOX_LIST_UPDATE(part, entry_mode, state_textblock);
         break;
      case ATTRIBUTE_PART_POINTER_MODE:
         PART_ATTR_1COMBOBOX_LIST_UPDATE(part, pointer_mode, state_textblock);
         break;
      case ATTRIBUTE_PART_CURSOR_MODE:
         PART_ATTR_1COMBOBOX_LIST_UPDATE(part, cursor_mode, state_textblock)
         break;
      case ATTRIBUTE_STATE_FILL_TYPE:
         STATE_ATTR_1COMBOBOX_LIST_UPDATE(state_fill, type, state_fill);
         break;
      case ATTRIBUTE_STATE_IMAGE_TWEEN:
         prop_item_state_image_tween_update(pd->attributes.state_image.tween, pd);
         break;
      case ATTRIBUTE_PROGRAM_SIGNAL:
         prop_program_signal_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_SOURCE:
         prop_program_source_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_ACTION:
         prop_program_action_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_STATE:
         prop_program_state_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_STATE2:
         prop_program_state2_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_VALUE:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, value, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_VALUE2:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, value2, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_TARGET:
         prop_program_targets_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_SAMPLE_NAME:
         prop_program_sample_name_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_SAMPLE_SPEED:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, sample_speed, program, int, 1)
         break;
      case ATTRIBUTE_PROGRAM_AFTER:
         prop_program_afters_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_CHANNEL:
         PROGRAM_ATTR_1COMBOBOX_LIST_UPDATE(program, channel, program)
         break;
      case ATTRIBUTE_PROGRAM_TONE_NAME:
         prop_program_tone_name_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_TONE_DURATION:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, tone_duration, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_TRANSITION_TYPE:
         prop_program_transition_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_TRANSITION_TIME:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, transition_time, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_TRANSITION_VALUE1:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, transition_value1, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_TRANSITION_VALUE2:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, transition_value2, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_TRANSITION_VALUE3:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, transition_value3, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_TRANSITION_VALUE4:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, transition_value4, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_TRANSITION_FROM_CURRENT:
         PROGRAM_ATTR_1CHECK_UPDATE(program, transition_from_current, program)
         break;
      case ATTRIBUTE_PROGRAM_IN_FROM:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, in_range, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_IN_RANGE:
         PROGRAM_ATTR_1SPINNER_UPDATE(program, in_from, program, double, 1)
         break;
      case ATTRIBUTE_PROGRAM_FILTER_PART:
         prop_program_filter_part_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_FILTER_STATE:
         prop_program_filter_state_update(pd);
         break;
      case ATTRIBUTE_PROGRAM_API_NAME:
      case ATTRIBUTE_PROGRAM_API_DESCRIPTION:
      case ATTRIBUTE_PROGRAM_NAME:
         TODO("implement");
         break;
         /* Don't add 'default:'. Compiler must warn about missing cases */
     }
}

Evas_Object *
ui_property_group_add(Evas_Object *parent)
{
   Evas_Object *box, *scroller;
   Group_Prop_Data *pd;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Group_Prop_Data));

   SCROLLER_ADD(parent, scroller);
   BOX_ADD(scroller, box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);

   pd->scroller = scroller;
   elm_scroller_policy_set(pd->scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

   evas_object_data_set(pd->scroller, GROUP_PROP_DATA, pd);

   /* register global callbacks */
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_SELECTED, _on_part_selected, pd->scroller);
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_STATE_SELECTED, _on_part_state_selected, pd->scroller);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROGRAM_SELECTED, _on_program_selected, pd->scroller);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROGRAM_UNSELECTED, _on_program_unselected, pd->scroller);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, _on_editor_attribute_changed, pd->scroller);

   return pd->scroller;
}

static void
_on_group_name_change(void *data,
                      Evas_Object *obj,
                      void *ei __UNUSED__)
{
return;
TODO("Implement rename. Note: groups list must remain sorted")
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   const char *value;
   char *entry;

   entry = elm_entry_markup_to_utf8(elm_entry_entry_get(obj));
//   value = wm_style_name_set(pd->wm_style, entry);

   if (!edje_edit_group_name_set(pd->group->edit_object, value)) return;

   elm_object_focus_set(obj, true);
   //project_changed(false);
   free(entry);
}
static void
_on_group_name_activated(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
return;
TODO("Implement rename. Note: groups list must remain sorted")
}

#define GROUP_ATTR_2SPINNER(TEXT, SUB1, SUB2, VALUE1, VALUE2, DESCRIPTION1, DESCRIPTION2) \
   GROUP_ATTR_2SPINNER_CALLBACK(SUB1, SUB2, VALUE1, DESCRIPTION1) \
   GROUP_ATTR_2SPINNER_CALLBACK(SUB1, SUB2, VALUE2, DESCRIPTION2) \
   GROUP_ATTR_2SPINNER_UPDATE(SUB1, VALUE1, VALUE2) \
   GROUP_ATTR_2SPINNER_ADD(TEXT, SUB1, VALUE1, VALUE2)

/*
 * Edje Edit API have't API for get the name from loaded image. It's first
 * reason for create this macro.
 *
 * Second: we need to get the different names for layouts and for styles. */
#define edje_edit_group_name_get(OBJ) \
   eina_stringshare_add(pd->group->name)

#define GROUP_ATTR_1ENTRY(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP) \
   GROUP_ATTR_1ENTRY_UPDATE(SUB, VALUE, MEMBER) \
   GROUP_ATTR_1ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP)

GROUP_ATTR_2SPINNER(_("min"), min, max, w, h, _("group_min_w changed from %d to %d"),
                                              _("group_min_h changed from %d to %d"))
GROUP_ATTR_2SPINNER(_("max"), max, min, w, h, _("group_max_w changed from %d to %d"),
                                              _("group_max_h changed from %d to %d"))
GROUP_ATTR_1ENTRY(_("name"), group, name, group, NULL, _("Name of the group."))

#define pd_group pd->attributes.group

static void
_on_dismissed(void *data __UNUSED__,
               Evas_Object *obj,
               void *event_info __UNUSED__)
{
   evas_object_hide(obj);
}
static void
_on_clicked(void *data,
             Evas *e,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   int x, y;
   Group_Prop_Data *pd = (Group_Prop_Data *)data;

   assert(pd != NULL);

   evas_object_smart_callback_del_full(pd_group.ctxpopup, "dismissed",
                                       _on_dismissed, pd);
   evas_object_smart_callback_add(pd_group.ctxpopup, "dismissed",
                                  _on_dismissed, pd);
   evas_pointer_canvas_xy_get(e, &x, &y);
   evas_object_move(pd_group.ctxpopup, x, y);
   evas_object_show(pd_group.ctxpopup);
}

static void
_prop_item_alias_update(Group_Prop_Data *pd, int aliases_count)
{
   Evas_Object *label, *label_ctx;
   Eina_List *l;
   const char *text_info = NULL;
   char *list_data;
   Eina_Strbuf *text_ctx;

   assert(pd != NULL);
   assert(pd->group != NULL);

   label = elm_object_part_content_get(pd_group.info, "elm.swallow.content");
   label_ctx = elm_object_content_get(pd_group.ctxpopup);
   if (pd->group->main_group != NULL)
     {
        text_info = eina_stringshare_printf(_("This is alias of <a>%s</a>"),
                                            pd->group->main_group->name);
     }
   else
     {
        text_info = eina_stringshare_printf(_("changes in this style will"
                                              "also affect <a>%d elements.</a>"),
                                            aliases_count);

        int count = 1;
        text_ctx = eina_strbuf_new();

        EINA_LIST_FOREACH(pd->group->aliases, l, list_data)
          {
             const char *step = "%d. %s";
             if (eina_strbuf_length_get(text_ctx) > 0)
               eina_strbuf_append(text_ctx, "</br>");
             step = eina_stringshare_printf(step, count++, list_data);
             eina_strbuf_append(text_ctx, step);
          }

        evas_object_event_callback_del_full(pd_group.info, EVAS_CALLBACK_MOUSE_DOWN,
                                       _on_clicked, pd);
        evas_object_event_callback_add(pd_group.info, EVAS_CALLBACK_MOUSE_DOWN,
                                       _on_clicked, pd);
        elm_object_text_set(label_ctx, eina_strbuf_string_get(text_ctx));
        eina_strbuf_free(text_ctx);
     }

   elm_object_text_set(label, text_info);
   evas_object_show(pd_group.info);
   eina_stringshare_del(text_info);
}

static void
_prop_item_shared_check_update(Evas_Object *item, int count)
{
   Evas_Object *entry;

   assert(item != NULL);

   entry = elm_object_part_content_get(item, "info");
   Eina_Bool bool = false;
   if (count > 0) bool = true;
   elm_check_state_set(entry, bool);
   evas_object_show(item);
}

void
ui_property_group_set(Evas_Object *property, Group *group)
{
   Evas_Object *group_frame, *box, *prop_box, *info_en = NULL;
   Evas_Object *item;
   Evas_Object *info_image;
   Evas_Object *check, *label_ctx;
   Eina_List *l;
   const char *text_info = NULL;
   int aliases_count = 0;
   char *list_data;
   Eina_Strbuf *text_ctx = NULL;

   GROUP_PROP_DATA_GET()
   assert(group != NULL);

   ui_property_group_unset(property);

   evas_object_show(property);

   elm_scroller_policy_set(pd->scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);

   pd->group = group;

   prop_box = elm_object_content_get(pd->scroller);
   aliases_count = eina_list_count(group->aliases);

   if (!pd_group.info)
     {
        LABEL_ADD(property, info_en, text_info)

        ICON_STANDARD_ADD(property, info_image, false, "info");

        pd_group.info = elm_layout_add(property);
        evas_object_size_hint_weight_set(pd_group.info, EVAS_HINT_EXPAND, 0.0);
        evas_object_size_hint_align_set(pd_group.info, EVAS_HINT_FILL, 0.0);
        elm_layout_file_set(pd_group.info, EFLETE_EDJ, "eflete/property/item/info");

        pd_group.ctxpopup = elm_ctxpopup_add(main_window_get());
        elm_ctxpopup_direction_priority_set(pd_group.ctxpopup,
                                            ELM_CTXPOPUP_DIRECTION_DOWN,
                                            ELM_CTXPOPUP_DIRECTION_DOWN,
                                            ELM_CTXPOPUP_DIRECTION_DOWN,
                                            ELM_CTXPOPUP_DIRECTION_DOWN);
        elm_object_style_set(pd_group.ctxpopup, "info");
        evas_object_hide(pd_group.ctxpopup);

        if (group->main_group != NULL) /* group is alias */
          text_info = eina_stringshare_printf(_("This is alias of <a>%s</a>"),
                                              group->main_group->name);
        else
          {
             text_info = eina_stringshare_printf(_("changes in this style will also"
                                                 "affect <a>%d elements.</a>"),
                                                 aliases_count);
             int count = 1;
             text_ctx = eina_strbuf_new();

             EINA_LIST_FOREACH(group->aliases, l, list_data)
               {
                  const char *step = "%d. %s";
                  if (eina_strbuf_length_get(text_ctx) > 0)
                    eina_strbuf_append(text_ctx, "</br>");
                  step = eina_stringshare_printf(step, count++, list_data);
                  eina_strbuf_append(text_ctx, step);
               }
             evas_object_event_callback_add(pd_group.info, EVAS_CALLBACK_MOUSE_DOWN,
                                            _on_clicked, pd);
             LABEL_ADD(property, label_ctx, eina_strbuf_string_get(text_ctx))
             eina_strbuf_free(text_ctx);
             elm_object_style_set(label_ctx, "info");
             elm_object_content_set(pd_group.ctxpopup, label_ctx);
          }

        elm_object_text_set(info_en, text_info);
        eina_stringshare_del(text_info);
        elm_object_style_set(info_en, "info");
        elm_object_part_content_set(pd_group.info, "elm.swallow.content", info_en);
        elm_object_part_content_set(pd_group.info, "info", info_image);
     }

   if (!pd_group.shared_check)
     {
        CHECK_ADD(property, check)
        elm_object_text_set(check, _("Shared style"));
        if (aliases_count > 0) elm_check_state_set(check, true);
        elm_object_disabled_set(check, true);

        pd_group.shared_check = elm_layout_add(property);
        evas_object_size_hint_weight_set(pd_group.shared_check, EVAS_HINT_EXPAND, 0.0);
        evas_object_size_hint_align_set(pd_group.shared_check, EVAS_HINT_FILL, 0.0);
        elm_layout_file_set(pd_group.shared_check, EFLETE_EDJ, "eflete/property/item/info");
        elm_layout_signal_emit(pd_group.shared_check, "prop_item,content,hide", "eflete");
        elm_object_part_content_set(pd_group.shared_check, "info", check);
        evas_object_show(pd_group.shared_check);
     }

   if (!pd_group.frame)
     {
        FRAME_PROPERTY_ADD(property, group_frame, true, _("Layout property"), pd->scroller)
        BOX_ADD(group_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(group_frame, box);

        item = prop_group_name_add(box, pd, NULL);
        elm_object_disabled_set(pd_group.name, true);

        elm_box_pack_end(box, item);
        pd_group.min_item = prop_group_min_w_h_add(box, pd,
                                                   _("Minimum group width in pixels."),
                                                   _("Minimum group height in pixels."));
        elm_box_pack_end(box, pd_group.min_item);
        pd_group.max_item = prop_group_max_w_h_add(box, pd,
                                                   _("Maximum group width in pixels."),
                                                   _("Maximum group height in pixels."));
        elm_box_pack_end(box, pd_group.max_item);

        elm_box_pack_start(prop_box, group_frame);
        pd_group.frame = group_frame;
     }
   else
     {
        if ((aliases_count > 0) || (group->main_group != NULL))
          {
             _prop_item_alias_update(pd, aliases_count);
             evas_object_show(pd_group.info);
          }
        _prop_item_shared_check_update(pd_group.shared_check, aliases_count);
        prop_group_name_update(pd);
        prop_group_min_w_h_update(pd);
        prop_group_max_w_h_update(pd);
        evas_object_show(pd_group.frame);
     }
   if ((aliases_count > 0) || (group->main_group != NULL))
     {
        elm_box_pack_start(prop_box, pd_group.info);
        evas_object_show(pd_group.info);
     }
   if (group->main_group != NULL)
     {
        elm_object_disabled_set(pd_group.min_item, true);
        elm_object_disabled_set(pd_group.max_item, true);
     }
   else
     {
        elm_object_disabled_set(pd_group.min_item, false);
        elm_object_disabled_set(pd_group.max_item, false);
     }
   elm_box_pack_start(prop_box, pd_group.shared_check);
}

void
ui_property_group_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   GROUP_PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->scroller);
   evas_object_hide(pd_group.info);
   elm_box_unpack(prop_box, pd_group.info);
   elm_box_unpack(prop_box, pd_group.shared_check);
   evas_object_event_callback_del_full(pd_group.info, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on_clicked, pd);
   evas_object_hide(pd_group.frame);
   evas_object_hide(pd_group.shared_check);
   _ui_property_program_unset(property);
   _ui_property_part_unset(property);
   elm_scroller_policy_set(pd->scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

   evas_object_hide(property);
}
#undef pd_group

static void
prop_part_name_update(Group_Prop_Data *pd)
{
   char *text;

   assert(pd != NULL);

   text = elm_entry_utf8_to_markup(pd->part->name);
   TODO("check that text is not NULL should be not needed after refactor");
   if ((elm_entry_entry_get(pd->attributes.part.name) != NULL) && (strcmp(text, elm_entry_entry_get(pd->attributes.part.name))))
     elm_entry_entry_set(pd->attributes.part.name, pd->part->name);
   free(text);
}

static void
_on_part_name_change(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *ei __UNUSED__)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   const char *text;
   char *value;
   assert(pd != NULL);
   if (resource_name_validator_status_get(pd->attributes.part.validator) != ELM_REG_NOERROR)
     return;
   if (!pd->change) pd->change = change_add(NULL);
   text = elm_entry_entry_get(obj);
   value = elm_entry_markup_to_utf8(text);
   if (!editor_part_name_set(pd->group->edit_object, pd->change, true, pd->part->name, value))
     {
        ERR("Wrong input value for name field");
        abort();
     }
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
   free(value);
}
static void
_on_part_name_unfocused(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   assert(pd != NULL);
   if (resource_name_validator_status_get(pd->attributes.part.validator) != ELM_REG_NOERROR)
     return;
   if (!pd->change)
     return;
   Eina_Stringshare *msg = eina_stringshare_printf(_("part name changed to \"%s\""), pd->part->name);
   change_description_set(pd->change, msg);
   history_change_add(pd->group->history, pd->change);
   pd->change = NULL;
   prop_part_name_update(pd);
   eina_stringshare_del(msg);
}

static Evas_Object *
prop_part_name_add(Evas_Object *parent, Group_Prop_Data *pd)
{
   assert(parent != NULL);
   assert(pd != NULL);

   PROPERTY_ITEM_ADD(parent,  _("name"), "1swallow");
   ENTRY_ADD(parent, pd->attributes.part.name, true);
   eo_event_callback_add(pd->attributes.part.name, ELM_ENTRY_EVENT_VALIDATE, resource_name_validator_helper, pd->attributes.part.validator);
   resource_name_validator_list_set(pd->attributes.part.validator, &pd->part->group->parts, false);
   resource_name_validator_resource_set(pd->attributes.part.validator, (Resource *)pd->part);
   elm_entry_entry_set(pd->attributes.part.name, pd->part->name);
   elm_object_tooltip_text_set(pd->attributes.part.name, _("Selected part name"));
   evas_object_smart_callback_add(pd->attributes.part.name, "changed,user", _on_part_name_change, pd);
   evas_object_smart_callback_add(pd->attributes.part.name, "unfocused", _on_part_name_unfocused, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->attributes.part.name);
   prop_part_name_update(pd);

   return item;
}

static void
prop_part_clip_to_update(Group_Prop_Data *pd)
{
   Part *part;
   Eina_List *l;
   Eina_Stringshare *value;

   assert(pd != NULL);

   ewe_combobox_items_list_free(pd->attributes.part.clip_to, true);
   value = edje_edit_part_clip_to_get(pd->group->edit_object, pd->part->name);
   if (value)
     ewe_combobox_text_set(pd->attributes.part.clip_to, value);
   else
     ewe_combobox_text_set(pd->attributes.part.clip_to, _("None"));
   ewe_combobox_item_add(pd->attributes.part.clip_to, _("None"));
   EINA_LIST_FOREACH(pd->group->parts, l, part)
     {
        if ((part != pd->part) && (part->type == EDJE_PART_TYPE_RECTANGLE))
           ewe_combobox_item_add(pd->attributes.part.clip_to, part->name);
     }
   edje_edit_string_free(value);
}

inline static void
prop_part_drag_control_disable_set(Group_Prop_Data *pd, Eina_Bool collapse)
{
   Eina_Bool bx, by;

   assert(pd != NULL);

   bx = edje_edit_part_drag_x_get(pd->group->edit_object, pd->part->name);
   by = edje_edit_part_drag_y_get(pd->group->edit_object, pd->part->name);

   elm_object_disabled_set(pd->attributes.part_drag.step_x, !bx);
   elm_object_disabled_set(pd->attributes.part_drag.step_y, !by);
   elm_object_disabled_set(pd->attributes.part_drag.confine, !(bx | by));
   elm_object_disabled_set(pd->attributes.part_drag.threshold, !(bx | by));

   if (collapse)
     elm_frame_collapse_set(pd->attributes.part_drag.frame, !(bx | by));
}

PART_ATTR_PARTS_LIST(part_drag, confine, part_drag)
PART_ATTR_PARTS_LIST(part_drag, threshold, part_drag)
PART_ATTR_PARTS_LIST(part_drag, event, part_drag)

PART_ATTR_SOURCE_UPDATE(part, source)

#define PROGRAMM_ATTR_1ENTRY(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP, DESCRIPTION) \
   PROGRAM_ATTR_1ENTRY_UPDATE(SUB, VALUE, MEMBER) \
   PROGRAM_ATTR_1ENTRY_CALLBACK(SUB, VALUE, VALIDATOR, DESCRIPTION) \
   PROGRAM_ATTR_1ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP)

#define PROGRAM_ATTR_1COMBOBOX_LIST(TEXT, SUB, VALUE, MEMBER, LIST, TYPE, DESCRIPTION, TOOLTIP) \
   PROGRAM_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, DESCRIPTION) \
   PROGRAM_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP)

#define PROGRAM_ATTR_1SPINNER(TEXT, SUB, VALUE, MEMBER, TYPE, MIN, MAX, STEP, FMT, \
                              L_START, L_END, TOOLTIP, MULTIPLIER, DESCRIPTION) \
   PROGRAM_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, DESCRIPTION) \
   PROGRAM_ATTR_1SPINNER_ADD(TEXT, SUB, VALUE, MEMBER, MIN, MAX, STEP, FMT, \
                             L_START, L_END, TOOLTIP, MULTIPLIER)
#define PROGRAM_ATTR_1CHECK(TEXT, SUB, VALUE, MEMBER, TOOLTIP, DESCRIPTION) \
   PROGRAM_ATTR_CHECK_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   PROGRAM_ATTR_1CHECK_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

static void
_on_program_name_change(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   return;
   TODO("Implement rename. Note: program(resource) list must remain sorted")
}

static void
_on_program_name_activated(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *ei __UNUSED__)
{
   return;
   TODO("Implement rename. Note: program(resource) list must remain sorted")
}

static void
prop_program_name_update(Group_Prop_Data *pd)
{
   elm_entry_entry_set(pd->attributes.program.name, pd->attributes.program.program);
}

COMMON_ENTRY_ADD(_("name"), program, name, program, NULL, _("Name of the group."))
PROGRAMM_ATTR_1ENTRY(_("signal"), program, signal, program, NULL,
   _("The signal name for triger"), _("signal is changed to '%s'"))
PROGRAMM_ATTR_1ENTRY(_("source"), program, source, program, NULL,
   _("The source of signal"), _("signal source is changed to '%s'"))
PROGRAM_ATTR_1SPINNER("don't forgot to change this title", program, value, program, double, 0.0, 1.0, 0.1, "%.2f",
   NULL, NULL, "", 1, _("Program action value is changed from %f to %f"))
PROGRAM_ATTR_1SPINNER("don't forgot to change this title", program, value2, program, double, 0.0, 1.0, 0.1, "%.2f",
   NULL, NULL, "", 1, _("Program action value2 is changed from %f to %f"))
PROGRAM_ATTR_1SPINNER(_("sample speed"), program, sample_speed, program, int, 0.0, 9999.0, 1.0, "%.0f",
   NULL, NULL, "", 1, _("Program sample speed is changed from %d to %d"))
PROGRAM_ATTR_1COMBOBOX_LIST(_("sample channel"), program, channel, program, sound_channel, unsigned char,
   _("Program action state is changed to '%s'"), "")
PROGRAM_ATTR_1SPINNER(_("time"), program, transition_time, program, double, 0.0, 9999.0, 0.1, "%.2f",
   NULL, NULL, "", 1, _("Program transition time is changed from %f to %f"))
PROGRAM_ATTR_1SPINNER("don't forgot to change this title", program, transition_value1, program, double, 0.0, 9999.0, 0.1, "%.2f",
   NULL, NULL, "", 1, _("Program transition value1 is changed from %f to %f"))
PROGRAM_ATTR_1SPINNER("don't forgot to change this title", program, transition_value2, program, double, 0.0, 9999.0, 0.1, "%.2f",
   NULL, NULL, "", 1, _("Program transition value1 is changed from %f to %f"))
PROGRAM_ATTR_1SPINNER("don't forgot to change this title", program, transition_value3, program, double, 0.0, 9999.0, 0.1, "%.2f",
   NULL, NULL, "", 1, _("Program transition value1 is changed from %f to %f"))
PROGRAM_ATTR_1SPINNER("don't forgot to change this title", program, transition_value4, program, double, 0.0, 9999.0, 0.1, "%.2f",
   NULL, NULL, "", 1, _("Program transition value1 is changed from %f to %f"))
PROGRAM_ATTR_1CHECK(_("from current"), program, transition_from_current, program, "",
                    _("Program transition attribure 'from current' is changed to %s'"))

COMMON_ENTRY_UPDATE(program, state, program, PROGRAM_ARGS)
COMMON_ENTRY_CALLBACK(program, state, NULL, PROGRAM_ARGS, _("Program action state is changed to '%s'"))
COMMON_ENTRY_UPDATE(program, state2, program, PROGRAM_ARGS)
COMMON_ENTRY_CALLBACK(program, state2, NULL, PROGRAM_ARGS, _("Program action state2 is changed to '%s'"))
COMMON_SPINNER_CALLBACK(program, tone_duration, program, double, 1, PROGRAM_ARGS, _("Program action value is changed from %f to %f"))

static Evas_Object *
_prop_action_state_add(Group_Prop_Data *pd, Evas_Object *parent, const char *title, const char *tooltip)
{
   Eina_Stringshare *state;

   PROPERTY_ITEM_ADD(parent, title, "1swallow")
   ENTRY_ADD(item, pd->attributes.program.state, true);
   state = edje_edit_program_state_get(pd->group->edit_object, pd->attributes.program.program);
   elm_entry_entry_set(pd->attributes.program.state, state);
   eina_stringshare_del(state);
   evas_object_smart_callback_add(pd->attributes.program.state, "changed,user", _on_program_state_change, pd);
   evas_object_smart_callback_add(pd->attributes.program.state, "activated", _on_program_state_activated, pd);
   evas_object_smart_callback_add(pd->attributes.program.state, "unfocused", _on_program_state_activated, pd);
   elm_object_tooltip_text_set(pd->attributes.program.state, tooltip);
   elm_layout_content_set(item, NULL, pd->attributes.program.state);

   return item;
}

static Evas_Object *
_prop_action_state2_add(Group_Prop_Data *pd, Evas_Object *parent, const char *title, const char *tooltip)
{
   Eina_Stringshare *state2;

   PROPERTY_ITEM_ADD(parent, title, "1swallow")
   ENTRY_ADD(item, pd->attributes.program.state2, true)
   state2 = edje_edit_program_state2_get(pd->group->edit_object, pd->attributes.program.program);
   elm_entry_entry_set(pd->attributes.program.state2, state2);
   eina_stringshare_del(state2);
   evas_object_smart_callback_add(pd->attributes.program.state2, "changed,user", _on_program_state2_change, pd);
   evas_object_smart_callback_add(pd->attributes.program.state2, "activated", _on_program_state2_activated, pd);
   evas_object_smart_callback_add(pd->attributes.program.state2, "unfocused", _on_program_state2_activated, pd);
   elm_object_tooltip_text_set(pd->attributes.program.state2, tooltip);
   elm_layout_content_set(item, NULL, pd->attributes.program.state2);

   return item;
}

static void
prop_program_sample_name_update(Group_Prop_Data *pd)
{
   Eina_Stringshare *value;
   Eina_List *l;
   Resource *sample;

   value = edje_edit_program_sample_name_get(pd->group->edit_object, pd->attributes.program.program);
   ewe_combobox_text_set(pd->attributes.program.sample_name, value ? value : _("None"));
   ewe_combobox_item_add(pd->attributes.program.sample_name, _("None"));
   EINA_LIST_FOREACH(ap.project->sounds, l, sample)
     {
        ewe_combobox_item_add(pd->attributes.program.sample_name, sample->name);
     }
}

static void
prop_program_tone_name_update(Group_Prop_Data *pd)
{
   Eina_Stringshare *value;
   Eina_List *l;
   Resource *tone;

   value = edje_edit_program_tone_name_get(pd->group->edit_object, pd->attributes.program.program);
   ewe_combobox_text_set(pd->attributes.program.tone_name, value ? value : _("None"));
   ewe_combobox_item_add(pd->attributes.program.tone_name, _("None"));
   EINA_LIST_FOREACH(ap.project->tones, l, tone)
     {
        ewe_combobox_item_add(pd->attributes.program.tone_name, tone->name);
     }
}

static void
_on_program_sample(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info;
   Change *change = change_add("Change the action sount to '%s'");

   if (!editor_program_sample_name_set(pd->group->edit_object, change, false, pd->attributes.program.program, item->title))
     {
        ERR("Cann't apply value '%s' for attribute 'program sample'.", item->title);
        abort();
     }
   history_change_add(pd->group->history, change);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
}

static Evas_Object *
_prop_action_sample_name(Group_Prop_Data *pd, Evas_Object *parent)
{
   PROPERTY_ITEM_ADD(parent, "name", "1swallow")
   EWE_COMBOBOX_ADD(item, pd->attributes.program.sample_name);
   evas_object_smart_callback_add(pd->attributes.program.sample_name, "selected", _on_program_sample, pd);
   elm_object_tooltip_text_set(pd->attributes.program.sample_name, "");
   elm_layout_content_set(item, NULL, pd->attributes.program.sample_name);
   prop_program_sample_name_update(pd);
   return item;
}

static void
_on_program_tone(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info;
   Change *change = change_add("Change the action tone to '%s'");

   if (!editor_program_tone_name_set(pd->group->edit_object, change, false, pd->attributes.program.program, item->title))
     {
        ERR("Cann't apply value '%s' for attribute 'program tone'.", item->title);
        abort();
     }
   history_change_add(pd->group->history, change);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
}

static Evas_Object *
_prop_action_tone_name(Group_Prop_Data *pd, Evas_Object *parent)
{
   PROPERTY_ITEM_ADD(parent, "name", "1swallow")
   EWE_COMBOBOX_ADD(item, pd->attributes.program.tone_name);
   evas_object_smart_callback_add(pd->attributes.program.tone_name, "selected", _on_program_tone, pd);
   elm_object_tooltip_text_set(pd->attributes.program.tone_name, "");
   elm_layout_content_set(item, NULL, pd->attributes.program.tone_name);
   prop_program_tone_name_update(pd);
   return item;
}

static Evas_Object *
_prop_action_tone_duration_add(Group_Prop_Data *pd, Evas_Object *parent)
{
   PROPERTY_ITEM_ADD(parent, _("speed"), "2swallow")
   SPINNER_ADD(item, pd->attributes.program.tone_duration, 0.1, 10.0, 0.1, true);
   elm_spinner_label_format_set(pd->attributes.program.tone_duration, "%.1f");
   elm_spinner_value_set(pd->attributes.program.tone_duration,
                         edje_edit_program_tone_duration_get(pd->group->edit_object, pd->attributes.program.program));
   evas_object_smart_callback_add(pd->attributes.program.tone_duration, "changed", _on_program_tone_duration_change, pd);
   evas_object_smart_callback_add(pd->attributes.program.tone_duration, "spinner,drag,start", _on_program_tone_duration_start, pd);
   evas_object_smart_callback_add(pd->attributes.program.tone_duration, "spinner,drag,stop", _on_program_tone_duration_stop, pd);
   elm_object_tooltip_text_set(pd->attributes.program.tone_duration, "");
   elm_layout_content_set(item, "swallow.content1", pd->attributes.program.tone_duration);

   return item;
}

static void
_program_action_param_set(Group_Prop_Data *pd, Edje_Action_Type type)
{
   Evas_Object *box, *item;

   elm_frame_collapse_set(pd->attributes.program.action_params, false);
   elm_object_disabled_set(pd->attributes.program.action_params, false);

   evas_object_del(elm_object_content_get(pd->attributes.program.action_params));
   BOX_ADD(pd->attributes.program.action_params, box, false, false)
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(pd->attributes.program.action_params, box);

   switch (type)
     {
      case EDJE_ACTION_TYPE_STATE_SET:
         item = _prop_action_state_add(pd, box, _("state name"), "");
         elm_box_pack_end(box, item);
         item = prop_program_value_add(box, pd);
         elm_object_text_set(item, _("state value"));
         elm_box_pack_end(box, item);
         break;
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
         item = _prop_action_state_add(pd, box, _("signal name"), "");
         elm_box_pack_end(box, item);
         item = _prop_action_state2_add(pd, box, _("emmiter"), "");
         elm_box_pack_end(box, item);
         break;
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
         item = prop_program_value_add(box, pd);
         elm_object_text_set(item, _("axis X"));
         elm_box_pack_end(box, item);
         item = prop_program_value2_add(box, pd);
         elm_object_text_set(item, _("axis Y"));
         elm_box_pack_end(box, item);
         break;
      case EDJE_ACTION_TYPE_SOUND_SAMPLE:
         item = _prop_action_sample_name(pd, box);
         elm_box_pack_end(box, item);
         item = prop_program_sample_speed_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_channel_add(box, pd);
         elm_box_pack_end(box, item);
         break;
      case EDJE_ACTION_TYPE_SOUND_TONE:
         item = _prop_action_tone_name(pd, box);
         elm_box_pack_end(box, item);
         item = _prop_action_tone_duration_add(pd, box);
         elm_box_pack_end(box, item);
         break;
      case EDJE_ACTION_TYPE_NONE:
      default:
         elm_frame_collapse_set(pd->attributes.program.action_params, true);
         elm_object_disabled_set(pd->attributes.program.action_params, true);
         break;
     }
}

static void
prop_program_action_update(Group_Prop_Data *pd)
{
   Edje_Action_Type type = EDJE_ACTION_TYPE_NONE;

   type = edje_edit_program_action_get(pd->group->edit_object, pd->attributes.program.program);
   switch (type)
     {
      case EDJE_ACTION_TYPE_NONE:
         ewe_combobox_select_item_set(pd->attributes.program.action, 0);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_NONE);
         break;
      case EDJE_ACTION_TYPE_STATE_SET:
         ewe_combobox_select_item_set(pd->attributes.program.action, 1);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_STATE_SET);
         break;
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
         ewe_combobox_select_item_set(pd->attributes.program.action, 2);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_SIGNAL_EMIT);
         break;
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
         ewe_combobox_select_item_set(pd->attributes.program.action, 3);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_DRAG_VAL_SET);
         break;
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
         ewe_combobox_select_item_set(pd->attributes.program.action, 4);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_DRAG_VAL_STEP);
         break;
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
         ewe_combobox_select_item_set(pd->attributes.program.action, 5);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_DRAG_VAL_PAGE);
         break;
      case EDJE_ACTION_TYPE_SOUND_SAMPLE:
         ewe_combobox_select_item_set(pd->attributes.program.action, 6);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_SOUND_SAMPLE);
         break;
      case EDJE_ACTION_TYPE_SOUND_TONE:
         ewe_combobox_select_item_set(pd->attributes.program.action, 7);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_SOUND_TONE);
         break;
      case EDJE_ACTION_TYPE_ACTION_STOP:
         ewe_combobox_select_item_set(pd->attributes.program.action, 8);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_ACTION_STOP);
         break;
      default:
         ewe_combobox_select_item_set(pd->attributes.program.action, 0);
         _program_action_param_set(pd, EDJE_ACTION_TYPE_NONE);
         break;
     }
}

static void
_on_program_action_change(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info;
   Edje_Action_Type old_type, new_type = EDJE_ACTION_TYPE_NONE;
   Eina_Stringshare *msg;
   Change *change;

   old_type = edje_edit_program_action_get(pd->group->edit_object, pd->attributes.program.program);
   switch (item->index)
     {
      case 0:
         new_type = EDJE_ACTION_TYPE_NONE;
         break;
      case 1:
         new_type = EDJE_ACTION_TYPE_STATE_SET;
         break;
      case 2:
         new_type = EDJE_ACTION_TYPE_SIGNAL_EMIT;
         break;
      case 3:
         new_type = EDJE_ACTION_TYPE_DRAG_VAL_SET;
         break;
      case 4:
         new_type = EDJE_ACTION_TYPE_DRAG_VAL_STEP;
         break;
      case 5:
         new_type = EDJE_ACTION_TYPE_DRAG_VAL_PAGE;
         break;
      case 6:
         new_type = EDJE_ACTION_TYPE_SOUND_SAMPLE;
         break;
      case 7:
         new_type = EDJE_ACTION_TYPE_SOUND_TONE;
         break;
      case 8:
         new_type = EDJE_ACTION_TYPE_ACTION_STOP;
         break;
     }
   if (old_type == new_type) return;
   msg = eina_stringshare_printf(_("The program action changed to '%s'"), item->title);
   change = change_add(msg);
   eina_stringshare_del(msg);
   if (!editor_program_action_set(pd->group->edit_object, change, false, pd->attributes.program.program, new_type))
     {
        ERR("Cann't apply value '%s' for attribute 'action'.", item->title);
        abort();
     }
   history_change_add(pd->group->history, change);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
}

static Evas_Object *
prop_program_action_add(Evas_Object *parent, Group_Prop_Data *pd)
{
   int i;

   PROPERTY_ITEM_ADD(parent, _("action"), "1swallow")
   EWE_COMBOBOX_ADD(parent, pd->attributes.program.action)
   for (i = 0; edje_program_actions[i]; i++)
      ewe_combobox_item_add(pd->attributes.program.action, edje_program_actions[i]);
   elm_object_tooltip_text_set(pd->attributes.program.action, _("The program action type"));
   evas_object_smart_callback_add(pd->attributes.program.action, "selected", _on_program_action_change, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->attributes.program.action);

   return item;
}

static void
_program_transition_param_set(Group_Prop_Data *pd, Edje_Tween_Mode type)
{
   Evas_Object *box, *item;

   elm_frame_collapse_set(pd->attributes.program.transition_params, false);
   elm_object_disabled_set(pd->attributes.program.transition_params, false);

   evas_object_del(elm_object_content_get(pd->attributes.program.transition_params));
   BOX_ADD(pd->attributes.program.transition_params, box, false, false)
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(pd->attributes.program.transition_params, box);
   switch (type)
     {
      case EDJE_TWEEN_MODE_LINEAR:
         item = prop_program_transition_from_current_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_time_add(box, pd);
         elm_box_pack_end(box, item);
         break;
      case EDJE_TWEEN_MODE_ACCELERATE_FACTOR:
      case EDJE_TWEEN_MODE_DECELERATE_FACTOR:
      case EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR:
         item = prop_program_transition_from_current_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_time_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_value1_add(box, pd);
         elm_object_text_set(item, "factor");
         elm_box_pack_end(box, item);
         break;
      case EDJE_TWEEN_MODE_DIVISOR_INTERP:
         item = prop_program_transition_from_current_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_time_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_value1_add(box, pd);
         elm_object_text_set(item, "gradient");
         elm_box_pack_end(box, item);
         item = prop_program_transition_value2_add(box, pd);
         elm_object_text_set(item, "factor");
         elm_box_pack_end(box, item);
         break;
      case EDJE_TWEEN_MODE_BOUNCE:
         item = prop_program_transition_from_current_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_time_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_value1_add(box, pd);
         elm_object_text_set(item, "decay");
         elm_box_pack_end(box, item);
         item = prop_program_transition_value2_add(box, pd);
         elm_object_text_set(item, "bounces");
         elm_box_pack_end(box, item);
         break;
      case EDJE_TWEEN_MODE_SPRING:
         item = prop_program_transition_from_current_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_time_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_value1_add(box, pd);
         elm_object_text_set(item, "decay");
         elm_box_pack_end(box, item);
         item = prop_program_transition_value2_add(box, pd);
         elm_object_text_set(item, "swings");
         elm_box_pack_end(box, item);
         break;
      case EDJE_TWEEN_MODE_CUBIC_BEZIER:
         item = prop_program_transition_from_current_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_time_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_program_transition_value1_add(box, pd);
         elm_object_text_set(item, "x1");
         elm_box_pack_end(box, item);
         item = prop_program_transition_value2_add(box, pd);
         elm_object_text_set(item, "y1");
         elm_box_pack_end(box, item);
         item = prop_program_transition_value3_add(box, pd);
         elm_object_text_set(item, "x2");
         elm_box_pack_end(box, item);
         item = prop_program_transition_value4_add(box, pd);
         elm_object_text_set(item, "y2");
         elm_box_pack_end(box, item);
         break;
      case EDJE_TWEEN_MODE_NONE:
      default:
         break;
     }
}

static void
prop_program_transition_update(Group_Prop_Data *pd)
{
   Edje_Tween_Mode type = EDJE_TWEEN_MODE_NONE;

   /* disable the transition controls, and enable it if action is STATE_SET */
   if (EDJE_ACTION_TYPE_STATE_SET != edje_edit_program_action_get(pd->group->edit_object, pd->attributes.program.program))
     {
        elm_object_disabled_set(pd->attributes.program.transition, true);
        elm_object_disabled_set(pd->attributes.program.transition_params, true);
        ewe_combobox_select_item_set(pd->attributes.program.transition, 0);
        return;
     }
   elm_object_disabled_set(pd->attributes.program.transition, false);
   elm_object_disabled_set(pd->attributes.program.transition_params, false);

   type = editor_program_transition_type_get(pd->group->edit_object, pd->attributes.program.program);
   switch (type)
     {
      case EDJE_TWEEN_MODE_NONE:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 0);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_NONE);
         break;
      case EDJE_TWEEN_MODE_LINEAR:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 1);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_LINEAR);
         break;
      case EDJE_TWEEN_MODE_ACCELERATE_FACTOR:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 2);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_ACCELERATE_FACTOR);
         break;
      case EDJE_TWEEN_MODE_DECELERATE_FACTOR:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 3);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_DECELERATE_FACTOR);
         break;
      case EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 4);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR);
         break;
      case EDJE_TWEEN_MODE_DIVISOR_INTERP:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 5);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_DIVISOR_INTERP);
         break;
      case EDJE_TWEEN_MODE_BOUNCE:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 6);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_BOUNCE);
         break;
      case EDJE_TWEEN_MODE_SPRING:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 7);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_SPRING);
         break;
      case EDJE_TWEEN_MODE_CUBIC_BEZIER:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 8);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_CUBIC_BEZIER);
         break;
      default:
         ewe_combobox_select_item_set(pd->attributes.program.transition, 0);
         _program_transition_param_set(pd, EDJE_TWEEN_MODE_NONE);
         break;
     }
}
static void
_on_program_transition_change(void *data,
                              Evas_Object *obj __UNUSED__,
                              void *event_info)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info;
   Edje_Tween_Mode old_type, new_type = EDJE_TWEEN_MODE_NONE;
   Eina_Stringshare *msg;
   Change *change;

   old_type = editor_program_transition_type_get(pd->group->edit_object, pd->attributes.program.program);
   switch (item->index)
     {
      case 0:
         new_type = EDJE_TWEEN_MODE_NONE;
         break;
      case 1:
         new_type = EDJE_TWEEN_MODE_LINEAR;
         break;
      case 2:
         new_type = EDJE_TWEEN_MODE_ACCELERATE_FACTOR;
         break;
      case 3:
         new_type = EDJE_TWEEN_MODE_DECELERATE_FACTOR;
         break;
      case 4:
         new_type = EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR;
         break;
      case 5:
         new_type = EDJE_TWEEN_MODE_DIVISOR_INTERP;
         break;
      case 6:
         new_type = EDJE_TWEEN_MODE_BOUNCE;
         break;
      case 7:
         new_type = EDJE_TWEEN_MODE_SPRING;
         break;
      case 8:
         new_type = EDJE_TWEEN_MODE_CUBIC_BEZIER;
         break;
     }
   if (old_type == new_type) return;
   msg = eina_stringshare_printf(_("The program transition changed to '%s'"), item->title);
   change = change_add(msg);
   eina_stringshare_del(msg);
   if (!editor_program_transition_type_set(pd->group->edit_object, change, false, pd->attributes.program.program, new_type))
     {
        ERR("Cann't apply value '%s' for attribute 'transition'.", item->title);
        abort();
     }
   history_change_add(pd->group->history, change);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
}

static Evas_Object *
prop_program_transition_add(Evas_Object *parent, Group_Prop_Data *pd)
{
   int i;

   PROPERTY_ITEM_ADD(parent, _("transition"), "1swallow")
   EWE_COMBOBOX_ADD(parent, pd->attributes.program.transition)
   for (i = 0; edje_program_transitions[i]; i++)
      ewe_combobox_item_add(pd->attributes.program.transition, edje_program_transitions[i]);
   elm_object_tooltip_text_set(pd->attributes.program.transition, _("The program transition type"));
   evas_object_smart_callback_add(pd->attributes.program.transition, "selected", _on_program_transition_change, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->attributes.program.transition);

   return item;
}

PROGRAM_MULTIPLE_COMBOBOX(target, "Previous_Target", _("target can be part or program"), true)
PROGRAM_MULTIPLE_COMBOBOX(after, "Previous_After", _("after can be program"), false)

static void
prop_program_filter_part_update(Group_Prop_Data *pd)
{
   Part *part;
   Eina_List *l;
   Eina_Stringshare *value;
   ewe_combobox_items_list_free(pd->attributes.program.filter_part, true);
   value = edje_edit_program_filter_part_get(pd->group->edit_object PROGRAM_ARGS);
   ewe_combobox_item_add(pd->attributes.program.filter_part, _("None"));
   ewe_combobox_text_set(pd->attributes.program.filter_part, value ? value : _("None"));
   if (!value)
     {
        elm_object_disabled_set(pd->attributes.program.filter_state, true);
        ewe_combobox_text_set(pd->attributes.program.filter_state, _("None"));
     }
   EINA_LIST_FOREACH(pd->group->parts, l, part)
     {
        ewe_combobox_item_add(pd->attributes.program.filter_part, part->name);
     }
   edje_edit_string_free(value);
   prop_program_filter_state_update(pd);
}

static void
prop_program_filter_state_update(Group_Prop_Data *pd)
{
   Part *part;
   State *state;
   Eina_List *l;
   Eina_Stringshare *value, *part_name;
   ewe_combobox_items_list_free(pd->attributes.program.filter_state, true);
   part_name = edje_edit_program_filter_part_get(pd->group->edit_object PROGRAM_ARGS);
   if (!part_name)
     {
        elm_object_disabled_set(pd->attributes.program.filter_state, true);
        ewe_combobox_text_set(pd->attributes.program.filter_state, _("None"));
        return;
     }
   else
     elm_object_disabled_set(pd->attributes.program.filter_state, false);

   value = edje_edit_program_filter_state_get(pd->group->edit_object PROGRAM_ARGS);
   ewe_combobox_text_set(pd->attributes.program.filter_state, value);
   part = pm_resource_unsorted_get(pd->group->parts, part_name);
   EINA_LIST_FOREACH(part->states, l, state)
     {
        ewe_combobox_item_add(pd->attributes.program.filter_state, state->parsed_name);
     }
   edje_edit_string_free(value);
   edje_edit_string_free(part_name);
}

static void
_on_program_filter_part_change(void *data,
                               Evas_Object *obj __UNUSED__,
                               void *ei)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Ewe_Combobox_Item *item = ei;
   Eina_Bool isNone = false;
   Eina_Stringshare *old_val = edje_edit_program_filter_part_get(pd->group->edit_object
         PROGRAM_ARGS);
   Part *part;
   State *state;
   if (((item->index != 0) && (item->title == old_val)) /*stringshares*/ ||
       ((item->index == 0) && (old_val == NULL)))
     {
       eina_stringshare_del(old_val);
       return;
     }
   eina_stringshare_del(old_val);
   isNone = !strcmp(item->title, _("None"));
   Eina_Stringshare *msg = eina_stringshare_printf(_("filter part changed to \"%s\""),
                                                   isNone ? NULL : item->title);
   /* making change */
   Change *change = change_add(msg);
   eina_stringshare_del(msg);

   if (isNone)
     {
        editor_program_filter_state_set(pd->group->edit_object, change, false PROGRAM_ARGS,
                                        NULL);
     }
   else
     {
        part = pm_resource_unsorted_get(pd->group->parts, item->title);
        state = eina_list_data_get(part->states);
        editor_program_filter_state_set(pd->group->edit_object, change, false PROGRAM_ARGS,
                                        state->parsed_name);
     }

   editor_program_filter_part_set(pd->group->edit_object, change, false PROGRAM_ARGS,
         isNone ? NULL : item->title);

   history_change_add(pd->group->history, change);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
   prop_program_filter_state_update(pd);
}
static void
_on_program_filter_state_change(void *data,
                                Evas_Object *obj __UNUSED__,
                                void *ei)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Ewe_Combobox_Item *item = ei;
   Eina_Bool isNone = false;
   Eina_Stringshare *old_val = edje_edit_program_filter_state_get(pd->group->edit_object
         PROGRAM_ARGS);
   if (((item->index != 0) && (item->title == old_val)) /*stringshares*/ ||
       ((item->index == 0) && (old_val == NULL)))
     {
       eina_stringshare_del(old_val);
       return;
     }
   eina_stringshare_del(old_val);

   isNone = !strcmp(item->title, _("None"));
   Eina_Stringshare *msg = eina_stringshare_printf(_("filter state changed to \"%s\""),
                                                   isNone ? NULL : item->title);
   Change *change = change_add(msg);
   eina_stringshare_del(msg);
   editor_program_filter_state_set(pd->group->edit_object, change, false PROGRAM_ARGS,
         isNone ? NULL : item->title);
   history_change_add(pd->group->history, change);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
   prop_program_filter_state_update(pd);
}

PROGRAM_2COMBOBOX_ADD(_("filter"), program, filter_part, filter_state, program,
                      _("Filter signals to be only accepted if the part is "
                        "in certain state (part value)"),
                      _("Filter signals to be only accepted if the part is "
                        "in certain state (state value)"),
                      _("part:"), _("state:"))

#define PROGRAM_ATTR_2SPINNER(TEXT, SUB, VALUE1, VALUE2, MEMBER, MIN, MAX, STEP, FMT, \
                              L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER, \
                              TYPE, DESC1, DESC2) \
   PROGRAM_SPINNER_CALLBACK(SUB, VALUE1, MEMBER, TYPE, MULTIPLIER, DESC1) \
   PROGRAM_SPINNER_CALLBACK(SUB, VALUE2, MEMBER, TYPE, MULTIPLIER, DESC2) \
   COMMON_2SPINNER_ADD(PROGRAM, TEXT, "2swallow", SUB, VALUE1, VALUE2, MEMBER, TYPE, MIN, MAX, STEP, FMT, \
                           L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER)

PROGRAM_ATTR_2SPINNER(_("in"), program, in_from, in_range, program, 0.0, 9999.0, 0.1, "%.1f", "from:", "sec", "range:", "sec",
                    _("Constant time to wait till program would start"),
                    _("Random number of seconds (from 0 to 'range') added to constant time"),
                    1, double,
                    _("in from changed from %.2f to %.2f"),
                    _("in range changed from %.2f to %.2f"))

static void
_ui_property_program_set(Evas_Object *property, const char *program)
{
   Evas_Object *prop_box, *box, *item;
   GROUP_PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->scroller);
   pd->attributes.program.program = eina_stringshare_add(program);
   if (!pd->attributes.program.frame)
     {
        FRAME_PROPERTY_ADD(property, pd->attributes.program.frame, true, _("Program property"), pd->scroller)
        BOX_ADD(pd->attributes.program.frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd->attributes.program.frame, box);

        item = prop_program_name_add(box, pd, NULL);
        elm_object_disabled_set(pd->attributes.program.name, true);
        elm_box_pack_end(box, item);
        item = prop_program_signal_add(box, pd, NULL);
        elm_box_pack_end(box, item);
        item = prop_program_source_add(box, pd, NULL);
        elm_box_pack_end(box, item);
        item = prop_program_in_from_in_range_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_program_action_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_program_filter_part_filter_state_add(box, pd);
        elm_box_pack_end(box, item);
        FRAME_PROPERTY_ADD(box, pd->attributes.program.action_params, true, _("Action params"), pd->scroller)
        elm_object_style_set(pd->attributes.program.action_params, "outdent_top");
        elm_box_pack_end(box,pd->attributes.program.action_params);
        /* as frame needed for create the action params controls, update the
         * action ites */
        prop_program_action_update(pd);
        item = prop_program_transition_add(box, pd);
        elm_box_pack_end(box, item);
        FRAME_PROPERTY_ADD(box, pd->attributes.program.transition_params, true, _("Transition params"), pd->scroller)
        elm_object_style_set(pd->attributes.program.transition_params, "outdent_top");
        elm_box_pack_end(box,pd->attributes.program.transition_params);
        /* as frame needed for create the transition params controls, update the
         * transition ites */
        prop_program_transition_update(pd);

        /* targets */
        FRAME_PROPERTY_ADD(box, pd->attributes.program.targets_frame, true, _("Targets"), pd->scroller)
        elm_object_style_set(pd->attributes.program.targets_frame, "outdent_top");
        elm_box_pack_end(box, pd->attributes.program.targets_frame);

        BOX_ADD(pd->attributes.program.targets_frame, pd->attributes.program.target_box, false, false)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd->attributes.program.targets_frame, pd->attributes.program.target_box);
        item = prop_program_target_add(pd->attributes.program.target_box, pd);
        elm_box_pack_end(pd->attributes.program.target_box, item);
        prop_program_targets_update(pd);

        /* afters */
        FRAME_PROPERTY_ADD(box, pd->attributes.program.afters_frame, true, _("Afters"), pd->scroller)
        elm_object_style_set(pd->attributes.program.afters_frame, "outdent_top");
        elm_box_pack_end(box, pd->attributes.program.afters_frame);

        BOX_ADD(pd->attributes.program.targets_frame, pd->attributes.program.after_box, false, false)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd->attributes.program.afters_frame, pd->attributes.program.after_box);
        item = prop_program_after_add(pd->attributes.program.after_box, pd);
        elm_box_pack_end(pd->attributes.program.after_box, item);
        prop_program_afters_update(pd);
     }
   else
     {
        prop_program_name_update(pd);
        prop_program_signal_update(pd);
        prop_program_source_update(pd);
        prop_program_action_update(pd);
        prop_program_transition_update(pd);
        prop_program_filter_part_update(pd);
        prop_program_filter_state_update(pd);
        prop_program_targets_update(pd);
        prop_program_afters_update(pd);
     }
   elm_box_pack_end(prop_box, pd->attributes.program.frame);
}

static void
_ui_property_program_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   GROUP_PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->scroller);
   PROP_ITEM_UNSET(prop_box, pd->attributes.program.frame);
   eina_stringshare_del(pd->attributes.program.program);
   pd->attributes.program.program = NULL;
}

#define PART_ATTR_1CHECK(TEXT, SUB, VALUE, MEMBER, TOOLTIP, DESCRIPTION) \
   PART_ATTR_1CHECK_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   PART_ATTR_1CHECK_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

#define PART_ATTR_1COMBOBOX(TEXT, SUB, VALUE, MEMBER, TOOLTIP, DESCRIPTION) \
   PART_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER, PART_ARGS, DESCRIPTION) \
   PART_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

#define PART_ATTR_1COMBOBOX_LIST(TEXT, SUB, VALUE, MEMBER, TYPE, LIST, TOOLTIP, DESCRIPTION) \
   PART_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, DESCRIPTION) \
   PART_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP)

#define PART_ATTR_DRAG(TEXT, SUB, VALUE1, VALUE2, DESCRIPTION1, DESCRIPTION2) \
   PART_ATTR_DRAG_CALLBACK(SUB, VALUE1, VALUE2, DESCRIPTION1, DESCRIPTION2) \
   PART_ATTR_DRAG_UPDATE(SUB, VALUE1, VALUE2) \
   PART_ATTR_DRAG_ADD(TEXT, SUB, VALUE1, VALUE2)

/* part property */
PART_ATTR_1CHECK(_("scalable"), part, scale, part,
                 _("Specifies whether the part will scale it's size with an edje scaling factor"),
                 _("scalable changed to %s"))
PART_ATTR_1CHECK(_("mouse events"), part, mouse_events, part,
                 _("Enable mouse events in this part"),
                 _("mouse events changed to %s"))
PART_ATTR_1CHECK(_("event propagation"), part, repeat_events, part,
                 _("Enable repeat mouse events to the parts below"),
                 _("event propagation changed to %s"))
PART_ATTR_1COMBOBOX(_("clipper"), part, clip_to, part,
                    _("Show only the area of part that coincides with another part's container"),
                    _("clipper changed to %s"))
PART_ATTR_1COMBOBOX_LIST(_("ignore flags"), part, ignore_flags, part, Evas_Event_Flags, edje_ignore_flags,
                         _("Specifies whether events with the given flags should be ignored"),
                         _("ignore_flags changed to %s"))
PART_ATTR_1COMBOBOX(_("group source"), part, source, part,
                    _("Used for the group to be loaded and used to display GROUP part."),
                    _("group source changed to %s"))
/* part drag property */
PART_ATTR_DRAG(_("axis X"), part_drag, x, step_x, _("drag x changed to %s"), _("drag step x changed from %d to %d"))
PART_ATTR_DRAG(_("axis Y"), part_drag, y, step_y, _("drag y changed to %s"), _("drag step y changed from %d to %d"))
PART_ATTR_1COMBOBOX(_("drag area"), part_drag, confine, part_drag,
                    _("Limits the movement of the dragged part to another part's container"),
                    _("drag area changed to %s"))
PART_ATTR_1COMBOBOX(_("threshold"), part_drag, threshold, part_drag,
                    _("Movement of the dragged part can only start when it get moved enough to be outside of this threshold part"),
                    _("threshold changed to %s"))
PART_ATTR_1COMBOBOX(_("forward events"), part_drag, event, part_drag,
                    _("It causes the part to forward the drag events "
                    "to another part, thus ignoring them for itself."),
                    _("forward events changed to %s"))

#define pd_part pd->attributes.part
#define pd_drag pd->attributes.part_drag
void
ui_property_part_set(Evas_Object *property, Part *part)
{
   Evas_Object *item;
   Evas_Object *box, *prop_box;
   int y_reg, h_reg, h_box;

   GROUP_PROP_DATA_GET()
   assert(part != NULL);

   elm_scroller_policy_set(pd->scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
   pd->part = part;
   prop_box = elm_object_content_get(pd->scroller);

   _ui_property_program_unset(property);
   if (!pd_part.frame)
     {
        FRAME_PROPERTY_ADD(property, pd_part.frame, true, _("Part property"), pd->scroller)
        BOX_ADD(pd_part.frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd_part.frame, box);

        if (pd->attributes.part.validator == NULL)
          pd->attributes.part.validator = resource_name_validator_new(PART_NAME_REGEX, NULL);

        item = prop_part_name_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_type_add(box, _("type"), gm_part_type_text_get(pd->part->type));
        elm_box_pack_end(box, item);
        pd->attributes.part.scale_item = prop_part_scale_add(box, pd);
        elm_box_pack_end(box, pd->attributes.part.scale_item);
        pd->attributes.part.mouse_events_item = prop_part_mouse_events_add(box, pd);
        elm_box_pack_end(box, pd->attributes.part.mouse_events_item);
        pd->attributes.part.repeat_events_item = prop_part_repeat_events_add(box, pd);
        elm_box_pack_end(box, pd->attributes.part.repeat_events_item);
        pd->attributes.part.clip_to_item = prop_part_clip_to_add(box, pd);
        elm_box_pack_end(box, pd->attributes.part.clip_to_item);
        pd->attributes.part.ignore_flags_item = prop_part_ignore_flags_add(box, pd);
        elm_box_pack_end(box, pd->attributes.part.ignore_flags_item);
        pd->attributes.part.source_item = prop_part_source_add(box, pd);
        elm_box_pack_end(box, pd->attributes.part.source_item);

        FRAME_PROPERTY_ADD(pd->scroller, pd_drag.frame, true, _("Part dragable property"), pd->scroller)
        elm_object_style_set(pd_drag.frame, "outdent_top");
        elm_box_pack_end(box, pd_drag.frame);
        BOX_ADD(pd_drag.frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd_drag.frame, box);

        item = prop_part_drag_x_step_x_add(box, pd,
                                 _("Enable/Disable draggin along X axis"),
                                 _("Set a drag step value"));
        elm_box_pack_end(box, item);
        item = prop_part_drag_y_step_y_add(box, pd,
                                 _("Enable/Disable draggin along Y axis"),
                                 _("Set a drag step value"));
        elm_box_pack_end(box, item);
        item = prop_part_drag_confine_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_drag_threshold_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_drag_event_add(box, pd);
        elm_box_pack_end(box, item);

        elm_box_pack_after(prop_box, pd_part.frame, pd->attributes.group.frame);
     }
   else
     {
        prop_part_name_update(pd);
        prop_part_type_update(gm_part_type_text_get(pd->part->type));
        PART_ATTR_1CHECK_UPDATE(part, scale, part)
        PART_ATTR_1CHECK_UPDATE(part, mouse_events, part)
        PART_ATTR_1CHECK_UPDATE(part, repeat_events, part)
        prop_part_clip_to_update(pd);
        PART_ATTR_1COMBOBOX_LIST_UPDATE(part, ignore_flags, part)
        prop_part_source_update(pd);

        prop_part_drag_x_step_x_update(pd);
        prop_part_drag_y_step_y_update(pd);
        prop_part_drag_confine_update(pd);
        prop_part_drag_threshold_update(pd);
        prop_part_drag_event_update(pd);
     }
   resource_name_validator_list_set(pd->attributes.part.validator, &part->group->parts, false);
   resource_name_validator_resource_set(pd->attributes.part.validator, (Resource *)part);

   box = elm_object_content_get(pd_part.frame);
   if (pd->part->type == EDJE_PART_TYPE_SPACER)
     {
        /* Unpack from box, if selected part is SPACER and hide.
         * Check the visibility of 'mouse_events_item', to identify
         * previous selected part is SPACER or not. For identification
         * enough to check only one attribute, which not available
         * for SPACER */
        if (evas_object_visible_get(pd->attributes.part.mouse_events_item))
          {
             elm_box_unpack(box, pd->attributes.part.mouse_events_item);
             elm_box_unpack(box, pd->attributes.part.repeat_events_item);
             elm_box_unpack(box, pd->attributes.part.clip_to_item);
             elm_box_unpack(box, pd->attributes.part.ignore_flags_item);
             evas_object_hide(pd->attributes.part.mouse_events_item);
             evas_object_hide(pd->attributes.part.repeat_events_item);
             evas_object_hide(pd->attributes.part.clip_to_item);
             evas_object_hide(pd->attributes.part.ignore_flags_item);
          }
     }
   else
     {
        if (!evas_object_visible_get(pd->attributes.part.mouse_events_item))
          {
             elm_box_pack_after(box, pd->attributes.part.mouse_events_item, pd->attributes.part.scale_item);
             elm_box_pack_after(box, pd->attributes.part.repeat_events_item, pd->attributes.part.mouse_events_item);
             elm_box_pack_after(box, pd->attributes.part.clip_to_item, pd->attributes.part.repeat_events_item);
             elm_box_pack_after(box, pd->attributes.part.ignore_flags_item, pd->attributes.part.clip_to_item);
             evas_object_show(pd->attributes.part.mouse_events_item);
             evas_object_show(pd->attributes.part.repeat_events_item);
             evas_object_show(pd->attributes.part.clip_to_item);
             evas_object_show(pd->attributes.part.ignore_flags_item);
             evas_object_show(pd->attributes.part_drag.frame);
          }
     }
     if (pd->part->type == EDJE_PART_TYPE_GROUP)
       {
         /* pack to box, only in case when the previous selected part
          * is not GROUP. If previos selected part is GROUP then
          * this item is show and already packed to box  */
         if (!evas_object_visible_get(pd_part.source_item))
           {
              elm_box_pack_after(box, pd_part.source_item, pd->attributes.part.ignore_flags_item);
              evas_object_show(pd->attributes.part.source_item);
           }
       }
     else
       {
          elm_box_unpack(box, pd_part.source_item);
          evas_object_hide(pd_part.source_item);
       }

   prop_part_drag_control_disable_set(pd, true);
   evas_object_geometry_get(prop_box, NULL, NULL, NULL, &h_box);
   elm_scroller_region_get(pd->scroller, NULL, &y_reg, NULL, &h_reg);
   elm_scroller_region_bring_in(pd->scroller, 0.0, y_reg + 1, 0.0, h_reg);
   if (h_box == h_reg + y_reg)
     elm_scroller_region_show(pd->scroller, 0.0, y_reg + h_box, 0.0, h_reg);
}

static void
_ui_property_part_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   GROUP_PROP_DATA_GET()

   elm_scroller_policy_set(pd->scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   prop_box = elm_object_content_get(pd->scroller);

   /*
   if (pd->attributes.part.validator != NULL)
     {
        elm_validator_regexp_free(pd->attributes.part.validator);
        pd->attributes.part.validator = NULL;
     }
   */
   PROP_ITEM_UNSET(prop_box, pd->attributes.part.frame)
   PROP_ITEM_UNSET(prop_box, pd->attributes.state.frame)
   evas_object_del(pd->attributes.state.color1);
   evas_object_del(pd->attributes.state.color2);
   evas_object_del(pd->attributes.state.color3);
   PROP_ITEM_UNSET(prop_box, pd->attributes.state_object_area.frame)
   PROP_ITEM_UNSET(prop_box, pd->attributes.state_text.frame)
   PROP_ITEM_UNSET(prop_box, pd->attributes.state_image.frame)
   PROP_ITEM_UNSET(prop_box, pd->attributes.state_textblock.frame)
   PROP_ITEM_UNSET(prop_box, pd->attributes.state_fill.frame)
   PROP_ITEM_UNSET(prop_box, pd->attributes.state_container.frame)
   PROP_ITEM_UNSET(prop_box, pd->attributes.part_item.frame)
}

#undef pd_part
#undef pd_drag

/*****************************************************************************/
/*                          COLOR CLASS COMBOBOX                             */
/*****************************************************************************/
static void
_on_state_color_class_change(void *data,
                             Evas_Object *obj EINA_UNUSED,
                             void *event_info)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   int r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3;
   Eina_Stringshare *value = NULL, *old_value;
   Ewe_Combobox_Item *item = event_info;

   assert(pd != NULL);

   value = strcmp(item->title, "None") ? item->title : NULL;

   old_value = edje_edit_state_color_class_get(pd->group->edit_object,
                                               pd->part->name,
                                               pd->part->current_state->parsed_name,
                                               pd->part->current_state->parsed_val);
   if (value == old_value) /* stringshares */
     {
        eina_stringshare_del(old_value);
        return;
     }
   eina_stringshare_del(old_value);
   Eina_Stringshare *msg = eina_stringshare_printf(_("color class changed to %s"), item->title);
   Change *change = change_add(msg);
   eina_stringshare_del(msg);
   editor_state_color_class_set(pd->group->edit_object, change, false,
                                   pd->part->name,
                                   pd->part->current_state->parsed_name,
                                   pd->part->current_state->parsed_val,
                                   value);
   if (value && edje_edit_color_class_colors_get(pd->group->edit_object, value,
                                                 &r, &g, &b, &a,
                                                 &r2, &g2, &b2, &a2,
                                                 &r3, &g3, &b3, &a3))
     {
        evas_object_color_set(pd->attributes.state.color1, r * a / 255, g * a / 255, b * a / 255, a);
        evas_object_color_set(pd->attributes.state.color2, r2 * a2 / 255, g2 * a2 / 255, b2 * a2 / 255, a2);
        evas_object_color_set(pd->attributes.state.color3, r3 * a3 / 255, g3 * a3 / 255, b3 * a3 / 255, a3);
     }
   else
     {
        evas_object_color_set(pd->attributes.state.color1, 0, 0, 0, 0);
        evas_object_color_set(pd->attributes.state.color2, 0, 0, 0, 0);
        evas_object_color_set(pd->attributes.state.color3, 0, 0, 0, 0);
     }

   edje_edit_string_free(value);

   //project_changed(false);
   history_change_add(pd->group->history, change);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
}

static void
_color_class_items_fill(void *data,
                        Evas_Object *obj,
                        void *event_info __UNUSED__)
{
   int r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3;
   Eina_List *items, *l;
   Ewe_Combobox_Item *item;
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Evas_Object *color;
   Evas *canvas;

   assert(pd != NULL);

   items = (Eina_List *)ewe_combobox_items_list_get(obj);

   if (!items) return;

   //get combobbox item from first list item
   item = eina_list_data_get(items);
   canvas = evas_object_evas_get(item->content);
   EINA_LIST_FOREACH(items, l, item)
     {
        // this case available if user select 'None'
        if (!edje_edit_color_class_colors_get(pd->group->edit_object, item->title,
                                              &r, &g, &b, &a,
                                              &r2, &g2, &b2, &a2,
                                              &r3, &g3, &b3, &a3))
          continue;

       /* FIXME: this is bad solition, user shoud not use edje object for add contnent to a
        * combobox item. Need to move combobox from edje ocject to layout. */
       color = edje_object_add(canvas);
       edje_object_file_set(color, EFLETE_THEME, "elm/image/color/color_set");
       evas_object_color_set(color, r * a / 255, g * a / 255, b * a / 255, a);
       edje_object_part_swallow(item->content, "swallow.color1", color);
       color = edje_object_add(canvas);
       edje_object_file_set(color, EFLETE_THEME, "elm/image/color/color_set");
       evas_object_color_set(color, r2 * a2 / 255, g2 * a2 /  255, b2 * a2 / 255, a2);
       edje_object_part_swallow(item->content, "swallow.color2", color);
       color = edje_object_add(canvas);
       edje_object_file_set(color, EFLETE_THEME, "elm/image/color/color_set");
       evas_object_color_set(color, r3 * a3 / 255, g3 * a3 / 255, b3 * a3 / 255, a3);
       edje_object_part_swallow(item->content, "swallow.color3", color);
     }
}

static void
prop_state_color_class_update(Group_Prop_Data *pd)
{
   Ewe_Combobox_Item *it;
   Eina_List *cclist, *l;
   const char *ccname;
   int r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3;
   Eina_Stringshare *color_c;

   assert(pd != NULL);

   ewe_combobox_items_list_free(pd->attributes.state.color_class, true);
   ewe_combobox_item_add(pd->attributes.state.color_class, _("None"));
   cclist = edje_edit_color_classes_list_get(pd->group->edit_object);
   EINA_LIST_FOREACH(cclist, l, ccname)
     {
        it = ewe_combobox_item_add(pd->attributes.state.color_class, ccname);
        ewe_combobox_item_style_set(pd->attributes.state.color_class, it, "color_class");
     }
   color_c = edje_edit_state_color_class_get(pd->group->edit_object, pd->part->name,
                                             pd->part->current_state->parsed_name,
                                             pd->part->current_state->parsed_val);
   ewe_combobox_text_set(pd->attributes.state.color_class, color_c ? color_c : _("None"));
   if (color_c && edje_edit_color_class_colors_get(pd->group->edit_object, color_c,
                                                   &r, &g, &b, &a,
                                                   &r2, &g2, &b2, &a2,
                                                   &r3, &g3, &b3, &a3))
     {
        evas_object_color_set(pd->attributes.state.color1, r * a / 255, g * a / 255, b * a / 255, a);
        evas_object_color_set(pd->attributes.state.color2, r2 * a2 / 255, g2 * a2 / 255, b2 * a2 / 255, a2);
        evas_object_color_set(pd->attributes.state.color3, r3 * a3 / 255, g3 * a3 / 255, b3 * a3 / 255, a3);
     }
   else
     {
        evas_object_color_set(pd->attributes.state.color1, 0, 0, 0, 0);
        evas_object_color_set(pd->attributes.state.color2, 0, 0, 0, 0);
        evas_object_color_set(pd->attributes.state.color3, 0, 0, 0, 0);
     }

   edje_edit_string_list_free(cclist);
   edje_edit_string_free(color_c);
}

static Evas_Object *
prop_state_color_class_add(Evas_Object *parent, Group_Prop_Data *pd)
{
   assert(parent != NULL);
   assert(pd != NULL);

   PROPERTY_ITEM_ADD(parent, _("color class"), "1swallow")
   EWE_COMBOBOX_ADD(parent, pd->attributes.state.color_class)
   elm_object_tooltip_text_set(pd->attributes.state.color_class, "Set the color class");
   ewe_combobox_style_set(pd->attributes.state.color_class, "color_class");
   evas_object_smart_callback_add(pd->attributes.state.color_class, "selected", _on_state_color_class_change, pd);
   evas_object_smart_callback_add(pd->attributes.state.color_class, "expanded", _color_class_items_fill, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->attributes.state.color_class);
   IMAGE_ADD_NEW(parent, pd->attributes.state.color1, "color", "color_set")
   ewe_combobox_content_set(pd->attributes.state.color_class, "swallow.color1", pd->attributes.state.color1);
   IMAGE_ADD_NEW(parent, pd->attributes.state.color2, "color", "color_set")
   ewe_combobox_content_set(pd->attributes.state.color_class, "swallow.color2", pd->attributes.state.color2);
   IMAGE_ADD_NEW(parent, pd->attributes.state.color3, "color", "color_set")
   ewe_combobox_content_set(pd->attributes.state.color_class, "swallow.color3", pd->attributes.state.color3);

   prop_state_color_class_update(pd);
   return item;
}

#define STATE_ATTR_1CHECK(TEXT, SUB, VALUE, MEMBER, TOOLTIP, DESCRIPTION) \
   STATE_ATTR_1CHECK_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   STATE_ATTR_1CHECK_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

#define STATE_ATTR_2SPINNER(TEXT, SUB, VALUE1, VALUE2, MEMBER, MIN, MAX, STEP, FMT, \
                            L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER, \
                            TYPE, DESC1, DESC2) \
   STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE1, MEMBER, TYPE, MULTIPLIER, DESC1) \
   STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE2, MEMBER, TYPE, MULTIPLIER, DESC2) \
   STATE_ATTR_2SPINNER_ADD(TEXT, "2swallow", SUB, VALUE1, VALUE2, MEMBER, TYPE, MIN, MAX, STEP, FMT, \
                           L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER)

#define STATE_ATTR_2CHECK(TEXT, SUB, VALUE1, VALUE2, MEMBER, \
                          L1_START, L1_END, L2_START, L2_END, \
                          TOOLTIP1, TOOLTIP2, DESCRIPTION1, DESCRIPTION2) \
   STATE_ATTR_2CHECK_CALLBACK(SUB, VALUE1, VALUE2, MEMBER, DESCRIPTION1, DESCRIPTION2) \
   STATE_ATTR_2CHECK_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, \
                        L1_START, L1_END, L2_START, L2_END, \
                        TOOLTIP1, TOOLTIP2)

#define STATE_ATTR_1COMBOBOX_LIST(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP, TYPE, DESCRIPTION) \
   STATE_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, DESCRIPTION) \
   STATE_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP)

#define STATE_ATTR_COLOR(TEXT, SUB, VALUE, MEMBER, TOOLTIP, DESCRIPTION) \
   STATE_ATTR_COLOR_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   STATE_ATTR_COLOR_LIST_UPDATE(SUB, VALUE, MEMBER) \
   STATE_ATTR_COLOR_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

#define STATE_ATTR_COMBOBOX(TEXT, SUB, VALUE, MEMBER, TOOLTIP, DESCRIPTION) \
   STATE_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   STATE_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

#define STATE_MINMAX_ATTR_2SPINNER(TEXT, SUB, VALUE1, VALUE2, MEMBER, MIN, MAX, STEP, FMT, \
                            L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER, \
                            TYPE, DESC1, DESC2) \
   STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE1, MEMBER, TYPE, MULTIPLIER, DESC1) \
   STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE2, MEMBER, TYPE, MULTIPLIER, DESC2) \
   STATE_ATTR_2SPINNER_ADD(TEXT, "2swallow", SUB, VALUE1, VALUE2, MEMBER, TYPE, MIN, MAX, STEP, FMT, \
                           L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER)

#define STATE_STRSHARE_ATR_1COMBOBOX_LIST(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP) \
   STATE_STRSHARE_ATR_1COMBOBOX_LIST_CALLBACK(SUB, VALUE, MEMBER) \
   STATE_STRSHARE_ATR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP)

STATE_ATTR_1CHECK(_("visible"), state, visible, state,
                  _("Set visibility for part by current state"),
                  _("visible changed to %s"))
STATE_MINMAX_ATTR_2SPINNER(_("min"), state, min_w, min_h, state, 0.0, 9999.0, 1.0, "%.0f", "w:", "px", "h:", "px",
                    _("Minimal size of part width in pixels."), _("Minimal part height in pixels."),
                    1, int,
                    _("Parts min width changed from %d to %d"),
                    _("Parts min height changed from %d to %d"))
STATE_MINMAX_ATTR_2SPINNER(_("max"), state, max_w, max_h, state, -1.0, 9999.0, 1.0, "%.0f", "w:", "px", "h:", "px",
                    _("Maximal size of part width in pixels."), _("Maximal part height in pixels."),
                    1, int,
                    _("Parts max width changed from %d to %d"),
                    _("Parts max height changed from %d to %d"))
STATE_ATTR_2SPINNER(_("align"), state, align_x, align_y, state, 0, 100, 1, NULL, "x:", "%", "y:", "%",
                    _("Part align horizontally"), _("Part align vertically"),
                    100, double,
                    _("align x changed from %.2f to %.2f"),
                    _("align y changed from %.2f to %.2f"))
STATE_ATTR_2CHECK(_("fixed"), state, fixed_w, fixed_h, state, "w:", "", "h:", "",
                  _("This affects the minimum width calculation."),
                  _("This affects the minimum height calculation."),
                  _("fixed w changed to %s"),
                  _("fixed h changed to %s"))
STATE_ATTR_1COMBOBOX_LIST(_("aspect ratio mode"), state, aspect_pref, state, edje_aspect_pref,
                          _("The aspect control hints for this object."), unsigned char,
                          _("aspect ratio mode changed to %s"))
STATE_ATTR_2SPINNER(_("aspect ratio"), state, aspect_min, aspect_max, state, 0, 100, 1, NULL, "min:", "", "max:", "",
                   _("Normally width and height can be resized to any values independently"),
                   _("Normally width and height can be resized to any values independently"),
                   100, double,
                    _("aspect min changed from %.2f to %.2f"),
                    _("aspect max changed from %.2f to %.2f"))
STATE_ATTR_2SPINNER(_("multiplier"), state, minmul_w, minmul_h, state, 0.0, 9999.0, 0.1, "%.1f", "w:", "", "h:", "",
                    _("The minimal part width value multiplier for current state"),
                    _("The minimal part height value multiplier for current state"),
                    1, double,
                    _("multiplier w changed from %.2f to %.2f"),
                    _("multiplier h changed from %.2f to %.2f"))
STATE_ATTR_COLOR(_("color"), state, color, state, _("Part main color"),
                 _("color changed to [%d %d %d %d]"))

STATE_ATTR_SOURCE_UPDATE(state, proxy_source, state, EDJE_PART_TYPE_SPACER, !=)
STATE_ATTR_COMBOBOX(_("proxy source"), state, proxy_source, state,
                    _("Causes the part to use another part content as"
                    "the content of this part. Only work with PROXY part."),
                    _("proxy source changed to \"%s\""))
STATE_STRSHARE_ATR_1COMBOBOX_LIST(_("box layout"), state, box_layout, state, edje_box_layouts,
                          _("The aspect control hints for this object"))
STATE_ATTR_1COMBOBOX_LIST(_("table homogeneous"), state, table_homogeneous, state, edje_homogeneous,
                          _("The table homogeneous mode"), unsigned char,
                          _("table homogeneous mode changed to %s"))

void
ui_property_part_state_set(Evas_Object *property, Part *part)
{
   Evas_Object *item;
   Evas_Object *state_frame, *box, *prop_box;

   GROUP_PROP_DATA_GET()
   assert(part != NULL);

   #define pd_state pd->attributes.state

   prop_box = elm_object_content_get(pd->scroller);
   elm_box_unpack(prop_box, pd_state.frame);

   if (!pd_state.frame)
     {
        FRAME_PROPERTY_ADD(property, state_frame, true, _("State"), pd->scroller)
        BOX_ADD(state_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(state_frame, box);

        item = prop_state_state_add(box, _("state"), part->current_state->name);
        elm_box_pack_end(box, item);
        item = prop_state_visible_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_min_w_min_h_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_max_w_max_h_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_fixed_w_fixed_h_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_align_x_align_y_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_aspect_pref_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_aspect_min_aspect_max_add(box, pd);
        elm_box_pack_end(box, item);
        pd_state.color_class_item = prop_state_color_class_add(box, pd);
        elm_box_pack_end(box, pd_state.color_class_item);
        pd_state.color_item = prop_state_color_add(box, pd);
        elm_box_pack_after(box, pd_state.color_item, pd_state.color_class_item);
        item = prop_state_minmul_w_minmul_h_add(box, pd);
        elm_box_pack_end(box, item);
        pd_state.proxy_source_item = prop_state_proxy_source_add(box, pd);
        evas_object_hide(pd_state.proxy_source_item);
        pd_state.box_layout_item = prop_state_box_layout_add(box, pd);
        evas_object_hide(pd_state.box_layout_item);
        pd_state.table_homogeneous_item = prop_state_table_homogeneous_add(box, pd);
        evas_object_hide(pd_state.table_homogeneous_item);

        prop_box = elm_object_content_get(pd->scroller);
        elm_box_pack_after(prop_box, state_frame, pd->attributes.part.frame);
        pd_state.frame = state_frame;
     }
   else
     {
        box = elm_object_content_get(pd_state.frame);
        prop_state_state_update(part->current_state->name);
        STATE_ATTR_1CHECK_UPDATE(state, visible, state)
        STATE_ATTR_2SPINNER_UPDATE(state, min_w, min_h, state, int, 1)
        STATE_ATTR_2SPINNER_UPDATE(state, max_w, max_h, state, int, 1)
        STATE_ATTR_2CHECK_UPDATE(state, fixed_w, fixed_h, state)
        STATE_ATTR_2SPINNER_UPDATE(state, align_x, align_y, state, double, 100)
        STATE_ATTR_1COMBOBOX_LIST_UPDATE(state, aspect_pref, state)
        STATE_ATTR_2SPINNER_UPDATE(state, aspect_min, aspect_max, state, double, 100)
        prop_state_color_class_update(pd);
        prop_state_color_update(pd);
        STATE_ATTR_2SPINNER_UPDATE(state, minmul_w, minmul_h, state, double, 1)
        prop_state_proxy_source_update(pd);
        STATE_STRSHARE_ATTR_1COMBOBOX_LIST_UPDATE(state, box_layout, state)
        STATE_ATTR_1COMBOBOX_LIST_UPDATE(state, table_homogeneous, state)

        prop_box = elm_object_content_get(pd->scroller);
        elm_box_pack_end(prop_box, pd_state.frame);
        evas_object_show(pd_state.frame);
     }

   ui_property_state_obj_area_set(property);
   if (part->type == EDJE_PART_TYPE_TEXT) ui_property_state_text_set(property);
   else ui_property_state_text_unset(property);
   if (part->type == EDJE_PART_TYPE_TEXTBLOCK) ui_property_state_textblock_set(property);
   else ui_property_state_textblock_unset(property);
   if (part->type == EDJE_PART_TYPE_IMAGE) ui_property_state_image_set(property);
   else ui_property_state_image_unset(property);
   if ((part->type == EDJE_PART_TYPE_IMAGE) || (part->type == EDJE_PART_TYPE_PROXY))
     ui_property_state_fill_set(property);
   else if ((part->type != EDJE_PART_TYPE_IMAGE) && (part->type != EDJE_PART_TYPE_PROXY))
     ui_property_state_fill_unset(property);
   if (part->type == EDJE_PART_TYPE_TABLE)
     {
        ui_property_state_container_set(property);
        if (!evas_object_visible_get(pd_state.table_homogeneous_item))
          {
              elm_box_pack_end(box, pd_state.table_homogeneous_item);
              evas_object_show(pd_state.table_homogeneous_item);
          }
     }
   else
     {
        if (evas_object_visible_get(pd_state.table_homogeneous_item))
          {
              elm_box_unpack(box, pd_state.table_homogeneous_item);
              evas_object_hide(pd_state.table_homogeneous_item);
          }

     }
   if (part->type == EDJE_PART_TYPE_BOX)
     {
        ui_property_state_container_set(property);
        if (!evas_object_visible_get(pd_state.box_layout_item))
          {
              elm_box_pack_end(box, pd_state.box_layout_item);
              evas_object_show(pd_state.box_layout_item);
          }
     }
   else
     {
        if (evas_object_visible_get(pd_state.box_layout_item))
          {
              elm_box_unpack(box, pd_state.box_layout_item);
              evas_object_hide(pd_state.box_layout_item);
          }

     }
   if ((part->type != EDJE_PART_TYPE_TABLE) && (part->type != EDJE_PART_TYPE_BOX))
     ui_property_state_container_unset(property);

   /* hide/show the color class attribute control for GROUP */
   if ((part->type == EDJE_PART_TYPE_GROUP) ||
       (part->type == EDJE_PART_TYPE_SWALLOW) ||
       (part->type == EDJE_PART_TYPE_SPACER) ||
       (part->type == EDJE_PART_TYPE_TEXTBLOCK))
     {
        if (evas_object_visible_get(pd_state.color_class_item))
          {
             elm_box_unpack(box, pd_state.color_class_item);
             evas_object_hide(pd_state.color_class_item);
          }
     }
   else
     {
        if (!evas_object_visible_get(pd_state.color_class_item))
          {
             elm_box_pack_end(box, pd_state.color_class_item);
             evas_object_show(pd_state.color_class_item);
          }
     }
   /* hide/show the color attribute control */
   if (part->type == EDJE_PART_TYPE_TEXTBLOCK ||
       part->type == EDJE_PART_TYPE_SWALLOW ||
       part->type == EDJE_PART_TYPE_GROUP ||
       part->type == EDJE_PART_TYPE_SPACER)
     {
        if (evas_object_visible_get(pd_state.color_item))
          {
             elm_box_unpack(box, pd_state.color_item);
             evas_object_hide(pd_state.color_item);
          }
     }
   else
     {
        if (!evas_object_visible_get(pd_state.color_item))
          {
             elm_box_pack_after(box, pd_state.color_item, pd_state.color_class_item);
             evas_object_show(pd_state.color_item);
          }
     }

   if (part->type == EDJE_PART_TYPE_PROXY)
     {
        if (!evas_object_visible_get(pd_state.proxy_source_item))
          {
             elm_box_pack_end(box, pd_state.proxy_source_item);
             evas_object_show(pd_state.proxy_source_item);
          }
     }
   else
     {
        if (evas_object_visible_get(pd_state.proxy_source_item))
          {
             elm_box_unpack(box, pd_state.proxy_source_item);
             evas_object_hide(pd_state.proxy_source_item);
          }
     }

   elm_scroller_policy_set(pd->scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_ON);
   #undef pd_state
}

static void
_ui_property_part_state_unset(Evas_Object *property)
{
   GROUP_PROP_DATA_GET()

   evas_object_hide(pd->attributes.state.frame);
   /* hide the evas rectangles, it need to do, because those objects not a smart */
   evas_object_hide(pd->attributes.state.color1);
   evas_object_hide(pd->attributes.state.color2);
   evas_object_hide(pd->attributes.state.color3);

   ui_property_state_obj_area_unset(property);
   ui_property_state_text_unset(property);
   ui_property_state_image_unset(property);
   ui_property_state_textblock_unset(property);
   ui_property_state_fill_unset(property);
}

/* FIXME: edje_edit_state_relX_to do not update object properly.
   Setting of any other param completes the object update.
   Here min_w param is setted to its current value. */
#define ITEM_REL_TO_COMBOBOX_STATE_CALLBACK(SUB, VALUE) \
static void \
_on_combobox_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj EINA_UNUSED, \
                             void *ei) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   \
   assert(pd != NULL); \
   \
   Ewe_Combobox_Item *item = ei; \
   const char *value = item->title; \
   if (strcmp(item->title, _("Layout"))) \
     edje_edit_##SUB##_##VALUE##_set(pd->group->edit_object, pd->part->name, \
                                     pd->part->current_state->parsed_name, pd->part->current_state->parsed_val, \
                                     item->title); \
   else \
     { \
        edje_edit_##SUB##_##VALUE##_set(pd->group->edit_object, pd->part->name, \
                                        pd->part->current_state->parsed_name, pd->part->current_state->parsed_val, \
                                        NULL); \
        value = NULL; \
     } \
/*   int temp = edje_edit_state_min_w_get(pd->group->edit_object, pd->part->name, \
                                       pd->part->current_state->parsed_name, pd->part->current_state->parsed_val); \
   edje_edit_state_min_w_set(pd->group->edit_object, pd->part->name, \
                             pd->part->current_state->parsed_name, \
                             pd->part->current_state->parsed_val, temp); */\
   /*project_changed(false);*/ \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
}

#define STATE_ATTR_2COMBOBOX_V(TEXT, SUB, VALUE1, VALUE2, MEMBER, TOOLTIP1, TOOLTIP2, DESCRIPTION1, DESCRIPTION2, LABEL1, LABEL2) \
   STATE_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE1, MEMBER, DESCRIPTION1) \
   STATE_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE2, MEMBER, DESCRIPTION2) \
   STATE_ATTR_SOURCE_UPDATE(SUB, VALUE1, MEMBER, part->type, ==) \
   STATE_ATTR_SOURCE_UPDATE(SUB, VALUE2, MEMBER, part->type, ==) \
   STATE_ATTR_2COMBOBOX_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, TOOLTIP1, TOOLTIP2, LABEL1, LABEL2)

#define STATE_ATTR_2SPINNER_ICON(TEXT, SUB, VALUE1, VALUE2, MEMBER, MIN, MAX, STEP, FMT, \
                                 L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER, \
                                 TYPE, DESC1, DESC2) \
   STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE1, MEMBER, TYPE, MULTIPLIER, DESC1) \
   STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE2, MEMBER, TYPE, MULTIPLIER, DESC2) \
   STATE_ATTR_2SPINNER_ADD(TEXT, "2swallow_pad", SUB, VALUE1, VALUE2, MEMBER, TYPE, MIN, MAX, STEP, FMT, \
                           L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER)

STATE_ATTR_2COMBOBOX_V(_("relative to"), state, rel1_to_x, rel1_to_y, state_object_area,
                       _("Causes a corner to be positioned relatively to the X axis of another "
                         "part. Setting to \"\" will un-set this value"),
                       _("Causes a corner to be positioned relatively to the Y axis of another "
                         "part. Setting to \"\" will un-set this value"),
                       _("rel1.to_x changed to \"%s\""),
                       _("rel1.to_y changed to \"%s\""),
                       _("x:"), _("y:"))
STATE_ATTR_2SPINNER_ICON(_("align"), state, rel1_relative_x, rel1_relative_y, state_object_area,
                         -500, 500, 1, NULL, "x:", "%", "y:", "%",
                         _("Define the position of left-up corner of the part's container. "
                           "Moves a corner to a relative position inside the container "
                           "by X axis."),
                         _("Define the position of left-up corner of the part's container. "
                           "Moves a corner to a relative position inside the container "
                           "by Y axis."),
                         100, double,
                         _("rel1 align x changed from %.2f to %.2f"),
                         _("rel1 align y changed from %.2f to %.2f"))
STATE_ATTR_2SPINNER_ICON(_("offset"), state, rel1_offset_x, rel1_offset_y, state_object_area,
                         -9999, 9999, 1, NULL, "x:", "px", "y:", "px",
                         _("Left offset from relative position in pixels"),
                         _("Top offset from relative position in pixels"),
                         1, int,
                         _("rel1 offset x changed from %d to %d"),
                         _("rel1 offset y changed from %d to %d"))
STATE_ATTR_2COMBOBOX_V(_("relative to"), state, rel2_to_x, rel2_to_y, state_object_area,
                       _("Causes a corner to be positioned relatively to the X axis of another "
                         "part. Setting to \"\" will un-set this value"),
                       _("Causes a corner to be positioned relatively to the Y axis of another "
                         "part. Setting to \"\" will un-set this value"),
                       _("rel2.to_x changed to \"%s\""),
                       _("rel2.to_y changed to \"%s\""),
                       _("x:"), _("y:"))
STATE_ATTR_2SPINNER_ICON(_("align"), state, rel2_relative_x, rel2_relative_y, state_object_area,
                         -500, 500, 1, NULL, "x:", "%", "y:", "%",
                         _("Define the position of right-down corner of the part's container. "
                           "Moves a corner to a relative position inside the container "
                           "by X axis."),
                         _("Define the position of right-down corner of the part's container. "
                           "Moves a corner to a relative position inside the container "
                           "by Y axis."),
                         100, double,
                         _("rel2 align x changed from %.2f to %.2f"),
                         _("rel2 align y changed from %.2f to %.2f"))
STATE_ATTR_2SPINNER_ICON(_("offset"), state, rel2_offset_x, rel2_offset_y, state_object_area,
                         -9999, 9999, 1, NULL, "x:", "px", "y:", "px",
                         _("Right offset from relative position in pixels"),
                         _("Bottom offset from relative position in pixels"),
                         1, int,
                         _("rel2 offset x changed from %d to %d"),
                         _("rel2 offset y changed from %d to %d"))

#define pd_obj_area pd->attributes.state_object_area
static Eina_Bool
ui_property_state_obj_area_set(Evas_Object *property)
{
   Evas_Object *item, *icon;
   Evas_Object *obj_area_frame, *box, *prop_box;

   GROUP_PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->scroller);
   elm_box_unpack(prop_box, pd_obj_area.frame);

   if (!pd_obj_area.frame)
     {
        FRAME_PROPERTY_ADD(property, obj_area_frame, true, _("Object area"), pd->scroller)
        BOX_ADD(obj_area_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(obj_area_frame, box);

        item = elm_separator_add(obj_area_frame);
        elm_separator_horizontal_set(item, true);
        elm_object_style_set(item, "icon");
        elm_object_part_text_set(item, "eflete.text", _("Start point"));
        IMAGE_ADD_NEW(item, icon, "icon", "start-point")
        elm_object_part_content_set(item, "eflete.swallow.icon", icon);
        evas_object_show(item);
        elm_box_pack_end(box, item);

        item = prop_state_object_area_rel1_to_x_rel1_to_y_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_object_area_rel1_relative_x_rel1_relative_y_add(box, pd);
        IMAGE_ADD_NEW(item, icon, "icon", "align");
        elm_layout_content_set(item, NULL, icon);
        elm_box_pack_end(box, item);
        item = prop_state_object_area_rel1_offset_x_rel1_offset_y_add(box, pd);
        IMAGE_ADD_NEW(item, icon, "icon", "offset");
        elm_layout_content_set(item, NULL, icon);
        elm_box_pack_end(box, item);

        item = elm_separator_add(obj_area_frame);
        elm_separator_horizontal_set(item, true);
        elm_object_style_set(item, "icon");
        elm_object_part_text_set(item, "eflete.text", _("End point"));
        IMAGE_ADD_NEW(item, icon, "icon", "end-point");
        elm_object_part_content_set(item, "eflete.swallow.icon", icon);
        evas_object_show(item);
        elm_box_pack_end(box, item);

        item = prop_state_object_area_rel2_to_x_rel2_to_y_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_object_area_rel2_relative_x_rel2_relative_y_add(box, pd);
        IMAGE_ADD_NEW(item, icon, "icon", "align");
        elm_layout_content_set(item, NULL, icon);
        elm_box_pack_end(box, item);
        item = prop_state_object_area_rel2_offset_x_rel2_offset_y_add(box, pd);
        IMAGE_ADD_NEW(item, icon, "icon", "offset");
        elm_layout_content_set(item, NULL, icon);
        elm_box_pack_end(box, item);

        prop_box = elm_object_content_get(pd->scroller);
        elm_box_pack_end(prop_box, obj_area_frame);
        pd_obj_area.frame = obj_area_frame;
     }
   else
     {
        prop_state_object_area_rel1_to_x_update(pd);
        prop_state_object_area_rel1_to_y_update(pd);
        STATE_ATTR_2SPINNER_UPDATE(state, rel1_relative_x, rel1_relative_y, state_object_area, double, 100)
        STATE_ATTR_2SPINNER_UPDATE(state, rel1_offset_x, rel1_offset_y, state_object_area, int,  1)

        prop_state_object_area_rel2_to_x_update(pd);
        prop_state_object_area_rel2_to_y_update(pd);
        STATE_ATTR_2SPINNER_UPDATE(state, rel2_relative_x, rel2_relative_y, state_object_area, double, 100)
        STATE_ATTR_2SPINNER_UPDATE(state, rel2_offset_x, rel2_offset_y, state_object_area, int, 1)

        prop_box = elm_object_content_get(pd->scroller);
        elm_box_pack_end(prop_box, pd_obj_area.frame);
        evas_object_show(pd_obj_area.frame);
     }

   return true;
}

static void
ui_property_state_obj_area_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   GROUP_PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->scroller);
   elm_box_unpack(prop_box, pd_obj_area.frame);
   evas_object_hide(pd_obj_area.frame);
}
#undef pd_obj_area

#define STATE_ATTR_1ENTRY(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP, DESCRIPTION) \
   STATE_ATTR_1ENTRY_CALLBACK(SUB, VALUE, VALIDATOR, DESCRIPTION) \
   STATE_ATTR_1ENTRY_UPDATE(SUB, VALUE, MEMBER) \
   STATE_ATTR_1ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP)

#define STATE_ATTR_1SPINNER(TEXT, SUB, VALUE, MEMBER, MIN, MAX, STEP, FMT, \
                            L_START, L_END, TOOLTIP, MULTIPLIER, \
                            TYPE, DESC) \
   STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, DESC) \
   STATE_ATTR_1SPINNER_ADD(TEXT, SUB, VALUE, MEMBER, MIN, MAX, STEP, FMT, \
                           L_START, L_END, TOOLTIP, MULTIPLIER)

STATE_ATTR_1ENTRY(_("text"), state, text, state_text, NULL, _("The dispalyed text"),
                  _("text changed to %s"))
STATE_ATTR_1ENTRY(_("font"), state, font, state_text, pd->attributes.state_text.validator,
                  _("The text font, posible set a font style. Ex: Sans:style=italic"),
                  _("font changed to %s"))
STATE_ATTR_1SPINNER(_("size"), state_text, size, state_text, 1, 128, 1, "%.0f", "", "px",
                    _("The font size"), 1, int,
                    _("font size changed from %dpx to %dpx"))
STATE_ATTR_2SPINNER(_("align"), state_text, align_x, align_y, state_text,
                    0.0, 100.0, 1.0, "%.0f", "x:", "%", "y:", "%",
                    _("Text horizontal align"), _("Text vertical align"),
                    100, double,
                    _("text horizontal align changed from %.2f to %.2f"),
                    _("text vertical align changed from %.2f to %.2f"))
STATE_ATTR_2CHECK(_("min"), state_text, min_x, min_y, state_text, "w:", "", "h:", "",
                  _("When any of the parameters is enabled it forces \t"
                  "the minimum size of the container to be equal to\t"
                  "the minimum size of the text."),
                  _("When any of the parameters is enabled it forces \t"
                  "the minimum size of the container to be equal to\t"
                  "the minimum size of the text."),
                  _("min_x changed to %s"),
                  _("min_y changed to %s"));
STATE_ATTR_2CHECK(_("max"), state_text, max_x, max_y, state_text, "w:", "", "h:", "",
                  _("When any of the parameters is enabled it forces \t"
                  "the maximum size of the container to be equal to\t"
                  "the maximum size of the text."),
                  _("When any of the parameters is enabled it forces \t"
                  "the maximum size of the container to be equal to\t"
                  "the maximum size of the text."),
                  _("max_x changed to %s"),
                  _("max_y changed to %s"))
STATE_ATTR_2CHECK(_("fit"), state_text, fit_x, fit_y, state_text, "w:", "", "h:", "",
                  _("Resize the text for it to fit in it's container by X axis"),
                  _("Resize the text for it to fit in it's container by Y axis"),
                  _("fit_x changed to %s"),
                  _("fit_y changed to %s"))
STATE_ATTR_COLOR(_("shadow color"), state, color2, state_text, NULL,
                 _("shadow color changed to [%d %d %d %d]"))
STATE_ATTR_COLOR(_("outline color"), state, color3, state_text, NULL,
                 _("outline color changed [%d %d %d %d]"))

STATE_ATTR_SOURCE_UPDATE(state_text, source, state_text, EDJE_PART_TYPE_TEXT, ==)
STATE_ATTR_COMBOBOX(_("source"), state_text, source, state_text,
                    _("Causes the part to use the text properties "
                      "(like font and size) of another part "
                      "and update them as they change."),
                    _("source changed to \"%s\""))
STATE_ATTR_SOURCE_UPDATE(state_text, text_source, state_text, EDJE_PART_TYPE_TEXT, ==)
STATE_ATTR_COMBOBOX(_("text source"), state_text, text_source, state_text,
                    _("Causes the part to display the text content of "
                      "another part and update them as they change."),
                    _("text source changed to \"%s\""))

inline static void
_text_effect_update(Group_Prop_Data *pd)
{
   Edje_Text_Effect effect;

   assert(pd != NULL);
   assert(pd->change != NULL);

   effect = ewe_combobox_select_item_get(pd->attributes.state_text.effect)->index |
            ewe_combobox_select_item_get(pd->attributes.state_text.effect_direction)->index << 4;
   editor_part_effect_set(pd->group->edit_object, pd->change, false, pd->part->name, effect);

   history_change_add(pd->group->history, pd->change);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
   pd->change = NULL;
}

static void
_text_effect_contols_update(Group_Prop_Data *pd)
{
   assert(pd != NULL);

   switch (ewe_combobox_select_item_get(pd->attributes.state_text.effect)->index)
     {
      case EDJE_TEXT_EFFECT_NONE:
      case EDJE_TEXT_EFFECT_PLAIN:
        {
           elm_object_disabled_set(pd->attributes.state_text.effect_direction, true);
           ewe_combobox_text_set(pd->attributes.state_text.effect_direction, _("None"));
           elm_object_disabled_set(pd->attributes.state_text.color3, true);
           elm_object_disabled_set(pd->attributes.state_text.color2, true);
           break;
        }
      case EDJE_TEXT_EFFECT_OUTLINE:
      case EDJE_TEXT_EFFECT_SOFT_OUTLINE:
        {
           elm_object_disabled_set(pd->attributes.state_text.effect_direction, true);
           ewe_combobox_text_set(pd->attributes.state_text.effect_direction, _("None"));
           elm_object_disabled_set(pd->attributes.state_text.color3, false);
           elm_object_disabled_set(pd->attributes.state_text.color2, true);
           break;
        }
      case EDJE_TEXT_EFFECT_OUTLINE_SHADOW:
      case EDJE_TEXT_EFFECT_OUTLINE_SOFT_SHADOW:
      case EDJE_TEXT_EFFECT_GLOW:
        {
           elm_object_disabled_set(pd->attributes.state_text.effect_direction, false);
           elm_object_disabled_set(pd->attributes.state_text.color3, false);
           elm_object_disabled_set(pd->attributes.state_text.color2, false);
           break;
        }
      default:
        {
           elm_object_disabled_set(pd->attributes.state_text.effect_direction, false);
           elm_object_disabled_set(pd->attributes.state_text.color3, true);
           elm_object_disabled_set(pd->attributes.state_text.color2, false);
        }
     }
}

static void
_on_text_effect_direction_changed(void *data,
                                  Evas_Object *obj __UNUSED__,
                                  void *event_info __UNUSED__)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Ewe_Combobox_Item *item;

   assert(pd != NULL);
   assert(pd->change == NULL);

   item = ewe_combobox_select_item_get(pd->attributes.state_text.effect_direction);
   Eina_Stringshare *msg = eina_stringshare_printf(_("effect direction changed to %s"), item->title);
   pd->change = change_add(msg);
   eina_stringshare_del(msg);
   _text_effect_update((Group_Prop_Data *)data);
}

static void
_on_text_effect_changed(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Ewe_Combobox_Item *item;

   assert(pd != NULL);
   assert(pd->change == NULL);

   _text_effect_contols_update(pd);
   ewe_combobox_select_item_set(pd->attributes.state_text.effect_direction, 0);
   item = ewe_combobox_select_item_get(pd->attributes.state_text.effect);
   Eina_Stringshare *msg = eina_stringshare_printf(_("effect changed to %s"), item->title);
   pd->change = change_add(msg);
   eina_stringshare_del(msg);
   _text_effect_update((Group_Prop_Data *)data);
}


static void
prop_part_text_effect_update(Group_Prop_Data *pd)
{
   int type, direction;
   Edje_Text_Effect edje_effect;

   assert(pd != NULL);

   edje_effect = edje_edit_part_effect_get(pd->group->edit_object, pd->part->name);

   type = edje_effect & EDJE_TEXT_EFFECT_MASK_BASIC;
   if ((type >= EDJE_TEXT_EFFECT_SOFT_OUTLINE) && (type != EDJE_TEXT_EFFECT_GLOW))
     direction = (edje_effect & EDJE_TEXT_EFFECT_MASK_SHADOW_DIRECTION) >> 4;
   else direction = 0;
   ewe_combobox_select_item_set(pd->attributes.state_text.effect, type);

   if ((type <= EDJE_TEXT_EFFECT_SOFT_OUTLINE) || (type == EDJE_TEXT_EFFECT_GLOW))
     elm_object_disabled_set(pd->attributes.state_text.effect_direction, true);
   else
     elm_object_disabled_set(pd->attributes.state_text.effect_direction, false);
   ewe_combobox_select_item_set(pd->attributes.state_text.effect_direction, direction);

   _text_effect_contols_update(pd);
}

static Evas_Object *
prop_part_text_effect_add(Evas_Object *parent, Group_Prop_Data *pd)
{
   int i;

   assert(parent != NULL);
   assert(pd != NULL);

   PROPERTY_ITEM_ADD(parent, _("effect"), "2swallow_vertical")

   elm_object_part_text_set(item, "label.swallow1.start", _("type:"));
   EWE_COMBOBOX_ADD(item, pd->attributes.state_text.effect)
   for (i = 0; edje_text_effect_type[i];
        ewe_combobox_item_add(pd->attributes.state_text.effect, edje_text_effect_type[i]), i++) ;
   elm_object_tooltip_text_set(pd->attributes.state_text.effect, _("Causes Edje to draw the selected effect."));
   evas_object_smart_callback_add(pd->attributes.state_text.effect, "selected", _on_text_effect_changed, pd);
   elm_object_part_content_set(item, "swallow.content1", pd->attributes.state_text.effect);

   elm_object_part_text_set(item, "label.swallow2.start", _("direction:"));
   EWE_COMBOBOX_ADD(item, pd->attributes.state_text.effect_direction)
   for (i = 0; edje_text_effect_direction[i];
        ewe_combobox_item_add(pd->attributes.state_text.effect_direction, edje_text_effect_direction[i]), i++) ;
   elm_object_tooltip_text_set(pd->attributes.state_text.effect_direction, _("Shadow directions."));
   evas_object_smart_callback_add(pd->attributes.state_text.effect_direction, "selected", _on_text_effect_direction_changed, pd);
   elm_object_part_content_set(item, "swallow.content2", pd->attributes.state_text.effect_direction);

   return item;
}

#define pd_text pd->attributes.state_text

COMMON_SPINNER_CALLBACK(state_text, elipsis, state_text, double, 1, STATE_ARGS,
                        _("elipsis changed from %0.2f to %0.2f"))

static void
_on_state_text_elipsis_toggle_change(void *data,
                                      Evas_Object *obj,
                                      void *event_info __UNUSED__)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Eina_Bool val;
   Change *change;

   assert(pd != NULL);

   val = elm_check_state_get(obj);

   if (val)
     {
        elm_object_disabled_set(pd->attributes.state_text.elipsis, false);
        change = change_add(_("elipsis turned on"));
        editor_state_text_elipsis_set(pd->group->edit_object, change, false STATE_ARGS, 0.0);
     }
   else
     {
        elm_object_disabled_set(pd->attributes.state_text.elipsis, true);
        change = change_add(_("elipsis turned off"));
        editor_state_text_elipsis_set(pd->group->edit_object, change, false STATE_ARGS, -1.0);
     }
   history_change_add(pd->group->history, change);
}

static void
prop_state_text_elipsis_update(Group_Prop_Data *pd)
{
   double value;

   assert(pd != NULL);

   value = edje_edit_state_text_elipsis_get(pd->group->edit_object,
                                            pd->part->name,
                                            pd->part->current_state->parsed_name,
                                            pd->part->current_state->parsed_val);
   if (value < 0)
     {
        elm_check_state_set(pd->attributes.state_text.elipsis_toggle, false);
        elm_object_disabled_set(pd->attributes.state_text.elipsis, true);
        elm_spinner_value_set(pd->attributes.state_text.elipsis, 0.0);
     }
   else
     {
        elm_check_state_set(pd->attributes.state_text.elipsis_toggle, true);
        elm_object_disabled_set(pd->attributes.state_text.elipsis, false);
        elm_spinner_value_set(pd->attributes.state_text.elipsis, value);
     }
}

static Evas_Object *
prop_state_text_elipsis_add(Evas_Object *parent, Group_Prop_Data *pd)
{
   assert(parent != NULL);
   assert(pd != NULL);

   PROPERTY_ITEM_ADD(parent, _("elipsis"), "2swallow")
   CHECK_ADD(item, pd->attributes.state_text.elipsis_toggle)
   elm_object_style_set(pd->attributes.state_text.elipsis_toggle, "toggle");
   elm_object_tooltip_text_set(pd->attributes.state_text.elipsis_toggle,
                               _("Turn on/off the text elipsis"));
   elm_layout_content_set(item, "swallow.content1", pd->attributes.state_text.elipsis_toggle);
   evas_object_smart_callback_add(pd->attributes.state_text.elipsis_toggle, "changed",
                                  _on_state_text_elipsis_toggle_change, pd);

   SPINNER_ADD(item, pd->attributes.state_text.elipsis, 0.0, 1.0, 0.1, true)
   elm_spinner_label_format_set(pd->attributes.state_text.elipsis, "%1.2f");
   elm_object_tooltip_text_set(pd->attributes.state_text.elipsis,
                               _("Cut text if biggest then part's area"
                                 "0.0 = fix the left side  1.0 = right side"));
   elm_layout_content_set(item, "swallow.content2", pd->attributes.state_text.elipsis);
   evas_object_smart_callback_add(pd->attributes.state_text.elipsis, "spinner,drag,start", _on_state_text_elipsis_start, pd);
   evas_object_smart_callback_add(pd->attributes.state_text.elipsis, "spinner,drag,stop", _on_state_text_elipsis_stop, pd);
   evas_object_smart_callback_add(pd->attributes.state_text.elipsis, "changed", _on_state_text_elipsis_change, pd);
   evas_object_event_callback_priority_add(pd->attributes.state_text.elipsis, EVAS_CALLBACK_MOUSE_WHEEL,
                                           EVAS_CALLBACK_PRIORITY_BEFORE, _on_spinner_mouse_wheel, NULL);

   prop_state_text_elipsis_update(pd);
   return item;
}

static Eina_Bool
ui_property_state_text_set(Evas_Object *property)
{
   Evas_Object *item;
   Evas_Object *text_frame, *box, *prop_box;

   GROUP_PROP_DATA_GET()

   /* if previos selected part is TEXT too, unpack it */
   ui_property_state_text_unset(property);
   prop_box = elm_object_content_get(pd->scroller);
   if (!pd_text.frame)
     {
         FRAME_PROPERTY_ADD(property, text_frame, true, _("Text"), pd->scroller)
         BOX_ADD(text_frame, box, EINA_FALSE, EINA_FALSE)
         elm_box_align_set(box, 0.5, 0.0);
         elm_object_content_set(text_frame, box);

         if (pd_text.validator == NULL)
           pd_text.validator = elm_validator_regexp_new(FONT_STYLE_REGEX, NULL);

         item = prop_state_text_add(box, pd, NULL);
         elm_box_pack_end(box, item);
         item = prop_state_font_add(box, pd, NULL);
         elm_box_pack_end(box, item);
         item = prop_state_text_size_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_text_align_x_align_y_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_text_min_x_min_y_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_text_max_x_max_y_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_text_source_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_text_text_source_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_text_elipsis_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_text_fit_x_fit_y_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_part_text_effect_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_color3_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_color2_add(box, pd);
         elm_box_pack_end(box, item);

          /* disable/enable the effect and colors controls */
          prop_part_text_effect_update(pd);

         elm_box_pack_end(prop_box, text_frame);
         pd_text.frame = text_frame;
     }
   else
     {
        prop_state_text_update(pd);
        prop_state_font_update(pd);
        COMMON_1SPINNER_UPDATE(state_text, size, state_text, int,  1, STATE_ARGS)
        STATE_ATTR_2SPINNER_UPDATE(state_text, align_x, align_y, state_text, double, 100)
        STATE_ATTR_2CHECK_UPDATE(state_text, min_x, min_y, state_text)
        STATE_ATTR_2CHECK_UPDATE(state_text, max_x, max_y, state_text)
        prop_state_text_source_update(pd);
        prop_state_text_text_source_update(pd);
        prop_state_text_elipsis_update(pd);
        STATE_ATTR_2CHECK_UPDATE(state_text, fit_x, fit_y, state_text)
        prop_state_color2_update(pd);
        prop_state_color3_update(pd);
        prop_part_text_effect_update(pd);
        elm_box_pack_end(prop_box, pd_text.frame);
        evas_object_show(pd_text.frame);
     }

   return true;
}

static void
ui_property_state_text_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   GROUP_PROP_DATA_GET()
   /*
   if (pd_text.validator != NULL)
     {
        elm_validator_regexp_free(pd_text.validator);
        pd_text.validator = NULL;
     }
   */
   prop_box = elm_object_content_get(pd->scroller);
   elm_box_unpack(prop_box, pd_text.frame);
   evas_object_hide(pd_text.frame);
}
#undef pd_text

static void
prop_state_textblock_style_update(Group_Prop_Data *pd)
{
   Eina_List *slist, *l;
   const char *sname;

   assert(pd != NULL);

   ewe_combobox_items_list_free(pd->attributes.state_textblock.style, true);
   ewe_combobox_item_add(pd->attributes.state_textblock.style, _("None"));
   slist = edje_edit_styles_list_get(pd->group->edit_object);
   EINA_LIST_FOREACH(slist, l, sname)
     {
        ewe_combobox_item_add(pd->attributes.state_textblock.style, sname);
     }
   sname = edje_edit_state_text_style_get(pd->group->edit_object,
                                          pd->part->name,
                                          pd->part->current_state->parsed_name,
                                          pd->part->current_state->parsed_val);
   ewe_combobox_text_set(pd->attributes.state_textblock.style, sname);
   edje_edit_string_free(sname);
   edje_edit_string_list_free(slist);
}


PART_ATTR_SOURCE_UPDATE(state_textblock, source);
PART_ATTR_SOURCE_UPDATE(state_textblock, source2);
PART_ATTR_SOURCE_UPDATE(state_textblock, source3);
PART_ATTR_SOURCE_UPDATE(state_textblock, source4);
PART_ATTR_SOURCE_UPDATE(state_textblock, source5);
PART_ATTR_SOURCE_UPDATE(state_textblock, source6);

#define pd_textblock pd->attributes.state_textblock
STATE_ATTR_COMBOBOX(_("style"), state_text, style, state_textblock,
                    _("Set the text style of part."),
                    _("style changed to \"%s\""))
STATE_ATTR_2SPINNER(_("align"), state_text, align_x, align_y, state_textblock,
                    0.0, 100.0, 1.0, "%.0f", "x:", "%", "y:", "%",
                    _("Text horizontal align"), _("Text vertical align"),
                    100, double,
                    _("text horizontal align changed from %.2f to %.2f"),
                    _("text vertical align changed from %.2f to %.2f"))
STATE_ATTR_2CHECK(_("min"), state_text, min_x, min_y, state_textblock, "w:", "", "h:", "",
                  _("When any of the parameters is enabled it forces \t"
                  "the minimum size of the container to be equal to\t"
                  "the minimum size of the text."),
                  _("When any of the parameters is enabled it forces \t"
                  "the minimum size of the container to be equal to\t"
                  "the minimum size of the text."),
                  _("text min_x changed to %s"),
                  _("text min_y changed to %s"))
STATE_ATTR_2CHECK(_("max"), state_text, max_x, max_y, state_textblock, "w:", "", "h:", "",
                  _("When any of the parameters is enabled it forces \t"
                  "the maximum size of the container to be equal to\t"
                  "the maximum size of the text."),
                  _("When any of the parameters is enabled it forces \t"
                  "the maximum size of the container to be equal to\t"
                  "the maximum size of the text."),
                  _("text max_x changed to %s"),
                  _("text max_y changed to %s"))
PART_ATTR_1COMBOBOX_LIST(_("select mode"), part, select_mode, state_textblock, Edje_Edit_Select_Mode,
                         edje_select_mode, _("Sets the selection mode for a textblock part"),
                         _("select mode changed to %s"))
PART_ATTR_1COMBOBOX_LIST(_("entry mode"), part, entry_mode, state_textblock, Edje_Edit_Entry_Mode,
                         edje_entry_mode, _("Sets the edit mode for a textblock part"),
                         _("entry mode changed to %s"))
PART_ATTR_1COMBOBOX_LIST(_("pointer mode"), part, pointer_mode, state_textblock, Evas_Object_Pointer_Mode,
                         edje_pointer_mode, _("Sets the mouse pointer behavior for a given part"),
                         _("pointer mode changed to %s"))
PART_ATTR_1COMBOBOX_LIST(_("cursor mode"), part, cursor_mode, state_textblock, unsigned int,
                         edje_cursor_mode, _("Sets the cursor mode for a textblock part"),
                         _("cursor mode changed to %s"))
PART_ATTR_1CHECK(_("multiline"), part, multiline, state_textblock,
                 _("It causes a textblock that is editable to allow multiple lines for editing"),
                 _("multiline changed to %s"))
PART_ATTR_1COMBOBOX(_("under selected text"), part, source, state_textblock,
                    _("Used for the group to be loaded and used for selection \t"
                    "display UNDER the selected text the source \t"
                    "of TEXTBLOCK part."),
                    _("under selected text changed to %s"))
PART_ATTR_1COMBOBOX(_("before selected text"), part, source2, state_textblock,
                    _("It is used for the group to be loaded and used for \t"
                    "selection display OVER the selected text."),
                    _("before selected text changed to %s"))
PART_ATTR_1COMBOBOX(_("under cursor"), part, source3, state_textblock,
                    _("It is used for the group to be loaded and used for \t"
                    "cursor display UNDER the cursor position."),
                    _("under cursor changed to %s"))
PART_ATTR_1COMBOBOX(_("over cursor"), part, source4, state_textblock,
                    _("It is used for the group to be loaded and used \t"
                    "for cursor display OVER the cursor position."),
                    _("over cursor changed to %s"))
PART_ATTR_1COMBOBOX(_("under anchor"), part, source5, state_textblock,
                    _("It is used for the group to be loaded and used for \t"
                    "anchors display UNDER the anchor position."),
                    _("under anchor changed to %s"))
PART_ATTR_1COMBOBOX(_("over anchor"), part, source6, state_textblock,
                    _("It is used for the group to be loaded and used for \t"
                    "anchor display OVER the anchor position."),
                    _("over anchor changed to %s"))

static Eina_Bool
ui_property_state_textblock_set(Evas_Object *property)
{
   Evas_Object *item;
   Evas_Object *textblock_frame, *box, *prop_box;

   GROUP_PROP_DATA_GET()

   /* if previos selected part is TEXTBLOCK too, unpack it */
   ui_property_state_textblock_unset(property);
   prop_box = elm_object_content_get(pd->scroller);
   if (!pd_textblock.frame)
     {
         FRAME_PROPERTY_ADD(property, textblock_frame, true, _("TextBlock"), pd->scroller)
         BOX_ADD(textblock_frame, box, EINA_FALSE, EINA_FALSE)
         elm_box_align_set(box, 0.5, 0.0);
         elm_object_content_set(textblock_frame, box);

         item = prop_state_text_add(box, pd, NULL);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_style_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_align_x_align_y_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_min_x_min_y_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_max_x_max_y_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_select_mode_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_entry_mode_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_pointer_mode_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_cursor_mode_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_multiline_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_source_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_source2_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_source3_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_source4_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_source5_add(box, pd);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_source6_add(box, pd);
         elm_box_pack_end(box, item);

         elm_box_pack_end(prop_box, textblock_frame);
         pd_textblock.frame = textblock_frame;
     }
   else
     {
        prop_state_text_update(pd);
        prop_state_textblock_style_update(pd);
        STATE_ATTR_2SPINNER_UPDATE(state_text, align_x, align_y, state_textblock, double, 100)
        STATE_ATTR_2CHECK_UPDATE(state_text, min_x, min_y, state_textblock)
        STATE_ATTR_2CHECK_UPDATE(state_text, max_x, max_y, state_textblock)
        PART_ATTR_1COMBOBOX_LIST_UPDATE(part, select_mode, state_textblock)
        PART_ATTR_1COMBOBOX_LIST_UPDATE(part, entry_mode, state_textblock)
        PART_ATTR_1COMBOBOX_LIST_UPDATE(part, pointer_mode, state_textblock)
        PART_ATTR_1COMBOBOX_LIST_UPDATE(part, cursor_mode, state_textblock)
        PART_ATTR_1CHECK_UPDATE(part, multiline, state_textblock);
        prop_state_textblock_source_update(pd);
        prop_state_textblock_source2_update(pd);
        prop_state_textblock_source3_update(pd);
        prop_state_textblock_source4_update(pd);
        prop_state_textblock_source5_update(pd);
        prop_state_textblock_source6_update(pd);
        elm_box_pack_end(prop_box, pd_textblock.frame);
        evas_object_show(pd_textblock.frame);
     }
   return true;
}

static void
ui_property_state_textblock_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   GROUP_PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->scroller);
   elm_box_unpack(prop_box, pd_textblock.frame);
   evas_object_hide(pd_textblock.frame);
}
#undef pd_textblock


#define pd_image pd->attributes.state_image

static Eina_Bool
_on_image_editor_done(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   const char *value;
   Eina_List *list_selected = (Eina_List *)event_info;
   const char *selected;

   Group_Prop_Data *pd = (Group_Prop_Data *)data;

   if (!list_selected) return false;
   value = elm_entry_entry_get(pd->attributes.state_image.image);

   selected = eina_list_data_get(list_selected);

   if (strcmp(value, selected) == 0) return true;
   elm_entry_entry_set(pd->attributes.state_image.image, selected);

   Eina_Stringshare *msg = eina_stringshare_printf(_("image changes to %s"), selected);
   Change *change = change_add(msg);
   eina_stringshare_del(msg);
   editor_state_image_set(pd->group->edit_object, change, false,
                          pd->part->name,
                          pd->part->current_state->parsed_name,
                          pd->part->current_state->parsed_val,
                          selected);

   history_change_add(pd->group->history, change);
   evas_object_smart_callback_call(pd->attributes.state_image.image, "changed,user", NULL);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);

   return true;
}

static void
_on_state_image_choose(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *ei __UNUSED__)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;

   assert(pd != NULL);

   popup_gengrid_image_helper(NULL,
                              pd->attributes.state_image.image,
                              _on_image_editor_done,
                              pd,
                              false);
}

static void
_del_tween_image(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *tween_list = (Evas_Object *)data;
   const Eina_List *sel_list = elm_gengrid_selected_items_get(tween_list), *l;
   Eina_List *selected = NULL;
   Elm_Object_Item *it;
   const char *name;
   Group_Prop_Data *pd = evas_object_data_get(tween_list, GROUP_PROP_DATA);

   assert(pd != NULL);

   EINA_LIST_FOREACH(sel_list, l, it)
     {
        name = elm_object_item_data_get(it);
        selected = eina_list_append(selected, eina_stringshare_add(name));
     }

   Eina_Stringshare *msg = eina_stringshare_printf(_("removed %d tween images"),
                                                   eina_list_count(sel_list));
   Change *change = change_add(msg);
   eina_stringshare_del(msg);
   EINA_LIST_FOREACH(selected, l, name)
     {
        editor_state_tween_del(pd->group->edit_object, change, false,
                               pd->part->name,
                               pd->part->current_state->parsed_name,
                               pd->part->current_state->parsed_val,
                               name);
        eina_stringshare_del(name);
     }
   eina_list_free(selected);
   history_change_add(pd->group->history, change);
}

static Eina_Bool
_on_image_editor_tween_done(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info)
{
   Evas_Object *tween_list = (Evas_Object *)data;
   Eina_List *selected = (Eina_List *)event_info;
   Eina_List *l = NULL;
   const char *name = NULL;
   Group_Prop_Data *pd = evas_object_data_get(tween_list, GROUP_PROP_DATA);

   assert(pd != NULL);

   if (!selected) return false;
   Eina_Stringshare *msg = eina_stringshare_printf(_("added %d tween images"),
                                                   eina_list_count(selected));
   Change *change = change_add(msg);
   eina_stringshare_del(msg);
   EINA_LIST_FOREACH(selected, l, name)
     {
        editor_state_tween_add(pd->group->edit_object, change, false,
                               pd->part->name,
                               pd->part->current_state->parsed_name,
                               pd->part->current_state->parsed_val,
                               name);
     }
   history_change_add(pd->group->history, change);

   return true;
}

static void
_add_tween_image(void *data,
                 Evas_Object *obj,
                 void *event_info __UNUSED__)
{
   Evas_Object *tween_list = (Evas_Object *)data;
   Group_Prop_Data *pd = evas_object_data_get(tween_list, GROUP_PROP_DATA);

   assert(pd != NULL);

   popup_gengrid_image_helper(NULL,
                              obj,
                              _on_image_editor_tween_done,
                              tween_list,
                              true);

   return;
}

static void
_item_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   assert(data != NULL);

   eina_stringshare_del(data);
}

static char *
_item_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part __UNUSED__)
{
   return strdup(data);
}

static Evas_Object *
_item_content_get(void *data, Evas_Object *obj, const char *part)
{
   const char* buf = NULL;
   const char *file = NULL, *group = NULL;
   Evas_Load_Error err;
   Evas_Object *image;
   Group_Prop_Data *pd = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        pd = evas_object_data_get(obj, GROUP_PROP_DATA);

        assert(pd != NULL);

        edje_object_file_get((const Eo*)pd->group->edit_object, &file, &group);
        image = evas_object_image_add(evas_object_evas_get(obj));
        buf = eina_stringshare_printf("edje/images/%i",
                                      edje_edit_image_id_get(pd->group->edit_object, (const char*)data));
        evas_object_image_file_set(image, file, buf);
        err = evas_object_image_load_error_get(image);
        if (err != EVAS_LOAD_ERROR_NONE)
          {
             WARN("Image [%s] from file [%s] loaded with errors", buf, file);
             return NULL;
          }
        evas_object_image_filled_set(image, true);
        return image;
     }
   return NULL;
}

static void __UNUSED__
_tween_image_moved(Evas_Object *data,
                   Evas_Object *obj,
                   Elm_Object_Item *item EINA_UNUSED)
{
   Group_Prop_Data *pd = (Group_Prop_Data *)data;
   Eina_List *images_list, *l;
   Elm_Object_Item *next;
   const char *image_name;

   assert(pd != NULL);

   images_list = edje_edit_state_tweens_list_get(pd->group->edit_object,
                                                 pd->part->name,
                                                 pd->part->current_state->parsed_name,
                                                 pd->part->current_state->parsed_val);

   EINA_LIST_FOREACH(images_list, l, image_name)
     {
        edje_edit_state_tween_del(pd->group->edit_object, pd->part->name,
                                  pd->part->current_state->parsed_name, pd->part->current_state->parsed_val,
                                  image_name);
     }
   edje_edit_string_list_free(images_list);

   next = elm_genlist_first_item_get(obj);
   while (next)
     {
        image_name  = elm_object_item_data_get(next);
        edje_edit_state_tween_add(pd->group->edit_object, pd->part->name,
                                  pd->part->current_state->parsed_name, pd->part->current_state->parsed_val,
                                  image_name);
        next = elm_genlist_item_next_get(next);
     }
   //project_changed(false);
}

static Evas_Object *
_prop_item_state_image_tween_add(Evas_Object *box, Group_Prop_Data *pd)
{
   Evas_Object *tween_frame, *tween_list, *item, *button, *icon;
   Eina_List *images_list, *l;
   char *image_name;

   assert(box != NULL);
   assert(pd != NULL);

   FRAME_PROPERTY_ADD(box, tween_frame, true, _("Tweens"), pd->scroller)
   elm_object_style_set(tween_frame, "outdent_top");

   item = elm_layout_add(tween_frame);
   elm_layout_theme_set(item, "layout", "property", "tween");
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0);
   evas_object_show(item);
   elm_object_content_set(tween_frame, item);

   tween_list = elm_gengrid_add(tween_frame);
   elm_gengrid_item_size_set(tween_list, 96, 111);
   elm_gengrid_align_set(tween_list, 0.0, 0.0);
   elm_gengrid_horizontal_set(tween_list, true);
   elm_gengrid_multi_select_set(tween_list, true);

   evas_object_data_set(tween_list, GROUP_PROP_DATA, pd);

   if (!_itc_tween)
     {
        _itc_tween = elm_gengrid_item_class_new();
        _itc_tween->item_style = "default";
        _itc_tween->func.text_get = _item_label_get;
        _itc_tween->func.content_get = _item_content_get;
        _itc_tween->func.state_get = NULL;
        _itc_tween->func.del = _item_del;
     }

   images_list = edje_edit_state_tweens_list_get(pd->group->edit_object,
                                                 pd->part->name,
                                                 pd->part->current_state->parsed_name,
                                                 pd->part->current_state->parsed_val);
   if (!images_list) elm_frame_collapse_go(tween_frame, true);
   EINA_LIST_FOREACH(images_list, l, image_name)
     {
        elm_gengrid_item_append(tween_list, _itc_tween,
                                eina_stringshare_add(image_name), NULL, NULL);
     }
   edje_edit_string_list_free(images_list);
   elm_object_content_set(item, tween_list);

   BUTTON_ADD(tween_frame, button, NULL)
   ICON_STANDARD_ADD(button, icon, true, "plus");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, "clicked", _add_tween_image,
                                  tween_list);
   elm_layout_content_set(item, "elm.swallow.add", button);

   BUTTON_ADD(tween_frame, button, NULL)
   ICON_STANDARD_ADD(button, icon, true, "minus");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, "clicked", _del_tween_image,
                                  tween_list);
   elm_layout_content_set(item, "elm.swallow.del", button);

   evas_object_show(tween_list);

   return tween_frame;
}

void
prop_item_state_image_tween_update(Evas_Object *tween, Group_Prop_Data *pd)
{
   Evas_Object *tween_list;
   Eina_List *images_list, *l;
   const char *image_name = NULL;

   assert(tween != NULL);
   assert(pd != NULL);

   /* Tween_List -> Item(Layout) -> Frame(Tween) */
   tween_list = elm_object_content_get(elm_object_content_get(tween));
   elm_gengrid_clear(tween_list);
   images_list = edje_edit_state_tweens_list_get(pd->group->edit_object,
                                                 pd->part->name,
                                                 pd->part->current_state->parsed_name,
                                                 pd->part->current_state->parsed_val);
   if (!images_list) elm_frame_collapse_go(tween, true);
   else elm_frame_collapse_go(tween, false);

   EINA_LIST_FOREACH(images_list, l, image_name)
     {
        elm_gengrid_item_append(tween_list, _itc_tween,
                                eina_stringshare_add(image_name), NULL, NULL);
     }
   edje_edit_string_list_free(images_list);
   evas_object_show(tween_list);
}

#define SPINNER_SET(SUB, VALUE, SPINNER, PART) \
   SPINNER_ADD(item, SPINNER, 0.0, 999.0, 1.0, true) \
   evas_object_smart_callback_add(SPINNER, "changed", \
                                  _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(SPINNER, "spinner,drag,start", \
                                  _on_##SUB##_##VALUE##_start, pd); \
   evas_object_smart_callback_add(SPINNER, "spinner,drag,stop", \
                                  _on_##SUB##_##VALUE##_stop, pd); \
   evas_object_event_callback_priority_add(SPINNER, EVAS_CALLBACK_MOUSE_WHEEL, \
                                        EVAS_CALLBACK_PRIORITY_BEFORE, \
                                        _on_spinner_mouse_wheel, NULL); \
   elm_layout_content_set(item, PART, SPINNER);

#define ATTR_4SPINNERS(TEXT, SUB, VALUE, MEMBER, TOOLTIP, ARGS, DESCRIPTION) \
static void \
prop_##SUB##_##VALUE##_update(Group_Prop_Data *pd) \
{ \
   assert(pd != NULL); \
   int l = 0, r = 0, t = 0, b = 0; \
   edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS ,&l, &r, &t, &b); \
   elm_spinner_value_set(pd->attributes.MEMBER.VALUE##_l, l); \
   elm_spinner_value_set(pd->attributes.MEMBER.VALUE##_r, r); \
   elm_spinner_value_set(pd->attributes.MEMBER.VALUE##_t, t); \
   elm_spinner_value_set(pd->attributes.MEMBER.VALUE##_b, b); \
} \
static void \
_on_##SUB##_##VALUE##_start(void *data, \
                            Evas_Object *obj __UNUSED__, \
                            void *event_info __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   assert(pd->change == NULL); \
   pd->change = change_add(NULL); \
   edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS, \
                                   &pd->old_int_val, &pd->old_int_val2, \
                                   &pd->old_int_val3, &pd->old_int_val4); \
} \
static void \
_on_##SUB##_##VALUE##_stop(void *data, \
                           Evas_Object *obj __UNUSED__, \
                           void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Eina_Stringshare *msg; \
   assert(pd->change != NULL); \
   int new_val, new_val2, new_val3, new_val4; \
   edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS, \
                                   &new_val, &new_val2, &new_val3, &new_val4); \
   if ((new_val != pd->old_int_val) || \
       (new_val2 != pd->old_int_val2) || \
       (new_val3 != pd->old_int_val3) || \
       (new_val4 != pd->old_int_val4)) \
     { \
        msg = eina_stringshare_printf(DESCRIPTION, new_val, new_val2, new_val3, new_val4); \
        change_description_set(pd->change, msg); \
        eina_stringshare_del(msg); \
        history_change_add(pd->group->history, pd->change); \
     } \
   else \
     change_free(pd->change); \
   pd->change = NULL; \
} \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj __UNUSED__, \
                             void *event_info __UNUSED__) \
{ \
   int l, r, t, b; \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   assert(pd != NULL); \
   l = (int)elm_spinner_value_get(pd->attributes.MEMBER.VALUE##_l); \
   r = (int)elm_spinner_value_get(pd->attributes.MEMBER.VALUE##_r); \
   t = (int)elm_spinner_value_get(pd->attributes.MEMBER.VALUE##_t); \
   b = (int)elm_spinner_value_get(pd->attributes.MEMBER.VALUE##_b); \
   if (pd->change) \
     { \
        if (!editor_##SUB##_##VALUE##_set(pd->group->edit_object, pd->change, true \
                                          ARGS, l, r, t, b)) \
          { \
             ERR("editor_"#SUB"_"#VALUE"_set failed"); \
             abort(); \
          } \
     } \
   else \
     { \
        _on_##SUB##_##VALUE##_start(pd, obj, event_info); \
        if (!editor_##SUB##_##VALUE##_set(pd->group->edit_object, pd->change, true \
                                          ARGS, l, r, t, b)) \
          { \
             ERR("editor_"#SUB"_"#VALUE"_set failed"); \
             abort(); \
          } \
        _on_##SUB##_##VALUE##_stop(pd, obj, event_info); \
     } \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
} \
static void \
prop_##SUB##_##VALUE##_add(Evas_Object *box, Group_Prop_Data *pd) \
{ \
   Evas_Object *item; \
   assert(box != NULL); \
   assert(pd != NULL); \
   LAYOUT_PROP_ADD(box, TEXT, "property", "2swallow"); \
   elm_object_part_text_set(item, "label.swallow1.start", "left:"); \
   SPINNER_SET(SUB, VALUE, pd->attributes.MEMBER.VALUE##_l, "swallow.content1") \
   elm_object_part_text_set(item, "label.swallow2.start", "right:"); \
   SPINNER_SET(SUB, VALUE, pd->attributes.MEMBER.VALUE##_r, "swallow.content2") \
   elm_box_pack_end(box, item); \
   LAYOUT_PROP_ADD(box, NULL, "property", "2swallow"); \
   elm_object_part_text_set(item, "label.swallow1.start", "top:"); \
   SPINNER_SET(SUB, VALUE, pd->attributes.MEMBER.VALUE##_t, "swallow.content1") \
   elm_object_part_text_set(item, "label.swallow2.start", "bottom:"); \
   SPINNER_SET(SUB, VALUE, pd->attributes.MEMBER.VALUE##_b, "swallow.content2") \
   elm_box_pack_end(box, item); \
   prop_##SUB##_##VALUE##_update(pd); \
}
ATTR_4SPINNERS(_("border"), state_image, border, state_image, NULL, STATE_ARGS,
               _("border changed to [%d %d %d %d]"))

STATE_ATTR_1ENTRY_CALLBACK(state, image, NULL, _("image changed to %s"))
static void
prop_state_image_update(Group_Prop_Data *pd)
{
   const char *value;
   value = edje_edit_state_image_get(pd->group->edit_object STATE_ARGS);
   if (!strcmp(value, EFLETE_DUMMY_IMAGE_NAME))
     {
        edje_edit_string_free(value);
        value = eina_stringshare_add(_("None"));
     }
   char *text = elm_entry_utf8_to_markup(value);
   if (strcmp(text, elm_entry_entry_get(pd->attributes.state_image.image)))
     elm_entry_entry_set(pd->attributes.state_image.image, text);
   edje_edit_string_free(value);
   free(text);
}
STATE_ATTR_1ENTRY_ADD("image", state, image, state_image, NULL, NULL)

STATE_ATTR_1COMBOBOX_LIST(_("border fill"), state_image, border_fill, state_image,\
                          edje_middle_type, NULL, unsigned char,
                          _("border fill changed to %s"))

static Eina_Bool
ui_property_state_image_set(Evas_Object *property)
{
   Evas_Object *item;
   Evas_Object *image_frame, *box, *prop_box;

   GROUP_PROP_DATA_GET()

   /* if previos selected part is IMAGE too, unpack it */
   ui_property_state_image_unset(property);
   prop_box = elm_object_content_get(pd->scroller);
   if (!pd_image.frame)
     {
        FRAME_PROPERTY_ADD(property, image_frame, true, _("Image"), pd->scroller)
        BOX_ADD(image_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(image_frame, box);

        if (pd_image.validator == NULL)
          pd_image.validator = elm_validator_regexp_new(IMAGE_BORDER_REGEX, NULL);

        item = prop_state_image_add(box, pd, _on_state_image_choose);
        elm_box_pack_end(box, item);
        prop_state_image_border_add(box, pd);
        item = prop_state_image_border_fill_add(box, pd);
        elm_box_pack_end(box, item);

        pd_image.tween = _prop_item_state_image_tween_add(box, pd);
        elm_box_pack_end(box, pd_image.tween);

        elm_box_pack_end(prop_box, image_frame);
        pd_image.frame = image_frame;
     }
   else
     {
        prop_state_image_update(pd);
        prop_state_image_border_update(pd);
        STATE_ATTR_1COMBOBOX_LIST_UPDATE(state_image, border_fill, state_image)
        prop_item_state_image_tween_update(pd_image.tween, pd);
        elm_box_pack_end(prop_box, pd_image.frame);
        evas_object_show(pd_image.frame);
     }

   return true;
}

static void
ui_property_state_image_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   GROUP_PROP_DATA_GET()
   /*
   if (pd_image.validator != NULL)
     {
        elm_validator_regexp_free(pd_image.validator);
        pd_image.validator = NULL;
     }
   */
   prop_box = elm_object_content_get(pd->scroller);
   elm_box_unpack(prop_box, pd_image.frame);
   evas_object_hide(pd_image.frame);
}
#undef pd_image

#define pd_fill pd->attributes.state_fill
STATE_ATTR_1COMBOBOX_LIST(_("type"), state_fill, type, state_fill, edje_fill_type,
                          _("Set the image fill type"), unsigned char,
                          _("fill type changed to %s"))
STATE_ATTR_1CHECK(_("smooth"), state_fill, smooth, state_fill,
                  _("On/off image smooth on scaling"),
                  _("visible changed to %s"))
STATE_ATTR_2SPINNER_ICON(_("align"), state_fill, origin_relative_x, origin_relative_y, state_fill,
                         -500, 500, 1, NULL, "x:", "%", "y:", "%",
                         _("Sets the starting point X coordinate relatively to displayed element's content"),
                         _("Sets the starting point Y coordinate relatively to displayed element's content"),
                         100, double,
                         _("fill align x changed from %.2f to %.2f"),
                         _("fill align y changed from %.2f to %.2f"))
TODO("Fix offset edje_edit API: use int instead of double param")
STATE_ATTR_2SPINNER_ICON(_("offset"), state_fill, origin_offset_x, origin_offset_y, state_fill,
                         -9999, 9999, 1, NULL, "x:", "px", "y:", "px",
                         _("Affects the starting point a fixed number of pixels along X axis"),
                         _("Affects the starting point a fixed number of pixels along Y axis"),
                         1, double,
                         _("fill offset x changed from %.2f to %.2f"),
                         _("fill offset y changed from %.2f to %.2f"))
STATE_ATTR_2SPINNER_ICON(_("align"), state_fill, size_relative_x, size_relative_y, state_fill,
                         -500, 500, 1, NULL, "x:", "%", "y:", "%",
                         _("Value that represent the percentual value of the original size of the element by X axis"),
                         _("Value that represent the percentual value of the original size of the element by Y axis."),
                         100, double,
                         _("fill size align x changed from %.2f to %.2f"),
                         _("fill size align y changed from %.2f to %.2f"))
TODO("Fix offset edje_edit API: use int instead of double param")
STATE_ATTR_2SPINNER_ICON(_("offset"), state_fill, size_offset_x, size_offset_y, state_fill,
                         -9999, 9999, 1, NULL, "x:", "px", "y:", "px",
                         _("Affects the size of the tile a fixed number of pixels along X axis"),
                         _("Affects the size of the tile a fixed number of pixels along Y axis"),
                         1, double,
                         _("fill size offset x changed from %.2f to %.2f"),
                         _("fill size offset y changed from %.2f to %.2f"))

static Eina_Bool
ui_property_state_fill_set(Evas_Object *property)
{
   Evas_Object *item, *icon;
   Evas_Object *box, *prop_box;

   GROUP_PROP_DATA_GET()

   /* if previos selected part is IMAGE or PROXY too, unpack it */
   ui_property_state_fill_unset(property);
   prop_box = elm_object_content_get(pd->scroller);
   if (!pd_fill.frame)
     {
        FRAME_PROPERTY_ADD(property, pd->attributes.state_fill.frame, true, _("Fill"), pd->scroller)
        BOX_ADD(pd->attributes.state_fill.frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd->attributes.state_fill.frame, box);

        item = prop_state_fill_type_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_fill_smooth_add(box, pd);
        elm_box_pack_end(box, item);

        /* origin subblock of fill block */
        item = elm_separator_add(pd->attributes.state_fill.frame);
        elm_separator_horizontal_set(item, true);
        elm_object_style_set(item, "icon");
        elm_layout_text_set(item, "eflete.text", _("Origin point"));
        IMAGE_ADD_NEW(item, icon, "icon", "start-point")
        elm_layout_content_set(item, "eflete.swallow.icon", icon);
        evas_object_show(item);
        elm_box_pack_end(box, item);

        item = prop_state_fill_origin_relative_x_origin_relative_y_add(box, pd);
        IMAGE_ADD_NEW(item, icon, "icon", "align");
        elm_layout_content_set(item, NULL, icon);
        elm_box_pack_end(box, item);

        item = prop_state_fill_origin_offset_x_origin_offset_y_add(box, pd);
        IMAGE_ADD_NEW(item, icon, "icon", "offset");
        elm_layout_content_set(item, NULL, icon);
        elm_box_pack_end(box, item);

        /* size subblock of fill block */
        item = elm_separator_add(pd->attributes.state_fill.frame);
        elm_separator_horizontal_set(item, true);
        elm_object_style_set(item, "icon");
        elm_object_part_text_set(item, "eflete.text", _("Size"));
        IMAGE_ADD_NEW(item, icon, "icon", "end-point")
        elm_layout_content_set(item, "eflete.swallow.icon", icon);
        evas_object_show(item);
        elm_box_pack_end(box, item);

        item = prop_state_fill_size_relative_x_size_relative_y_add(box, pd);
        IMAGE_ADD_NEW(item, icon, "icon", "align");
        elm_layout_content_set(item, NULL, icon);
        elm_box_pack_end(box, item);

        item = prop_state_fill_size_offset_x_size_offset_y_add(box, pd);
        IMAGE_ADD_NEW(item, icon, "icon", "offset");
        elm_layout_content_set(item, NULL, icon);
        elm_box_pack_end(box, item);

        elm_box_pack_end(prop_box, pd_fill.frame);
     }
   else
     {
        STATE_ATTR_1COMBOBOX_LIST_UPDATE(state_fill, type, state_fill);
        STATE_ATTR_1CHECK_UPDATE(state_fill, smooth, state_fill)
        STATE_ATTR_2SPINNER_UPDATE(state_fill, origin_relative_x, origin_relative_y, state_fill, double, 100)
        STATE_ATTR_2SPINNER_UPDATE(state_fill, origin_offset_x, origin_offset_y, state_fill, int, 1)
        STATE_ATTR_2SPINNER_UPDATE(state_fill, size_relative_x, size_relative_y, state_fill, double, 100)
        STATE_ATTR_2SPINNER_UPDATE(state_fill, size_offset_x, size_offset_y, state_fill, int, 1)

        elm_box_pack_end(prop_box, pd_fill.frame);
     }
   evas_object_show(pd_fill.frame);
   return true;
}

static void
ui_property_state_fill_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   GROUP_PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->scroller);
   elm_box_unpack(prop_box, pd_fill.frame);
   evas_object_hide(pd_fill.frame);
}
#undef pd_fill
ATTR_4SPINNERS(_("padding"), part_item, padding, part_item, NULL, PART_ITEM_ARGS,
               _("item padding changed to [%d %d %d %d]"))

#define pd_item pd->attributes.part_item

static void
prop_part_item_source_update(Group_Prop_Data *pd)
{
   Eina_List *collections, *l;
   const char *group, *value;

   assert(pd != NULL);

   unsigned int i = 0;
   ewe_combobox_items_list_free(pd->attributes.part_item.source, true);
   value = edje_edit_part_item_source_get(pd->group->edit_object, pd->part->name, pd->item_name);
   ewe_combobox_text_set(pd->attributes.part_item.source, value ? value : _("None"));

TODO("do not use collection lists from edje_edit directly")
   collections = edje_mmap_collection_list(ap.project->mmap_file);
   collections = eina_list_sort(collections, eina_list_count(collections), sort_cb);
   EINA_LIST_FOREACH(collections, l, group)
     {
        if (group != pd->group->name)
          ewe_combobox_item_add(pd->attributes.part_item.source, group);
        if (group == value)
          pd->attributes.part.previous_source = i;
        i++;
     }
   edje_edit_string_free(value);
   edje_mmap_collection_list_free(collections);
}

#define PART_ITEM_ATTR_1COMBOBOX(TEXT, SUB, VALUE, MEMBER, TOOLTIP, DESCRIPTION) \
   PART_ITEM_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   PART_ITEM_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

#define PART_ITEM_ATTR_2SPINNER(TEXT, SUB, VALUE1, VALUE2, MEMBER, MIN, MAX, STEP, FMT, \
                                L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER, \
                                TYPE, DESC1, DESC2) \
   PART_ITEM_ATTR_SPINNER_CALLBACK(SUB, VALUE1, MEMBER, TYPE, MULTIPLIER, DESC1) \
   PART_ITEM_ATTR_SPINNER_CALLBACK(SUB, VALUE2, MEMBER, TYPE, MULTIPLIER, DESC2) \
   PART_ITEM_ATTR_2SPINNER_ADD(TEXT, "2swallow", SUB, VALUE1, VALUE2, MEMBER, TYPE, MIN, \
                               MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                               TOOLTIP1, TOOLTIP2, MULTIPLIER)

#define PART_ITEM_ATTR_1COMBOBOX_LIST(TEXT, SUB, VALUE, MEMBER, TYPE, LIST, TOOLTIP, DESCRIPTION) \
   PART_ITEM_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, DESCRIPTION) \
   PART_ITEM_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP)

PART_ITEM_ATTR_1COMBOBOX(_("source"), part_item, source, part_item, _("Sets the group this object will be made from."),
                         _("changed source to %s"))
PART_ITEM_ATTR_2SPINNER(_("min"), part_item, min_w, min_h, part_item,
                    0.0, 999.0, 1.0, NULL, "x:", "%", "y:", "%",
                    _("Set the item minimum size hint width in pixels"), _("Set the item minimum size hint height in pixels"),
                    1, int,
                    _("part item min width changed from %d to %d"),
                    _("part item min heigth changed from %d to %d"))
PART_ITEM_ATTR_2SPINNER(_("max"), part_item, max_w, max_h, part_item,
                    0.0, 999.0, 1.0, NULL, "x:", "%", "y:", "%",
                    _("Set the item maximum size hint width in pixels"), _("Set the item maximum size hint height in pixels"),
                    1, int,
                    _("part item min width changed from %d to %d"),
                    _("part item min heigth changed from %d to %d"))
PART_ITEM_ATTR_2SPINNER(_("prefer"), part_item, prefer_w, prefer_h, part_item,
                    0.0, 999.0, 1.0, NULL, "w:", "px", "h:", "px",
                    _("Set the item prefered size hint width in pixels"), _("Set the item prefered size hint height in pixels"),
                    1, int,
                    _("part item prefer width changed from %d to %d"),
                    _("part item prefer heigth changed from %d to %d"))
PART_ITEM_ATTR_2SPINNER(_("align"), part_item, align_x, align_y, part_item,
                    0.0, 999.0, 1.0, NULL, "x:", "px", "y:", "px",
                    _("Sets the alignment hint by x axiss"), _("Sets the alignment hint by y axiss"),
                    100, double,
                    _("part item align x changed from %.2f to %.2f"),
                    _("part item align y changed from %.2f to %.2f"))
PART_ITEM_ATTR_2SPINNER(_("weight"), part_item, weight_x, weight_y, part_item,
                    0.0, 999.0, 1.0, NULL, "x:", "", "y:", "",
                    _("Sets the weight hint by x axiss"),_("Sets the weight hint by y axiss"),
                    1, int,
                    _("part item weight x changed from %d to %d"),
                    _("part item weight y changed from %d to %d"))
PART_ITEM_ATTR_2SPINNER(_("aspect"), part_item, aspect_w, aspect_h, part_item,
                    0.0, 999.0, 1.0, NULL, "w:", "", "h:", "",
                    _("Set the item aspect width hint"), _("Set the item aspect height hint"),
                    1, int,
                    _("part item aspect width changed from %d to %d"),
                    _("part item aspect height changed from %d to %d"))
PART_ITEM_ATTR_2SPINNER(_("spread"), part_item, spread_w, spread_h, part_item,
                    1.0, 255.0, 1.0, NULL, "colunm:", "", "row:", "",
                    _("Replicate the item in width, starting from the current position"),
                    _("Replicate the item in height, starting from the current position"),
                    1, int,
                    _("part item spread width changed from %d to %d"),
                    _("part item spread height changed from %d to %d"))
PART_ITEM_ATTR_1COMBOBOX_LIST(_("aspect mode"), part_item, aspect_mode, part_item, Edje_Aspect_Control, edje_item_aspect_pref,
                              _("Sets the aspect control hints for this object."),
                              _("aspect mode changed to %s"))
PART_ITEM_ATTR_2SPINNER(_("span_col"), part_item, span_col, span_row, part_item,
                        1.0, 999.0, 1.0, NULL, "column:", "", "row:", "",
                        _("Sets how many columns this item will use"), _("Sets how many rows this item will use"),
                        1, int,
                        _("part item span col changed from %d to %d"),
                        _("part item span row changed from %d to %d"))
PART_ITEM_ATTR_2SPINNER(_("position"), part_item, position_col, position_row, part_item,
                        0.0, 999.0, 1.0, NULL, "column:", "", "row:", "",
                        _("Sets the column position this item"), _("Sets the row position this item"),
                        1, int,
                        _("part item position col changed from %d to %d"),
                        _("part item position row changed from %d to %d"))

static void
_ui_property_part_item_set(Evas_Object *property, Part *part)
{
   Evas_Object *item;
   Evas_Object *box, *prop_box;

   GROUP_PROP_DATA_GET()

   _ui_property_part_item_unset(property);
   pd->item_name = part->current_item_name;
   prop_box = elm_object_content_get(pd->scroller);
   if (!pd_item.frame)
     {
        FRAME_ADD(property, pd_item.frame, true, _("Item"))
        BOX_ADD(pd_item.frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd_item.frame, box);

        item = prop_part_item_name_add(box, _("name"), pd->item_name);
        elm_box_pack_end(box, item);
        pd->attributes.part_item.source_item = prop_part_item_source_add(box, pd);
        elm_box_pack_end(box, pd->attributes.part_item.source_item);
        item = prop_part_item_min_w_min_h_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_item_max_w_max_h_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_item_prefer_w_prefer_h_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_item_align_x_align_y_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_item_weight_x_weight_y_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_item_aspect_mode_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_item_aspect_w_aspect_h_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_item_spread_w_spread_h_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_part_item_span_col_span_row_add(box, pd);
        elm_box_pack_end(box, item);
        prop_part_item_padding_add(box, pd);

        pd_item.position_item = prop_part_item_position_col_position_row_add(box, pd);
        evas_object_hide(pd_item.position_item);

        elm_box_pack_after(prop_box, pd_item.frame, pd->attributes.state_container.frame);
     }
   else
     {
        box = elm_object_content_get(pd->attributes.part_item.frame);
        prop_part_item_name_update(pd->item_name);
        prop_part_item_source_update(pd);
        PART_ITEM_ATTR_2SPINNER_UPDATE(part_item, min_w, min_h, part_item, int,1)
        PART_ITEM_ATTR_2SPINNER_UPDATE(part_item, max_w, max_h, part_item, int,1)
        PART_ITEM_ATTR_2SPINNER_UPDATE(part_item, prefer_w, prefer_h, part_item, int, 1)
        PART_ITEM_ATTR_2SPINNER_UPDATE(part_item, align_x, align_y, part_item, double, 100)
        PART_ITEM_ATTR_2SPINNER_UPDATE(part_item, weight_x, weight_y, part_item, int, 1)
        PART_ITEM_ATTR_1COMBOBOX_LIST_UPDATE(part_item, aspect_mode, part_item)
        PART_ITEM_ATTR_2SPINNER_UPDATE(part_item, aspect_w, aspect_h, part_item, int, 1)
        PART_ITEM_ATTR_2SPINNER_UPDATE(part_item, spread_w, spread_h, part_item, int, 1)
        PART_ITEM_ATTR_2SPINNER_UPDATE(part_item, span_col, span_row, part_item, unsigned char, 1);
        PART_ITEM_ATTR_2SPINNER_UPDATE(part_item, position_col, position_row, part_item, unsigned short, 1);
        prop_part_item_padding_update(pd);

        elm_box_pack_after(prop_box, pd_item.frame, pd->attributes.state_container.frame);
        evas_object_show(pd_item.frame);
     }

     if (pd->part->type == EDJE_PART_TYPE_TABLE)
       {
          if (!evas_object_visible_get(pd->attributes.part_item.position_item))
            {
               elm_box_pack_after(box, pd->attributes.part_item.position_item, pd->attributes.part_item.source_item);
               evas_object_show(pd->attributes.part_item.position_item);
            }
       }
     else
       {
          if (evas_object_visible_get(pd->attributes.part_item.position_item))
            {
               elm_box_unpack(box, pd->attributes.part_item.position_item);
               evas_object_hide(pd->attributes.part_item.position_item);
            }
       }
}

static void
_ui_property_part_item_unset(Evas_Object *property)
{
   Evas_Object *prop_box, *item_box;

   GROUP_PROP_DATA_GET()

   if (!pd_item.frame) return;

   pd->item_name = NULL;
   prop_box = elm_object_content_get(pd->scroller);
   elm_box_unpack(prop_box, pd_item.frame);

   item_box = elm_object_content_get(pd_item.frame);
   elm_box_unpack(item_box, pd_item.position_item);

   evas_object_hide(pd_item.frame);
   evas_object_hide(pd_item.position_item);
}
#undef pd_item

#define pd_container pd->attributes.state_container

STATE_ATTR_2SPINNER(_("align"), state_container, align_x, align_y, state_container,
                    0.0, 100.0, 1.0, NULL, "x:", "%", "y:", "%",
                    _("Change the position of the point of balance inside the container"),
                    _("Change the position of the point of balance inside the container"),
                    100, double,
                    _("Part container align x changed from %.2f to %.2f"),
                    _("Part container glign y changed from %.2f to %.2f"))
STATE_ATTR_2SPINNER(_("padding"), state_container, padding_x, padding_y, state_container,
                    0.0, 999.0, 1.0, NULL, "x:", "px", "y:", "px",
                    _("Sets the horizontal space between cells in pixels"),
                    _("Sets the vertcal space between cells in pixels"),
                    1, int,
                    _("Part container padding x changed from %d to %d"),
                    _("Part container padding y changed from %d to %d"))
STATE_ATTR_2CHECK(_("min"), state_container, min_v, min_h, state_container,
                  "x:", NULL, "y:", NULL,
                  _("This affects the minimum width calculation"),
                  _("This affects the minimum height calculation"),
                  _("container min_x changed to %s"),
                  _("container min_y changed to %s"))

static Eina_Bool
ui_property_state_container_set(Evas_Object *property)
{
   Evas_Object *item, *box, *prop_box;

   GROUP_PROP_DATA_GET()

   ui_property_state_container_unset(property);
   prop_box = elm_object_content_get(pd->scroller);
   if (!pd_container.frame)
     {
        FRAME_ADD(property, pd_container.frame, true, _("Container"))
        BOX_ADD(pd_container.frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd_container.frame, box);

        item = prop_state_container_align_x_align_y_add(box, pd);
        elm_box_pack_end(box, item);
        item = prop_state_container_padding_x_padding_y_add(box, pd);
        elm_box_pack_end(box, item);

        item = prop_state_container_min_v_min_h_add(box, pd);
        elm_box_pack_end(box, item);
        elm_box_pack_after(prop_box, pd_container.frame, pd->attributes.state_object_area.frame);
     }
   else
     {
        STATE_ATTR_2SPINNER_UPDATE(state_container, align_x, align_y, state_container, double, 100)
        STATE_ATTR_2SPINNER_UPDATE(state_container, padding_x, padding_y, state_container, int, 1)
        STATE_ATTR_2CHECK_UPDATE(state_container, min_v, min_h, state_container)
        elm_box_pack_after(prop_box, pd_container.frame, pd->attributes.state_object_area.frame);
        evas_object_show(pd_container.frame);
     }
   return true;
}
static void
ui_property_state_container_unset(Evas_Object *property)
{
   Evas_Object *prop_box;

   GROUP_PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->scroller);
   elm_box_unpack(prop_box, pd_container.frame);
   evas_object_hide(pd_container.frame);
}

#undef pd_container

#undef GROUP_PROP_DATA
#undef GROUP_PROP_DATA_GET
