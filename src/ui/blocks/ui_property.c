/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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
   Evas_Object *visual;
   Evas_Object *code;
   struct {
      Evas_Object *frame;
      Evas_Object *info;
      Evas_Object *shared_check;
      Evas_Object *ctxpopup;
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
      Evas_Object *rel1_relative;
      Evas_Object *rel1_offset;
      Evas_Object *rel1_to;
      Evas_Object *rel2_relative;
      Evas_Object *rel2_offset;
      Evas_Object *rel2_to;
   } prop_state_object_area;
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

static const char *edje_aspect_pref[] = { N_("None"),
                                          N_("Vertical"),
                                          N_("Horizontal"),
                                          N_("Both"),
                                          N_("Source") };
#define ASPECT_PREF_TYPE_COUNT 5

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
   ITEM_ADD(parent, item, lab_text, "eflete/property/item/default")
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
                           Evas_Object *obj EINA_UNUSED,
                           void *ei)
{
   Prop_Data *pd = (Prop_Data *)data;
   Ewe_Combobox_Item *item = ei;
   if (!edje_edit_state_aspect_pref_set(pd->style->obj, pd->part->name,
                                        pd->part->curr_state, pd->part->curr_state_value,
                                        item->index))
     return;
   workspace_edit_object_recalc(pd->workspace);
   pd->style->isModify = true;
}

static Evas_Object *
prop_item_state_aspect_pref_add(Evas_Object *parent,
                                Prop_Data *pd,
                                Evas_Object *property EINA_UNUSED,
                                const char *tooltip)
{
   Evas_Object *item, *combobox;
   unsigned char asp_pref;
   int i = 0;
   ITEM_ADD(parent, item, _("aspect ratio mode"), "eflete/property/item/default")
   EWE_COMBOBOX_ADD(item, combobox)
   elm_object_tooltip_text_set(combobox, tooltip);
   asp_pref = edje_edit_state_aspect_pref_get(pd->style->obj,
                                              pd->part->name,
                                              pd->part->curr_state,
                                              pd->part->curr_state_value);
   for (i = 0; i < ASPECT_PREF_TYPE_COUNT; i++)
     ewe_combobox_item_add(combobox, _(edje_aspect_pref[i]));
   ewe_combobox_select_item_set(combobox, asp_pref);
   evas_object_smart_callback_add(combobox, "selected", _on_state_pref_pref_change, pd);
   elm_object_part_content_set(item, "elm.swallow.content", combobox);
   return item;
}

static void
prop_item_state_aspect_pref_update(Evas_Object *item,
                                   Prop_Data *pd)
{
   Evas_Object *combobox;
   unsigned char asp_pref;
   combobox = elm_object_part_content_get(item, "elm.swallow.content");
   asp_pref = edje_edit_state_aspect_pref_get(pd->style->obj,
                                              pd->part->name,
                                              pd->part->curr_state,
                                              pd->part->curr_state_value);
   ewe_combobox_select_item_set(combobox, asp_pref);
}

Evas_Object *
ui_property_add(Evas_Object *parent)
{
   Evas_Object *box, *scroller, *_bg, *tabs;
   Ewe_Tabs_Item *it;
   Prop_Data *pd;

   if (!parent) return NULL;
   pd = mem_calloc(1, sizeof(Prop_Data));
   tabs = ewe_tabs_add(parent);

   SCROLLER_ADD(tabs, scroller);
   BOX_ADD(scroller, box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);

   _bg = evas_object_image_add(evas_object_evas_get(parent));
   evas_object_image_filled_set(_bg, true);
   evas_object_image_file_set(_bg, EFLETE_IMG_PATH"section-item-bg.png", NULL);
   evas_object_image_border_set(_bg, 0, 0, 0, 2);
   elm_object_part_content_set(scroller, "elm.swallow.background", _bg);
   evas_object_show(_bg);
   pd->visual = scroller;
   it = ewe_tabs_item_append(tabs, NULL, _("Visual"), NULL);
   ewe_tabs_item_content_set(tabs, it, pd->visual);

   pd->code = elm_entry_add(tabs);
   elm_object_style_set(pd->code, DEFAULT_STYLE);
   elm_entry_single_line_set(pd->code, false);
   elm_scroller_policy_set(pd->code, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_ON);
   evas_object_size_hint_weight_set(pd->code, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pd->code, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_entry_scrollable_set(pd->code, true);
   it = ewe_tabs_item_append(tabs, it, _("Code"), NULL);
   ewe_tabs_item_content_set(tabs, it, pd->code);
   elm_entry_editable_set(pd->code, false);

   evas_object_data_set(tabs, PROP_DATA, pd);
   evas_object_event_callback_add(tabs, EVAS_CALLBACK_DEL, _del_prop_data, pd);

   return tabs;
}

#define ITEM_2SPINNER_GROUP_CREATE(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_SPINNER_INT_CALLBACK(SUB, VALUE1) \
   ITEM_SPINNER_INT_CALLBACK(SUB, VALUE2) \
   ITEM_2SPINNER_GROUP_ADD(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_2SPINNER_GROUP_UPDATE(SUB, VALUE1, VALUE2)

/* group property */
ITEM_2SPINNER_GROUP_CREATE(_("min"), group_min, w, h)
ITEM_2SPINNER_GROUP_CREATE(_("max"), group_max, w, h)

#define pd_group pd->prop_group

static void
_on__dismissed(void *data __UNUSED__,
               Evas_Object *obj,
               void *event_info __UNUSED__)
{
   evas_object_hide(obj);
}
static void
_on__clicked(void *data,
             Evas *e,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   int x, y;
   Prop_Data *pd = (Prop_Data *)data;
   evas_object_smart_callback_del_full(pd_group.ctxpopup, "dismissed",
                                       _on__dismissed, pd);
   evas_object_smart_callback_add(pd_group.ctxpopup, "dismissed",
                                  _on__dismissed, pd);
   evas_pointer_canvas_xy_get(e, &x, &y);
   evas_object_move(pd_group.ctxpopup, x, y);
   evas_object_show(pd_group.ctxpopup);
}
static void
_prop_item_alias_update(Prop_Data *pd, Style *style, int aliases_count)
{
   Evas_Object *label, *label_ctx;
   Eina_List *list = NULL, *l;
   const char *text_info = NULL;
   char *list_data;
   Eina_Strbuf *text_ctx;

   label = elm_object_part_content_get(pd_group.info, "elm.swallow.content");
   label_ctx = elm_object_content_get(pd_group.ctxpopup);
   list = edje_edit_group_aliases_get(style->obj, style->full_group_name);
   if (style->isAlias)
     {
        text_info = eina_stringshare_add("This is alias of <a>%s</a>");
        text_info = eina_stringshare_printf(text_info,
                                            style->main_group->full_group_name);
     }
   else
     {
        text_info = eina_stringshare_add("changes in this style will also affect <a>%d elements.</a>");
        text_info = eina_stringshare_printf(text_info, aliases_count);

        int count = 1;
        text_ctx = eina_strbuf_new();

        EINA_LIST_FOREACH(list, l, list_data)
          {
             const char *step = "%d. %s";
             if (eina_strbuf_length_get(text_ctx) > 0)
               eina_strbuf_append(text_ctx, "</br>");
             step = eina_stringshare_printf(step, count++, list_data);
             eina_strbuf_append(text_ctx, step);
          }

        evas_object_event_callback_del_full(pd_group.info, EVAS_CALLBACK_MOUSE_DOWN,
                                       _on__clicked, pd);
        evas_object_event_callback_add(pd_group.info, EVAS_CALLBACK_MOUSE_DOWN,
                                       _on__clicked, pd);
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
   entry = elm_object_part_content_get(item, "info");
   Eina_Bool bool = false;
   if (count > 0) bool = true;
   elm_check_state_set(entry, bool);
   evas_object_show(item);
}

Eina_Bool
ui_property_style_set(Evas_Object *property, Style *style, Evas_Object *workspace)
{
   Evas_Object *group_frame, *box, *prop_box, *info_en = NULL;
   Evas_Object *info_image;
   Evas_Object *check, *label_ctx;
   Eina_List *aliases = NULL, *l;
   const char *text_info = NULL;
   int aliases_count = 0;
   char *list_data;
   Eina_Strbuf *text_ctx = NULL;

   if ((!property) || (!workspace)) return EINA_FALSE;
   PROP_DATA_GET(EINA_FALSE)

   evas_object_show(property);

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_ON);

   pd->workspace = workspace;
   pd->style = style;
   if (style->isAlias) pd->style = style->main_group;
   if (pd->style != workspace_edit_object_get(workspace))
     {
        ERR("Cann't set the style! Style [%p] not matched"
            " with editable group in workspace", style);
        return false;
     }
   elm_object_text_set(pd->code, elm_entry_utf8_to_markup(edje_edit_source_generate(style->obj)));
   prop_box = elm_object_content_get(pd->visual);
   aliases = edje_edit_group_aliases_get(style->obj, style->full_group_name);
   aliases_count = eina_list_count(aliases);

   if (!pd_group.info)
     {
        LABEL_ADD(property, info_en, text_info)

        info_image = elm_image_add(property);
        elm_image_file_set(info_image, EFLETE_IMG_PATH"icon-notification.png", NULL);
        evas_object_show(info_image);

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
        elm_object_style_set(pd_group.ctxpopup, "eflete/info");
        evas_object_hide(pd_group.ctxpopup);

        if (style->isAlias)
          {
             text_info = eina_stringshare_add("This is alias of <a>%s</a>");
             text_info = eina_stringshare_printf(text_info,
                                                 style->main_group->full_group_name);
          }
        else
          {
             text_info = eina_stringshare_add("changes in this style will also affect <a>%d elements.</a>");
             text_info = eina_stringshare_printf(text_info, aliases_count);
             int count = 1;
             text_ctx = eina_strbuf_new();

             EINA_LIST_FOREACH(aliases, l, list_data)
               {
                  const char *step = "%d. %s";
                  if (eina_strbuf_length_get(text_ctx) > 0)
                    eina_strbuf_append(text_ctx, "</br>");
                  step = eina_stringshare_printf(step, count++, list_data);
                  eina_strbuf_append(text_ctx, step);
               }
             evas_object_event_callback_add(pd_group.info, EVAS_CALLBACK_MOUSE_DOWN,
                                            _on__clicked, pd);
          }

        LABEL_ADD(property, label_ctx, eina_strbuf_string_get(text_ctx))
        eina_strbuf_free(text_ctx);
        elm_object_style_set(label_ctx, "eflete/info");
        elm_object_content_set(pd_group.ctxpopup, label_ctx);

        elm_object_text_set(info_en, text_info);
        elm_object_style_set(info_en, "eflete/info");
        elm_object_part_content_set(pd_group.info, "elm.swallow.content", info_en);
        elm_object_part_content_set(pd_group.info, "info", info_image);
     }

   if (!pd_group.shared_check)
     {
        CHECK_ADD(property, check, DEFAULT_STYLE)
        elm_object_text_set(check, "Shared style");
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
        FRAME_ADD(property, group_frame, true, _("Layout property"))
        BOX_ADD(group_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(group_frame, box);

        pd_group.min = prop_item_group_min_w_h_add(box, pd,
                          0.0, 9999.0, 1.0,
                          _("Minimum group width in pixels."),
                          _("Minimum group height in pixels."));
        pd_group.max = prop_item_group_max_w_h_add(box, pd,
                          -1.0, 9999.0, 1.0,
                          _("Maximum group width in pixels."),
                          _("Maximum group height in pixels."));
        elm_box_pack_end(box, pd_group.min);
        elm_box_pack_end(box, pd_group.max);

        elm_box_pack_start(prop_box, group_frame);
        pd_group.frame = group_frame;
     }
   else
     {
        if ((aliases_count > 0) || (style->isAlias))
          {
             _prop_item_alias_update(pd, style, aliases_count);
             evas_object_show(pd_group.info);
          }
        _prop_item_shared_check_update(pd_group.shared_check, aliases_count);
        prop_item_group_min_w_h_update(pd_group.min, pd);
        prop_item_group_max_w_h_update(pd_group.max, pd);
        evas_object_show(pd_group.frame);
     }
   if ((aliases_count > 0) || (style->isAlias))
     {
        elm_box_pack_start(prop_box, pd_group.info);
        evas_object_show(pd_group.info);
     }
   elm_box_pack_start(prop_box, pd_group.shared_check);

   return true;
}

void
ui_property_style_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   if (!property) return;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->visual);
   evas_object_hide(pd_group.info);
   elm_box_unpack(prop_box, pd_group.info);
   elm_box_unpack(prop_box, pd_group.shared_check);
   evas_object_event_callback_del_full(pd_group.info, EVAS_CALLBACK_MOUSE_DOWN,
                                  _on__clicked, pd);
   evas_object_hide(pd_group.frame);
   evas_object_hide(pd_group.shared_check);
   ui_property_part_unset(property);
   elm_scroller_policy_set(property, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   evas_object_hide(property);
}
#undef pd_group

static void _clip_to_sel(void *data,
                         Evas_Object *obj EINA_UNUSED,
                         void *ei)
{
   Prop_Data *pd = (Prop_Data *)data;
   Ewe_Combobox_Item *item = ei;
   if (strcmp(item->title, "None")) edje_edit_part_clip_to_set(pd->style->obj, pd->part->name, item->title);
   else edje_edit_part_clip_to_set(pd->style->obj, pd->part->name, NULL);
}

#define ITEM_1CHECK_PART_CREATE(TEXT, SUB, VALUE) \
   ITEM_CHECK_PART_CALLBACK(SUB, VALUE) \
   ITEM_1CHEACK_PART_ADD(TEXT, SUB, VALUE) \
   ITEM_1CHEACK_PART_UPDATE(SUB, VALUE)

#define ITEM_1ENTRY_PART_CREATE(TEXT, SUB, VALUE) \
   ITEM_STRING_PART_CALLBACK(SUB, VALUE) \
   ITEM_1ENTRY_PART_ADD(TEXT, SUB, VALUE) \
   ITEM_1ENTRY_PART_UPDATE(SUB, VALUE)

#define ITEM_1ENTRY_PART_NAME_CREATE(TEXT, SUB, VALUE) \
   ITEM_STRING_PART_NAME_CALLBACK(SUB, VALUE) \
   ITEM_1ENTRY_PART_NAME_ADD(TEXT, SUB, VALUE)

#define ITEM_1COMBOBOX_PART_CREATE(TEXT, SUB, VALUE) \
   ITEM_1COMBOBOX_PART_ADD(TEXT, SUB, VALUE) \
   ITEM_1COMBOBOX_PART_UPDATE(SUB, VALUE)

#define ITEM_DRAG_PART_CREATE(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_CHECK_PART_CALLBACK(SUB, VALUE1) \
   ITEM_INT_PART_CALLBACK(SUB, VALUE2) \
   ITEM_DRAG_PART_ADD(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_DRAG_PART_UPDATE(SUB, VALUE1, VALUE2)

/* part property */
ITEM_1ENTRY_PART_NAME_CREATE(_("name"), part, name)
ITEM_1CHECK_PART_CREATE(_("scalable"), part, scale)
ITEM_1CHECK_PART_CREATE(_("mouse events"), part, mouse_events)
ITEM_1CHECK_PART_CREATE(_("event propagation"), part, repeat_events)
ITEM_1COMBOBOX_PART_CREATE(_("clip to"), part, clip_to)

/* part drag property */
ITEM_DRAG_PART_CREATE(_("x"), part_drag, x, step_x)
ITEM_DRAG_PART_CREATE(_("y"), part_drag, y, step_y)
ITEM_1ENTRY_PART_CREATE(_("drag area"), part_drag, confine)
ITEM_1ENTRY_PART_CREATE(_("forward events"), part_drag, event)

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
   prop_box = elm_object_content_get(pd->visual);

   elm_box_unpack(prop_box, pd->prop_part.frame);
   elm_box_unpack(prop_box, pd->prop_part_drag.frame);

   if (!pd_part.frame)
     {
        FRAME_ADD(pd->visual, part_frame, true, _("Part property"))
        BOX_ADD(part_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(part_frame, box);

        pd_part.name = prop_item_part_name_add(box, pd, _("Selected part name"));
        pd_part.type = prop_item_label_add(box, _("type"), wm_part_type_get(type));
        pd_part.scale = prop_item_part_scale_add(box, pd,
                           _("Specifies whether the part will scale "
                           "its size with an edje scaling factor."));
        pd_part.mouse = prop_item_part_mouse_events_add(box, pd,
                           _("Enable mouse events in this part."));
        pd_part.repeat = prop_item_part_repeat_events_add(box, pd,
                            _("Enable repeat mouse events to the parts below."));
        pd_part.clip_to = prop_item_part_clip_to_add(box, pd,
                             _("Show only the area of part that coincides with "
                             "another part's container"));
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
        FRAME_ADD(pd->visual, part_drag_frame, true, _("Part dragable property"))
        BOX_ADD(part_drag_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(part_drag_frame, box);

        pd_part_drag.drag_x = prop_item_part_drag_x_step_x_add(box, pd,
                                 0.0, 9999.0, 1.0, _("%.0f"),
                                 _("Enable/Disable draggin along X axis"),
                                 _("Set a drag step value"));
        pd_part_drag.drag_y = prop_item_part_drag_y_step_y_add(box, pd,
                                 0.0, 9999.0, 1.0, _("%.0f"),
                                 _("Enable/Disable draggin along Y axis"),
                                 _("Set a drag step value"));
        pd_part_drag.confine = prop_item_part_drag_confine_add(box, pd,
                                 _("Limits the movement of the dragged part to "
                                  "another part's container'"));
        pd_part_drag.event = prop_item_part_drag_event_add(box, pd,
                                _("It causes the part to forward the drag events "
                                "to another part, thus ignoring them for itself."));

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

   prop_box = elm_object_content_get(pd->visual);
   evas_object_hide(pd->prop_part.frame);
   evas_object_hide(pd->prop_part_drag.frame);
   elm_box_unpack(prop_box, pd->prop_part.frame);
   elm_box_unpack(prop_box, pd->prop_part_drag.frame);

   ui_property_state_unset(property);
}
#undef pd_part
#undef pd_part_drag

#define ITEM_2SPINNER_STATE_INT_CREATE(TEXT, SUB, VALUE1, VALUE2, STYLE) \
   ITEM_SPINNER_STATE_INT_CALLBACK(SUB, VALUE1) \
   ITEM_SPINNER_STATE_INT_CALLBACK(SUB, VALUE2) \
   ITEM_2SPINNER_STATE_ADD(TEXT, SUB, VALUE1, VALUE2, STYLE) \
   ITEM_2SPINNER_STATE_UPDATE(SUB, VALUE1, VALUE2)

#define ITEM_2SPINNER_STATE_DOUBLE_CREATE(TEXT, SUB, VALUE1, VALUE2, STYLE) \
   ITEM_SPINNER_STATE_DOUBLE_CALLBACK(SUB, VALUE1) \
   ITEM_SPINNER_STATE_DOUBLE_CALLBACK(SUB, VALUE2) \
   ITEM_2SPINNER_STATE_ADD(TEXT, SUB, VALUE1, VALUE2, STYLE) \
   ITEM_2SPINNER_STATE_UPDATE(SUB, VALUE1, VALUE2)

#define ITEM_1ENTRY_STATE_CREATE(TEXT, SUB, VALUE) \
   ITEM_STRING_STATE_CALLBACK(SUB, VALUE) \
   ITEM_1ENTRY_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_1ENTRY_STATE_UPDATE(SUB, VALUE)

#define ITEM_STATE_CCL_CREATE(TEXT, SUB, VALUE) \
   ITEM_1COMBOBOX_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_1COMBOBOX_STATE_UPDATE(SUB, VALUE)

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


ITEM_1CHECK_STATE_CREATE(_("visible"), state, visible)
ITEM_2SPINNER_STATE_INT_CREATE(_("min"), state_min, w, h, "eflete/property/item/default")
ITEM_2SPINNER_STATE_INT_CREATE(_("max"), state_max, w, h, "eflete/property/item/default")
ITEM_2CHECK_STATE_CREATE(_("fixed"), state_fixed, w, h)
ITEM_2SPINNER_STATE_DOUBLE_CREATE(_("align"), state_align, x, y, "eflete/property/item/default")
ITEM_2SPINNER_STATE_DOUBLE_CREATE(_("aspect ratio"), state_aspect, min, max, "eflete/property/item/default")
ITEM_STATE_CCL_CREATE(_("color class"), state, color_class)
ITEM_COLOR_STATE_CREATE(_("color"), state, color)

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
        FRAME_ADD(property, state_frame, true, _("State"))
        BOX_ADD(state_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(state_frame, box);

        pd_state.state = prop_item_label_add(box, _("state"), state);
        pd_state.visible = prop_item_state_visible_add(box, pd,
                                                       "");
        pd_state.min = prop_item_state_min_w_h_add(box, pd,
                          0.0, 9999.0, 1.0, "%.0f",
                          "w:", "px", "h:", "px",
                          _("Minimum part width in pixels."),
                          _("Minimum part height in pixels."),
                          false);
        pd_state.max = prop_item_state_max_w_h_add(box, pd,
                          -1.0, 9999.0, 1.0, "%.0f",
                          "w:", "px", "h:", "px",
                          _("Maximum part width in pixels."),
                          _("Maximum part height in pixels."),
                          false);
        pd_state.fixed = prop_item_state_fixed_w_h_add(box, pd,
                           _("This affects the minimum width calculation."),
                           _("This affects the minimum height calculation."));
        pd_state.align = prop_item_state_align_x_y_add(box, pd,
                            0, 100, 1, NULL,
                            "x:", "%", "y:", "%",
                            _("Part horizontal align: 0.0 = left  1.0 = right"),
                            _("Part vertical align: 0.0 = top  1.0 = bottom"),
                            true);
        pd_state.aspect = prop_item_state_aspect_min_max_add(box, pd,
                             0.0, 9999.0, 0.1, "%1.2f",
                             "", "", "", "",
                            _("Normally width and height can be "
                             "resized to any values independently"),
                            _("Normally width and height can be "
                             "resized to any values independently"),
                            false);
        pd_state.aspect_pref = prop_item_state_aspect_pref_add(box, pd, property,
                                   _("The aspect control hints for this object."));
        pd_state.color_class = prop_item_state_color_class_add(box, pd,
                                  _("Current color class"));
        pd_state.color = prop_item_state_color_add(box, pd,
                            _("Part main color."));

        elm_box_pack_end(box, pd_state.state);
        elm_box_pack_end(box, pd_state.visible);
        elm_box_pack_end(box, pd_state.min);
        elm_box_pack_end(box, pd_state.max);
        elm_box_pack_end(box, pd_state.fixed);
        elm_box_pack_end(box, pd_state.align);
        elm_box_pack_end(box, pd_state.aspect_pref);
        elm_box_pack_end(box, pd_state.aspect);
        elm_box_pack_end(box, pd_state.color_class);
        if (type == EDJE_PART_TYPE_SPACER)
          {
             evas_object_hide(pd_state.color);
             elm_box_unpack(box, pd_state.color);
          }
        else elm_box_pack_end(box, pd_state.color);
        prop_box = elm_object_content_get(pd->visual);
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
        prop_item_state_min_w_h_update(pd_state.min, pd, false);
        prop_item_state_max_w_h_update(pd_state.max, pd,false);
        prop_item_state_fixed_w_h_update(pd_state.fixed, pd);
        prop_item_state_align_x_y_update(pd_state.align, pd, true);
        prop_item_state_aspect_min_max_update(pd_state.aspect, pd, false);
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

   ui_property_state_obj_area_set(property);
   if (type == EDJE_PART_TYPE_TEXT) ui_property_state_text_set(property);
   else ui_property_state_text_unset(property);
   if (type == EDJE_PART_TYPE_TEXTBLOCK) ui_property_state_textblock_set(property);
   else ui_property_state_textblock_unset(property);
   if (type == EDJE_PART_TYPE_IMAGE) ui_property_state_image_set(property);
   else ui_property_state_image_unset(property);

   elm_object_text_set(pd->code, elm_entry_utf8_to_markup(edje_edit_source_generate(pd->style->obj)));

   #undef pd_state
   return true;
}

void
ui_property_state_unset(Evas_Object *property)
{
   if (!property) return;
   PROP_DATA_GET()

   evas_object_hide(pd->prop_state.frame);

   ui_property_state_obj_area_unset(property);
   ui_property_state_text_unset(property);
   ui_property_state_image_unset(property);
   ui_property_state_textblock_unset(property);
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
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   if (strcmp(item->title, "None")) edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                                        pd->part->curr_state, pd->part->curr_state_value, \
                                        item->title); \
   else edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                                        pd->part->curr_state, pd->part->curr_state_value, \
                                        NULL); \
  int temp = edje_edit_state_min_w_get(pd->style->obj, pd->part->name, \
                                        pd->part->curr_state, pd->part->curr_state_value); \
  edje_edit_state_min_w_set(pd->style->obj, pd->part->name, pd->part->curr_state, \
                                        pd->part->curr_state_value, temp); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_2COMBOBOX_STATE_CREATE(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_REL_TO_COMBOBOX_STATE_CALLBACK(SUB, VALUE1) \
   ITEM_REL_TO_COMBOBOX_STATE_CALLBACK(SUB, VALUE2) \
   ITEM_2COMBOBOX_STATE_ADD(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_2COMBOBOX_STATE_UPDATE(SUB, VALUE1, VALUE2)

ITEM_2SPINNER_STATE_DOUBLE_CREATE(_("align"), state_rel1_relative, x, y, "eflete/property/item/relative")
ITEM_2SPINNER_STATE_INT_CREATE(_("offset"), state_rel1_offset, x, y, "eflete/property/item/relative")
ITEM_2COMBOBOX_STATE_CREATE(_("relative to"), state_rel1_to, x, y)
ITEM_2SPINNER_STATE_DOUBLE_CREATE(_("align"), state_rel2_relative, x, y, "eflete/property/item/relative")
ITEM_2SPINNER_STATE_INT_CREATE(_("offset"), state_rel2_offset, x, y, "eflete/property/item/relative")
ITEM_2COMBOBOX_STATE_CREATE(_("relative to"), state_rel2_to, x, y)

#define pd_obj_area pd->prop_state_object_area
static Eina_Bool
ui_property_state_obj_area_set(Evas_Object *property)
{
   Evas_Object *obj_area_frame, *box, *prop_box;
   Evas_Object *separator, *icon;
   PROP_DATA_GET(EINA_FALSE)

   if (!pd_obj_area.frame)
     {
        FRAME_ADD(property, obj_area_frame, true, _("Object area"))
        BOX_ADD(obj_area_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(obj_area_frame, box);

        separator = elm_separator_add(obj_area_frame);
        elm_separator_horizontal_set(separator, true);
        elm_object_style_set(separator, "eflete/property");
        elm_object_part_text_set(separator, "eflete.text", _("Start point"));
        ICON_ADD(separator, icon, false, EFLETE_IMG_PATH"icon_start-point.png")
        elm_object_part_content_set(separator, "eflete.swallow.icon", icon);
        evas_object_show(separator);

        pd_obj_area.rel1_to = prop_item_state_rel1_to_x_y_add(box, pd,
                        "layout", "layout",
                        _("Causes a corner to be positioned relatively to the X axis of another "
                        "part. Setting to \"\" will un-set this value"),
                        _("Causes a corner to be positioned relatively to the Y axis of another "
                        "part. Setting to \"\" will un-set this value"));
        pd_obj_area.rel1_relative = prop_item_state_rel1_relative_x_y_add(box, pd,
                              -500, 500, 1, NULL,
                              "x:", "%", "y:", "%",
                              _("Define the position of left-up corner of the part's container. "
                              "Moves a corner to a relative position inside the container "
                              "by X axis."),
                              _("Define the position of left-up corner of the part's container. "
                              "Moves a corner to a relative position inside the container "
                              "by Y axis."),
                              true);
        ICON_ADD(pd_obj_area.rel1_relative, icon, false, EFLETE_IMG_PATH"icon_align.png");
        elm_object_part_content_set(pd_obj_area.rel1_relative, "eflete.swallow.icon", icon);
        pd_obj_area.rel1_offset = prop_item_state_rel1_offset_x_y_add(box, pd,
                            -9999.0, 9999.0, 1.0, "%.0f",
                            "x:", "px", "y:", "px",
                            _("Left offset from relative position in pixels"),
                            _("Top offset from relative position in pixels"),
                            false);
        ICON_ADD(pd_obj_area.rel1_offset, icon, false, EFLETE_IMG_PATH"icon_offset.png");
        elm_object_part_content_set(pd_obj_area.rel1_offset, "eflete.swallow.icon", icon);

        elm_box_pack_end(box, separator);
        elm_box_pack_end(box, pd_obj_area.rel1_to);
        elm_box_pack_end(box, pd_obj_area.rel1_relative);
        elm_box_pack_end(box, pd_obj_area.rel1_offset);

        separator = elm_separator_add(obj_area_frame);
        elm_separator_horizontal_set(separator, true);
        elm_object_style_set(separator, "eflete/property");
        elm_object_part_text_set(separator, "eflete.text", _("End point"));
        ICON_ADD(separator, icon, false, EFLETE_IMG_PATH"icon_end-point.png");
        elm_object_part_content_set(separator, "eflete.swallow.icon", icon);
        evas_object_show(separator);

        pd_obj_area.rel2_to = prop_item_state_rel2_to_x_y_add(box, pd,
                        "layout", "layout",
                        _("Causes a corner to be positioned relatively to the X axis of another "
                        "part. Setting to \"\" will un-set this value"),
                        _("Causes a corner to be positioned relatively to the Y axis of another "
                        "part. Setting to \"\" will un-set this value"));
        pd_obj_area.rel2_relative = prop_item_state_rel2_relative_x_y_add(box, pd,
                              -500, 500, 1, NULL,
                              "x:", "%", "y:", "%",
                              _("Define the position of right-down corner of the part's container. "
                              "Moves a corner to a relative position inside the container "
                              "by X axis."),
                              _("Define the position of right-down corner of the part's container. "
                              "Moves a corner to a relative position inside the container "
                              "by Y axis."),
                              true);
        ICON_ADD(pd_obj_area.rel2_relative, icon, false, EFLETE_IMG_PATH"icon_align.png");
        elm_object_part_content_set(pd_obj_area.rel2_relative, "eflete.swallow.icon", icon);
        pd_obj_area.rel2_offset = prop_item_state_rel2_offset_x_y_add(box, pd,
                            -9999.0, 9999.0, 1.0, "%.0f",
                            "x:", "px", "y:", "px",
                            _("Left offset from relative position in pixels"),
                            _("Top offset from relative position in pixels"),
                            false);
        ICON_ADD(pd_obj_area.rel2_offset, icon, false, EFLETE_IMG_PATH"icon_offset.png");
        elm_object_part_content_set(pd_obj_area.rel2_offset, "eflete.swallow.icon", icon);


        elm_box_pack_end(box, separator);
        elm_box_pack_end(box, pd_obj_area.rel2_to);
        elm_box_pack_end(box, pd_obj_area.rel2_relative);
        elm_box_pack_end(box, pd_obj_area.rel2_offset);


        prop_box = elm_object_content_get(pd->visual);
        elm_box_pack_end(prop_box, obj_area_frame);
        pd_obj_area.frame = obj_area_frame;
     }
   else
     {
        prop_item_state_rel1_relative_x_y_update(pd_obj_area.rel1_relative, pd, true);
        prop_item_state_rel1_offset_x_y_update(pd_obj_area.rel1_offset, pd, false);
        prop_item_state_rel1_to_x_y_update(pd_obj_area.rel1_to, pd);

        prop_item_state_rel2_relative_x_y_update(pd_obj_area.rel2_relative, pd, true);
        prop_item_state_rel2_offset_x_y_update(pd_obj_area.rel2_offset, pd, false);
        prop_item_state_rel2_to_x_y_update(pd_obj_area.rel2_to, pd);

        evas_object_show(pd_obj_area.frame);
     }

   return true;
}

static void
ui_property_state_obj_area_unset(Evas_Object *property)
{
   PROP_DATA_GET()
   evas_object_hide(pd_obj_area.frame);
}
#undef pd_obj_area

#define ITEM_1SPINNER_STATE_INT_CREATE(TEXT, SUB, VALUE) \
   ITEM_SPINNER_STATE_INT_CALLBACK(SUB, VALUE) \
   ITEM_1SPINNER_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_1SPINNER_STATE_UPDATE(SUB, VALUE)

#define ITEM_1SPINNER_STATE_DOUBLE_CREATE(TEXT, SUB, VALUE) \
   ITEM_SPINNER_STATE_DOUBLE_CALLBACK(SUB, VALUE) \
   ITEM_1SPINNER_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_1SPINNER_STATE_UPDATE(SUB, VALUE)


ITEM_1ENTRY_STATE_CREATE(_("text"), state, text)
ITEM_1ENTRY_STATE_CREATE(_("font"), state, font)
ITEM_1SPINNER_STATE_INT_CREATE(_("size"), state_text, size)
ITEM_2SPINNER_STATE_DOUBLE_CREATE(_("align"), state_text_align, x, y, "eflete/property/item/default")
ITEM_2CHECK_STATE_CREATE(_("max"), state_text_max, x, y)
ITEM_2CHECK_STATE_CREATE(_("min"), state_text_min, x, y)
ITEM_1SPINNER_STATE_DOUBLE_CREATE(_("elipsis"), state_text, elipsis)
ITEM_2CHECK_STATE_CREATE(_("fit"), state_text_fit, x, y)
ITEM_COLOR_STATE_CREATE(_("shadow color"), state, color2)
ITEM_COLOR_STATE_CREATE(_("outline color"), state, color3)

#define pd_text pd->prop_state_text
static Eina_Bool
ui_property_state_text_set(Evas_Object *property)
{
   Evas_Object *text_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is TEXT too, unpack it */
   ui_property_state_text_unset(property);
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_text.frame)
     {
         FRAME_ADD(property, text_frame, true, _("Text"))
         BOX_ADD(text_frame, box, EINA_FALSE, EINA_FALSE)
         elm_box_align_set(box, 0.5, 0.0);
         elm_object_content_set(text_frame, box);

         pd_text.text = prop_item_state_text_add(box, pd, NULL,
                           _("Set the text of part."), NULL);
         pd_text.font = prop_item_state_font_add(box, pd, NULL,
                           _("Change the text's font"), NULL);
         pd_text.size = prop_item_state_text_size_add(box, pd,
                           0.0, 128.0, 1.0, "%.0f pt",
                           _("Change text font's size.'"));
         pd_text.align = prop_item_state_text_align_x_y_add(box, pd,
                            0, 100, 1, NULL,
                            "x:", "%", "y:", "%",
                            _("Text horizontal align. "
                            "0.0 = left  1.0 = right"),
                            _("Text vertical align. "
                            "0.0 = top  1.0 = bottom"),
                            true);
         pd_text.min = prop_item_state_text_min_x_y_add(box, pd,
                           _("When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text."),
                           _("When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text."));
         pd_text.max = prop_item_state_text_max_x_y_add(box, pd,
                           _("When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text."),
                           _("When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text."));
         pd_text.elipsis = prop_item_state_text_elipsis_add(box, pd,
                              0.0, 1.0, 0.1, "%1.2f",
                              _("Cut text if biggest then part's area"
                              "0.0 = fix the left side  1.0 = right side"));
         pd_text.fit = prop_item_state_text_fit_x_y_add(box, pd,
                          _("Resize the text for it to fit in it's container by X axis"),
                          _("Resize the text for it to fit in it's container by Y axis"));
         pd_text.color2 = prop_item_state_color2_add(box, pd,
                             _("Text shadow color."));
         pd_text.color3 = prop_item_state_color3_add(box, pd,
                             _("Text outline color."));

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
        prop_item_state_text_align_x_y_update(pd_text.align, pd, true);
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

   prop_box = elm_object_content_get(pd->visual);
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
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_textblock.frame)
     {
         FRAME_ADD(property, textblock_frame, true, _("TextBlock"))
         BOX_ADD(textblock_frame, box, EINA_FALSE, EINA_FALSE)
         elm_box_align_set(box, 0.5, 0.0);
         elm_object_content_set(textblock_frame, box);

         pd_textblock.text = prop_item_state_text_add(box, pd, NULL,
                           _("Set the text of part."), NULL);
         pd_textblock.min = prop_item_state_text_min_x_y_add(box, pd,
                           _("When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text."),
                           _("When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text."));
         pd_textblock.max = prop_item_state_text_max_x_y_add(box, pd,
                           _("When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text."),
                           _("When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text."));

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

   prop_box = elm_object_content_get(pd->visual);
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
   ewe_entry_entry_set(entry, selected);
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

ITEM_1ENTRY_STATE_CREATE(_("image"), state, image)
ITEM_IM_BORDER_STATE_CREATE(_("border"), state_image, border)

static Eina_Bool
ui_property_state_image_set(Evas_Object *property)
{
   Evas_Object *image_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is IMAGE too, unpack it */
   ui_property_state_image_unset(property);
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_image.frame)
     {
        FRAME_ADD(property, image_frame, true, _("Image"))
        BOX_ADD(image_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(image_frame, box);

        pd_image.normal = prop_item_state_image_add(box, pd, _on_state_image_choose,
                             _("Current image name"),
                             _("Change image"));
        Evas_Object *entry = elm_object_part_content_get(pd_image.normal, "elm.swallow.content");
        ewe_entry_regex_set(entry, IMAGE_NAME_REGEX, EWE_REG_ICASE | EWE_REG_EXTENDED);
        ewe_entry_regex_autocheck_set(entry, true);
        ewe_entry_regex_glow_set(entry, true);
        pd_image.border = prop_item_state_image_border_add(box, pd,
                             _("Image's border value"));
        entry = elm_object_part_content_get(pd_image.border, "elm.swallow.content");
        ewe_entry_regex_set(entry, IMAGE_BORDER_REGEX, EWE_REG_EXTENDED);
        ewe_entry_regex_autocheck_set(entry, true);
        ewe_entry_regex_glow_set(entry, true);

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

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_image.frame);
   evas_object_hide(pd_image.frame);
}
#undef pd_image

static void
_on_state_color_class_change(void *data,
                             Evas_Object *obj EINA_UNUSED,
                             void *event_info)
{
   Prop_Data *pd = (Prop_Data *)data;
   int r, g, b, a, r1, g1, b1, a1, r2, g2, b2, a2;
   r = g = b = a = r1 = g1 = b1 = a1 = r2 = g2 = b2 = a2 = 0;

   Ewe_Combobox_Item *item = event_info;
   if (strcmp(item->title, "None"))
     {
        edje_edit_state_color_class_set(pd->style->obj, pd->part->name,
                                     pd->part->curr_state,
                                     pd->part->curr_state_value,
                                     item->title);
        edje_edit_color_class_colors_get(pd->style->obj, item->title, &r, &g, &b, &a,
                                         &r1, &g1, &b1, &a1, &r2, &g2, &b2, &a2);
        edje_edit_state_color_set(pd->style->obj, pd->part->name,
                             pd->part->curr_state, pd->part->curr_state_value,
                             r, g, b, a);
        edje_edit_state_color2_set(pd->style->obj, pd->part->name,
                             pd->part->curr_state, pd->part->curr_state_value,
                             r1, g1, b1, a1);
        edje_edit_state_color3_set(pd->style->obj, pd->part->name,
                             pd->part->curr_state, pd->part->curr_state_value,
                             r2, g2, b2, a2);

        prop_item_state_color_update(pd->prop_state.color, pd);
        prop_item_state_color2_update(pd->prop_state_text.color2, pd);
        prop_item_state_color3_update(pd->prop_state_text.color3, pd);
     }
   else edje_edit_state_color_class_set(pd->style->obj, pd->part->name,
                                        pd->part->curr_state,
                                        pd->part->curr_state_value,
                                        NULL);

   workspace_edit_object_recalc(pd->workspace);
   pd->style->isModify = true;
}


#undef PROP_DATA
#undef PROP_DATA_GET
