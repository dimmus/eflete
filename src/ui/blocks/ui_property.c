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
 * along with this program; If not, see .
 */

#include "ui_property.h"
#include "ui_property_define.h"
#include "widget_define.h"
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
   Group *group; /* for registation/unreg callbacks*/
   Part *part; /* for registation/unreg callbacks*/
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
      Evas_Object *align;
      Evas_Object *aspect;
      Evas_Object *aspect_pref; /* not implemented yet in the edje*/
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
      Evas_Object *min; /* not implemented in yet the edje*/
      Evas_Object *max; /* not implemented in yet the edje*/
      Evas_Object *fit;
      Evas_Object *style; /* not implemented in yet the edje*/
      Evas_Object *source; /* not implemented in yet the edje*/
      Evas_Object *color2;
      Evas_Object *color3;
   } prop_state_text;
   struct {
      Evas_Object *frame;
      Evas_Object *normal;
      Evas_Object *border;
      Evas_Object *middle;
      Evas_Object *tween;
   } prop_state_image;
};

typedef struct _Prop_Data Prop_Data;

static Eina_Bool
ui_property_state_rel1_set(Evas_Object *property, Part *part);

static void
ui_property_state_rel1_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_rel2_set(Evas_Object *property, Part *part);

static void
ui_property_state_rel2_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_text_set(Evas_Object *property, Part *part);

static void
ui_property_state_text_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_image_set(Evas_Object *property, Part *part);

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

Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    const char *lab_text,
                    const char *text)
{
   Evas_Object *item, *label;
   ITEM_ADD(parent, item, lab_text)
   LABEL_ADD(parent, label, text)
   elm_object_part_content_set(item, "elm.swallow.content", label);
   return item;
}

void
prop_item_label_update(Evas_Object *item,
                       const char *text)
{
   Evas_Object *label;
   label = elm_object_part_content_get(item, "elm.swallow.content");
   elm_object_text_set(label, text);
}

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
   evas_object_image_filled_set(_bg, EINA_TRUE);
   evas_object_image_file_set(_bg, TET_IMG_PATH"section-item-bg.png", NULL);
   evas_object_image_border_set(_bg, 0, 0, 0, 2);
   elm_object_part_content_set(scroller, "elm.swallow.background", _bg);
   evas_object_show(_bg);

   evas_object_data_set(scroller, PROP_DATA, pd);
   evas_object_event_callback_add(scroller, EVAS_CALLBACK_DEL, _del_prop_data, pd);

   return scroller;
}

#define ITEM_2SPINNER_GROUP_CREATE(text, sub, value1, value2) \
   ITEM_SPINNER_CALLBACK(sub, value1, int) \
   ITEM_SPINNER_CALLBACK(sub, value2, int) \
   ITEM_2SPINNER_GROUP_ADD(text, sub, value1, value2) \
   ITEM_2SPINNER_GROUP_UPDATE(sub, value1, value2)

/* group property */
ITEM_2SPINNER_GROUP_CREATE("min", group_min, w, h)
ITEM_2SPINNER_GROUP_CREATE("max", group_max, w, h)

#define pd_group pd->prop_group
Eina_Bool
ui_property_group_set(Evas_Object *property, Group *group)
{
   Evas_Object *group_frame, *box, *prop_box;

   if ((!property) || (!group)) return EINA_FALSE;
   PROP_DATA_GET(EINA_FALSE)

   pd->group = group;

   if (!pd_group.frame)
     {
        FRAME_ADD(property, group_frame, EINA_TRUE, "Group property")
        BOX_ADD(group_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(group_frame, box);

        pd_group.min = prop_item_group_min_w_h_add(box, group,
                          0.0, 9999.0, 1.0,
                          "Minimum group width in pixels.",
                          "Minimum group height in pixels.");
        pd_group.max = prop_item_group_max_w_h_add(box, group,
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
        prop_item_group_min_w_h_update(pd_group.min, group);
        prop_item_group_max_w_h_update(pd_group.max, group);
        evas_object_show(pd_group.frame);
     }

   return EINA_TRUE;
}

void
ui_property_group_unset(Evas_Object *property)
{
   PROP_DATA_GET()
   evas_object_hide(pd_group.frame);
   ui_property_part_unset(property);
}
#undef pd_group

#define ITEM_1CHECK_PART_CREATE(text, sub, value) \
   ITEM_CHECK_PART_CALLBACK(sub, value) \
   ITEM_1CHEACK_PART_ADD(text, sub, value) \
   ITEM_1CHEACK_PART_UPDATE(sub, value)

#define ITEM_1ENTRY_PART_CREATE(text, sub, value) \
   ITEM_STRING_PART_CALLBACK(sub, value) \
   ITEM_1ENTRY_PART_ADD(text, sub, value) \
   ITEM_1ENTRY_PART_UPDATE(sub, value)

#define ITEM_DRAG_PART_CREATE(text, sub, value1, value2) \
   ITEM_CHECK_PART_CALLBACK(sub, value1) \
   ITEM_INT_PART_CALLBACK(sub, value2) \
   ITEM_DRAG_PART_ADD(text, sub, value1, value2) \
   ITEM_DRAG_PART_UPDATE(sub, value1, value2)

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

   type = edje_edit_part_type_get(pd->group->obj, part->name);
   prop_box = elm_object_content_get(property);

   if (!pd_part.frame)
     {
        FRAME_ADD(property, part_frame, EINA_TRUE, "Part property")
        BOX_ADD(part_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(part_frame, box);

        pd_part.name = prop_item_label_add(box, "name", part->name);
        pd_part.type = prop_item_label_add(box, "type", wm_part_type_get(type));
        pd_part.scale = prop_item_part_scale_add(box, pd->group, part,
                           "Specifies whether the part will scale "
                           "its size with an edje scaling factor.");
        pd_part.mouse = prop_item_part_mouse_events_add(box, pd->group, part,
                           "Enable mouse events in this part.");
        pd_part.repeat = prop_item_part_repeat_events_add(box, pd->group, part,
                            "Enable repeat mouse events to the parts below.");
        pd_part.clip_to = prop_item_part_clip_to_add(box, pd->group, part,
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
         prop_item_part_scale_update(pd_part.scale, pd->group, part);
         prop_item_part_mouse_events_update(pd_part.mouse, pd->group, part);
         prop_item_part_repeat_events_update(pd_part.repeat, pd->group, part);
         prop_item_part_clip_to_update(pd_part.clip_to, pd->group, part);
         evas_object_show(pd_part.frame);
     }
   if (!pd_part_drag.frame)
     {
        FRAME_ADD(property, part_drag_frame, EINA_TRUE, "Part dragable property")
        BOX_ADD(part_drag_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(part_drag_frame, box);

        pd_part_drag.drag_x = prop_item_part_drag_x_step_x_add(box, pd->group, part,
                                 0.0, 9999.0, 1.0, "%.0f",
                                 "Enable/Disable draggin along X axis",
                                 "Set a drag step value");
        pd_part_drag.drag_y = prop_item_part_drag_y_step_y_add(box, pd->group, part,
                                 0.0, 9999.0, 1.0, "%.0f",
                                 "Enable/Disable draggin along Y axis",
                                 "Set a drag step value");
        pd_part_drag.confine = prop_item_part_drag_confine_add(box, pd->group, part,
                                  "Limits the movement of the dragged part to "
                                  "another part's container'");
        pd_part_drag.event = prop_item_part_drag_event_add(box, pd->group, part,
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
        prop_item_part_drag_x_step_x_update(pd_part_drag.drag_x, pd->group, part);
        prop_item_part_drag_y_step_y_update(pd_part_drag.drag_y, pd->group, part);
        prop_item_part_drag_confine_update(pd_part_drag.confine, pd->group, part);
        prop_item_part_drag_event_update(pd_part_drag.confine, pd->group, part);
        evas_object_show(pd_part_drag.frame);
     }

   return EINA_TRUE;
}

void
ui_property_part_unset(Evas_Object *property)
{
   if (!property) return;
   PROP_DATA_GET()

   evas_object_hide(pd->prop_part.frame);
   evas_object_hide(pd->prop_part_drag.frame);

   ui_property_state_unset(property);
}
#undef pd_part
#undef pd_part_drag

/*   miss:
     Evas_Object *aspect_pref;
 */
#define ITEM_2SPINNER_STATE_CREATE(text, sub, value1, value2, type) \
   ITEM_SPINNER_STATE_CALLBACK(sub, value1, type) \
   ITEM_SPINNER_STATE_CALLBACK(sub, value2, type) \
   ITEM_2SPINNER_STATE_ADD(text, sub, value1, value2) \
   ITEM_2SPINNER_STATE_UPDATE(sub, value1, value2)

#define ITEM_1ENTRY_STATE_CREATE(text, sub, value, func) \
   ITEM_STRING_STATE_CALLBACK(sub, value) \
   ITEM_1ENTRY_STATE_ADD(text, sub, value, func) \
   ITEM_1ENTRY_STATE_UPDATE(sub, value)

#define ITEM_COLOR_STATE_CREATE(text, sub, value) \
   ITEM_COLOR_STATE_CALLBACK(sub, value) \
   ITEM_COLOR_STATE_ADD(text, sub, value) \
   ITEM_COLOR_STATE_UPDATE(sub, value)

#define ITEM_1CHECK_STATE_CREATE(text, sub, value) \
   ITEM_CHECK_STATE_CALLBACK(sub, value) \
   ITEM_1CHEACK_STATE_ADD(text, sub, value) \
   ITEM_1CHEACK_STATE_UPDATE(sub, value)

ITEM_1CHECK_STATE_CREATE("visible", state, visible)
ITEM_2SPINNER_STATE_CREATE("min", state_min, w, h, int)
ITEM_2SPINNER_STATE_CREATE("max", state_max, w, h, int)
ITEM_2SPINNER_STATE_CREATE("align", state_align, x, y, double)
ITEM_2SPINNER_STATE_CREATE("acpect", state_aspect, min, max, double)
ITEM_1ENTRY_STATE_CREATE("color class", state, color_class, NULL)
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

   type = edje_edit_part_type_get(pd->group->obj, part->name);
   sprintf(state, "%s %1.1f", part->curr_state, part->curr_state_value);

   if (!pd_state.frame)
     {
        FRAME_ADD(property, state_frame, EINA_TRUE, "State")
        BOX_ADD(state_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(state_frame, box);

        pd_state.state = prop_item_label_add(box, "state", state);
        pd_state.visible = prop_item_state_visible_add(box, pd->group, part,
                                                       "");
        pd_state.min = prop_item_state_min_w_h_add(box, pd->group, part,
                          0.0, 9999.0, 1.0, "%.0f",
                          "Minimum part width in pixels.",
                          "Minimum part height in pixels.");
        pd_state.max = prop_item_state_max_w_h_add(box, pd->group, part,
                          -1.0, 9999.0, 1.0, "%.0f",
                          "Maximum part width in pixels.",
                          "Maximum part height in pixels.");
        pd_state.align = prop_item_state_align_x_y_add(box, pd->group, part,
                            0.0, 1.0, 0.1, "%1.2f",
                            "Part horizontal align: 0.0 = left  1.0 = right",
                            "Part vertical align: 0.0 = top  1.0 = bottom");
        pd_state.aspect = prop_item_state_aspect_min_max_add(box, pd->group, part,
                             0.0, 1.0, 0.1, "%1.2f",
                             "Normally width and height can be "
                             "resized to any values independently",
                             "Normally width and height can be "
                             "resized to any values independently");
        /* pd_state.aspect_pref = */
        pd_state.color_class = prop_item_state_color_class_add(box, pd->group, part,
                                  "Current color class",
                                  "Choose another color class");
        pd_state.color = prop_item_state_color_add(box, pd->group, part,
                            "Part main color.");

        elm_box_pack_end(box, pd_state.state);
        elm_box_pack_end(box, pd_state.visible);
        elm_box_pack_end(box, pd_state.min);
        elm_box_pack_end(box, pd_state.max);
        elm_box_pack_end(box, pd_state.align);
        elm_box_pack_end(box, pd_state.aspect);
        /* elm_box_pack_end(box, pd_state.aspect_pref); */
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
        prop_item_state_visible_update(pd_state.visible, pd->group, part);
        prop_item_state_min_w_h_update(pd_state.min, pd->group, part);
        prop_item_state_max_w_h_update(pd_state.max, pd->group, part);
        prop_item_state_align_x_y_update(pd_state.align, pd->group, part);
        prop_item_state_aspect_min_max_update(pd_state.aspect, pd->group, part);
        /* prop_item_state_aspect_pref_update */
        prop_item_state_color_class_update(pd_state.color_class, pd->group, part);
        if (type != EDJE_PART_TYPE_SPACER)
          {
             prop_item_state_color_update(pd_state.color, pd->group, part);
             evas_object_show(pd_state.color);
             elm_box_pack_end(box, pd_state.color);
          }
        else evas_object_hide(pd_state.color);
        evas_object_show(pd_state.frame);
     }

   ui_property_state_rel1_set(property, part);
   ui_property_state_rel2_set(property, part);
   if (type == EDJE_PART_TYPE_TEXT) ui_property_state_text_set(property, part);
   else ui_property_state_text_unset(property);
   if (type == EDJE_PART_TYPE_IMAGE) ui_property_state_image_set(property, part);
   else ui_property_state_image_unset(property);

   #undef pd_state
   return EINA_TRUE;
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
}

#define ITEM_2ENTRY_STATE_CREATE(text, sub, value1, value2) \
   ITEM_STRING_STATE_CALLBACK(sub, value1) \
   ITEM_STRING_STATE_CALLBACK(sub, value2) \
   ITEM_2ENTRY_STATE_ADD(text, sub, value1, value2) \
   ITEM_2ENTRY_STATE_UPDATE(sub, value1, value2)

ITEM_2SPINNER_STATE_CREATE("relative", state_rel1_relative, x, y, double)
ITEM_2SPINNER_STATE_CREATE("offset", state_rel1_offset, x, y, int)
ITEM_2ENTRY_STATE_CREATE("to", state_rel1_to, x, y)

#define pd_rel1 pd->prop_state_rel1
static Eina_Bool
ui_property_state_rel1_set(Evas_Object *property, Part *part)
{
   Evas_Object *rel1_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   if (!pd_rel1.frame)
     {
        FRAME_ADD(property, rel1_frame, EINA_TRUE, "rel1: top-left")
        BOX_ADD(rel1_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(rel1_frame, box);

        pd_rel1.relative = prop_item_state_rel1_relative_x_y_add(box, pd->group, part,
                              -5.0, 5.0, 0.1, "%1.2f",
                              "Relative 1 x",
                              "Relative 1 y");
        pd_rel1.offset = prop_item_state_rel1_offset_x_y_add(box, pd->group, part,
                            -9999.0, 9999.0, 1.0, "%.0f",
                            "Left offset from relative position in pixels",
                            "Top offset from relative position in pixels");
        pd_rel1.to = prop_item_state_rel1_to_x_y_add(box, pd->group, part,
                        "Left reference part.",
                        "Top reference part.");

        elm_box_pack_end(box, pd_rel1.relative);
        elm_box_pack_end(box, pd_rel1.offset);
        elm_box_pack_end(box, pd_rel1.to);

        prop_box = elm_object_content_get(property);
        elm_box_pack_end(prop_box, rel1_frame);
        pd_rel1.frame = rel1_frame;
     }
   else
     {
        prop_item_state_rel1_relative_x_y_update(pd_rel1.relative, pd->group, part);
        prop_item_state_rel1_offset_x_y_update(pd_rel1.offset, pd->group, part);
        prop_item_state_rel1_to_x_y_update(pd_rel1.to, pd->group, part);
        evas_object_show(pd_rel1.frame);
     }

   return EINA_TRUE;
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
ui_property_state_rel2_set(Evas_Object *property, Part *part)
{
   Evas_Object *rel2_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   if (!pd_rel2.frame)
     {
        FRAME_ADD(property, rel2_frame, EINA_TRUE, "rel2: bottom-right")
        BOX_ADD(rel2_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(rel2_frame, box);

        pd_rel2.relative = prop_item_state_rel2_relative_x_y_add(box, pd->group, part,
                              -5.0, 5.0, 0.1, "%1.2f",
                              "Relative 2 x",
                              "Relative 2 y");
        pd_rel2.offset = prop_item_state_rel2_offset_x_y_add(box, pd->group, part,
                            -9999.0, 9999.0, 1.0, "%.0f",
                            "Left offset from relative position in pixels",
                            "Top offset from relative position in pixels");
        pd_rel2.to = prop_item_state_rel2_to_x_y_add(box, pd->group, part,
                        "Right reference part.",
                        "Bottom reference part.");

        elm_box_pack_end(box, pd_rel2.relative);
        elm_box_pack_end(box, pd_rel2.offset);
        elm_box_pack_end(box, pd_rel2.to);

        prop_box = elm_object_content_get(property);
        elm_box_pack_end(prop_box, rel2_frame);
        pd_rel2.frame = rel2_frame;
     }
   else
     {
        prop_item_state_rel2_relative_x_y_update(pd_rel2.relative, pd->group, part);
        prop_item_state_rel2_offset_x_y_update(pd_rel2.offset, pd->group, part);
        prop_item_state_rel2_to_x_y_update(pd_rel2.to, pd->group, part);
        evas_object_show(pd_rel2.frame);
     }

   return EINA_TRUE;
}

static void
ui_property_state_rel2_unset(Evas_Object *property)
{
   PROP_DATA_GET()
   evas_object_hide(pd_rel2.frame);
}
#undef pd_rel2

#define ITEM_1SPINNER_STATE_CREATE(text, sub, value, type) \
   ITEM_SPINNER_STATE_CALLBACK(sub, value, type) \
   ITEM_1SPINNER_STATE_ADD(text, sub, value) \
   ITEM_1SPINNER_STATE_UPDATE(sub, value)

#define ITEM_2CHECK_STATE_CREATE(text, sub, value1, value2) \
   ITEM_CHECK_STATE_CALLBACK(sub, value1) \
   ITEM_CHECK_STATE_CALLBACK(sub, value2) \
   ITEM_2CHEACK_STATE_ADD(text, sub, value1, value2) \
   ITEM_2CHEACK_STATE_UPDATE(sub, value1, value2)

ITEM_1ENTRY_STATE_CREATE("text", state, text, NULL)
ITEM_1ENTRY_STATE_CREATE("font", state, font, NULL)
ITEM_1SPINNER_STATE_CREATE("size", state_text, size, int)
ITEM_2SPINNER_STATE_CREATE("align", state_text_align, x, y, double)
ITEM_1SPINNER_STATE_CREATE("elipsis", state_text, elipsis, double)
ITEM_2CHECK_STATE_CREATE("fit", state_text_fit, x, y)
ITEM_COLOR_STATE_CREATE("shadow color", state, color2)
ITEM_COLOR_STATE_CREATE("outline color", state, color3)

#define pd_text pd->prop_state_text
static Eina_Bool
ui_property_state_text_set(Evas_Object *property, Part *part)
{
   Evas_Object *text_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is TEXT too, unpack it */
   ui_property_state_text_unset(property);
   prop_box = elm_object_content_get(property);
   if (!pd_text.frame)
     {
         FRAME_ADD(property, text_frame, EINA_TRUE, "Text")
         BOX_ADD(text_frame, box, EINA_FALSE, EINA_FALSE)
         elm_box_align_set(box, 0.5, 0.0);
         elm_object_content_set(text_frame, box);

         pd_text.text = prop_item_state_text_add(box, pd->group, part,
                           "Set the text of part.", "Choose different text");
         pd_text.font = prop_item_state_font_add(box, pd->group, part,
                           "Change the text's font", "Choose different font");
         pd_text.size = prop_item_state_text_size_add(box, pd->group, part,
                           0.0, 128.0, 1.0, "%.0f pt",
                           "Change text font's size.'");
         pd_text.align = prop_item_state_text_align_x_y_add(box, pd->group, part,
                            0.0, 1.0, 0.1, "%1.2f",
                            "Text horizontal align. "
                            "0.0 = left  1.0 = right",
                            "Text vertical align. "
                            "0.0 = top  1.0 = bottom");
         pd_text.elipsis = prop_item_state_text_elipsis_add(box, pd->group, part,
                              0.0, 1.0, 0.1, "%1.2f",
                              "Cut text if biggest then part's area"
                              "0.0 = fix the left side  1.0 = right side");
         pd_text.fit = prop_item_state_text_fit_x_y_add(box, pd->group, part,
                          "Resize the text for it to fit in it's container by X axis",
                          "Resize the text for it to fit in it's container by Y axis");
         pd_text.color2 = prop_item_state_color2_add(box, pd->group, part,
                             "Text shadow color.");
         pd_text.color3 = prop_item_state_color3_add(box, pd->group, part,
                             "Text outline color.");

         elm_box_pack_end(box, pd_text.text);
         elm_box_pack_end(box, pd_text.font);
         elm_box_pack_end(box, pd_text.size);
         elm_box_pack_end(box, pd_text.align);
         elm_box_pack_end(box, pd_text.elipsis);
         elm_box_pack_end(box, pd_text.fit);
         elm_box_pack_end(box, pd_text.color2);
         elm_box_pack_end(box, pd_text.color3);

         elm_box_pack_end(prop_box, text_frame);
         pd_text.frame = text_frame;
     }
   else
     {
        prop_item_state_text_update(pd_text.text, pd->group, part);
        prop_item_state_font_update(pd_text.font, pd->group, part);
        prop_item_state_text_size_update(pd_text.size, pd->group, part);
        prop_item_state_text_align_x_y_update(pd_text.align, pd->group, part);
        prop_item_state_text_elipsis_update(pd_text.elipsis, pd->group, part);
        prop_item_state_text_fit_x_y_update(pd_text.fit, pd->group, part);
        prop_item_state_color2_update(pd_text.color2, pd->group, part);
        prop_item_state_color3_update(pd_text.color3, pd->group, part);
        elm_box_pack_end(prop_box, pd_text.frame);
        evas_object_show(pd_text.frame);
     }

   return EINA_TRUE;
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

#define ITEM_IM_BORDER_STATE_CREATE(text, sub, value) \
   ITEM_IM_BORDER_STATE_CALLBACK(sub, value) \
   ITEM_IM_BORDER_STATE_ADD(text, sub, value) \
   ITEM_IM_BORDER_STATE_UPDATE(sub, value)

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

   img_edit = image_editor_window_add(ap->win, SINGLE);
   image_editor_init(img_edit, ap->project);
   image_editor_file_choose(img_edit, selected);
   image_editor_callback_add(img_edit, _on_image_editor_done, entry);
}

ITEM_1ENTRY_STATE_CREATE("image", state, image, _on_state_image_choose)
ITEM_IM_BORDER_STATE_CREATE("border", state_image, border)

static Eina_Bool
ui_property_state_image_set(Evas_Object *property, Part *part)
{
   Evas_Object *image_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is IMAGE too, unpack it */
   ui_property_state_image_unset(property);
   prop_box = elm_object_content_get(property);
   if (!pd_image.frame)
     {
        FRAME_ADD(property, image_frame, EINA_TRUE, "Image")
        BOX_ADD(image_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(image_frame, box);

        pd_image.normal = prop_item_state_image_add(box, pd->group, part,
                             "Current image name",
                             "Change image");
        pd_image.border = prop_item_state_image_border_add(box, pd->group, part,
                             "Image's border value");

        elm_box_pack_end(box, pd_image.normal);
        elm_box_pack_end(box, pd_image.border);

        elm_box_pack_end(prop_box, image_frame);
        pd_image.frame = image_frame;
     }
   else
     {
        prop_item_state_image_update(pd_image.normal, pd->group, part);
        prop_item_state_image_border_update(pd_image.border, pd->group, part);
        elm_box_pack_end(prop_box, pd_image.frame);
        evas_object_show(pd_image.frame);
     }

   return EINA_TRUE;
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
