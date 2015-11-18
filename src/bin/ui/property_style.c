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
#include "property_private.h"
#include "property_macros.h"

struct _Style_Prop_Data
{
   Evas_Object *box;
   Evas_Object *font_name;
   Evas_Object *font_style;
   Evas_Object *font_color;
   Evas_Object *font_weight;
   Evas_Object *font_size;
   Evas_Object *hor_align;
   Evas_Object *vert_align;
   Evas_Object *left_margin;
   Evas_Object *right_margin;
   Evas_Object *tab_stops;
   Evas_Object *line_size;
   Evas_Object *relative_size;
   Evas_Object *replace_ellisis;
   Evas_Object *check_psw;
   Evas_Object *check_color;
   Evas_Object *bg_color;
   Evas_Object *style;
   Evas_Object *outer_gl_color;
   Evas_Object *inner_gl_color;
   Evas_Object *check_s_color;
   Evas_Object *s_color;
   Evas_Object *check_unline;
   Evas_Object *outer_color_singl;
   Evas_Object *outer_color_double;
};
typedef struct _Style_Prop_Data Style_Prop_Data;

#define ADD_1SWALLOW_ITEM(TEXT, BOX, OBJECT, ITEM, TYPE_OBJECT) \
{ \
   PROPERTY_ITEM_ADD(BOX, TEXT, "1swallow"); \
   TYPE_OBJECT##_ADD(BOX, OBJECT); \
   elm_layout_content_set(ITEM, "elm.swallow.content", OBJECT); \
   elm_box_pack_end(BOX, ITEM); \
}

#define ADD_COLOR_ELEMENT(OBJECT, ITEM, LAYOUT) \
   OBJECT = elm_layout_add(ITEM); \
   elm_layout_theme_set(OBJECT, "layout", "property", "color"); \
   evas_object_size_hint_weight_set(OBJECT,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(OBJECT, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   LAYOUT = elm_layout_add(ITEM); \
   elm_layout_theme_set(LAYOUT, "image", "color", "color_set"); \
   elm_layout_content_set(OBJECT, NULL, LAYOUT);

static void
_prop_text_color_add(const char *text, Evas_Object *box, Evas_Object *object)
{
   Evas_Object *layout;
   PROPERTY_ITEM_ADD(box, text, "1swallow_subtext")
   ADD_COLOR_ELEMENT(object, item, layout)
   elm_layout_content_set(item, NULL, object);
   elm_box_pack_end(box, item);
}

static void
_prop_text_2color_add(const char *text,
                      Evas_Object *box,
                      Evas_Object *object_left,
                      Evas_Object *object_right)
{
   Evas_Object *layout;
   PROPERTY_ITEM_ADD(box, text, "2swallow")

   ADD_COLOR_ELEMENT(object_left, item, layout)
   elm_layout_content_set(item, NULL, object_left);
   ADD_COLOR_ELEMENT(object_right, item, layout)
   elm_layout_content_set(item, NULL, object_right);

   elm_layout_content_set(item, "swallow.content1", object_left);
   elm_layout_content_set(item, "swallow.content2", object_right);

   elm_box_pack_end(box, item);
}

static void
_prop_spin_add(const char *text,
               const char *LS,
               const char *RS,
               Evas_Object *box,
               Evas_Object *object)
{
   PROPERTY_ITEM_ADD(box, text, "2swallow")
   SPINNER_ADD(item, object, 0, 0, 0, true)

   elm_layout_content_set(item, "swallow.content1", object);
   elm_layout_text_set(item, "label.swallow1.start", LS);
   elm_layout_text_set(item, "label.swallow1.end", RS);

   elm_box_pack_end(box, item);
}

static void
_add_1check_color_item(const char *text,
                       Evas_Object *box,
                       Evas_Object *object_left,
                       Evas_Object *object_right)
{
   Evas_Object *layout;
   PROPERTY_ITEM_ADD(box, text, "2swallow")
   CHECK_ADD(item, object_left)

   ADD_COLOR_ELEMENT(object_right, item, layout)

   elm_layout_content_set(item, "swallow.content1", object_left);
   elm_layout_content_set(item, "swallow.content2", object_right);

   elm_box_pack_end(box, item);
}

static void
_add_1check_2button_item(const char *text,
                         Evas_Object *box,
                         Evas_Object *object_left)
{
   Evas_Object *segment_ctrl;

   PROPERTY_ITEM_ADD(box, text, "2swallow")
   CHECK_ADD(item, object_left)

   SEGMENT_CONTROL_ADD(item, segment_ctrl)
   elm_segment_control_item_add(segment_ctrl, NULL, "single");
   elm_segment_control_item_add(segment_ctrl, NULL, "double");

   elm_layout_content_set(item, "swallow.content1", object_left);
   elm_layout_content_set(item, "swallow.content2", segment_ctrl);

   elm_box_pack_end(box, item);
}

static Evas_Object *
_add_text_frame(Style_Prop_Data *pd)
{
   Evas_Object *frame_text, *box_frame;

   FRAME_ADD(pd->box, frame_text, true, _("Text"))

   BOX_ADD(frame_text, box_frame, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(box_frame, 6, 0);
   elm_object_content_set(frame_text, box_frame);

   ADD_1SWALLOW_ITEM(_("Font name:"), box_frame, pd->font_name, item, EWE_COMBOBOX);
   ADD_1SWALLOW_ITEM(_("Font style:"), box_frame, pd->font_style, item, EWE_COMBOBOX);
   ADD_1SWALLOW_ITEM(_(""), box_frame, pd->font_weight, item, EWE_COMBOBOX);

   _prop_text_color_add(_("Color:"), box_frame, pd->font_color);
   _prop_spin_add(_("Size:"), _(""), _("px"), box_frame, pd->font_size);

   return frame_text;
}

static Evas_Object *
_add_position_frame(Style_Prop_Data *pd)
{
   Evas_Object *frame_position, *box_frame;

   FRAME_ADD(pd->box, frame_position, true, _("Positioning option"))

   BOX_ADD(frame_position, box_frame, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(box_frame, 6, 0);
   elm_object_content_set(frame_position, box_frame);

   ADD_1SWALLOW_ITEM(_("Horizontal align:"), box_frame, pd->hor_align, item, EWE_COMBOBOX);
   ADD_1SWALLOW_ITEM(_("Vertical align:"), box_frame, pd->vert_align, item, EWE_COMBOBOX);
   _prop_spin_add(_("Margin:"), _("left"), _("px"), box_frame, pd->left_margin);
   _prop_spin_add(_(""), _("Right"), _("px"), box_frame, pd->right_margin);

   return frame_position;
}

static Evas_Object *
_add_text_format_frame(Style_Prop_Data *pd)
{
   Evas_Object *frame_text_format, *box_frame;

   FRAME_ADD(pd->box, frame_text_format, true, _("Text format"))

   BOX_ADD(frame_text_format, box_frame, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(box_frame, 6, 0);
   elm_object_content_set(frame_text_format, box_frame);

   _prop_spin_add(_("Text tabstops:"), _(""), _("px"), box_frame, pd->tab_stops);
   _prop_spin_add(_("Line size:"), _(""), _("px"), box_frame, pd->line_size);
   _add_1check_color_item(_("Background color"), box_frame, pd->check_color, pd->bg_color);
   ADD_1SWALLOW_ITEM(_("Password field:"), box_frame, pd->check_psw, item, CHECK);
   _prop_spin_add(_("Replace overflow characters with ellisis:"), _(""), _(""), box_frame, pd->replace_ellisis);
   _prop_spin_add(_("Line relative size:"), _(""), _("px"), box_frame, pd->relative_size);

   return frame_text_format;
}

static Evas_Object *
add_glow_shadow_frame(Style_Prop_Data *pd)
{
   Evas_Object *frame_glow, *box_frame;

   FRAME_ADD(pd->box, frame_glow, true, _("Glow & Shadow"))

   BOX_ADD(frame_glow, box_frame, EINA_FALSE, EINA_FALSE);
   elm_object_content_set(frame_glow, box_frame);

   elm_box_padding_set(box_frame, 6, 0);

   ADD_1SWALLOW_ITEM(_("Style:"), box_frame, pd->style, item, EWE_COMBOBOX);
   _prop_text_color_add(_("Outer glow color:"), box_frame, pd->outer_gl_color);
   _prop_text_color_add(_("Inner glow color:"), box_frame, pd->inner_gl_color);

   return frame_glow;
}

static Evas_Object *
add_lines_frame(Style_Prop_Data *pd)
{
   Evas_Object *frame_lines, *box_frame;

   FRAME_ADD(pd->box, frame_lines, true, _("Lines"))

   BOX_ADD(frame_lines, box_frame, EINA_FALSE, EINA_FALSE);
   elm_object_content_set(frame_lines, box_frame);

   _add_1check_color_item(_("Strikethrough color"), box_frame, pd->check_s_color, pd->s_color);
   _add_1check_2button_item(_("Underline"), box_frame, pd->check_unline);
   _prop_text_2color_add(_("Underline color:"), box_frame, pd->outer_color_singl, pd->outer_color_double);
   elm_box_padding_set(box_frame, 6, 0);

   return frame_lines;
}

Evas_Object *
ui_property_style_add(Evas_Object *parent)
{
   Style_Prop_Data *pd;
   Evas_Object *scroller;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Style_Prop_Data));

   SCROLLER_ADD(parent, scroller);

   BOX_ADD(scroller, pd->box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(pd->box, 0.5, 0.0);
   elm_object_content_set(scroller, pd->box);

   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);

   elm_box_pack_end(pd->box, _add_text_frame(pd));
   elm_box_pack_end(pd->box, _add_position_frame(pd));
   elm_box_pack_end(pd->box, _add_text_format_frame(pd));
   elm_box_pack_end(pd->box, add_glow_shadow_frame(pd));
   elm_box_pack_end(pd->box, add_lines_frame(pd));

   return scroller;
}