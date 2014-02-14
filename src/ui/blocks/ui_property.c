/* Edje Theme Editor
 * Copyright (C) 2013 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

#include "ui_property.h"
#include "ui_property_macro.h"
#include "common_macro.h"
#include "image_editor.h"

#define PROP_DATA "prop_data"

#define PROP_DATA_GET(ret) \
   Prop_Data *pd = evas_object_data_get(property, PROP_DATA); \
   if (!pd) \
     { \
        ERR("Can not show group data, because object[%p] is not a property object", property); \
        return ret; \
     }

struct _Prop_Data
{
   Evas_Object *workspace;
   Style *style;
   Part *part;
   struct {
      Evas_Object *frame;
      Evas_Object *min;
      Evas_Object *max;
      Evas_Object *current;
   } prop_group;
   struct {
      Evas_Object *frame;
      Evas_Object *name;
      Evas_Object *type;
      Evas_Object *scale;
      Evas_Object *mouse;
      Evas_Object *repeat;
      Evas_Object *clip_to;
   } prop_part;
   struct {
      Evas_Object *frame;
      Evas_Object *drag_x;
      Evas_Object *drag_y;
      Evas_Object *confine;
      Evas_Object *event;
   } prop_part_drag;
   struct {
      Evas_Object *frame;
      Evas_Object *state;
      Evas_Object *visible;
      Evas_Object *min;
      Evas_Object *max;
      Evas_Object *fixed;
      Evas_Object *align;
      Evas_Object *aspect;
      Evas_Object *aspect_pref;
      Evas_Object *color_class;
      Evas_Object *color;
   } prop_state;
   struct {
      Evas_Object *frame;
      Evas_Object *relative;
      Evas_Object *offset;
      Evas_Object *to;
   } prop_state_rel1;
   struct {
      Evas_Object *frame;
      Evas_Object *relative;
      Evas_Object *offset;
      Evas_Object *to;
   } prop_state_rel2;
   struct {
      Evas_Object *frame;
      Evas_Object *text;
      Evas_Object *font;
      Evas_Object *size;
      Evas_Object *align;
      Evas_Object *elipsis;
      Evas_Object *min;
      Evas_Object *max;
      Evas_Object *fit;
      Evas_Object *text_source; /* not implemented in yet the edje */
      Evas_Object *color2;
      Evas_Object *color3;
   } prop_state_text;
   struct {
      Evas_Object *frame;
      Evas_Object *text;
      Evas_Object *style; /* not implemented in yet the edje  */
      Evas_Object *min;
      Evas_Object *max;
   } prop_state_textblock;
   struct {
      Evas_Object *frame;
      Evas_Object *normal;
      Evas_Object *border;
      Evas_Object *middle;
      Evas_Object *tween;
   } prop_state_image;
};
typedef struct _Prop_Data Prop_Data;

struct _Index
{
   int i;
};
typedef struct _Index Index;

static const char *edje_aspect_pref[] = { "None",
                                          "Vertical",
                                          "Horizontal",
                                          "Both",
                                          "Source" };
#define ASPECT_PREF_TYPE_COUNT 5

static Eina_Bool
ui_property_state_rel1_set(Evas_Object *property);

static void
ui_property_state_rel1_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_rel2_set(Evas_Object *property);

static void
ui_property_state_rel2_unset(Evas_Object *property);

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

static void
_del_prop_data(void *data,
               Evas *e __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *ei __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;
   free(pd);
}

static Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    const char *lab_text,
                    const char *text)
{
   Evas_Object *item, *label;
   ITEM_ADD(parent, item, lab_text)
   LABEL_ADD(parent, label, text)
   evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(item, "elm.swallow.content", label);
   return item;
}

static void
prop_item_label_update(Evas_Object *item,
                       const char *text)
{
   Evas_Object *label;
   label = elm_object_part_content_get(item, "elm.swallow.content");
   elm_object_text_set(label, text);
}

static void
_on_state_pref_pref_change(void *data,
                           Evas_Object *obj,
                           void *ei)
{
   Prop_Data *pd = (Prop_Data *)data;
   Index *index = elm_object_item_data_get((Elm_Object_Item *)ei);
   elm_object_text_set(obj, edje_aspect_pref[index->i]);
   if (!edje_edit_state_aspect_pref_set(pd->style->obj, pd->part->name,
                                        pd->part->curr_state, pd->part->curr_state_value,
                                        index->i))
     return;
   workspace_edit_object_recalc(pd->workspace);
   pd->style->isModify = true;
}

#define INDEX_APPEND(VALUE) \
   index = mem_malloc(sizeof(Index)); \
   index->i = VALUE; \
   elm_object_item_data_set(hovit, index);

static Evas_Object *
prop_item_state_aspect_pref_add(Evas_Object *parent,
                                Evas_Object *hoversel_parent,
                                Prop_Data *pd,
                                const char *tooltip)
{
   Evas_Object *item, *hoversel;
   Elm_Object_Item *hovit;
   unsigned char asp_pref;
   Index *index;
   int i = 0;
   ITEM_ADD(parent, item, "aspect pref")
   HOVERSEL_ADD(item, hoversel, false)
   elm_hoversel_hover_parent_set(hoversel, hoversel_parent);
   elm_object_tooltip_text_set(hoversel, tooltip);
   asp_pref = edje_edit_state_aspect_pref_get(pd->style->obj,
                                              pd->part->name,
                                              pd->part->curr_state,
                                              pd->part->curr_state_value);
   for (i = 0; i < ASPECT_PREF_TYPE_COUNT; i++)
     {
        hovit = elm_hoversel_item_add(hoversel, edje_aspect_pref[i], NULL,
                  ELM_ICON_NONE, NULL, NULL);
        INDEX_APPEND(i)
     }
   elm_object_text_set(hoversel, edje_aspect_pref[asp_pref]);
   evas_object_smart_callback_add(hoversel, "selected", _on_state_pref_pref_change, pd);
   elm_object_part_content_set(item, "elm.swallow.content", hoversel);
   return item;
}
#undef INDEX_APPEND

static void
prop_item_state_aspect_pref_update(Evas_Object *item,
                                   Prop_Data *pd)
{
   Evas_Object *hoversel;
   unsigned char asp_pref;
   hoversel = elm_object_part_content_get(item, "elm.swallow.content");
   asp_pref = edje_edit_state_aspect_pref_get(pd->style->obj,
                                              pd->part->name,
                                              pd->part->curr_state,
                                              pd->part->curr_state_value);
   elm_object_text_set(hoversel, edje_aspect_pref[asp_pref]);
}
#undef HOVERSEL_LABEL_SET

Evas_Object *
ui_property_add(Evas_Object *parent)
{
   Evas_Object *box, *scroller, *_bg;
   Prop_Data *pd;

   if (!parent) return NULL;
   pd = mem_calloc(1, sizeof(Prop_Data));
   SCROLLER_ADD(parent, scroller);
   BOX_ADD(scroller, box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);

   _bg = evas_object_image_add(evas_object_evas_get(parent));
   evas_object_image_filled_set(_bg, true);
   evas_object_image_file_set(_bg, EFLETE_IMG_PATH"section-item-bg.png", NULL);
   evas_object_image_border_set(_bg, 0, 0, 0, 2);
   elm_object_part_content_set(scroller, "elm.swallow.background", _bg);
   evas_object_show(_bg);

   evas_object_data_set(scroller, PROP_DATA, pd);
   evas_object_event_callback_add(scroller, EVAS_CALLBACK_DEL, _del_prop_data, pd);

   return scroller;
}

#define ITEM_2SPINNER_GROUP_CREATE(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_SPINNER_CALLBACK(SUB, VALUE1, int) \
   ITEM_SPINNER_CALLBACK(SUB, VALUE2, int) \
   ITEM_2SPINNER_GROUP_ADD(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_2SPINNER_GROUP_UPDATE(SUB, VALUE1, VALUE2)

/* group property */
ITEM_2SPINNER_GROUP_CREATE("min", group_min, w, h)
ITEM_2SPINNER_GROUP_CREATE("max", group_max, w, h)

#define pd_group pd->prop_group
Eina_Bool
ui_property_style_set(Evas_Object *property, Style *style, Evas_Object *workspace)
{
   Evas_Object *group_frame, *box, *prop_box;

   if ((!property) || (!workspace)) return EINA_FALSE;
   PROP_DATA_GET(EINA_FALSE)

   elm_scroller_policy_set(property, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);

   pd->workspace = workspace;
   if (style != workspace_edit_object_get(workspace))
     {
        ERR("Cann't set the style! Style [%p] not matched"
            " with editable group in workspace", style);
        return false;
     }
   pd->style = style;

   if (!pd_group.frame)
     {
        FRAME_ADD(property, group_frame, true, "Group property")
        BOX_ADD(group_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(group_frame, box);

        pd_group.min = prop_item_group_min_w_h_add(box, pd,
                          0.0, 9999.0, 1.0,
                          "Minimum group width in pixels.",
                          "Minimum group height in pixels.");
        pd_group.max = prop_item_group_max_w_h_add(box, pd,
                          -1.0, 9999.0, 1.0,
                          "Maximum group width in pixels.",
                          "Maximum group height in pixels.");
        elm_box_pack_end(box, pd_group.min);
        elm_box_pack_end(box, pd_group.max);

        prop_box = elm_object_content_get(property);
        elm_box_pack_start(prop_box, group_frame);
        pd_group.frame = group_frame;
     }
   else
     {
        prop_item_group_min_w_h_update(pd_group.min, pd);
        prop_item_group_max_w_h_update(pd_group.max, pd);
        evas_object_show(pd_group.frame);
     }

   return true;
}

void
ui_property_style_unset(Evas_Object *property)
{
   PROP_DATA_GET()
   evas_object_hide(pd_group.frame);
   ui_property_part_unset(property);
   elm_scroller_policy_set(property, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
}
#undef pd_group

#define ITEM_1CHECK_PART_CREATE(TEXT, SUB, VALUE) \
   ITEM_CHECK_PART_CALLBACK(SUB, VALUE) \
   ITEM_1CHEACK_PART_ADD(TEXT, SUB, VALUE) \
   ITEM_1CHEACK_PART_UPDATE(SUB, VALUE)

#define ITEM_1ENTRY_PART_CREATE(TEXT, SUB, VALUE) \
   ITEM_STRING_PART_CALLBACK(SUB, VALUE) \
   ITEM_1ENTRY_PART_ADD(TEXT, SUB, VALUE) \
   ITEM_1ENTRY_PART_UPDATE(SUB, VALUE)

#define ITEM_DRAG_PART_CREATE(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_CHECK_PART_CALLBACK(SUB, VALUE1) \
   ITEM_INT_PART_CALLBACK(SUB, VALUE2) \
   ITEM_DRAG_PART_ADD(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_DRAG_PART_UPDATE(SUB, VALUE1, VALUE2)

/* part property */
ITEM_1CHECK_PART_CREATE("scale", part, scale)
ITEM_1CHECK_PART_CREATE("mouse", part, mouse_events)
ITEM_1CHECK_PART_CREATE("repeat", part, repeat_events)
ITEM_1ENTRY_PART_CREATE("clip to", part, clip_to)

/* part drag property */
ITEM_DRAG_PART_CREATE("x", part_drag, x, step_x)
ITEM_DRAG_PART_CREATE("y", part_drag, y, step_y)
ITEM_1ENTRY_PART_CREATE("confine", part_drag, confine)
ITEM_1ENTRY_PART_CREATE("event", part_drag, event)

#define pd_part pd->prop_part
#define pd_part_drag pd->prop_part_drag
Eina_Bool
ui_property_part_set(Evas_Object *property, Part *part)
{
   Evas_Object *part_frame, *part_drag_frame, *box, *prop_box;
   Edje_Part_Type type;

   if ((!property) || (!part)) return EINA_FALSE;
   PROP_DATA_GET(EINA_FALSE)

   pd->part = part;

   type = edje_edit_part_type_get(pd->style->obj, part->name);
   prop_box = elm_object_content_get(property);

   elm_box_unpack(prop_box, pd->prop_part.frame);
   elm_box_unpack(prop_box, pd->prop_part_drag.frame);

   if (!pd_part.frame)
     {
        FRAME_ADD(property, part_frame, true, "Part property")
        BOX_ADD(part_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(part_frame, box);

        pd_part.name = prop_item_label_add(box, "name", part->name);
        pd_part.type = prop_item_label_add(box, "type", wm_part_type_get(type));
        pd_part.scale = prop_item_part_scale_add(box, pd,
                           "Specifies whether the part will scale "
                           "its size with an edje scaling factor.");
        pd_part.mouse = prop_item_part_mouse_events_add(box, pd,
                           "Enable mouse events in this part.");
        pd_part.repeat = prop_item_part_repeat_events_add(box, pd,
                            "Enable repeat mouse events to the parts below.");
        pd_part.clip_to = prop_item_part_clip_to_add(box, pd,
                             "Show only the area of part that coincides with "
                             "another part's container");
        elm_box_pack_end(box, pd_part.name);
        elm_box_pack_end(box, pd_part.type);
        elm_box_pack_end(box, pd_part.scale);
        elm_box_pack_end(box, pd_part.mouse);
        elm_box_pack_end(box, pd_part.repeat);
        elm_box_pack_end(box, pd_part.clip_to);

        elm_box_pack_after(prop_box, part_frame, pd->prop_group.frame);
        pd_part.frame = part_frame;
     }
   else
     {
         prop_item_label_update(pd_part.name, part->name);
         prop_item_label_update(pd_part.type, wm_part_type_get(type));
         prop_item_part_scale_update(pd_part.scale, pd);
         prop_item_part_mouse_events_update(pd_part.mouse, pd);
         prop_item_part_repeat_events_update(pd_part.repeat, pd);
         prop_item_part_clip_to_update(pd_part.clip_to, pd);
         elm_box_pack_after(prop_box, pd_part.frame, pd->prop_group.frame);
         evas_object_show(pd_part.frame);
     }
   if (!pd_part_drag.frame)
     {
        FRAME_ADD(property, part_drag_frame, true, "Part dragable property")
        BOX_ADD(part_drag_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(part_drag_frame, box);

        pd_part_drag.drag_x = prop_item_part_drag_x_step_x_add(box, pd,
                                 0.0, 9999.0, 1.0, "%.0f",
                                 "Enable/Disable draggin along X axis",
                                 "Set a drag step value");
        pd_part_drag.drag_y = prop_item_part_drag_y_step_y_add(box, pd,
                                 0.0, 9999.0, 1.0, "%.0f",
                                 "Enable/Disable draggin along Y axis",
                                 "Set a drag step value");
        pd_part_drag.confine = prop_item_part_drag_confine_add(box, pd,
                                  "Limits the movement of the dragged part to "
                                  "another part's container'");
        pd_part_drag.event = prop_item_part_drag_event_add(box, pd,
                                "It causes the part to forward the drag events "
                                "to another part, thus ignoring them for itself.");

        elm_box_pack_end(box, pd_part_drag.drag_x);
        elm_box_pack_end(box, pd_part_drag.drag_y);
        elm_box_pack_end(box, pd_part_drag.confine);
        elm_box_pack_end(box, pd_part_drag.event);

        elm_box_pack_after(prop_box, part_drag_frame, pd_part.frame);
        pd_part_drag.frame = part_drag_frame;
     }
   else
     {
        prop_item_part_drag_x_step_x_update(pd_part_drag.drag_x, pd);
        prop_item_part_drag_y_step_y_update(pd_part_drag.drag_y, pd);
        prop_item_part_drag_confine_update(pd_part_drag.confine, pd);
        prop_item_part_drag_event_update(pd_part_drag.event, pd);
        elm_box_pack_after(prop_box, pd_part_drag.frame, pd_part.frame);
        evas_object_show(pd_part_drag.frame);
     }

   return true;
}

void
ui_property_part_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   if (!property) return;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(property);
   evas_object_hide(pd->prop_part.frame);
   evas_object_hide(pd->prop_part_drag.frame);
   elm_box_unpack(prop_box, pd->prop_part.frame);
   elm_box_unpack(prop_box, pd->prop_part_drag.frame);

   ui_property_state_unset(property);
}
#undef pd_part
#undef pd_part_drag

#define ITEM_2SPINNER_STATE_CREATE(TEXT, SUB, VALUE1, VALUE2, type) \
   ITEM_SPINNER_STATE_CALLBACK(SUB, VALUE1, type) \
   ITEM_SPINNER_STATE_CALLBACK(SUB, VALUE2, type) \
   ITEM_2SPINNER_STATE_ADD(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_2SPINNER_STATE_UPDATE(SUB, VALUE1, VALUE2)

#define ITEM_1ENTRY_STATE_CREATE(TEXT, SUB, VALUE) \
   ITEM_STRING_STATE_CALLBACK(SUB, VALUE) \
   ITEM_1ENTRY_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_1ENTRY_STATE_UPDATE(SUB, VALUE)

#define ITEM_COLOR_STATE_CREATE(TEXT, SUB, VALUE) \
   ITEM_COLOR_STATE_CALLBACK(SUB, VALUE) \
   ITEM_COLOR_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_COLOR_STATE_UPDATE(SUB, VALUE)

#define ITEM_1CHECK_STATE_CREATE(TEXT, SUB, VALUE) \
   ITEM_CHECK_STATE_CALLBACK(SUB, VALUE) \
   ITEM_1CHEACK_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_1CHEACK_STATE_UPDATE(SUB, VALUE)

#define ITEM_2CHECK_STATE_CREATE(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_CHECK_STATE_CALLBACK(SUB, VALUE1) \
   ITEM_CHECK_STATE_CALLBACK(SUB, VALUE2) \
   ITEM_2CHEACK_STATE_ADD(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_2CHEACK_STATE_UPDATE(SUB, VALUE1, VALUE2)

ITEM_1CHECK_STATE_CREATE("visible", state, visible)
ITEM_2SPINNER_STATE_CREATE("min", state_min, w, h, int)
ITEM_2SPINNER_STATE_CREATE("max", state_max, w, h, int)
ITEM_2CHECK_STATE_CREATE("fixed", state_fixed, w, h)
ITEM_2SPINNER_STATE_CREATE("align", state_align, x, y, double)
ITEM_2SPINNER_STATE_CREATE("aspect", state_aspect, min, max, double)
ITEM_1ENTRY_STATE_CREATE("color class", state, color_class)
ITEM_COLOR_STATE_CREATE("color", state, color)

Eina_Bool
ui_property_state_set(Evas_Object *property, Part *part)
{
   Evas_Object *state_frame, *box, *prop_box;
   Edje_Part_Type type;
   char state[BUFF_MAX];

   if ((!property) || (!part)) return EINA_FALSE;
   PROP_DATA_GET(EINA_FALSE)

   if (pd->part != part) return EINA_FALSE; /* time for panic */
   #define pd_state pd->prop_state

   type = edje_edit_part_type_get(pd->style->obj, part->name);
   sprintf(state, "%s %1.2f", part->curr_state, part->curr_state_value);

   if (!pd_state.frame)
     {
        FRAME_ADD(property, state_frame, true, "State")
        BOX_ADD(state_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(state_frame, box);

        pd_state.state = prop_item_label_add(box, "state", state);
        pd_state.visible = prop_item_state_visible_add(box, pd,
                                                       "");
        pd_state.min = prop_item_state_min_w_h_add(box, pd,
                          0.0, 9999.0, 1.0, "%.0f",
                          "Minimum part width in pixels.",
                          "Minimum part height in pixels.");
        pd_state.max = prop_item_state_max_w_h_add(box, pd,
                          -1.0, 9999.0, 1.0, "%.0f",
                          "Maximum part width in pixels.",
                          "Maximum part height in pixels.");
        pd_state.fixed = prop_item_state_fixed_w_h_add(box, pd,
                           "This affects the minimum width calculation.",
                           "This affects the minimum height calculation.");
        pd_state.align = prop_item_state_align_x_y_add(box, pd,
                            0.0, 1.0, 0.1, "%1.2f",
                            "Part horizontal align: 0.0 = left  1.0 = right",
                            "Part vertical align: 0.0 = top  1.0 = bottom");
        pd_state.aspect = prop_item_state_aspect_min_max_add(box, pd,
                             0.0, 1.0, 0.1, "%1.2f",
                             "Normally width and height can be "
                             "resized to any values independently",
                             "Normally width and height can be "
                             "resized to any values independently");
        pd_state.aspect_pref = prop_item_state_aspect_pref_add(box, property, pd,
                                   "The aspect control hints for this object.");
        pd_state.color_class = prop_item_state_color_class_add(box, pd, NULL,
                                  "Current color class",
                                  NULL);
        pd_state.color = prop_item_state_color_add(box, pd,
                            "Part main color.");

        elm_box_pack_end(box, pd_state.state);
        elm_box_pack_end(box, pd_state.visible);
        elm_box_pack_end(box, pd_state.min);
        elm_box_pack_end(box, pd_state.max);
        elm_box_pack_end(box, pd_state.fixed);
        elm_box_pack_end(box, pd_state.align);
        elm_box_pack_end(box, pd_state.aspect);
        elm_box_pack_end(box, pd_state.aspect_pref);
        elm_box_pack_end(box, pd_state.color_class);
        if (type == EDJE_PART_TYPE_SPACER)
          {
             evas_object_hide(pd_state.color);
             elm_box_unpack(box, pd_state.color);
          }
        else elm_box_pack_end(box, pd_state.color);
        prop_box = elm_object_content_get(property);
        elm_box_pack_after(prop_box, state_frame, pd->prop_part_drag.frame);
        pd_state.frame = state_frame;
     }
   else
     {
        box = elm_object_content_get(pd_state.frame);
        /* unpack item for part color, because we don't know whether it is necessary */
        elm_box_unpack(box, pd_state.color);
        prop_item_label_update(pd_state.state, state);
        prop_item_state_visible_update(pd_state.visible, pd);
        prop_item_state_min_w_h_update(pd_state.min, pd);
        prop_item_state_max_w_h_update(pd_state.max, pd);
        prop_item_state_fixed_w_h_update(pd_state.fixed, pd);
        prop_item_state_align_x_y_update(pd_state.align, pd);
        prop_item_state_aspect_min_max_update(pd_state.aspect, pd);
        prop_item_state_aspect_pref_update(pd_state.aspect_pref, pd);
        prop_item_state_color_class_update(pd_state.color_class, pd);
        if (type != EDJE_PART_TYPE_SPACER)
          {
             prop_item_state_color_update(pd_state.color, pd);
             evas_object_show(pd_state.color);
             elm_box_pack_end(box, pd_state.color);
          }
        else evas_object_hide(pd_state.color);
        evas_object_show(pd_state.frame);
     }

   ui_property_state_rel1_set(property);
   ui_property_state_rel2_set(property);
   if (type == EDJE_PART_TYPE_TEXT) ui_property_state_text_set(property);
   else ui_property_state_text_unset(property);
   if (type == EDJE_PART_TYPE_TEXTBLOCK) ui_property_state_textblock_set(property);
   else ui_property_state_textblock_unset(property);
   if (type == EDJE_PART_TYPE_IMAGE) ui_property_state_image_set(property);
   else ui_property_state_image_unset(property);

   #undef pd_state
   return true;
}

void
ui_property_state_unset(Evas_Object *property)
{
   if (!property) return;
   PROP_DATA_GET()

   evas_object_hide(pd->prop_state.frame);

   ui_property_state_rel1_unset(property);
   ui_property_state_rel2_unset(property);
   ui_property_state_text_unset(property);
   ui_property_state_image_unset(property);
   ui_property_state_textblock_unset(property);
}

#define ITEM_2ENTRY_STATE_CREATE(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_STRING_STATE_CALLBACK(SUB, VALUE1) \
   ITEM_STRING_STATE_CALLBACK(SUB, VALUE2) \
   ITEM_2ENTRY_STATE_ADD(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_2ENTRY_STATE_UPDATE(SUB, VALUE1, VALUE2)

ITEM_2SPINNER_STATE_CREATE("relative", state_rel1_relative, x, y, double)
ITEM_2SPINNER_STATE_CREATE("offset", state_rel1_offset, x, y, int)
ITEM_2ENTRY_STATE_CREATE("to", state_rel1_to, x, y)

#define pd_rel1 pd->prop_state_rel1
static Eina_Bool
ui_property_state_rel1_set(Evas_Object *property)
{
   Evas_Object *rel1_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   if (!pd_rel1.frame)
     {
        FRAME_ADD(property, rel1_frame, true, "rel1: top-left")
        BOX_ADD(rel1_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(rel1_frame, box);

        pd_rel1.relative = prop_item_state_rel1_relative_x_y_add(box, pd,
                              -5.0, 5.0, 0.1, "%1.2f",
                              "Define the position of left-up corner of the part's container. "
                              "Moves a corner to a relative position inside the container "
                              "by X axis.",
                              "Define the position of left-up corner of the part's container. "
                              "Moves a corner to a relative position inside the container "
                              "by Y axis.");
        pd_rel1.offset = prop_item_state_rel1_offset_x_y_add(box, pd,
                            -9999.0, 9999.0, 1.0, "%.0f",
                            "Left offset from relative position in pixels",
                            "Top offset from relative position in pixels");
        pd_rel1.to = prop_item_state_rel1_to_x_y_add(box, pd,
                        "Causes a corner to be positioned relatively to the X axis of another "
                        "part. Setting to \"\" will un-set this value",
                        "Causes a corner to be positioned relatively to the Y axis of another "
                        "part. Setting to \"\" will un-set this value");

        elm_box_pack_end(box, pd_rel1.relative);
        elm_box_pack_end(box, pd_rel1.offset);
        elm_box_pack_end(box, pd_rel1.to);

        prop_box = elm_object_content_get(property);
        elm_box_pack_end(prop_box, rel1_frame);
        pd_rel1.frame = rel1_frame;
     }
   else
     {
        prop_item_state_rel1_relative_x_y_update(pd_rel1.relative, pd);
        prop_item_state_rel1_offset_x_y_update(pd_rel1.offset, pd);
        prop_item_state_rel1_to_x_y_update(pd_rel1.to, pd);
        evas_object_show(pd_rel1.frame);
     }

   return true;
}

static void
ui_property_state_rel1_unset(Evas_Object *property)
{
   PROP_DATA_GET()
   evas_object_hide(pd_rel1.frame);
}
#undef pd_rel1

ITEM_2SPINNER_STATE_CREATE("relative", state_rel2_relative, x, y, double)
ITEM_2SPINNER_STATE_CREATE("offset", state_rel2_offset, x, y, int)
ITEM_2ENTRY_STATE_CREATE("to", state_rel2_to, x, y)

#define pd_rel2 pd->prop_state_rel2
static Eina_Bool
ui_property_state_rel2_set(Evas_Object *property)
{
   Evas_Object *rel2_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   if (!pd_rel2.frame)
     {
        FRAME_ADD(property, rel2_frame, true, "rel2: bottom-right")
        BOX_ADD(rel2_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(rel2_frame, box);

        pd_rel2.relative = prop_item_state_rel2_relative_x_y_add(box, pd,
                              -5.0, 5.0, 0.1, "%1.2f",
                              "Define the position of right-down corner of the part's container. "
                              "Moves a corner to a relative position inside the container "
                              "by X axis.",
                              "Define the position of right-down corner of the part's container. "
                              "Moves a corner to a relative position inside the container "
                              "by Y axis.");
        pd_rel2.offset = prop_item_state_rel2_offset_x_y_add(box, pd,
                            -9999.0, 9999.0, 1.0, "%.0f",
                            "Left offset from relative position in pixels",
                            "Top offset from relative position in pixels");
        pd_rel2.to = prop_item_state_rel2_to_x_y_add(box, pd,
                        "Causes a corner to be positioned relatively to the X axis of another "
                        "part. Setting to \"\" will un-set this value",
                        "Causes a corner to be positioned relatively to the Y axis of another "
                        "part. Setting to \"\" will un-set this value");

        elm_box_pack_end(box, pd_rel2.relative);
        elm_box_pack_end(box, pd_rel2.offset);
        elm_box_pack_end(box, pd_rel2.to);

        prop_box = elm_object_content_get(property);
        elm_box_pack_end(prop_box, rel2_frame);
        pd_rel2.frame = rel2_frame;
     }
   else
     {
        prop_item_state_rel2_relative_x_y_update(pd_rel2.relative, pd);
        prop_item_state_rel2_offset_x_y_update(pd_rel2.offset, pd);
        prop_item_state_rel2_to_x_y_update(pd_rel2.to, pd);
        evas_object_show(pd_rel2.frame);
     }

   return true;
}

static void
ui_property_state_rel2_unset(Evas_Object *property)
{
   PROP_DATA_GET()
   evas_object_hide(pd_rel2.frame);
}
#undef pd_rel2

#define ITEM_1SPINNER_STATE_CREATE(TEXT, SUB, VALUE, type) \
   ITEM_SPINNER_STATE_CALLBACK(SUB, VALUE, type) \
   ITEM_1SPINNER_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_1SPINNER_STATE_UPDATE(SUB, VALUE)

ITEM_1ENTRY_STATE_CREATE("text", state, text)
ITEM_1ENTRY_STATE_CREATE("font", state, font)
ITEM_1SPINNER_STATE_CREATE("size", state_text, size, int)
ITEM_2SPINNER_STATE_CREATE("align", state_text_align, x, y, double)
ITEM_2CHECK_STATE_CREATE("max", state_text_max, x, y)
ITEM_2CHECK_STATE_CREATE("min", state_text_min, x, y)
ITEM_1SPINNER_STATE_CREATE("elipsis", state_text, elipsis, double)
ITEM_2CHECK_STATE_CREATE("fit", state_text_fit, x, y)
ITEM_COLOR_STATE_CREATE("shadow color", state, color2)
ITEM_COLOR_STATE_CREATE("outline color", state, color3)

#define pd_text pd->prop_state_text
static Eina_Bool
ui_property_state_text_set(Evas_Object *property)
{
   Evas_Object *text_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is TEXT too, unpack it */
   ui_property_state_text_unset(property);
   prop_box = elm_object_content_get(property);
   if (!pd_text.frame)
     {
         FRAME_ADD(property, text_frame, true, "Text")
         BOX_ADD(text_frame, box, EINA_FALSE, EINA_FALSE)
         elm_box_align_set(box, 0.5, 0.0);
         elm_object_content_set(text_frame, box);

         pd_text.text = prop_item_state_text_add(box, pd, NULL,
                           "Set the text of part.", NULL);
         pd_text.font = prop_item_state_font_add(box, pd, NULL,
                           "Change the text's font", NULL);
         pd_text.size = prop_item_state_text_size_add(box, pd,
                           0.0, 128.0, 1.0, "%.0f pt",
                           "Change text font's size.'");
         pd_text.align = prop_item_state_text_align_x_y_add(box, pd,
                            0.0, 1.0, 0.1, "%1.2f",
                            "Text horizontal align. "
                            "0.0 = left  1.0 = right",
                            "Text vertical align. "
                            "0.0 = top  1.0 = bottom");
         pd_text.min = prop_item_state_text_min_x_y_add(box, pd,
                           "When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text.",
                           "When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text.");
         pd_text.max = prop_item_state_text_max_x_y_add(box, pd,
                           "When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text.",
                           "When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text.");
         pd_text.elipsis = prop_item_state_text_elipsis_add(box, pd,
                              0.0, 1.0, 0.1, "%1.2f",
                              "Cut text if biggest then part's area"
                              "0.0 = fix the left side  1.0 = right side");
         pd_text.fit = prop_item_state_text_fit_x_y_add(box, pd,
                          "Resize the text for it to fit in it's container by X axis",
                          "Resize the text for it to fit in it's container by Y axis");
         pd_text.color2 = prop_item_state_color2_add(box, pd,
                             "Text shadow color.");
         pd_text.color3 = prop_item_state_color3_add(box, pd,
                             "Text outline color.");

         elm_box_pack_end(box, pd_text.text);
         elm_box_pack_end(box, pd_text.font);
         elm_box_pack_end(box, pd_text.size);
         elm_box_pack_end(box, pd_text.align);
         elm_box_pack_end(box, pd_text.min);
         elm_box_pack_end(box, pd_text.max);
         elm_box_pack_end(box, pd_text.elipsis);
         elm_box_pack_end(box, pd_text.fit);
         elm_box_pack_end(box, pd_text.color2);
         elm_box_pack_end(box, pd_text.color3);

         elm_box_pack_end(prop_box, text_frame);
         pd_text.frame = text_frame;
     }
   else
     {
        prop_item_state_text_update(pd_text.text, pd);
        prop_item_state_font_update(pd_text.font, pd);
        prop_item_state_text_size_update(pd_text.size, pd);
        prop_item_state_text_align_x_y_update(pd_text.align, pd);
        prop_item_state_text_min_x_y_update(pd_text.min, pd);
        prop_item_state_text_max_x_y_update(pd_text.max, pd);
        prop_item_state_text_elipsis_update(pd_text.elipsis, pd);
        prop_item_state_text_fit_x_y_update(pd_text.fit, pd);
        prop_item_state_color2_update(pd_text.color2, pd);
        prop_item_state_color3_update(pd_text.color3, pd);
        elm_box_pack_end(prop_box, pd_text.frame);
        evas_object_show(pd_text.frame);
     }

   return true;
}

static void
ui_property_state_text_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(property);
   elm_box_unpack(prop_box, pd_text.frame);
   evas_object_hide(pd_text.frame);
}
#undef pd_text

#define pd_textblock pd->prop_state_textblock
static Eina_Bool
ui_property_state_textblock_set(Evas_Object *property)
{
   Evas_Object *textblock_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is TEXTBLOCK too, unpack it */
   ui_property_state_textblock_unset(property);
   prop_box = elm_object_content_get(property);
   if (!pd_textblock.frame)
     {
         FRAME_ADD(property, textblock_frame, true, "TextBlock")
         BOX_ADD(textblock_frame, box, EINA_FALSE, EINA_FALSE)
         elm_box_align_set(box, 0.5, 0.0);
         elm_object_content_set(textblock_frame, box);

         pd_textblock.text = prop_item_state_text_add(box, pd, NULL,
                           "Set the text of part.", NULL);
         pd_textblock.min = prop_item_state_text_min_x_y_add(box, pd,
                           "When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text.",
                           "When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text.");
         pd_textblock.max = prop_item_state_text_max_x_y_add(box, pd,
                           "When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text.",
                           "When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text.");

         elm_box_pack_end(box, pd_textblock.text);
         elm_box_pack_end(box, pd_textblock.min);
         elm_box_pack_end(box, pd_textblock.max);

         elm_box_pack_end(prop_box, textblock_frame);
         pd_textblock.frame = textblock_frame;
     }
   else
     {
        prop_item_state_text_update(pd_textblock.text, pd);
        prop_item_state_text_min_x_y_update(pd_textblock.min, pd);
        prop_item_state_text_max_x_y_update(pd_textblock.max, pd);
        elm_box_pack_end(prop_box, pd_textblock.frame);
        evas_object_show(pd_textblock.frame);
     }
   return true;
}

static void
ui_property_state_textblock_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(property);
   elm_box_unpack(prop_box, pd_textblock.frame);
   evas_object_hide(pd_textblock.frame);
}
#undef pd_textblock


#define ITEM_IM_BORDER_STATE_CREATE(TEXT, SUB, VALUE) \
   ITEM_IM_BORDER_STATE_CALLBACK(SUB, VALUE) \
   ITEM_IM_BORDER_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_IM_BORDER_STATE_UPDATE(SUB, VALUE)

#define pd_image pd->prop_state_image

static void
_on_image_editor_done(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *ei)
{
   Evas_Object *entry = (Evas_Object *)data;
   const char *value = elm_entry_entry_get(entry);
   const char *selected = (const char *)ei;
   if (!selected) return;
   if (strcmp(value, selected) == 0) return;
   elm_entry_entry_set(entry, selected);
   evas_object_smart_callback_call(entry, "activated", NULL);
}

static void
_on_state_image_choose(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   Evas_Object *img_edit;
   Evas_Object *entry = (Evas_Object *)data;
   const char *selected = elm_entry_entry_get(entry);

   App_Data *ap = app_create();

   img_edit = image_editor_window_add(ap->project, SINGLE);
   image_editor_file_choose(img_edit, selected);
   image_editor_callback_add(img_edit, _on_image_editor_done, entry);
}

ITEM_1ENTRY_STATE_CREATE("image", state, image)
ITEM_IM_BORDER_STATE_CREATE("border", state_image, border)

static Eina_Bool
ui_property_state_image_set(Evas_Object *property)
{
   Evas_Object *image_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is IMAGE too, unpack it */
   ui_property_state_image_unset(property);
   prop_box = elm_object_content_get(property);
   if (!pd_image.frame)
     {
        FRAME_ADD(property, image_frame, true, "Image")
        BOX_ADD(image_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(image_frame, box);

        pd_image.normal = prop_item_state_image_add(box, pd, _on_state_image_choose,
                             "Current image name",
                             "Change image");
        pd_image.border = prop_item_state_image_border_add(box, pd,
                             "Image's border value");

        elm_box_pack_end(box, pd_image.normal);
        elm_box_pack_end(box, pd_image.border);

        elm_box_pack_end(prop_box, image_frame);
        pd_image.frame = image_frame;
     }
   else
     {
        prop_item_state_image_update(pd_image.normal, pd);
        prop_item_state_image_border_update(pd_image.border, pd);
        elm_box_pack_end(prop_box, pd_image.frame);
        evas_object_show(pd_image.frame);
     }

   return true;
}

static void
ui_property_state_image_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(property);
   elm_box_unpack(prop_box, pd_image.frame);
   evas_object_hide(pd_image.frame);
}
#undef pd_image

#undef PROP_DATA
#undef PROP_DATA_GET
