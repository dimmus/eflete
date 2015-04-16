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
#include "ui_property.h"
#include "ui_property_macro_old.h"
#include "main_window.h"

#ifndef HAVE_ENVENTOR
#include "syntax_color.h"
#endif

#define PROP_DATA "prop_data"

/* TODO: I dont know why this regex not work properly in the ewe entry.
 * It test in the elm_entry - works good, in online checkers - work */
#define PROPERTY_REGEX_STATE_FONT "^\\w+(:(style|slant|weight|width|spacing|lang)=\\w+)?"

#define PROP_DATA_GET(ret) \
   Prop_Data *pd = evas_object_data_get(property, PROP_DATA); \
   if (!pd) \
     { \
        ERR("Can not show group data, because object[%p] is not a property object", property); \
        return ret; \
     }

/*
 * Callback is added for frames at property box to correct scroller
 * work while each frame would be expanded/collapsed
 */
#define FRAME_PROPERTY_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE, SCROLLER) \
FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
evas_object_smart_callback_add(FRAME, "clicked", _on_frame_click, SCROLLER);

struct _Prop_Data
{
   Evas_Object *workspace;
   Style *wm_style;
   Part *wm_part;
   Ewe_Tabs_Item *visual_tab;
   Evas_Object *visual;
   Evas_Object *code;
   Eina_Stringshare *item_name;
#ifndef HAVE_ENVENTOR
   color_data *color_data;
   Eina_Strbuf *strbuf;
#else
   Evas_Object *code_bg;
#endif
   struct {
      Evas_Object *frame;
      Evas_Object *info;
      Evas_Object *shared_check;
      Evas_Object *ctxpopup;
      Evas_Object *min_w, *min_h;
      Evas_Object *max_w, *max_h;
      Evas_Object *current;
   } group;
   struct {
      Evas_Object *frame;
      Evas_Object *name;
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
      Evas_Object *proxy_source;
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
   } state;
   struct {
      Evas_Object *frame;
      Evas_Object *rel1_relative;
      Evas_Object *rel1_offset;
      Evas_Object *rel1_to;
      Evas_Object *rel2_relative;
      Evas_Object *rel2_offset;
      Evas_Object *rel2_to;
   } state_object_area;
   struct {
      Evas_Object *frame;
      Evas_Object *text;
      Evas_Object *font;
      Evas_Object *size;
      Evas_Object *align;
      Evas_Object *source;
      Evas_Object *ellipsis;
      Evas_Object *min;
      Evas_Object *max;
      Evas_Object *fit;
      Evas_Object *text_source; /* not implemented in yet the edje */
      Evas_Object *color2;
      Evas_Object *color3;
      Evas_Object *effect;
   } state_text;
   struct {
      Evas_Object *frame;
      Evas_Object *text;
      Evas_Object *style; /* not implemented in yet the edje  */
      Evas_Object *align;
      Evas_Object *min;
      Evas_Object *max;
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
      Evas_Object *normal;
      Evas_Object *border;
      Evas_Object *middle;
      Evas_Object *tween;
   } state_image;
   struct {
      Evas_Object *frame;
      Evas_Object *type;
      Evas_Object *smooth;
      Evas_Object *origin_relative;
      Evas_Object *origin_offset;
      Evas_Object *size_relative;
      Evas_Object *size_offset;
   } state_fill;
   struct {
      Evas_Object *frame;
      Evas_Object *align;
      Evas_Object *padding;
      Evas_Object *min;
   } state_container;
   struct {
      Evas_Object *frame;
      Evas_Object *layout;
      Evas_Object *custom_layout;
      Evas_Object *alt_layout;
      Evas_Object *custom_alt_layout;
   } state_box;
   struct {
      Evas_Object *frame;
      Evas_Object *name;
      Evas_Object *source;
      Evas_Object *min;
      Evas_Object *max;
      Evas_Object *spread;
      Evas_Object *prefer;
      Evas_Object *padding;
      Evas_Object *align;
      Evas_Object *weight;
      Evas_Object *aspect;
      Evas_Object *aspect_mode;
      Evas_Object *position; /* Only for items in part TABLE */
      Evas_Object *span; /* Only for items in part TABLE */
   } part_item;
   struct {
      Evas_Object *frame;
      Evas_Object *homogeneous;
   } state_table;
};
typedef struct _Prop_Data Prop_Data;

enum Property_Combo
{
   CLIP_TO,
   FORWARD_EVENTS,
   SOURCE,
   DRAG_AREA,
   COLOR_CLASS,
   TEXT_STYLE,
   TEXT_SOURCE
};

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

static const char *edje_text_effect_direction[] = { N_("None"),
                                                    N_("Bottom Right"),
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
                                          N_("Solid")};

static const char *edje_homogeneous[] = { N_("None"),
                                          N_("Table"),
                                          N_("Item"),
                                          NULL};

static const char *edje_box_layouts[] = { N_("None"),
                                          N_("horizontal"),
                                          N_("vertical"),
                                          N_("horizontal_homogeneous"),
                                          N_("vertical_homogeneous"),
                                          N_("horizontal_max"),
                                          N_("vertical_max"),
                                          N_("horizontal_flow"),
                                          N_("vertical_flow"),
                                          N_("stack"),
                                          N_("Custom Layout"),
                                          NULL};

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

static Eina_Bool
ui_property_state_table_set(Evas_Object *property);

static void
ui_property_state_table_unset(Evas_Object *property);

static Eina_Bool
ui_property_state_box_set(Evas_Object *property);

static void
ui_property_state_box_unset(Evas_Object *property);

static Elm_Genlist_Item_Class *_itc_tween = NULL;

static void
_on_spinner_mouse_wheel(void *data __UNUSED__,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info)
{
   Evas_Event_Mouse_Wheel *mev = event_info;
   mev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
}

/* TODO: remove this hack after scroller would be fixed
 * Hack start
 */
static void
_on_frame_click(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   Evas_Object *scroller = (Evas_Object *)data;
   Evas_Object *box, *frame_box;
   int h_box, h_frame_box, h_scr, y_reg, h_reg, y_frame;
   box = elm_object_content_get(scroller);
   evas_object_geometry_get(scroller, NULL, NULL, NULL, &h_scr);
   evas_object_geometry_get(box, NULL, NULL, NULL, &h_box);
   frame_box = elm_object_content_get(obj);
   evas_object_geometry_get(frame_box, NULL, &y_frame, NULL, &h_frame_box);
   elm_scroller_region_get(scroller, NULL, &y_reg, NULL, &h_reg);
   elm_scroller_region_bring_in(scroller, 0.0, y_reg + 1, 0.0, h_reg);
   if (!elm_frame_collapse_get(obj))
     {
        if (h_box == h_scr + y_reg)
          elm_scroller_region_show(scroller, 0.0, y_reg + h_frame_box, 0.0, h_reg);
        else
          elm_scroller_region_bring_in(scroller, 0.0, y_reg + 1, 0.0, h_reg);
     }
}
/* Hack end */

static void
_del_prop_data(void *data,
               Evas *e __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *ei __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;
#ifndef HAVE_ENVENTOR
   color_term(pd->color_data);
   eina_strbuf_free(pd->strbuf);
#endif
   free(pd);
}

static Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    Evas_Object **label,
                    const char *lab_text,
                    const char *text)
{
   PROPERTY_ITEM_ADD(parent, lab_text, "1swallow")
   LABEL_ADD(item, *label, text)
   elm_object_part_content_set(item, "elm.swallow.content", *label);
   return item;
}

#define prop_part_type_add(PARENT, NAME, TEXT) prop_item_label_add(PARENT, &pd->part.type, NAME, TEXT)
#define prop_state_state_add(PARENT, NAME, TEXT) prop_item_label_add(PARENT, &pd->state.state, NAME, TEXT)
#define prop_part_item_name_add(PARENT, NAME, TEXT) prop_item_label_add(PARENT, &pd->part_item.name, NAME, TEXT)

#define prop_part_type_update(TEXT) elm_object_text_set(pd->part.type, TEXT)
#define prop_state_state_update(TEXT) elm_object_text_set(pd->state.state, TEXT)
#define prop_part_item_name_update(TEXT) elm_object_text_set(pd->part_item.name, TEXT)

#ifdef HAVE_ENVENTOR

static void
_on_tab_activated(void *data,
                  Evas_Object *obj,
                  void *event_info)
{
   App_Data *ap;
   Ewe_Tabs_Item *it = (Ewe_Tabs_Item *) event_info;
   Prop_Data *pd = (Prop_Data *)data;

   ap = app_data_get();

   Eina_Stringshare *item_name = ewe_tabs_item_title_get(obj, it);

   if (!item_name) return;

   if (!strcmp(item_name, "Code"))
     {
        if (!pd->code_bg)
          {
             pd->code = ap->enventor;
             pd->code_bg = elm_bg_add(obj);
             elm_bg_color_set(pd->code_bg, ENVENTOR_CODE_BG_COLOR);
             elm_object_part_content_set(pd->code_bg, "elm.swallow.content", pd->code);
             ewe_tabs_item_content_set(obj, it, pd->code_bg);
          }
        code_edit_mode_switch(ap, true);

        enventor_object_project_load(ap->enventor, ap->project);

        ui_menu_disable_set(ap->menu, MENU_FILE_SAVE, false);
        pm_project_changed(ap->project);
        evas_object_smart_callback_call(ap->enventor, "enventor,mode,on", NULL);
     }
   else
     {
        code_edit_mode_switch(ap, false);
        evas_object_smart_callback_call(ap->enventor, "enventor,mode,off",
                                        ap->project->current_style);
     }

   ap->enventor_mode = !ap->enventor_mode;
}

#else

static void
_code_of_group_setup(Prop_Data *pd)
{
   char *markup_code;
   const char *colorized_code;
   Eina_Stringshare *code;

   code = edje_edit_source_generate(pd->wm_style->obj);
   if (!code)
     ERR("Something wrong. Can not generate code for style %s", pd->wm_style->name);
   markup_code = elm_entry_utf8_to_markup(code);
   colorized_code = color_apply(pd->color_data, markup_code,
                                strlen(markup_code), NULL, NULL);
   if (colorized_code) elm_object_text_set(pd->code, colorized_code);
   evas_object_show(pd->code);
   free(markup_code);
   eina_stringshare_del(code);
}
#endif

Evas_Object *
ui_property_add(Evas_Object *parent)
{
   Evas_Object *box, *scroller, *tabs;
   Ewe_Tabs_Item *it;
   Prop_Data *pd;

   if (!parent) return NULL;
   pd = mem_calloc(1, sizeof(Prop_Data));
   tabs = ewe_tabs_add(parent);

   SCROLLER_ADD(tabs, scroller);
   BOX_ADD(scroller, box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);

   pd->visual = scroller;
   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   it = ewe_tabs_item_append(tabs, NULL, _("Visual"), NULL);
   ewe_tabs_item_content_set(tabs, it, pd->visual);
   pd->visual_tab = it;

   it = ewe_tabs_item_append(tabs, it, _("Code"), NULL);

#ifdef HAVE_ENVENTOR
   evas_object_smart_callback_add(tabs, "ewe,tabs,item,activated",
                                  _on_tab_activated, pd);
#else
   pd->code = elm_entry_add(tabs);
   elm_entry_single_line_set(pd->code, false);
   elm_scroller_policy_set(pd->code, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_ON);
   evas_object_size_hint_weight_set(pd->code, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pd->code, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_entry_scrollable_set(pd->code, true);
   elm_entry_editable_set(pd->code, false);
   pd->strbuf = eina_strbuf_new();
   pd->color_data = color_init(pd->strbuf);
   ewe_tabs_item_content_set(tabs, it, pd->code);
#endif

   evas_object_data_set(tabs, PROP_DATA, pd);
   evas_object_event_callback_add(tabs, EVAS_CALLBACK_DEL, _del_prop_data, pd);

   return tabs;
}

#define GROUP_ATTR_2SPINNER(TEXT, SUB1, SUB2, VALUE1, VALUE2, CHECK) \
   GROUP_ATTR_2SPINNER_CALLBACK(SUB1, SUB2, VALUE1, CHECK) \
   GROUP_ATTR_2SPINNER_CALLBACK(SUB1, SUB2, VALUE2, CHECK) \
   GROUP_ATTR_2SPINNER_UPDATE(SUB1, VALUE1, VALUE2) \
   GROUP_ATTR_2SPINNER_ADD(TEXT, SUB1, VALUE1, VALUE2)

GROUP_ATTR_2SPINNER(_("min"), min, max, w, h, >)
GROUP_ATTR_2SPINNER(_("max"), max, min, w, h, <)

#define pd_group pd->group

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
   Prop_Data *pd = (Prop_Data *)data;
   evas_object_smart_callback_del_full(pd_group.ctxpopup, "dismissed",
                                       _on_dismissed, pd);
   evas_object_smart_callback_add(pd_group.ctxpopup, "dismissed",
                                  _on_dismissed, pd);
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
   if (style->isAlias)
     {
        text_info = eina_stringshare_add("This is alias of <a>%s</a>");
        text_info = eina_stringshare_printf(text_info,
                                            style->main_group->full_group_name);
     }
   else
     {
        list = edje_edit_group_aliases_get(style->obj, style->full_group_name);
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
                                       _on_clicked, pd);
        evas_object_event_callback_add(pd_group.info, EVAS_CALLBACK_MOUSE_DOWN,
                                       _on_clicked, pd);
        elm_object_text_set(label_ctx, eina_strbuf_string_get(text_ctx));
        eina_strbuf_free(text_ctx);
        edje_edit_string_list_free(list);
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
   Evas_Object *item;
   Evas_Object *info_image;
   Evas_Object *check, *label_ctx;
   Eina_List *aliases = NULL, *l;
   const char *text_info = NULL;
   int aliases_count = 0;
   char *list_data;
   Eina_Strbuf *text_ctx = NULL;

   if ((!property) || (!workspace)) return EINA_FALSE;
   ui_property_style_unset(property);
   PROP_DATA_GET(EINA_FALSE)

   evas_object_show(property);

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);

   ewe_tabs_active_item_set(property, pd->visual_tab);

   pd->workspace = workspace;
   pd->wm_style = style;
   if (style->isAlias) pd->wm_style = style->main_group;
   if (pd->wm_style != workspace_edit_object_get(workspace))
     {
        ERR("Cann't set the style! Style [%p] not matched"
            " with editable group in workspace", style);
        return false;
     }

#ifndef HAVE_ENVENTOR
   _code_of_group_setup(pd);
#endif

   prop_box = elm_object_content_get(pd->visual);
   aliases = edje_edit_group_aliases_get(style->obj, style->full_group_name);
   aliases_count = eina_list_count(aliases);

   if (!pd_group.info)
     {
        LABEL_ADD(property, info_en, text_info)

        GET_IMAGE(info_image, property, "icon-notification");
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
        elm_object_style_set(pd_group.ctxpopup, "info");
        evas_object_hide(pd_group.ctxpopup);

        if (style->isAlias)
          text_info = eina_stringshare_printf("This is alias of <a>%s</a>",
                                              style->main_group->full_group_name);
        else
          {
             text_info = eina_stringshare_printf("changes in this style will also"
                                                 "affect <a>%d elements.</a>",
                                                 aliases_count);
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
                                            _on_clicked, pd);
             LABEL_ADD(property, label_ctx, eina_strbuf_string_get(text_ctx))
             eina_strbuf_free(text_ctx);
             elm_object_style_set(label_ctx, "info");
             elm_object_content_set(pd_group.ctxpopup, label_ctx);
          }

        elm_object_text_set(info_en, text_info);
        elm_object_style_set(info_en, "info");
        elm_object_part_content_set(pd_group.info, "elm.swallow.content", info_en);
        elm_object_part_content_set(pd_group.info, "info", info_image);
     }

   if (!pd_group.shared_check)
     {
        CHECK_ADD(property, check)
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
        FRAME_PROPERTY_ADD(property, group_frame, true, _("Layout property"), pd->visual)
        BOX_ADD(group_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(group_frame, box);

        item = prop_group_min_w_h_add(box, pd,
                                      _("Minimum group width in pixels."),
                                      _("Minimum group height in pixels."));
        elm_box_pack_end(box, item);
        item = prop_group_max_w_h_add(box, pd,
                                      _("Maximum group width in pixels."),
                                      _("Maximum group height in pixels."));
        elm_box_pack_end(box, item);

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
        prop_group_min_w_h_update(pd);
        prop_group_max_w_h_update(pd);
        evas_object_show(pd_group.frame);
     }
   if ((aliases_count > 0) || (style->isAlias))
     {
        elm_box_pack_start(prop_box, pd_group.info);
        evas_object_show(pd_group.info);
     }
   elm_box_pack_start(prop_box, pd_group.shared_check);

   edje_edit_string_list_free(aliases);

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
                                  _on_clicked, pd);
   evas_object_hide(pd_group.frame);
   evas_object_hide(pd_group.shared_check);
   ui_property_part_unset(property);
   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

#ifdef HAVE_ENVENTOR
   elm_object_part_content_unset(pd->code_bg, "elm.swallow.content");
   evas_object_del(pd->code_bg);
   evas_object_hide(pd->code);
   pd->code_bg = NULL;
   pd->code = NULL;
#else
  evas_object_hide(pd->code);
#endif

  evas_object_hide(property);
}
#undef pd_group

static void
_on_part_name_unfocus(void *data,
                      Evas_Object *obj,
                      void *ei __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;

   const char *value = elm_entry_entry_get(obj);

   if (strcmp(value, pd->wm_part->name))
     elm_entry_entry_set(obj, pd->wm_part->name);
}

static void
_on_part_name_change(void *data,
                     Evas_Object *obj,
                     void *ei __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;
   int pos;
   const char *value;
   const char *old_value = pd->wm_part->name;

   if (elm_entry_is_empty(obj)) return;

   value = elm_entry_entry_get(obj);
   if (!edje_edit_part_name_set(pd->wm_style->obj, pd->wm_part->name, value))
     {
        NOTIFY_INFO(5, "Wrong input value for name field");
        return;
     }

   project_changed();
   workspace_edit_object_part_rename(pd->workspace, pd->wm_part->name, value);
   pd->wm_part->name = value;
   pd->wm_style->isModify = true;
   pos = elm_entry_cursor_pos_get(obj);
   evas_object_smart_callback_call(pd->workspace, "part,name,changed", pd->wm_part);
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_RENAME, old_value, value,
                      pd->wm_style->full_group_name,
                      (void*)edje_edit_part_name_set, "rename",
                      pd->wm_part->name, NULL, 0.0);
   elm_object_focus_set(obj, true);
   elm_entry_cursor_pos_set(obj, pos);
   workspace_edit_object_recalc(pd->workspace);
}

static void
prop_part_name_update(Prop_Data *pd)
{
   elm_entry_entry_set(pd->part.name, pd->wm_part->name);
}

static Evas_Object *
prop_item_part_name_add(Evas_Object *parent,
                        Prop_Data *pd,
                        const char *tooltip)
{
   PROPERTY_ITEM_ADD(parent,  _("name"), "1swallow");
   EWE_ENTRY_ADD(parent, pd->part.name, true);
   elm_entry_markup_filter_append(pd->part.name, elm_entry_filter_accept_set, &accept_prop);
   ewe_entry_entry_set(pd->part.name, pd->wm_part->name);
   elm_object_tooltip_text_set(pd->part.name, tooltip);
   evas_object_smart_callback_add(pd->part.name, "changed,user", _on_part_name_change, pd);
   evas_object_smart_callback_add(pd->part.name, "unfocused", _on_part_name_unfocus, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->part.name);
   prop_part_name_update(pd);

   return item;
}

#define ITEM_ATTR_1CHECK_UPDATE(SUB, VALUE) \
   elm_check_state_set(pd->SUB.VALUE, edje_edit_part_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name));

static void
prop_part_clip_to_update(Prop_Data *pd,
                         Evas_Object *combobox __UNUSED__)
{
   Part *part;
   Eina_Inlist *list_n = NULL;
   Eina_Stringshare *value;

   ewe_combobox_items_list_free(pd->part.clip_to, true);
   value = edje_edit_part_clip_to_get(pd->wm_style->obj, pd->wm_part->name);
   if (value)
     ewe_combobox_text_set(pd->part.clip_to, value);
   else
     ewe_combobox_text_set(pd->part.clip_to, _("None"));
   ewe_combobox_item_add(pd->part.clip_to, _("None"));
   EINA_INLIST_FOREACH_SAFE(pd->wm_style->parts, list_n, part)
     {
        if ((part != pd->wm_part) && (part->type == EDJE_PART_TYPE_RECTANGLE))
           ewe_combobox_item_add(pd->part.clip_to, part->name);
     }
   edje_edit_string_free(value);
}

/* avaliable only for SOURCE part type */
static void
prop_part_source_update(Prop_Data *pd, Evas_Object *combobox)
{
   Eina_List *collections, *l;
   const char *group, *value;
   App_Data *ap = app_data_get();
   unsigned int i = 0;

   ewe_combobox_items_list_free(combobox, true);
   value = edje_edit_part_clip_to_get(pd->wm_style->obj, pd->wm_part->name);
   if (value)
     ewe_combobox_text_set(combobox, value);
   else
     ewe_combobox_text_set(combobox, _("None"));
   ewe_combobox_item_add(combobox, _("None"));
   collections = edje_mmap_collection_list(ap->project->mmap_file);
   EINA_LIST_FOREACH(collections, l, group)
     {
        if (group != pd->wm_style->full_group_name)
          ewe_combobox_item_add(combobox, group);
        if (group == value)
          pd->part.previous_source = i;
        i++;
     }
   edje_edit_string_free(value);
   edje_mmap_collection_list_free(collections);
}

inline static void
prop_part_drag_control_disable_set(Prop_Data *pd, Eina_Bool collapse)
{
   Eina_Bool bx, by;

   bx = edje_edit_part_drag_x_get(pd->wm_style->obj, pd->wm_part->name);
   by = edje_edit_part_drag_y_get(pd->wm_style->obj, pd->wm_part->name);

   elm_object_disabled_set(pd->part_drag.step_x, !bx);
   elm_object_disabled_set(pd->part_drag.step_y, !by);
   elm_object_disabled_set(pd->part_drag.confine, !(bx | by));
   elm_object_disabled_set(pd->part_drag.threshold, !(bx | by));

   if (collapse)
     elm_frame_collapse_set(pd->part_drag.frame, !(bx | by));
}

PART_ATTR_PARTS_LIST(part_drag, confine, part_drag)
PART_ATTR_PARTS_LIST(part_drag, threshold, part_drag)
PART_ATTR_PARTS_LIST(part_drag, event, part_drag)

#define PART_ATTR_1CHECK(TEXT, SUB, VALUE) \
   PART_ATTR_1CHECK_CALLBACK(SUB, VALUE) \
   PART_ATTR_1CHECK_ADD(TEXT, SUB, VALUE)

#define PART_ATTR_1COMBOBOX(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
   PART_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER) \
   PART_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

#define PART_ATTR_1COMBOBOX_LIST(TEXT, SUB, VALUE, TYPE) \
   PART_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE) \
   PART_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE)

#define PART_ATTR_DRAG(TEXT, SUB, VALUE1, VALUE2) \
   PART_ATTR_DRAG_CALLBACK(SUB, VALUE1, VALUE2) \
   PART_ATTR_DRAG_UPDATE(SUB, VALUE1, VALUE2) \
   PART_ATTR_DRAG_ADD(TEXT, SUB, VALUE1, VALUE2)

/* part property */
PART_ATTR_1CHECK(_("scalable"), part, scale)
PART_ATTR_1CHECK(_("mouse events"), part, mouse_events)
PART_ATTR_1CHECK(_("event propagation"), part, repeat_events)
PART_ATTR_1COMBOBOX(_("clipper"), part, clip_to, part,
                    _("Show only the area of part that coincides with another part's container"))
PART_ATTR_1COMBOBOX_LIST(_("ignore flags"), part, ignore_flags, Evas_Event_Flags)
PART_ATTR_1COMBOBOX(_("group source"), part, source, part,
                    _("Used for the group to be loaded and used to display GROUP part."))
/* part drag property */
PART_ATTR_DRAG(_("axis X"), part_drag, x, step_x)
PART_ATTR_DRAG(_("axis Y"), part_drag, y, step_y)
PART_ATTR_1COMBOBOX(_("drag area"), part_drag, confine, part_drag,
                    _("Limits the movement of the dragged part to another part's container"))
PART_ATTR_1COMBOBOX(_("threshold"), part_drag, threshold, part_drag,
                    _("Movement of the dragged part can only start when it get moved enough to be outside of this threshold part"))
PART_ATTR_1COMBOBOX(_("forward events"), part_drag, event, part_drag,
                    _("It causes the part to forward the drag events "
                    "to another part, thus ignoring them for itself."))

#define pd_part pd->part
#define pd_drag pd->part_drag
Eina_Bool
ui_property_part_set(Evas_Object *property, Part *part)
{
   Evas_Object *item;
   Evas_Object *box, *prop_box;
   int y_reg, h_reg, h_box;

   if ((!property) || (!part)) return EINA_FALSE;
   PROP_DATA_GET(EINA_FALSE)

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
   pd->wm_part = part;
   prop_box = elm_object_content_get(pd->visual);

   if (!pd_part.frame)
     {
        FRAME_PROPERTY_ADD(pd->visual, pd_part.frame, true, _("Part property"), pd->visual)
        BOX_ADD(pd_part.frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd_part.frame, box);

        item = prop_item_part_name_add(box, pd, _("Selected part name"));
        elm_box_pack_end(box, item);
        item = prop_part_type_add(box, _("type"), wm_part_type_get(pd->wm_part->type));
        elm_box_pack_end(box, item);
        pd->part.scale_item = prop_part_scale_add(box, pd,
                           _("Specifies whether the part will scale "
                           "its size with an edje scaling factor."));
        elm_box_pack_end(box, pd->part.scale_item);
        pd->part.mouse_events_item = prop_part_mouse_events_add(box, pd,
                           _("Enable mouse events in this part."));
        elm_box_pack_end(box, pd->part.mouse_events_item);
        pd->part.repeat_events_item = prop_part_repeat_events_add(box, pd,
                            _("Enable repeat mouse events to the parts below."));
        elm_box_pack_end(box, pd->part.repeat_events_item);
        pd->part.clip_to_item = prop_part_clip_to_add(box, pd);
        elm_box_pack_end(box, pd->part.clip_to_item);
        pd->part.ignore_flags_item = prop_part_ignore_flags_add(box, pd,
                                  _("Specifies whether events with the given "
                                  " flags should be ignored"), edje_ignore_flags);
        elm_box_pack_end(box, pd->part.ignore_flags_item);
        pd->part.source_item = prop_part_source_add(box, pd);
        elm_box_pack_end(box, pd->part.source_item);

        FRAME_PROPERTY_ADD(pd->visual, pd_drag.frame, true, _("Part dragable property"), pd->visual)
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

        elm_box_pack_after(prop_box, pd_part.frame, pd->group.frame);
     }
   else
     {
        prop_part_name_update(pd);
        prop_part_type_update(wm_part_type_get(pd->wm_part->type));
        ITEM_ATTR_1CHECK_UPDATE(part, scale)
        ITEM_ATTR_1CHECK_UPDATE(part, mouse_events)
        ITEM_ATTR_1CHECK_UPDATE(part, repeat_events)
        prop_part_clip_to_update(pd, NULL);
        PART_ATTR_1COMBOBOX_LIST_UPDATE(part, ignore_flags)
        prop_part_source_update(pd, pd->part.source);

        prop_part_drag_x_step_x_update(pd);
        prop_part_drag_y_step_y_update(pd);
        prop_part_drag_confine_update(pd, pd->part_drag.confine);
        prop_part_drag_threshold_update(pd, pd->part_drag.threshold);
        prop_part_drag_event_update(pd, pd->part_drag.event);
     }

   box = elm_object_content_get(pd_part.frame);
   if (pd->wm_part->type == EDJE_PART_TYPE_SPACER)
     {
        /* Unpack from box, if selected part is SPACER and hide.
         * Check the visibility of 'mouse_events_item', to identify
         * previous selected part is SPACER or not. For identification
         * enough to check only one attribute, which not available
         * for SPACER */
        if (evas_object_visible_get(pd->part.mouse_events_item))
          {
             elm_box_unpack(box, pd->part.mouse_events_item);
             elm_box_unpack(box, pd->part.repeat_events_item);
             elm_box_unpack(box, pd->part.clip_to_item);
             elm_box_unpack(box, pd->part.ignore_flags_item);
             elm_box_unpack(box, pd->part_drag.frame);
             evas_object_hide(pd->part.mouse_events_item);
             evas_object_hide(pd->part.repeat_events_item);
             evas_object_hide(pd->part.clip_to_item);
             evas_object_hide(pd->part.ignore_flags_item);
             evas_object_hide(pd->part_drag.frame);
          }
     }
   else
     {
        if (!evas_object_visible_get(pd->part.mouse_events_item))
          {
             elm_box_pack_after(box, pd->part.mouse_events_item, pd->part.scale_item);
             elm_box_pack_after(box, pd->part.repeat_events_item, pd->part.mouse_events_item);
             elm_box_pack_after(box, pd->part.clip_to_item, pd->part.repeat_events_item);
             elm_box_pack_after(box, pd->part.ignore_flags_item, pd->part.clip_to_item);
             elm_box_pack_end(box, pd_drag.frame);
             evas_object_show(pd->part.mouse_events_item);
             evas_object_show(pd->part.repeat_events_item);
             evas_object_show(pd->part.clip_to_item);
             evas_object_show(pd->part.ignore_flags_item);
             evas_object_show(pd->part_drag.frame);
          }
     }
     if (pd->wm_part->type == EDJE_PART_TYPE_GROUP)
       {
         /* pack to box, only in case when the previous selected part
          * is not GROUP. If previos selected part is GROUP then
          * this item is show and already packed to box  */
         if (!evas_object_visible_get(pd_part.source_item))
           {
              elm_box_pack_after(box, pd_part.source_item, pd->part.ignore_flags_item);
              evas_object_show(pd->part.source_item);
           }
       }
     else
       {
          elm_box_unpack(box, pd_part.source_item);
          evas_object_hide(pd_part.source_item);
       }

   prop_part_drag_control_disable_set(pd, true);
   evas_object_geometry_get(prop_box, NULL, NULL, NULL, &h_box);
   elm_scroller_region_get(pd->visual, NULL, &y_reg, NULL, &h_reg);
   elm_scroller_region_bring_in(pd->visual, 0.0, y_reg + 1, 0.0, h_reg);
   if (h_box == h_reg + y_reg)
     elm_scroller_region_show(pd->visual, 0.0, y_reg + h_box, 0.0, h_reg);

   return true;
}

#define PROP_ITEM_UNSET(BOX, ITEM) \
   if (ITEM) \
     {\
        evas_object_smart_callback_del(ITEM, "clicked", _on_frame_click); \
        elm_box_unpack(BOX, ITEM); \
        evas_object_del(ITEM); \
        ITEM = NULL; \
     }

void
ui_property_part_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   if (!property) return;
   PROP_DATA_GET()

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   prop_box = elm_object_content_get(pd->visual);

   PROP_ITEM_UNSET(prop_box, pd->part.frame)
   PROP_ITEM_UNSET(prop_box, pd->state.frame)
   evas_object_del(pd->state.color1);
   evas_object_del(pd->state.color2);
   evas_object_del(pd->state.color3);
   PROP_ITEM_UNSET(prop_box, pd->state_object_area.frame)
   PROP_ITEM_UNSET(prop_box, pd->state_text.frame)
   PROP_ITEM_UNSET(prop_box, pd->state_image.frame)
   PROP_ITEM_UNSET(prop_box, pd->state_textblock.frame)
   PROP_ITEM_UNSET(prop_box, pd->state_fill.frame)
   PROP_ITEM_UNSET(prop_box, pd->state_container.frame)
   PROP_ITEM_UNSET(prop_box, pd->state_table.frame)
   PROP_ITEM_UNSET(prop_box, pd->state_box.frame)
   PROP_ITEM_UNSET(prop_box, pd->part_item.frame)
}

#undef PROP_ITEM_UNSET
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
   Prop_Data *pd = (Prop_Data *)data;
   int r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3;
   Eina_Stringshare *old_value = NULL, *value = NULL;
   Ewe_Combobox_Item *item = event_info;

   old_value = edje_edit_state_color_class_get(pd->wm_style->obj, pd->wm_part->name,
                                               pd->wm_part->curr_state,
                                               pd->wm_part->curr_state_value);
   value = strcmp(item->title, "None") ? item->title : NULL;
   edje_edit_state_color_class_set(pd->wm_style->obj, pd->wm_part->name,
                                   pd->wm_part->curr_state,
                                   pd->wm_part->curr_state_value,
                                   value);

   if (edje_edit_color_class_colors_get(pd->wm_style->obj, item->title,
                                        &r, &g, &b, &a,
                                        &r2, &g2, &b2, &a2,
                                        &r3, &g3, &b3, &a3))
     {
        evas_object_color_set(pd->state.color1, r * a / 255, g * a / 255, b * a / 255, a);
        evas_object_color_set(pd->state.color2, r2 * a2 / 255, g2 * a2 / 255, b2 * a2 / 255, a2);
        evas_object_color_set(pd->state.color3, r3 * a3 / 255, g3 * a3 / 255, b3 * a3 / 255, a3);
     }
   else
     {
        evas_object_color_set(pd->state.color1, 0, 0, 0, 0);
        evas_object_color_set(pd->state.color2, 0, 0, 0, 0);
        evas_object_color_set(pd->state.color3, 0, 0, 0, 0);
     }

   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_STRING, old_value, value,
                    pd->wm_style->full_group_name,
                    (void*)edje_edit_state_color_class_set, "color class",
                    pd->wm_part->name, pd->wm_part->curr_state,
                    pd->wm_part->curr_state_value);

   edje_edit_string_free(value);
   edje_edit_string_free(old_value);

   project_changed();
   workspace_edit_object_recalc(pd->workspace);
   pd->wm_style->isModify = true;
}

static void
_color_class_items_fill(void *data,
                        Evas_Object *obj,
                        void *event_info __UNUSED__)
{
   int r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3;
   Eina_List *items, *l;
   Ewe_Combobox_Item *item;
   Prop_Data *pd = (Prop_Data *)data;
   Evas_Object *color;
   Evas *canvas;

   items = (Eina_List *)ewe_combobox_items_list_get(obj);
   if (!items) return;

   //get combobbox item from first list item
   item = eina_list_data_get(items);
   canvas = evas_object_evas_get(item->content);
   EINA_LIST_FOREACH(items, l, item)
     {
        // this case available if user select 'None'
        if (!edje_edit_color_class_colors_get(pd->wm_style->obj, item->title,
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
prop_state_color_class_update(Prop_Data *pd)
{
   Ewe_Combobox_Item *it;
   Eina_List *cclist, *l;
   const char *ccname;
   int r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3;
   Eina_Stringshare *color_c;
   ewe_combobox_items_list_free(pd->state.color_class, true);
   ewe_combobox_item_add(pd->state.color_class, _("None"));
   cclist = edje_edit_color_classes_list_get(pd->wm_style->obj);
   EINA_LIST_FOREACH(cclist, l, ccname)
     {
        it = ewe_combobox_item_add(pd->state.color_class, ccname);
        ewe_combobox_item_style_set(pd->state.color_class, it, "color_class");
     }
   color_c = edje_edit_state_color_class_get(pd->wm_style->obj, pd->wm_part->name,
                                             pd->wm_part->curr_state,
                                             pd->wm_part->curr_state_value);
   ewe_combobox_text_set(pd->state.color_class, color_c ? color_c : _("None"));
   if (color_c && edje_edit_color_class_colors_get(pd->wm_style->obj, color_c,
                                                   &r, &g, &b, &a,
                                                   &r2, &g2, &b2, &a2,
                                                   &r3, &g3, &b3, &a3))
     {
        evas_object_color_set(pd->state.color1, r * a / 255, g * a / 255, b * a / 255, a);
        evas_object_color_set(pd->state.color2, r2 * a2 / 255, g2 * a2 / 255, b2 * a2 / 255, a2);
        evas_object_color_set(pd->state.color3, r3 * a3 / 255, g3 * a3 / 255, b3 * a3 / 255, a3);
     }
   else
     {
        evas_object_color_set(pd->state.color1, 0, 0, 0, 0);
        evas_object_color_set(pd->state.color2, 0, 0, 0, 0);
        evas_object_color_set(pd->state.color3, 0, 0, 0, 0);
     }

   edje_edit_string_list_free(cclist);
   edje_edit_string_free(color_c);
}

static Evas_Object *
prop_state_color_class_add(Evas_Object *parent, Prop_Data *pd)
{
   PROPERTY_ITEM_ADD(parent, _("color class"), "1swallow")
   EWE_COMBOBOX_ADD(parent, pd->state.color_class)
   elm_object_tooltip_text_set(pd->state.color_class, "Set the color class");
   ewe_combobox_style_set(pd->state.color_class, "color_class");
   evas_object_smart_callback_add(pd->state.color_class, "selected", _on_state_color_class_change, pd);
   evas_object_smart_callback_add(pd->state.color_class, "expanded", _color_class_items_fill, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->state.color_class);
   pd->state.color1 = elm_layout_add(parent);
   elm_layout_theme_set(pd->state.color1, "image", "color", "color_set");
   ewe_combobox_content_set(pd->state.color_class, "swallow.color1", pd->state.color1);
   pd->state.color2 = elm_layout_add(parent);
   elm_layout_theme_set(pd->state.color2, "image", "color", "color_set");
   ewe_combobox_content_set(pd->state.color_class, "swallow.color2", pd->state.color2);
   pd->state.color3 = elm_layout_add(parent);
   elm_layout_theme_set(pd->state.color3, "image", "color", "color_set");
   ewe_combobox_content_set(pd->state.color_class, "swallow.color3", pd->state.color3);

   prop_state_color_class_update(pd);
   return item;
}

static void
prop_state_proxy_source_update(Prop_Data *pd,
                               Evas_Object *combobox __UNUSED__)
{
   Part *part;
   Eina_Inlist *list_n = NULL;
   Eina_Stringshare *value;

   /* clears the list, because we don't know about changes in the part list */
   ewe_combobox_items_list_free(pd->state.proxy_source, true);
   value = edje_edit_state_proxy_source_get(pd->wm_style->obj,
                                            pd->wm_part->name,
                                            pd->wm_part->curr_state,
                                            pd->wm_part->curr_state_value);
   ewe_combobox_item_add(pd->state.proxy_source, _("None"));
   ewe_combobox_text_set(pd->state.proxy_source, value ? value : _("None"));
   EINA_INLIST_FOREACH_SAFE(pd->wm_style->parts, list_n, part)
     {
        if ((part != pd->wm_part) && (part->type != EDJE_PART_TYPE_SPACER))
          ewe_combobox_item_add(pd->state.proxy_source, part->name);
     }
   edje_edit_string_free(value);
}

#define ITEM_2SPINNER_STATE_INT_CREATE(TEXT, SUB, VALUE1, VALUE2, STYLE) \
   ITEM_SPINNER_STATE_INT_CALLBACK(SUB, VALUE1) \
   ITEM_SPINNER_STATE_INT_CALLBACK(SUB, VALUE2) \
   ITEM_2SPINNER_STATE_UPDATE(SUB, VALUE1, VALUE2) \
   ITEM_2SPINNER_STATE_ADD(TEXT, SUB, VALUE1, VALUE2, STYLE)

#define ITEM_2SPINNER_STATE_DOUBLE_CREATE(TEXT, SUB, VALUE1, VALUE2, STYLE) \
   ITEM_SPINNER_STATE_DOUBLE_CALLBACK(SUB, VALUE1) \
   ITEM_SPINNER_STATE_DOUBLE_CALLBACK(SUB, VALUE2) \
   ITEM_2SPINNER_STATE_UPDATE(SUB, VALUE1, VALUE2) \
   ITEM_2SPINNER_STATE_ADD(TEXT, SUB, VALUE1, VALUE2, STYLE)

#define ITEM_1ENTRY_STATE_CREATE(TEXT, SUB, VALUE, FILTER, TYPE) \
   ITEM_STRING_STATE_CALLBACK(SUB, VALUE, TYPE) \
   ITEM_1ENTRY_STATE_ADD(TEXT, SUB, VALUE, FILTER) \
   ITEM_1ENTRY_STATE_UPDATE(SUB, VALUE)

#define ITEM_COLOR_STATE_CREATE(TEXT, SUB, VALUE) \
   ITEM_COLOR_STATE_CALLBACK(SUB, VALUE) \
   ITEM_COLOR_STATE_ADD(TEXT, SUB, VALUE) \
   ITEM_COLOR_STATE_UPDATE(SUB, VALUE)

#define ITEM_2CHECK_STATE_CREATE(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_CHECK_STATE_CALLBACK(SUB, VALUE1) \
   ITEM_CHECK_STATE_CALLBACK(SUB, VALUE2) \
   ITEM_2CHEACK_STATE_ADD(TEXT, SUB, VALUE1, VALUE2) \
   ITEM_2CHEACK_STATE_UPDATE(SUB, VALUE1, VALUE2)

#define ITEM_1COMBOBOX_PART_STATE_CREATE(TEXT, SUB, VALUE, TYPE) \
   ITEM_1COMBOBOX_STATE_PART_CALLBACK(SUB, VALUE, TYPE) \
   ITEM_1COMBOBOX_STATE_PART_UPDATE(TEXT, SUB, VALUE, TYPE) \
   ITEM_1COMBOBOX_STATE_PART_ADD(TEXT, SUB, VALUE)

#define ITEM_1COMBOBOX_STATE_PROXY_CREATE(TEXT, SUB, VALUE) \
   ITEM_COMBOBOX_STATE_CALLBACK(-1, TEXT, SUB, VALUE) \
   ITEM_1COMBOBOX_STATE_PROXY_ADD(TEXT, SUB, VALUE) \
   ITEM_1COMBOBOX_STATE_PROXY_UPDATE(SUB, VALUE)

#define STATE_ATTR_CHECK_UPDATE(SUB, VALUE) \
   elm_check_state_set(pd->SUB.VALUE, edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                                                                      pd->wm_part->name, \
                                                                      pd->wm_part->curr_state, \
                                                                      pd->wm_part->curr_state_value));

#define STATE_ATTR_2CHECK_UPDATE(SUB, VALUE1, VALUE2) \
   STATE_ATTR_CHECK_UPDATE(SUB, VALUE1) \
   STATE_ATTR_CHECK_UPDATE(SUB, VALUE2)

#define STATE_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MULTIPLIER)\
   elm_spinner_value_set(pd->SUB.VALUE1, \
                         MULTIPLIER * edje_edit_##SUB##_##VALUE1##_get(pd->wm_style->obj, \
                                                                       pd->wm_part->name, \
                                                                       pd->wm_part->curr_state, \
                                                                       pd->wm_part->curr_state_value)); \
   elm_spinner_value_set(pd->SUB.VALUE2, \
                         MULTIPLIER * edje_edit_##SUB##_##VALUE2##_get(pd->wm_style->obj, \
                                                                       pd->wm_part->name, \
                                                                       pd->wm_part->curr_state, \
                                                                       pd->wm_part->curr_state_value));


#define STATE_ATTR_1CHECK(TEXT, SUB, VALUE) \
   STATE_ATTR_1CHECK_CALLBACK(SUB, VALUE) \
   STATE_ATTR_1CHECK_ADD(TEXT, SUB, VALUE)

#define STATE_ATTR_2SPINNER(TEXT, SUB, VALUE1, VALUE2, MIN, MAX, STEP, FMT, \
                            L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER, \
                            TYPE, HISTORY_TYPE) \
   STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE1, TYPE, HISTORY_TYPE, MULTIPLIER) \
   STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE2, TYPE, HISTORY_TYPE, MULTIPLIER) \
   STATE_ATTR_2SPINNER_ADD(TEXT, SUB, VALUE1, VALUE2, MIN, MAX, STEP, FMT, \
                           L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2, MULTIPLIER)

#define STATE_ATTR_2CHECK(TEXT, SUB, VALUE1, VALUE2, TOOLTIP1, TOOLTIP2) \
   STATE_ATTR_2CHECK_CALLBACK(SUB, VALUE1, VALUE2) \
   STATE_ATTR_2CHECK_ADD(TEXT, SUB, VALUE1, VALUE2, TOOLTIP1, TOOLTIP2)

#define STATE_ATTR_1COMBOBOX_LIST(TEXT, SUB, VALUE, LIST, TOOLTIP, TYPE) \
   STATE_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE) \
   STATE_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, LIST, TOOLTIP)

#define STATE_ATTR_COLOR(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
   STATE_ATTR_COLOR_CALLBACK(SUB, VALUE, MEMBER) \
   STATE_ATTR_COLOR_LIST_UPDATE(SUB, VALUE, MEMBER) \
   STATE_ATTR_COLOR_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

#define STATE_ATTR_COMBOBOX(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
   STATE_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER) \
   STATE_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

STATE_ATTR_1CHECK(_("visible"), state, visible)
STATE_ATTR_2SPINNER(_("min"), state, min_w, min_h, 0.0, 9999.0, 1.0, "%.0f", "w:", "px", "h:", "px",
                    _("Minimal size of part width in pixels."), _("Minimal part height in pixels."),
                    1, int, VAL_INT)
STATE_ATTR_2SPINNER(_("max"), state, max_w, max_h, -1.0, 9999.0, 1.0, "%.0f", "w:", "px", "h:", "px",
                    _("Maximal size of part width in pixels."), _("Maximal part height in pixels."),
                    1, int, VAL_INT)
STATE_ATTR_2SPINNER(_("align"), state, align_x, align_y, 0, 100, 1, NULL, "x:", "%", "y:", "%",
                    _("Part align horizontally"), _("Part align vertically"),
                    100, double, VAL_DOUBLE)
STATE_ATTR_2CHECK(_("fixed"), state, fixed_w, fixed_h, _("This affects the minimum width calculation."),
                  _("This affects the minimum height calculation."))
STATE_ATTR_1COMBOBOX_LIST(_("aspect ratio mode"), state, aspect_pref, edje_aspect_pref,
                          _("The aspect control hints for this object."), unsigned char)
STATE_ATTR_2SPINNER(_("aspect ratio"), state, aspect_min, aspect_max, 0, 100, 1, NULL, "min:", "", "max:", "",
                   _("Normally width and height can be resized to any values independently"),
                   _("Normally width and height can be resized to any values independently"),
                   100, double, VAL_DOUBLE)
STATE_ATTR_2SPINNER(_("multiplier"), state, minmul_w, minmul_h, 1.0, 9999.0, 0.1, "%.1f", "w:", "", "h:", "",
                    _("The minimal part width value multiplier for current state"),
                    _("The minimal part height value multiplier for current state"),
                    1, double, VAL_DOUBLE)
STATE_ATTR_COLOR(_("color"), state, color, state, _("Part main color"))
STATE_ATTR_COMBOBOX(_("proxy source"), state, proxy_source, state,
                    _("Causes the part to use another part content as"
                    "the content of this part. Only work with PROXY part."))

Eina_Bool
ui_property_state_set(Evas_Object *property, Part *part)
{
   Evas_Object *item;
   Evas_Object *state_frame, *box, *prop_box;
   char state[BUFF_MAX];

   if ((!property) || (!part)) return EINA_FALSE;
   PROP_DATA_GET(EINA_FALSE)

   if (pd->wm_part != part) return EINA_FALSE; /* time for panic */
   #define pd_state pd->state

   sprintf(state, "%s %.2f", part->curr_state, part->curr_state_value);

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_state.frame);

   if (!pd_state.frame)
     {
        FRAME_PROPERTY_ADD(property, state_frame, true, _("State"), pd->visual)
        BOX_ADD(state_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(state_frame, box);

        item = prop_state_state_add(box, _("state"), state);
        elm_box_pack_end(box, item);
        item = prop_state_visible_add(box, pd, "Set visibility for part by current state");
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
        pd_state.proxy_source = prop_state_proxy_source_add(box, pd);
        evas_object_hide(pd_state.proxy_source);

        prop_box = elm_object_content_get(pd->visual);
        elm_box_pack_after(prop_box, state_frame, pd->part.frame);
        pd_state.frame = state_frame;
     }
   else
     {
        box = elm_object_content_get(pd_state.frame);
        prop_state_state_update(state);
        STATE_ATTR_CHECK_UPDATE(state, visible)
        STATE_ATTR_2SPINNER_UPDATE(state, min_w, min_h, 1)
        STATE_ATTR_2SPINNER_UPDATE(state, max_w, max_h, 1)
        STATE_ATTR_2CHECK_UPDATE(state, fixed_w, fixed_h)
        STATE_ATTR_2SPINNER_UPDATE(state, align_x, align_y, 100)
        STATE_ATTR_1COMBOBOX_LIST_UPDATE(state, aspect_pref)
        STATE_ATTR_2SPINNER_UPDATE(state, aspect_min, aspect_max, 100)
        prop_state_color_class_update(pd);
        prop_state_color_update(pd);
        STATE_ATTR_2SPINNER_UPDATE(state, minmul_w, minmul_h, 1)
        prop_state_proxy_source_update(pd, NULL);

        prop_box = elm_object_content_get(pd->visual);
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
        ui_property_state_table_set(property);
     }
   else
     ui_property_state_table_unset(property);
   if (part->type == EDJE_PART_TYPE_BOX)
     {
        ui_property_state_container_set(property);
        ui_property_state_box_set(property);
     }
   else
     ui_property_state_box_unset(property);

   if ((part->type != EDJE_PART_TYPE_TABLE) && (part->type != EDJE_PART_TYPE_BOX))
     ui_property_state_container_unset(property);

#ifndef HAVE_ENVENTOR
   _code_of_group_setup(pd);
#endif

   /* hide/show the color attribute control */
   if (part->type == EDJE_PART_TYPE_TEXTBLOCK ||
       part->type == EDJE_PART_TYPE_SWALLOW ||
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
        if (!evas_object_visible_get(pd_state.proxy_source))
          {
             elm_box_pack_end(box, pd_state.proxy_source);
             evas_object_show(pd_state.proxy_source);
          }
     }
   else
     {
        if (evas_object_visible_get(pd_state.proxy_source))
          {
             elm_box_unpack(box, pd_state.proxy_source);
             evas_object_hide(pd_state.proxy_source);
          }
     }

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_ON);
   #undef pd_state
   return true;
}

void
ui_property_state_unset(Evas_Object *property)
{
   if (!property) return;
   PROP_DATA_GET()

   evas_object_hide(pd->state.frame);
   /* hide the evas rectangles, it need to do, because those objects not a smart */
   evas_object_hide(pd->state.color1);
   evas_object_hide(pd->state.color2);
   evas_object_hide(pd->state.color3);

   ui_property_state_obj_area_unset(property);
   ui_property_state_text_unset(property);
   ui_property_state_image_unset(property);
   ui_property_state_textblock_unset(property);
   ui_property_state_fill_unset(property);
   ui_property_state_box_unset(property);
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
   const char *old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                                     pd->wm_part->name, pd->wm_part->curr_state, \
                                     pd->wm_part->curr_state_value); \
   const char *value = item->title; \
   if (strcmp(item->title, _("Layout"))) \
     edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, \
                                     pd->wm_part->curr_state, pd->wm_part->curr_state_value, \
                                     item->title); \
   else \
     { \
        edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, \
                                        pd->wm_part->curr_state, pd->wm_part->curr_state_value, \
                                        NULL); \
        value = NULL; \
     } \
   int temp = edje_edit_state_min_w_get(pd->wm_style->obj, pd->wm_part->name, \
                                       pd->wm_part->curr_state, pd->wm_part->curr_state_value); \
   edje_edit_state_min_w_set(pd->wm_style->obj, pd->wm_part->name, \
                             pd->wm_part->curr_state, \
                             pd->wm_part->curr_state_value, temp); \
   const char *text = eina_stringshare_printf("%s_%s", #SUB, #VALUE); \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_STRING, old_value, value, \
                    pd->wm_style->full_group_name, \
                    (void*)edje_edit_##SUB##_##VALUE##_set, text, \
                    pd->wm_part->name, pd->wm_part->curr_state, \
                    pd->wm_part->curr_state_value); \
   eina_stringshare_del(text); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
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

#define pd_obj_area pd->state_object_area
static Eina_Bool
ui_property_state_obj_area_set(Evas_Object *property)
{
   Evas_Object *obj_area_frame, *box, *prop_box;
   Evas_Object *separator, *icon;
   PROP_DATA_GET(EINA_FALSE)

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_obj_area.frame);

   if (!pd_obj_area.frame)
     {
        FRAME_PROPERTY_ADD(property, obj_area_frame, true, _("Object area"), pd->visual)
        BOX_ADD(obj_area_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(obj_area_frame, box);

        separator = elm_separator_add(obj_area_frame);
        elm_separator_horizontal_set(separator, true);
        elm_object_part_text_set(separator, "eflete.text", _("Start point"));
        ICON_ADD(separator, icon, false, "icon_start-point")
        elm_object_part_content_set(separator, "eflete.swallow.icon", icon);
        evas_object_show(separator);

        pd_obj_area.rel1_to = prop_item_state_rel1_to_x_y_add(box, pd,
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
        ICON_ADD(pd_obj_area.rel1_relative, icon, false, "icon_align");
        elm_object_part_content_set(pd_obj_area.rel1_relative, "eflete.swallow.icon", icon);
        pd_obj_area.rel1_offset = prop_item_state_rel1_offset_x_y_add(box, pd,
                            -9999.0, 9999.0, 1.0, "%.0f",
                            "x:", "px", "y:", "px",
                            _("Left offset from relative position in pixels"),
                            _("Top offset from relative position in pixels"),
                            false);
        ICON_ADD(pd_obj_area.rel1_offset, icon, false, "icon_offset");
        elm_object_part_content_set(pd_obj_area.rel1_offset, "eflete.swallow.icon", icon);

        elm_box_pack_end(box, separator);
        elm_box_pack_end(box, pd_obj_area.rel1_to);
        elm_box_pack_end(box, pd_obj_area.rel1_relative);
        elm_box_pack_end(box, pd_obj_area.rel1_offset);

        separator = elm_separator_add(obj_area_frame);
        elm_separator_horizontal_set(separator, true);
        elm_object_part_text_set(separator, "eflete.text", _("End point"));
        ICON_ADD(separator, icon, false, "icon_end-point");
        elm_object_part_content_set(separator, "eflete.swallow.icon", icon);
        evas_object_show(separator);

        pd_obj_area.rel2_to = prop_item_state_rel2_to_x_y_add(box, pd,
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
        ICON_ADD(pd_obj_area.rel2_relative, icon, false, "icon_align");
        elm_object_part_content_set(pd_obj_area.rel2_relative, "eflete.swallow.icon", icon);
        pd_obj_area.rel2_offset = prop_item_state_rel2_offset_x_y_add(box, pd,
                            -9999.0, 9999.0, 1.0, "%.0f",
                            "x:", "px", "y:", "px",
                            _("Left offset from relative position in pixels"),
                            _("Top offset from relative position in pixels"),
                            false);
        ICON_ADD(pd_obj_area.rel2_offset, icon, false, "icon_offset");
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

        prop_box = elm_object_content_get(pd->visual);
        elm_box_pack_end(prop_box, pd_obj_area.frame);
        evas_object_show(pd_obj_area.frame);
     }

   return true;
}

static void
ui_property_state_obj_area_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_obj_area.frame);
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

#define ITEM_1COMBOBOX_STATE_CREATE(TYPE, TEXT, SUB, VALUE, LIST) \
   ITEM_COMBOBOX_STATE_CALLBACK(TYPE, TEXT, SUB, VALUE) \
   ITEM_1COMBOBOX_STATE_ADD(TYPE, TEXT, SUB, VALUE, LIST) \
   ITEM_1COMBOBOX_STATE_UPDATE(TYPE, TEXT, SUB, VALUE, LIST)

#define STATE_ATTR_1ENTRY(TEXT, SUB, VALUE, MEMBER, REGEX, TOOLTIP) \
   STATE_ATTR_1ENTRY_CALLBACK(SUB, VALUE) \
   STATE_ATTR_1ENTRY_UPDATE(SUB, VALUE, MEMBER) \
   STATE_ATTR_1ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, REGEX, TOOLTIP)

STATE_ATTR_1ENTRY(_("text"), state, text, state_text, NULL, _("The dispalyed text"))
STATE_ATTR_1ENTRY(_("font"), state, font, state_text, PROPERTY_REGEX_STATE_FONT,
                  _("The text font, posible set a font style. Ex: Sans:style=italic"))
ITEM_1SPINNER_STATE_INT_CREATE(_("size"), state_text, size)
ITEM_2SPINNER_STATE_DOUBLE_CREATE(_("align"), state_text_align, x, y, "eflete/property/item/default")
ITEM_2CHECK_STATE_CREATE(_("max"), state_text_max, x, y)
ITEM_2CHECK_STATE_CREATE(_("min"), state_text_min, x, y)
ITEM_2CHECK_STATE_CREATE(_("fit"), state_text_fit, x, y)
ITEM_COLOR_STATE_CREATE(_("shadow color"), state, color3)
ITEM_COLOR_STATE_CREATE(_("outline color"), state, color2)
ITEM_1COMBOBOX_STATE_CREATE(SOURCE, _("source"), state_text, source, styles)
ITEM_1COMBOBOX_STATE_CREATE(TEXT_SOURCE, _("text source"), state_text, text_source, styles)


static Eina_Bool
_text_effect_get(Prop_Data *pd, int *type, int *direction)
{
   Edje_Text_Effect edje_effect = edje_edit_part_effect_get(pd->wm_style->obj,
                                                            pd->wm_part->name);

   if ((!type) || (!direction))
     return false;

     *type = edje_effect & EDJE_TEXT_EFFECT_MASK_BASIC;
   if ((*type >= EDJE_TEXT_EFFECT_SOFT_OUTLINE) &&
       (*type != EDJE_TEXT_EFFECT_GLOW))
     *direction = (edje_effect & EDJE_TEXT_EFFECT_MASK_SHADOW_DIRECTION) >> 4;
   else *direction = 0;

   return true;
}

typedef struct {
   Evas_Object *type_combobox;
   Evas_Object *direction_combobox;
   Prop_Data *pd;
} _text_effect_callback_data;

static void
_text_effect_value_update(_text_effect_callback_data *effect_data)
{
   Edje_Text_Effect edje_effect;
   Edje_Text_Effect old_value = edje_edit_part_effect_get(effect_data->pd->wm_style->obj,
                             effect_data->pd->wm_part->name);
   edje_effect = ewe_combobox_select_item_get(effect_data->type_combobox)->index;
   edje_effect |= ewe_combobox_select_item_get(effect_data->direction_combobox)->index << 4;
   edje_edit_part_effect_set(effect_data->pd->wm_style->obj,
                             effect_data->pd->wm_part->name, edje_effect);

   workspace_edit_object_recalc(effect_data->pd->workspace);
   history_diff_add(effect_data->pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT,
                    old_value, edje_effect,
                    effect_data->pd->wm_style->full_group_name,
                    (void*)edje_edit_part_effect_set, "text effect",
                    effect_data->pd->wm_part->name, NULL, 0);
   effect_data->pd->wm_style->isModify = true;
}

static void
prop_item_state_effect_update(Evas_Object *item, Prop_Data *pd)
{
   int type, direction;
   Evas_Object *combobox;
   if (!_text_effect_get(pd, &type, &direction))
     {
        ERR("Please, contact DEVS! This error should never appear!");
        return;
     }
   combobox = evas_object_data_get(item, ITEM1);
   ewe_combobox_select_item_set(combobox, type);
   combobox = evas_object_data_get(item, ITEM2);
   if ((type <= EDJE_TEXT_EFFECT_SOFT_OUTLINE) ||
       (type == EDJE_TEXT_EFFECT_GLOW))
     {
        elm_object_disabled_set(combobox, true);
        ewe_combobox_select_item_set(combobox, 0);
     }
   else
     {
        ewe_combobox_select_item_set(combobox, direction);
        elm_object_disabled_set(combobox, false);
     }
}

static void
_on_text_effect_type_change(void *data,
                            Evas_Object *obj EINA_UNUSED,
                            void *event_info)
{
   _text_effect_callback_data *effect_data = (_text_effect_callback_data *)data;
   Ewe_Combobox_Item *selected_item = event_info;

   switch (selected_item->index)
     {
      case EDJE_TEXT_EFFECT_NONE:
      case EDJE_TEXT_EFFECT_PLAIN:
      case EDJE_TEXT_EFFECT_OUTLINE:
      case EDJE_TEXT_EFFECT_SOFT_OUTLINE:
      case EDJE_TEXT_EFFECT_GLOW:
        {
           ewe_combobox_select_item_set(effect_data->direction_combobox, 0);
           elm_object_disabled_set(effect_data->direction_combobox, true);
           break;
        }
      default:
        {
           if (elm_object_disabled_get(effect_data->direction_combobox))
             {
                ewe_combobox_select_item_set(effect_data->direction_combobox, 1);
                elm_object_disabled_set(effect_data->direction_combobox, false);
             }
        }
     }
   _text_effect_value_update(effect_data);
   project_changed();
}

static void
_on_text_effect_direction_change(void *data,
                                 Evas_Object *obj EINA_UNUSED,
                                 void *event_info EINA_UNUSED)
{
   _text_effect_value_update(data);
   project_changed();
}

static void
_del_text_effect_callback_data(void *data,
                               Evas *e __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *ei __UNUSED__)
{
   _text_effect_callback_data *data_to_del = (_text_effect_callback_data *)data;
   free(data_to_del);
}

#define ADD_TEXT_EFFECT_COMBOBOX(_prop_name, _prop_callback, _prop_names_array) \
   i = 0; \
   ITEM_ADD(box, layout, _(#_prop_name":"), "eflete/property/item/small_label") \
   EWE_COMBOBOX_ADD(layout, combobox) \
   while (_prop_names_array[i]) \
     { \
        ewe_combobox_item_add(combobox, _prop_names_array[i]); \
        i++; \
     } \
   evas_object_smart_callback_add(combobox, "selected", \
                                  _prop_callback, callback_data); \
   elm_object_part_content_set(layout, "elm.swallow.content", combobox); \
   elm_box_pack_end(box, layout);

static Evas_Object *
prop_item_state_effect_add(Evas_Object *parent, Prop_Data *pd)
{
   int i = 0;
   Evas_Object *item, *box, *layout;
   Evas_Object *combobox;

   _text_effect_callback_data *callback_data = (_text_effect_callback_data *)
                                   mem_malloc(sizeof(_text_effect_callback_data));
   if (!callback_data)
     return NULL;

   ITEM_ADD(parent, item, _("effect"), "eflete/property/item/multiple_prop")
   evas_object_event_callback_add(item, EVAS_CALLBACK_DEL,
                                  _del_text_effect_callback_data,
                                  callback_data);
   callback_data->pd = pd;

   BOX_ADD(item, box, false, true)
   elm_box_padding_set(box, 0, 6);

   ADD_TEXT_EFFECT_COMBOBOX(type, _on_text_effect_type_change, edje_text_effect_type);
   callback_data->type_combobox = combobox;
   evas_object_data_set(item, ITEM1, combobox);
   elm_object_tooltip_text_set(combobox, _("Causes Edje to draw the selected effect."));

   ADD_TEXT_EFFECT_COMBOBOX(direction, _on_text_effect_direction_change, edje_text_effect_direction);
   callback_data->direction_combobox = combobox;
   evas_object_data_set(item, ITEM2, combobox);
   elm_object_tooltip_text_set(combobox, _("Shadow directions."));

   elm_object_part_content_set(item, "elm.swallow.content", box);
   prop_item_state_effect_update(item, pd);
   return item;
}
#undef ADD_TEXT_EFFECT_COMBOBOX

#define pd_text pd->state_text


static void
_on_state_text_ellipsis_change(void *data,
                               Evas_Object *obj,
                               void *event_info __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;
   double value = elm_spinner_value_get(obj);
   double old_value = edje_edit_state_text_elipsis_get(pd->wm_style->obj,
                                                       pd->wm_part->name,
                                                       pd->wm_part->curr_state,
                                                       pd->wm_part->curr_state_value);
   if (!edje_edit_state_text_elipsis_set(pd->wm_style->obj,
                                         pd->wm_part->name,
                                         pd->wm_part->curr_state,
                                         pd->wm_part->curr_state_value,
                                         value))
     return;
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_DOUBLE, old_value, value,
                    pd->wm_style->full_group_name,
                    (void*)edje_edit_state_text_elipsis_set, "elipsis",
                    pd->wm_part->name, pd->wm_part->curr_state,
                    pd->wm_part->curr_state_value);
   workspace_edit_object_recalc(pd->workspace);
   pd->wm_style->isModify = true;
   project_changed();
}

static void
_on_state_text_ellipsis_toggle_change(void *data,
                                      Evas_Object *obj,
                                      void *event_info __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;
   Eina_Bool state = elm_check_state_get(obj);
   Evas_Object *spinner = evas_object_data_get(pd_text.ellipsis, ITEM2);
   double value = 0.0;
   double old_value = edje_edit_state_text_elipsis_get(pd->wm_style->obj,
                                         pd->wm_part->name,
                                         pd->wm_part->curr_state,
                                         pd->wm_part->curr_state_value);

   if (state)
     {
        elm_object_disabled_set(spinner, false);
        value = elm_spinner_value_get(spinner);
     }
   else
     {
        elm_object_disabled_set(spinner, true);
        value = -1.0;
     }
   edje_edit_state_text_elipsis_set(pd->wm_style->obj,
                                    pd->wm_part->name,
                                    pd->wm_part->curr_state,
                                    pd->wm_part->curr_state_value,
                                    value);
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_DOUBLE, old_value, value,
                    pd->wm_style->full_group_name,
                    (void*)edje_edit_state_text_elipsis_set, "elipsis",
                    pd->wm_part->name, pd->wm_part->curr_state,
                    pd->wm_part->curr_state_value);
   workspace_edit_object_recalc(pd->workspace);
   pd->wm_style->isModify = true;
   project_changed();
}

static Evas_Object *
prop_item_state_text_ellipsis_add(Evas_Object *parent,
                                  Prop_Data *pd,
                                  const char *tooltip)
{
   Evas_Object *item, *box, *layout, *check, *spinner;
   double value;

   ITEM_ADD(parent, item, "ellipsis", "eflete/property/item/default")
   BOX_ADD(item, box, true, true)
   elm_box_align_set(box, 0.0, 0.5);
   ITEM_CONTEINER_2LABEL_ADD(box, layout, "turn", NULL);
   elm_object_tooltip_text_set(item, tooltip);

   CHECK_ADD(layout, check)
   elm_object_style_set(check, "toggle");
   elm_object_part_content_set(layout, "eflete.content", check);
   evas_object_smart_callback_add(check, "changed",
                                  _on_state_text_ellipsis_toggle_change, pd);

   SPINNER_ADD(box, spinner, 0.0, 1.0, 0.1, true)
   elm_spinner_label_format_set(spinner, "%1.2f");
   value = edje_edit_state_text_elipsis_get(pd->wm_style->obj,
                                            pd->wm_part->name,
                                            pd->wm_part->curr_state,
                                            pd->wm_part->curr_state_value);
   evas_object_smart_callback_add(spinner, "changed",
                                  _on_state_text_ellipsis_change, pd);

   if (value < 0)
     {
        elm_check_state_set(check, false);
        elm_object_disabled_set(spinner, true);
        elm_spinner_value_set(spinner, 0.0);
     }
   else
     {
        elm_check_state_set(check, true);
        elm_object_disabled_set(spinner, false);
        elm_spinner_value_set(spinner, value);
     }

   elm_box_pack_end(box, layout);
   elm_box_pack_end(box, spinner);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   evas_object_data_set(item, ITEM1, check);
   evas_object_data_set(item, ITEM2, spinner);
   return item;

}

static void
prop_item_state_text_ellipsis_update(Evas_Object *item,
                                     Prop_Data *pd)
{
   Evas_Object *check, *spinner;
   double value;

   check = evas_object_data_get(item, ITEM1);
   evas_object_smart_callback_del_full(check, "changed",
                                       _on_state_text_ellipsis_toggle_change, pd);
   evas_object_smart_callback_add(check, "changed",
                                  _on_state_text_ellipsis_toggle_change, pd);

   spinner = evas_object_data_get(item, ITEM2);
   evas_object_smart_callback_del_full(spinner, "changed",
                                       _on_state_text_ellipsis_change, pd);
   evas_object_smart_callback_add(spinner, "changed",
                                  _on_state_text_ellipsis_change, pd);

   value = edje_edit_state_text_elipsis_get(pd->wm_style->obj,
                                            pd->wm_part->name,
                                            pd->wm_part->curr_state,
                                            pd->wm_part->curr_state_value);
    if (value < 0)
     {
        elm_check_state_set(check, false);
        elm_object_disabled_set(spinner, true);
        elm_spinner_value_set(spinner, 0.0);
     }
   else
     {
        elm_check_state_set(check, true);
        elm_object_disabled_set(spinner, false);
        elm_spinner_value_set(spinner, value);
     }
}

static Eina_Bool
ui_property_state_text_set(Evas_Object *property)
{
   Evas_Object *item;
   Evas_Object *text_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is TEXT too, unpack it */
   ui_property_state_text_unset(property);
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_text.frame)
     {
         FRAME_PROPERTY_ADD(property, text_frame, true, _("Text"), pd->visual)
         BOX_ADD(text_frame, box, EINA_FALSE, EINA_FALSE)
         elm_box_align_set(box, 0.5, 0.0);
         elm_object_content_set(text_frame, box);

         item = prop_state_text_add(box, pd, NULL);
         elm_box_pack_end(box, item);
         item = prop_state_font_add(box, pd, NULL);
         elm_box_pack_end(box, item);
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
         pd_text.source = prop_item_state_text_source_add(box, pd, NULL,
                            _("Causes the part to use the text properties\t"
                            "(like font and size) of another part\t"
                            "and update them as they change."));
         pd_text.text_source = prop_item_state_text_text_source_add(box, pd, NULL,
                               _("Causes the part to display the text content of \t"
                               "another part and update them as they change."));
         pd_text.ellipsis = prop_item_state_text_ellipsis_add(box, pd,
                            _("Cut text if biggest then part's area"
                            "0.0 = fix the left side  1.0 = right side"));
         pd_text.fit = prop_item_state_text_fit_x_y_add(box, pd,
                          _("Resize the text for it to fit in it's container by X axis"),
                          _("Resize the text for it to fit in it's container by Y axis"));
         pd_text.color2 = prop_item_state_color2_add(box, pd,
                            _("Text shadow color."));
         pd_text.color3 = prop_item_state_color3_add(box, pd,
                            _("Text outline color."));
         pd_text.effect = prop_item_state_effect_add(box, pd);

         elm_box_pack_end(box, pd_text.size);
         elm_box_pack_end(box, pd_text.align);
         elm_box_pack_end(box, pd_text.min);
         elm_box_pack_end(box, pd_text.max);
         elm_box_pack_end(box, pd_text.source);
         elm_box_pack_end(box, pd_text.text_source);
         elm_box_pack_end(box, pd_text.ellipsis);
         elm_box_pack_end(box, pd_text.fit);
         elm_box_pack_end(box, pd_text.color2);
         elm_box_pack_end(box, pd_text.color3);
         elm_box_pack_end(box, pd_text.effect);

         elm_box_pack_end(prop_box, text_frame);
         pd_text.frame = text_frame;
     }
   else
     {
        prop_state_text_update(pd);
        prop_state_font_update(pd);
        prop_item_state_text_size_update(pd_text.size, pd);
        prop_item_state_text_align_x_y_update(pd_text.align, pd, true);
        prop_item_state_text_min_x_y_update(pd_text.min, pd);
        prop_item_state_text_max_x_y_update(pd_text.max, pd);
        prop_item_state_text_source_update(pd_text.source, pd);
        prop_item_state_text_text_source_update(pd_text.text_source, pd);
        prop_item_state_text_ellipsis_update(pd_text.ellipsis, pd);
        prop_item_state_text_fit_x_y_update(pd_text.fit, pd);
        prop_item_state_color2_update(pd_text.color2, pd);
        prop_item_state_color3_update(pd_text.color3, pd);
        prop_item_state_effect_update(pd_text.effect, pd);
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

#define prop_state_textblock_source_update(PD, COMBOBOX) prop_part_source_update(PD, COMBOBOX);
#define prop_state_textblock_source2_update(PD, COMBOBOX) prop_part_source_update(PD, COMBOBOX);
#define prop_state_textblock_source3_update(PD, COMBOBOX) prop_part_source_update(PD, COMBOBOX);
#define prop_state_textblock_source4_update(PD, COMBOBOX) prop_part_source_update(PD, COMBOBOX);
#define prop_state_textblock_source5_update(PD, COMBOBOX) prop_part_source_update(PD, COMBOBOX);
#define prop_state_textblock_source6_update(PD, COMBOBOX) prop_part_source_update(PD, COMBOBOX);

#define pd_textblock pd->state_textblock
PART_ATTR_1COMBOBOX_LIST(_("select mode"), state_textblock, select_mode, Edje_Edit_Select_Mode)
PART_ATTR_1COMBOBOX_LIST(_("entry mode"), state_textblock, entry_mode, Edje_Edit_Entry_Mode)
PART_ATTR_1COMBOBOX_LIST(_("pointer mode"), state_textblock, pointer_mode, Evas_Object_Pointer_Mode)
PART_ATTR_1COMBOBOX_LIST(_("cursor mode"), state_textblock, cursor_mode, unsigned int)
PART_ATTR_1CHECK(_("multiline"), state_textblock, multiline)
PART_ATTR_1COMBOBOX(_("under selected text"), part, source, state_textblock,
                    _("Used for the group to be loaded and used for selection \t"
                    "display UNDER the selected text the source \t"
                    "of TEXTBLOCK part."))
PART_ATTR_1COMBOBOX(_("before selected text"), part, source2, state_textblock,
                    _("It is used for the group to be loaded and used for \t"
                    "selection display OVER the selected text."))
PART_ATTR_1COMBOBOX(_("under cursor"), part, source3, state_textblock,
                    _("It is used for the group to be loaded and used for \t"
                    "cursor display UNDER the cursor position."))
PART_ATTR_1COMBOBOX(_("over cursor"), part, source4, state_textblock,
                    _("It is used for the group to be loaded and used \t"
                    "for cursor display OVER the cursor position."))
PART_ATTR_1COMBOBOX(_("under anchor"), part, source5, state_textblock,
                    _("It is used for the group to be loaded and used for \t"
                    "anchors display UNDER the anchor position."))
PART_ATTR_1COMBOBOX(_("over anchor"), part, source6, state_textblock,
                    _("It is used for the group to be loaded and used for \t"
                    "anchor display OVER the anchor position."))

ITEM_1COMBOBOX_STATE_CREATE(TEXT_STYLE, _("text style"), state, text_style, styles)

static Eina_Bool
ui_property_state_textblock_set(Evas_Object *property)
{
   Evas_Object *item;
   Evas_Object *textblock_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is TEXTBLOCK too, unpack it */
   ui_property_state_textblock_unset(property);
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_textblock.frame)
     {
         FRAME_PROPERTY_ADD(property, textblock_frame, true, _("TextBlock"), pd->visual)
         BOX_ADD(textblock_frame, box, EINA_FALSE, EINA_FALSE)
         elm_box_align_set(box, 0.5, 0.0);
         elm_object_content_set(textblock_frame, box);

         item = prop_state_text_add(box, pd, NULL);
         elm_box_pack_end(box, item);
         pd_textblock.style = prop_item_state_text_style_add(box, pd,
                           _on_state_text_style_change,
                           _("Set the text style of part."));
         elm_box_pack_end(box, pd_textblock.style);
         pd_textblock.align = prop_item_state_text_align_x_y_add(box, pd,
                             0, 100, 1, NULL, "x:", "%", "y:", "%",
                             _("Text horizontal align. 0 = left  100 = right"),
                             _("Text vertical align. 0 = top  100 = bottom"),
                             true);
         elm_box_pack_end(box, pd_textblock.align);
         pd_textblock.min = prop_item_state_text_min_x_y_add(box, pd,
                           _("When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text."),
                           _("When any of the parameters is enabled it forces \t"
                           "the minimum size of the container to be equal to\t"
                           "the minimum size of the text."));
         elm_box_pack_end(box, pd_textblock.min);
         pd_textblock.max = prop_item_state_text_max_x_y_add(box, pd,
                           _("When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text."),
                           _("When any of the parameters is enabled it forces \t"
                           "the maximum size of the container to be equal to\t"
                           "the maximum size of the text."));
         elm_box_pack_end(box, pd_textblock.max);
         item = prop_state_textblock_select_mode_add(box, pd,
                                    _("Sets the selection mode for a textblock part"),
                                    edje_select_mode);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_entry_mode_add(box, pd,
                                   _("Sets the edit mode for a textblock part"),
                                   edje_entry_mode);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_pointer_mode_add(box, pd,
                                     _("Sets the mouse pointer behavior for a given part."),
                                     edje_pointer_mode);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_cursor_mode_add(box, pd,
                                    _("Sets the cursor mode for a textblock part"),
                                    edje_cursor_mode);
         elm_box_pack_end(box, item);
         item = prop_state_textblock_multiline_add(box, pd,
                _("It causes a textblock that is editable to allow multiple lines for editing."));
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
        prop_item_state_text_style_update(pd_textblock.style, pd);
        prop_item_state_text_style_update(pd_textblock.align, pd);
        prop_item_state_text_min_x_y_update(pd_textblock.min, pd);
        prop_item_state_text_max_x_y_update(pd_textblock.max, pd);
        PART_ATTR_1COMBOBOX_LIST_UPDATE(state_textblock, select_mode)
        PART_ATTR_1COMBOBOX_LIST_UPDATE(state_textblock, entry_mode)
        PART_ATTR_1COMBOBOX_LIST_UPDATE(state_textblock, pointer_mode)
        PART_ATTR_1COMBOBOX_LIST_UPDATE(state_textblock, cursor_mode)
        ITEM_ATTR_1CHECK_UPDATE(state_textblock, multiline);
        prop_state_textblock_source_update(pd, pd_textblock.source);
        prop_state_textblock_source2_update(pd, pd_textblock.source2);
        prop_state_textblock_source3_update(pd, pd_textblock.source3);
        prop_state_textblock_source4_update(pd, pd_textblock.source4);
        prop_state_textblock_source5_update(pd, pd_textblock.source5);
        prop_state_textblock_source6_update(pd, pd_textblock.source6);
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

#define pd_image pd->state_image

static void
_on_image_editor_done(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Evas_Object *image_entry, * border_entry;
   Prop_Data *pd = (Prop_Data *)data;
   const char *value;
   const char *selected = (const char *)event_info;

   if (!selected) return;
   image_entry = elm_object_part_content_get(pd_image.normal, "elm.swallow.content");
   border_entry = elm_object_part_content_get(pd_image.border, "elm.swallow.content");
   value = elm_entry_entry_get(image_entry);

   if (strcmp(value, selected) == 0) return;
   ewe_entry_entry_set(image_entry, selected);
   evas_object_smart_callback_call(image_entry, "changed,user", NULL);
   ewe_entry_entry_set(border_entry, NULL);
   evas_object_smart_callback_call(border_entry, "changed,user", NULL);
   project_changed();
}

static void
_on_state_image_choose(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *ei __UNUSED__)
{
   Evas_Object *img_edit;
   Prop_Data *pd = (Prop_Data *)data;
   Evas_Object *entry = elm_object_part_content_get(pd_image.normal, "elm.swallow.content");
   const char *selected = elm_entry_entry_get(entry);

   App_Data *ap = app_data_get();

   img_edit = image_editor_window_add(ap->project, SINGLE);
   image_editor_file_choose(img_edit, selected);
   evas_object_smart_callback_add(img_edit, SIG_IMAGE_SELECTED, _on_image_editor_done, pd);
}

static void
_del_tween_image(void *data,
                 Evas_Object *obj,
                 void *event_info __UNUSED__)
{
   Evas_Object *tween_list = elm_object_parent_widget_get(obj);
   const char *selected = (const char *)data;
   Elm_Object_Item *it = elm_genlist_selected_item_get(tween_list);
   Prop_Data *pd = evas_object_data_get(tween_list, PROP_DATA);

   if ((!selected) || (!it) || (!pd)) return;
   if (edje_edit_state_tween_del(pd->wm_style->obj, pd->wm_part->name,
                                 pd->wm_part->curr_state, pd->wm_part->curr_state_value,
                                 selected))
     {
        elm_object_item_del(it);
        pd->wm_style->isModify = true;
        history_diff_add(pd->wm_style->obj, PROPERTY, DEL, VAL_STRING,
                         selected, edje_edit_state_tween_add,
                         pd->wm_style->full_group_name,
                         (void*)edje_edit_state_tween_del, "tween image",
                         pd->wm_part->name, pd->wm_part->curr_state,
                         pd->wm_part->curr_state_value);
        project_changed();
     }
}

static void
_on_image_editor_tween_done(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *ei)
{
   Evas_Object *tween_list = (Evas_Object *)data;
   Eina_List *selected = (Eina_List *)ei;
   Eina_List *l = NULL;
   const char *name = NULL;
   Prop_Data *pd = evas_object_data_get(tween_list, PROP_DATA);

   if ((!selected) || (!pd)) return;

   EINA_LIST_FOREACH(selected, l, name)
     {
        if (edje_edit_state_tween_add(pd->wm_style->obj, pd->wm_part->name,
                                      pd->wm_part->curr_state,
                                      pd->wm_part->curr_state_value, name))
          {
             elm_genlist_item_append(tween_list, _itc_tween, name, NULL,
                                     ELM_GENLIST_ITEM_NONE, NULL, NULL);
             pd->wm_style->isModify = true;
             history_diff_add(pd->wm_style->obj, PROPERTY, ADD, VAL_STRING,
                              name, edje_edit_state_tween_del,
                              pd->wm_style->full_group_name,
                              (void*)edje_edit_state_tween_add, "tween image",
                              pd->wm_part->name, pd->wm_part->curr_state,
                              pd->wm_part->curr_state_value);
             project_changed();
          }
     }
   elm_frame_collapse_go(pd->state_image.tween, false);
   edje_edit_string_list_free(selected);
}

static void
_add_tween_image(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *img_edit;
   Evas_Object *tween_list = (Evas_Object *)data;

   App_Data *ap = app_data_get();

   img_edit = image_editor_window_add(ap->project, TWEENS);
   evas_object_smart_callback_add(img_edit, SIG_IMAGE_SELECTED,
                                  _on_image_editor_tween_done, tween_list);

   return;
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
   Evas_Object *button, *image;
   Prop_Data *pd = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
    {
       pd = evas_object_data_get(obj, PROP_DATA);
       if (!pd) return NULL;
       edje_object_file_get((const Eo*)pd->wm_style->obj, &file, &group);
       image = evas_object_image_add(evas_object_evas_get(obj));
       buf = eina_stringshare_printf("edje/images/%i",
                        edje_edit_image_id_get(pd->wm_style->obj, (const char*)data));
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
   if (!strcmp(part, "elm.swallow.end"))
    {
       BUTTON_ADD(obj, button, NULL)
       ICON_ADD(button, image, true, "icon-remove");
       elm_object_part_content_set(button, NULL, image);
       evas_object_smart_callback_add(button, "clicked", _del_tween_image,
                                      (const char*)data);
       elm_object_style_set(button, "simple");
       return button;
    }
    return NULL;

}

static void
_tween_image_moved(Evas_Object *data,
                   Evas_Object *obj,
                   Elm_Object_Item *item EINA_UNUSED)
{
   Prop_Data *pd = (Prop_Data *)data;
   Eina_List *images_list, *l;
   Elm_Object_Item *next;
   const char *image_name;

   images_list = edje_edit_state_tweens_list_get(pd->wm_style->obj,
                                                 pd->wm_part->name,
                                                 pd->wm_part->curr_state,
                                                 pd->wm_part->curr_state_value);

   EINA_LIST_FOREACH(images_list, l, image_name)
     {
        edje_edit_state_tween_del(pd->wm_style->obj, pd->wm_part->name,
                                  pd->wm_part->curr_state, pd->wm_part->curr_state_value,
                                  image_name);
     }

   next = elm_genlist_first_item_get(obj);
   while (next)
     {
        image_name  = elm_object_item_data_get(next);
        edje_edit_state_tween_add(pd->wm_style->obj, pd->wm_part->name,
                                  pd->wm_part->curr_state, pd->wm_part->curr_state_value,
                                  image_name);
        next = elm_genlist_item_next_get(next);
     }
   project_changed();
}

Evas_Object *
prop_item_state_image_tween_add(Evas_Object *box, Prop_Data *pd)
{
   Evas_Object *tween_frame, *tween_list;
   Evas_Object *button, *icon;
   Eina_List *images_list, *l;
   char *image_name;

   FRAME_PROPERTY_ADD(box, tween_frame, true, _("Tweens"), pd->visual)
   elm_object_style_set(tween_frame, "tween");

   tween_list = elm_genlist_add(tween_frame);
   elm_genlist_longpress_timeout_set(tween_list, 0.2);
   elm_genlist_reorder_mode_set(tween_list, true);
   evas_object_data_set(tween_list, PROP_DATA, pd);
   elm_object_style_set(tween_list, "default");

   if (!_itc_tween)
     {
        _itc_tween = elm_genlist_item_class_new();
        _itc_tween->item_style = "tween";
        _itc_tween->func.text_get = _item_label_get;
        _itc_tween->func.content_get = _item_content_get;
        _itc_tween->func.state_get = NULL;
        _itc_tween->func.del = NULL;
     }

   images_list = edje_edit_state_tweens_list_get(pd->wm_style->obj,
                                                 pd->wm_part->name,
                                                 pd->wm_part->curr_state,
                                                 pd->wm_part->curr_state_value);
   if (!images_list) elm_frame_collapse_go(tween_frame, true);
   EINA_LIST_FOREACH(images_list, l, image_name)
     {
       elm_genlist_item_append(tween_list, _itc_tween,
                               eina_stringshare_add(image_name), NULL,
                               ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   edje_edit_string_list_free(images_list);
   elm_object_content_set(tween_frame, tween_list);

   BUTTON_ADD(tween_frame, button, NULL)
   ICON_ADD(button, icon, true, "icon-add");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, "clicked", _add_tween_image,
                                  tween_list);
   elm_object_style_set(button, "simple");
   elm_object_part_content_set(tween_frame, "elm.swallow.add", button);
   evas_object_smart_callback_add(tween_list, "moved",
                                  (Evas_Smart_Cb)_tween_image_moved, pd);

   evas_object_show(tween_list);

   return tween_frame;
}

void
prop_item_state_image_tween_update(Evas_Object *tween, Prop_Data *pd)
{
   Evas_Object *tween_list;
   Eina_List *images_list, *l;
   const char *image_name = NULL;

   tween_list = elm_object_content_get(tween);
   elm_genlist_clear(tween_list);
   images_list = edje_edit_state_tweens_list_get(pd->wm_style->obj,
                                                 pd->wm_part->name,
                                                 pd->wm_part->curr_state,
                                                 pd->wm_part->curr_state_value);
   if (!images_list) elm_frame_collapse_go(tween, true);
   else elm_frame_collapse_go(tween, false);

   EINA_LIST_FOREACH(images_list, l, image_name)
     {
       elm_genlist_item_append(tween_list, _itc_tween, image_name, NULL,
                               ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
}

ITEM_1ENTRY_STATE_CREATE(_("image"), state, image, &accept_prop, const char *)
ITEM_IM_BORDER_STATE_CREATE(_("border"), state_image, border)
ITEM_1COMBOBOX_PART_STATE_CREATE(_("middle"), state_image, border_fill, unsigned char)

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
        FRAME_PROPERTY_ADD(property, image_frame, true, _("Image"), pd->visual)
        BOX_ADD(image_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(image_frame, box);

        pd_image.normal = prop_item_state_image_add(box, pd, _on_state_image_choose,
                             _("Current image name"),
                             _("Change image"));
        Evas_Object *entry = elm_object_part_content_get(pd_image.normal, "elm.swallow.content");
        pd_image.border = prop_item_state_image_border_add(box, pd,
                             _("Image's border value"));
        entry = elm_object_part_content_get(pd_image.border, "elm.swallow.content");
        ewe_entry_regex_set(entry, IMAGE_BORDER_REGEX, EWE_REG_EXTENDED);
        ewe_entry_regex_autocheck_set(entry, true);
        ewe_entry_regex_glow_set(entry, true);
        pd_image.middle = prop_item_state_image_border_fill_add(box, pd,
                             _("Image's middle value"), edje_middle_type);

        pd_image.tween = prop_item_state_image_tween_add(box, pd);

        elm_box_pack_end(box, pd_image.normal);
        elm_box_pack_end(box, pd_image.border);
        elm_box_pack_end(box, pd_image.middle);
        elm_box_pack_end(box, pd_image.tween);

        elm_box_pack_end(prop_box, image_frame);
        pd_image.frame = image_frame;
     }
   else
     {
        prop_item_state_image_update(pd_image.normal, pd);
        prop_item_state_image_border_update(pd_image.border, pd);
        prop_item_state_image_border_fill_update(pd_image.middle, pd);
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
   PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_image.frame);
   evas_object_hide(pd_image.frame);
}
#undef pd_image

#define pd_fill pd->state_fill
STATE_ATTR_1CHECK(_("smooth"), state_fill, smooth)
ITEM_1COMBOBOX_PART_STATE_CREATE(_("type"), state_fill, type, unsigned char)
ITEM_2SPINNER_STATE_DOUBLE_CREATE(_("align"), state_fill_origin_relative, x, y, "eflete/property/item/relative")
ITEM_2SPINNER_STATE_INT_CREATE(_("offset"), state_fill_origin_offset, x, y, "eflete/property/item/relative")
ITEM_2SPINNER_STATE_DOUBLE_CREATE(_("align"), state_fill_size_relative, x, y, "eflete/property/item/relative")
ITEM_2SPINNER_STATE_INT_CREATE(_("offset"), state_fill_size_offset, x, y, "eflete/property/item/relative")

static Eina_Bool
ui_property_state_fill_set(Evas_Object *property)
{
   Evas_Object *item;
   Evas_Object *fill_frame, *box, *prop_box;
   Evas_Object *icon, *separator;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is IMAGE or PROXY too, unpack it */
   ui_property_state_fill_unset(property);
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_fill.frame)
     {
        FRAME_PROPERTY_ADD(property, fill_frame, true, _("Fill"), pd->visual)
        BOX_ADD(fill_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(fill_frame, box);

        pd_fill.type = prop_item_state_fill_type_add(box, pd,
                             _("Sets the image fill type."),
                             edje_fill_type);
        elm_box_pack_end(box, pd_fill.type);
        item = prop_state_fill_smooth_add(box, pd,
                             _("On/off image smooth on scaling"));
        elm_box_pack_end(box, item);


       /* origin subblock of fill block */
        separator = elm_separator_add(fill_frame);
        elm_separator_horizontal_set(separator, true);
        elm_object_part_text_set(separator, "eflete.text", _("Origin"));
        ICON_ADD(separator, icon, false, "icon_start-point")
        elm_object_part_content_set(separator, "eflete.swallow.icon", icon);
        evas_object_show(separator);

        pd_fill.origin_relative = prop_item_state_fill_origin_relative_x_y_add(box,
                                     pd, -500, 500, 1, NULL,
                                     "x:", "%", "y:", "%",
                                     _("Sets the starting point X coordinate "
                                       "relatively to displayed element's content."),
                                     _("Sets the starting point Y coordinate "
                                       "relatively to displayed element's content."),
                                     true);
        ICON_ADD(pd_fill.origin_relative, icon, false, "icon_align");
        elm_object_part_content_set(pd_fill.origin_relative, "eflete.swallow.icon", icon);
        pd_fill.origin_offset = prop_item_state_fill_origin_offset_x_y_add(box,
                                     pd, -9999.0, 9999.0, 1.0, "%.0f",
                                     "x:", "px", "y:", "px",
                                     _("Affects the starting point a fixed "
                                       "number of pixels along X axis"),
                                     _("Affects the starting point a fixed "
                                       "number of pixels along Y axis"),
                                     false);
        ICON_ADD(pd_fill.origin_offset, icon, false, "icon_offset");
        elm_object_part_content_set(pd_fill.origin_offset, "eflete.swallow.icon", icon);

        elm_box_pack_end(box, separator);
        elm_box_pack_end(box, pd_fill.origin_relative);
        elm_box_pack_end(box, pd_fill.origin_offset);

        /* size subblock of fill block */
        separator = elm_separator_add(fill_frame);
        elm_separator_horizontal_set(separator, true);
        elm_object_part_text_set(separator, "eflete.text", _("Size"));
        ICON_ADD(separator, icon, false, "icon_end-point")
        elm_object_part_content_set(separator, "eflete.swallow.icon", icon);
        evas_object_show(separator);

        pd_fill.size_relative = prop_item_state_fill_size_relative_x_y_add(box,
                                     pd, -500, 500, 1, NULL,
                                     "x:", "%", "y:", "%",
                                     _("Value that represent the percentual value "
                                       "of the original size of the element by X axis."),
                                     _("Value that represent the percentual value "
                                       "of the original size of the element by Y axis."),
                                      true);
        ICON_ADD(pd_fill.size_relative, icon, false, "icon_align");
        elm_object_part_content_set(pd_fill.size_relative, "eflete.swallow.icon", icon);
        pd_fill.size_offset = prop_item_state_fill_size_offset_x_y_add(box,
                                     pd, -9999.0, 9999.0, 1.0, "%.0f",
                                     "x:", "px", "y:", "px",
                                     _("Affects the size of the tile a fixed "
                                       "number of pixels along X axis."),
                                     _("Affects the size of the tile a fixed "
                                       "number of pixels along Y axis."),
                                     false);
        ICON_ADD(pd_fill.size_offset, icon, false, "icon_offset");
        elm_object_part_content_set(pd_fill.size_offset, "eflete.swallow.icon", icon);

        elm_box_pack_end(box, separator);
        elm_box_pack_end(box, pd_fill.size_relative);
        elm_box_pack_end(box, pd_fill.size_offset);

        pd_fill.frame = fill_frame;
        elm_box_pack_end(prop_box, pd_fill.frame);
     }
   else
     {
        prop_item_state_fill_type_update(pd_fill.type, pd);
        STATE_ATTR_CHECK_UPDATE(state_fill, smooth)
        prop_item_state_fill_origin_relative_x_y_update(pd_fill.origin_relative, pd, true);
        prop_item_state_fill_origin_offset_x_y_update(pd_fill.origin_offset, pd, false);
        prop_item_state_fill_size_relative_x_y_update(pd_fill.size_relative, pd, true);
        prop_item_state_fill_size_offset_x_y_update(pd_fill.size_offset, pd, false);

        elm_box_pack_end(prop_box, pd_fill.frame);
     }
   evas_object_show(pd_fill.frame);
   return true;
}

static void
ui_property_state_fill_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_fill.frame);
   evas_object_hide(pd_fill.frame);
}
#undef pd_fill

#define pd_box pd->state_box
#define ITEM_1COMBOBOX_PART_BOX_STATE_CREATE(TEXT, SUB, VALUE) \
   ITEM_COMBOBOX_STATE_BOX_CALLBACK(SUB, VALUE) \
   ITEM_1COMBOBOX_STATE_PART_BOX_UPDATE(SUB, VALUE) \
   ITEM_1COMBOBOX_STATE_PART_BOX_ADD(TEXT, SUB, VALUE)

ITEM_1COMBOBOX_PART_BOX_STATE_CREATE(_("layout"), state_box, layout)
ITEM_1COMBOBOX_PART_BOX_STATE_CREATE(_("alternative layout"), state_box, alt_layout)
ITEM_1ENTRY_STATE_CREATE(NULL, state_box, layout, &accept_prop, char *)
ITEM_1ENTRY_STATE_CREATE(NULL, state_box, alt_layout, &accept_prop, char *)

static Eina_Bool
ui_property_state_box_set(Evas_Object *property)
{
   Evas_Object *box_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   /* if previos selected part is BOX too, unpack it */
   ui_property_state_box_unset(property);
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_box.frame)
     {
        FRAME_PROPERTY_ADD(property, box_frame, true, _("Box"), pd->visual)
        BOX_ADD(box_frame, box, false, false)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(box_frame, box);

        pd_box.custom_layout = prop_item_state_box_layout_add(box, pd, NULL,
                               _("Used for typing custom name of "
                               "main (primary) layout "
                               "(default horizontal layout would be "
                               "applied to workspace and live object)."), NULL);
        pd_box.layout = prop_item_box_state_box_layout_add(box, pd,
                        _("Used for setting layout for box."),
                        edje_box_layouts);
        pd_box.custom_alt_layout = prop_item_state_box_alt_layout_add(box, pd, NULL,
                                   _("Used for typing custom name of "
                                   "alternative (fallback) layout "
                                   "(default horizontal layout would be "
                                   "applied to workspace and live object)."), NULL);
        pd_box.alt_layout = prop_item_box_state_box_alt_layout_add(box, pd,
                            _("Used for setting alternative layout for box."),
                            edje_box_layouts);
        elm_box_pack_end(box, pd_box.layout);
        elm_box_pack_end(box, pd_box.custom_layout);
        elm_box_pack_end(box, pd_box.alt_layout);
        elm_box_pack_end(box, pd_box.custom_alt_layout);

        pd_box.frame = box_frame;

        elm_box_pack_after(prop_box, pd_box.frame, pd->state_container.frame);
     }
   else
     {
        prop_item_box_state_box_layout_update(pd_box.layout, pd, edje_box_layouts);
        prop_item_state_box_layout_update(pd_box.custom_layout, pd);
        prop_item_box_state_box_alt_layout_update(pd_box.alt_layout, pd, edje_box_layouts);
        prop_item_state_box_alt_layout_update(pd_box.custom_alt_layout, pd);

        elm_box_pack_after(prop_box, pd_box.frame, pd->state_container.frame);
     }
   evas_object_show(pd_box.frame);
   return true;
}

static void
ui_property_state_box_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_box.frame);
   evas_object_hide(pd_box.frame);
}

#define ITEM_4SPINNER_ITEM_CALLBACK(VALUE) \
static void \
_on_part_item_padding_##VALUE##_change(void *data, \
                                       Evas_Object *obj, \
                                       void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Evas_Object *item = evas_object_data_get(obj, ITEM1); \
   Evas_Object *spinner1 = evas_object_data_get(item, ITEM1); \
   Evas_Object *spinner2 = evas_object_data_get(item, ITEM2); \
   Evas_Object *spinner3 = evas_object_data_get(item, ITEM3); \
   Evas_Object *spinner4 = evas_object_data_get(item, ITEM4); \
   int l = (int)elm_spinner_value_get(spinner1); \
   int r = (int)elm_spinner_value_get(spinner2); \
   int t = (int)elm_spinner_value_get(spinner3); \
   int b = (int)elm_spinner_value_get(spinner4); \
   if (!edje_edit_part_item_padding_set(pd->wm_style->obj, pd->wm_part->name,\
                                        pd->item_name, l, r, t, b)) \
     return; \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

ITEM_4SPINNER_ITEM_CALLBACK(l);
ITEM_4SPINNER_ITEM_CALLBACK(r);
ITEM_4SPINNER_ITEM_CALLBACK(t);
ITEM_4SPINNER_ITEM_CALLBACK(b);

#define ITEM_SPINNER_ITEM_PADDING_UPDATE(KEY, VALUE) \
   spinner = evas_object_data_get(item, KEY); \
   elm_spinner_value_set(spinner, VALUE); \
   evas_object_smart_callback_del_full(spinner, "changed", _on_part_item_padding_##VALUE##_change, pd); \
   evas_object_smart_callback_add(spinner, "changed", _on_part_item_padding_##VALUE##_change, pd);

static void
prop_item_part_item_padding_update(Evas_Object *item,
                                   Prop_Data *pd)
{
   int l = 0, r = 0, t = 0, b = 0;
   Evas_Object *spinner;

   edje_edit_part_item_padding_get(pd->wm_style->obj, pd->wm_part->name, pd->item_name,
                                   &l, &r, &t, &b);
   ITEM_SPINNER_ITEM_PADDING_UPDATE(ITEM1, l)
   ITEM_SPINNER_ITEM_PADDING_UPDATE(ITEM2, r)
   ITEM_SPINNER_ITEM_PADDING_UPDATE(ITEM3, t)
   ITEM_SPINNER_ITEM_PADDING_UPDATE(ITEM4, b)
}

static Evas_Object *
prop_item_part_item_padding_add(Evas_Object *parent,
                                Prop_Data *pd,
                                double min,
                                double max,
                                double step,
                                const char *fmt,
                                const char *sp1_lb_start,
                                const char *sp2_lb_start,
                                const char *sp3_lb_start,
                                const char *sp4_lb_start)
{
   Evas_Object *item, *item2, *spinner1, *spinner2, *spinner3, *spinner4;
   Evas_Object *box, *layout, *main_box;
   BOX_ADD(parent, main_box, false, false)

   ITEM_ADD(parent, item, _("Padding: "), "eflete/property/item/default")
   BOX_ADD(item, box, true, false)
   ITEM_CONTEINER_1LABEL_ADD(box, layout, sp1_lb_start);
   SPINNER_ADD(item, spinner1, min, max, step, true)
   elm_spinner_label_format_set(spinner1, fmt);
   elm_object_part_content_set(layout, "eflete.content", spinner1);
   elm_box_pack_end(box, layout);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   evas_object_event_callback_priority_add(spinner1, EVAS_CALLBACK_MOUSE_WHEEL,
                                           EVAS_CALLBACK_PRIORITY_BEFORE,
                                           _on_spinner_mouse_wheel, NULL);

   ITEM_CONTEINER_1LABEL_ADD(box, layout, sp2_lb_start);
   SPINNER_ADD(item, spinner2, min, max, step, true)
   elm_spinner_label_format_set(spinner2, fmt);
   elm_object_part_content_set(layout, "eflete.content", spinner2);
   elm_box_pack_end(box, layout);
   evas_object_event_callback_priority_add(spinner2, EVAS_CALLBACK_MOUSE_WHEEL,
                                           EVAS_CALLBACK_PRIORITY_BEFORE,
                                           _on_spinner_mouse_wheel, NULL);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   elm_box_pack_end(main_box, item);

   ITEM_ADD(parent, item2, "", "eflete/property/item/default")
   BOX_ADD(item2, box, true, true)
   elm_object_part_content_set(item2, "elm.swallow.content", box);
   ITEM_CONTEINER_1LABEL_ADD(box, layout, sp3_lb_start);
   SPINNER_ADD(item2, spinner3, min, max, step, true)
   elm_spinner_label_format_set(spinner3, fmt);
   elm_object_part_content_set(layout, "eflete.content", spinner3);
   elm_box_pack_end(box, layout);
   evas_object_event_callback_priority_add(spinner3, EVAS_CALLBACK_MOUSE_WHEEL,
                                           EVAS_CALLBACK_PRIORITY_BEFORE,
                                           _on_spinner_mouse_wheel, NULL);

   ITEM_CONTEINER_1LABEL_ADD(box, layout, sp4_lb_start);
   SPINNER_ADD(item2, spinner4, min, max, step, true)
   elm_spinner_label_format_set(spinner4, fmt);
   elm_object_part_content_set(layout, "eflete.content", spinner4);
   elm_box_pack_end(box, layout);
   evas_object_event_callback_priority_add(spinner4, EVAS_CALLBACK_MOUSE_WHEEL,
                                           EVAS_CALLBACK_PRIORITY_BEFORE,
                                           _on_spinner_mouse_wheel, NULL);
   elm_object_part_content_set(item2, "elm.swallow.content", box);
   elm_box_pack_end(main_box, item2);

   evas_object_data_set(main_box, ITEM1, spinner1);
   evas_object_data_set(main_box, ITEM2, spinner2);
   evas_object_data_set(main_box, ITEM3, spinner3);
   evas_object_data_set(main_box, ITEM4, spinner4);
   evas_object_data_set(spinner1, ITEM1, main_box);
   evas_object_data_set(spinner2, ITEM1, main_box);
   evas_object_data_set(spinner3, ITEM1, main_box);
   evas_object_data_set(spinner4, ITEM1, main_box);

   prop_item_part_item_padding_update(main_box, pd);
   return main_box;
}
#undef ITEM_SPINNER_ITEM_PADDING_UPDATE
#undef ITEM_4SPINNER_ITEM_CALLBACK

#define ITEM_COMBOBOX_PART_ITEM_CREATE(TEXT, SUB, VALUE) \
   ITEM_COMBOBOX_PART_ITEM_CALLBACK(SUB, VALUE) \
   ITEM_COMBOBOX_PART_ITEM_UPDATE(SUB, VALUE) \
   ITEM_COMBOBOX_PART_ITEM_ADD(TEXT, SUB, VALUE)

#define ITEM_2_SPINNERS_ITEM_CREATE(TYPE, TEXT, SUB, VALUE1, VALUE2, STYLE, MULTIPLIER) \
   ITEM_SPINNER_PART_ITEM_CALLBACK(TYPE, SUB, VALUE1, MULTIPLIER) \
   ITEM_SPINNER_PART_ITEM_CALLBACK(TYPE, SUB, VALUE2, MULTIPLIER) \
   ITEM_2SPINNER_PART_ITEM_UPDATE(TYPE, SUB, VALUE1, VALUE2, MULTIPLIER) \
   ITEM_2SPINNER_STATE_ADD(TEXT, SUB, VALUE1, VALUE2, STYLE)

#define ITEM_2_SPINNERS_ITEM_2INT_CREATE(TYPE, TEXT, SUB, VALUE, STYLE) \
   ITEM_SPINNER_PART_ITEM_2INT_CALLBACK(TYPE, SUB, VALUE) \
   ITEM_2SPINNER_PART_ITEM_2UPDATE(TYPE, SUB, VALUE, suf) \
   ITEM_2SPINNER_STATE_ADD(TEXT, SUB, VALUE, suf, STYLE)

#define ITEM_PREDEFINED_COMBOBOX_PART_ITEM_CREATE(TEXT, SUB, VALUE) \
   ITEM_PREDEFINED_COMBOBOX_PART_ITEM_CALLBACK(SUB, VALUE) \
   ITEM_PREDEFINED_COMBOBOX_PART_ITEM_UPDATE(SUB, VALUE) \
   ITEM_1COMBOBOX_STATE_PART_ADD(TEXT, SUB, VALUE)

#define pd_item pd->part_item

ITEM_COMBOBOX_PART_ITEM_CREATE(_("source"), part_item, source);
ITEM_2_SPINNERS_ITEM_CREATE(int, _("min"), part_item_min, w, h, "eflete/property/item/default", 1)
ITEM_2_SPINNERS_ITEM_CREATE(int, _("max"), part_item_max, w, h, "eflete/property/item/default", 1)
ITEM_2_SPINNERS_ITEM_CREATE(int, _("prefer"), part_item_prefer, w, h, "eflete/property/item/default", 1)
ITEM_2_SPINNERS_ITEM_CREATE(int, _("spread"), part_item_spread, w, h, "eflete/property/item/default", 1)
ITEM_2_SPINNERS_ITEM_CREATE(int, _("aspect"), part_item_aspect, w, h, "eflete/property/item/default", 1)
ITEM_2_SPINNERS_ITEM_2INT_CREATE(unsigned short int, _("position"), part_item, position, "eflete/property/item/default")
ITEM_2_SPINNERS_ITEM_2INT_CREATE(unsigned char, _("span"), part_item, span, "eflete/property/item/default")
ITEM_2_SPINNERS_ITEM_CREATE(double, _("align"), part_item_align, x, y, "eflete/property/item/default", 100)
ITEM_2_SPINNERS_ITEM_CREATE(double, _("weight"), part_item_weight, x, y, "eflete/property/item/default", 1)
ITEM_PREDEFINED_COMBOBOX_PART_ITEM_CREATE(_("aspect mode"), part_item, aspect_mode)

Eina_Bool
ui_property_item_set(Evas_Object *property, Eina_Stringshare *item_name)
{
   Evas_Object *item;
   Evas_Object *box, *prop_box, *item_box;
   PROP_DATA_GET(false)

   ui_property_item_unset(property);
   pd->item_name = item_name;
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_item.frame)
     {
        FRAME_ADD(property, pd_item.frame, true, _("Item"))
        BOX_ADD(pd_item.frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(pd_item.frame, box);

        item = prop_part_item_name_add(box, _("name"), pd->item_name);
        elm_box_pack_end(box, item);
        pd_item.source = prop_item_part_item_source_add(box, pd,
                               _("Sets the group this object will be made from."));
        pd_item.min = prop_item_part_item_min_w_h_add(box, pd,
                          0.0, 999.0, 1.0, "%.0f",
                          "w:", "px", "h:", "px",
                          _("Set the item minimum size hint width in pixels."),
                          _("Set the item minimum size hint height in pixels."),
                          false);
        pd_item.max = prop_item_part_item_max_w_h_add(box, pd,
                          0.0, 999.0, 1.0, "%.0f",
                          "w:", "px", "h:", "px",
                          _("Set the item maximum size hint width in pixels."),
                          _("Set the item maximum size hint height in pixels."),
                          false);
        pd_item.prefer = prop_item_part_item_prefer_w_h_add(box, pd,
                          0.0, 999.0, 1.0, "%.0f",
                          "w:", "px", "h:", "px",
                          _("Set the item prefered size hint width in pixels."),
                          _("Set the item prefered size hint height in pixels."),
                          false);
        pd_item.aspect = prop_item_part_item_aspect_w_h_add(box, pd,
                          0, 999, 1, "%.0f",
                          "x:", "", "y:", "",
                          _("Set the item aspect width hint."),
                          _("Set the item aspect height hint."),
                          false);
        pd_item.spread = prop_item_part_item_spread_w_h_add(box, pd,
                          1.0, 255.0, 1.0, "%.0f",
                          "col:", "", "row", "",
                          _("Replicate the item in width, starting from the current position."),
                          _("Replicate the item in height, starting from the current position."),
                          false);
        pd_item.span = prop_item_part_item_span_suf_add(box, pd,
                          1.0, 999.0, 1.0, "%.0f",
                          "col", "", "row", "",
                          _("Sets how many columns this item will use."),
                          _("Sets how many rows this item will use."), false);
        pd_item.align = prop_item_part_item_align_x_y_add(box, pd,
                          -1, 100, 1, NULL,
                          "x:", "%", "y:", "%",
                          _("Sets the alignment hint by x axiss."),
                          _("Sets the alignment hint by y axiss."),
                          true);
        pd_item.weight = prop_item_part_item_weight_x_y_add(box, pd,
                          0.0, 999.0, 1.0, NULL,
                          "x:", "%", "y:", "",
                          _("Sets the weight hint by x axiss."),
                          _("Sets the weight hint by y axiss."),
                          false);
        pd_item.padding =  prop_item_part_item_padding_add(box, pd,
                             0.0, 999.0, 1.0, "%.0f",
                            _("left:"), _("right:"), _("top:"), _("bottom:"));
        pd_item.aspect_mode = prop_item_part_item_aspect_mode_add(box, pd,
                              _("Sets the aspect control hints for this object."),
                              edje_item_aspect_pref);
        pd_item.position = prop_item_part_item_position_suf_add(box, pd,
                           0.0, 999.0, 1.0, "%.0f",
                           "col", "", "row", "",
                           _("Sets the column position this item."),
                           _("Sets the row position this item."), false);
        evas_object_hide(pd_item.position);

        elm_box_pack_end(box, pd_item.source);
        if (pd->wm_part->type == EDJE_PART_TYPE_TABLE)
          {
             elm_box_pack_end(box, pd_item.position);
             evas_object_show(pd_item.position);
          }

        elm_box_pack_end(box, pd_item.min);
        elm_box_pack_end(box, pd_item.max);
        elm_box_pack_end(box, pd_item.prefer);
        elm_box_pack_end(box, pd_item.align);
        elm_box_pack_end(box, pd_item.weight);
        elm_box_pack_end(box, pd_item.aspect);
        elm_box_pack_end(box, pd_item.aspect_mode);
        elm_box_pack_end(box, pd_item.padding);
        elm_box_pack_end(box, pd_item.spread);
        elm_box_pack_end(box, pd_item.span);
        elm_box_pack_before(prop_box, pd_item.frame, pd->part.frame);
     }
   else
     {
        prop_part_item_name_update(item_name);
        prop_item_part_item_source_update(pd_item.source, pd);
        prop_item_part_item_min_w_h_update(pd_item.min, pd, false);
        prop_item_part_item_max_w_h_update(pd_item.max, pd, false);
        prop_item_part_item_prefer_w_h_update(pd_item.prefer, pd, false);
        prop_item_part_item_align_x_y_update(pd_item.align, pd, false);
        prop_item_part_item_weight_x_y_update(pd_item.weight, pd, false);
        prop_item_part_item_aspect_w_h_update(pd_item.aspect, pd, false);
        prop_item_part_item_spread_w_h_update(pd_item.spread, pd, false);
        prop_item_part_item_span_suf_update(pd_item.span, pd, false);
        if (pd->wm_part->type == EDJE_PART_TYPE_TABLE)
          {
             prop_item_part_item_position_suf_update(pd_item.position, pd, false);
             item_box = elm_object_content_get(pd_item.frame);
             elm_box_pack_before(item_box, pd_item.position, pd_item.min);
             evas_object_show(pd_item.position);
          }
        prop_item_part_item_padding_update(pd_item.padding, pd);
        prop_item_part_item_aspect_mode_update(pd_item.aspect_mode, pd);
        elm_box_pack_before(prop_box, pd_item.frame, pd->part.frame);
        evas_object_show(pd_item.frame);
     }
   return true;
}

void
ui_property_item_unset(Evas_Object *property)
{
   Evas_Object *prop_box, *item_box;
   PROP_DATA_GET()
   if (!pd_item.frame) return;

   pd->item_name = NULL;
   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_item.frame);

   item_box = elm_object_content_get(pd_item.frame);
   elm_box_unpack(item_box, pd_item.position);

   evas_object_hide(pd_item.frame);
   evas_object_hide(pd_item.position);
}
#undef pd_item

#define pd_container pd->state_container

#define ITEM_2SPINNER_STATE_2DOUBLE_CREATE(TYPE, TEXT, SUB, VALUE1, VALUE2, STYLE) \
   ITEM_2SPINNER_STATE_VALUE_CALLBACK(TYPE, SUB, VALUE1) \
   ITEM_2SPINNER_STATE_VALUE_CALLBACK(TYPE, SUB, VALUE2) \
   ITEM_2SPINNER_STATE_VALUE_UPDATE(TYPE, SUB, VALUE1, VALUE2) \
   ITEM_2SPINNER_STATE_ADD(TEXT, SUB, VALUE1, VALUE2, STYLE)

ITEM_2SPINNER_STATE_2DOUBLE_CREATE(double, _("align"), state_container_align, x, y, "eflete/property/item/default")
ITEM_2SPINNER_STATE_2DOUBLE_CREATE(int, _("padding"), state_container_padding, h, v, "eflete/property/item/default")

static void
_on_container_min_change(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;
   Eina_Bool min_v, min_h;
   Evas_Object *check1, *check2;

   check1 = evas_object_data_get(pd_container.min, ITEM1);
   min_v = elm_check_state_get(check1);
   check2 = evas_object_data_get(pd_container.min, ITEM2);
   min_h = elm_check_state_get(check2);

   if (!edje_edit_state_container_min_set(pd->wm_style->obj, pd->wm_part->name,
                                          pd->wm_part->curr_state,
                                          pd->wm_part->curr_state_value,
                                          min_v, min_h))
     return;

   project_changed();
   workspace_edit_object_recalc(pd->workspace);
   pd->wm_style->isModify = true;
}

static void
prop_item_state_container_min_h_v_update(Evas_Object *item,
                                         Prop_Data *pd)
{
   Evas_Object *check1, *check2;
   Eina_Bool min_v, min_h;

   edje_edit_state_container_min_get(pd->wm_style->obj,
                                     pd->wm_part->name,
                                     pd->wm_part->curr_state,
                                     pd->wm_part->curr_state_value,
                                     &min_v,
                                     &min_h);

   check1 = evas_object_data_get(item, ITEM1);
   elm_check_state_set(check1, min_v);
   check2 = evas_object_data_get(item, ITEM2);
   elm_check_state_set(check2, min_h);

   evas_object_smart_callback_del_full(check1, "changed", _on_container_min_change, pd);
   evas_object_smart_callback_add(check1, "changed", _on_container_min_change, pd);
   evas_object_smart_callback_del_full(check2, "changed", _on_container_min_change, pd);
   evas_object_smart_callback_add(check2, "changed", _on_container_min_change, pd);
}

static Evas_Object *
prop_item_state_container_min_h_v_add(Evas_Object *parent,
                                      Prop_Data *pd,
                                      const char *tooltip1,
                                      const char *tooltip2)
{
   Evas_Object *item, *box, *layout, *check1, *check2;
   Eina_Bool min_v, min_h;

   edje_edit_state_container_min_get(pd->wm_style->obj,
                                     pd->wm_part->name,
                                     pd->wm_part->curr_state,
                                     pd->wm_part->curr_state_value,
                                     &min_v,
                                     &min_h);

   ITEM_ADD(parent, item, _("Min: "), "eflete/property/item/default")
   BOX_ADD(item, box, true, true)

   ITEM_CONTEINER_2LABEL_ADD(box, layout, NULL, NULL);
   CHECK_ADD(layout, check1)
   elm_object_style_set(check1, "toggle");
   elm_check_state_set(check1, min_v);
   elm_object_tooltip_text_set(check1, tooltip1);
   evas_object_smart_callback_add(check1, "changed", _on_container_min_change, pd);
   elm_object_part_content_set(layout, "eflete.content", check1);
   elm_box_pack_end(box, layout);

   ITEM_CONTEINER_2LABEL_ADD(box, layout, NULL, NULL);
   CHECK_ADD(layout, check2)
   elm_object_style_set(check2, "toggle");
   elm_check_state_set(check2, min_h);
   elm_object_tooltip_text_set(check2, tooltip2);
   evas_object_smart_callback_add(check2, "changed", _on_container_min_change, pd);
   elm_object_part_content_set(layout, "eflete.content", check2);
   elm_box_pack_end(box, layout);

   elm_object_part_content_set(item, "elm.swallow.content", box);
   evas_object_data_set(item, ITEM1, check1);
   evas_object_data_set(item, ITEM2, check2);
   return item;
}

static Eina_Bool
ui_property_state_container_set(Evas_Object *property)
{
   Evas_Object *container_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   ui_property_state_container_unset(property);
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_container.frame)
     {
        FRAME_ADD(property, container_frame, true, _("Container"))
        BOX_ADD(container_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(container_frame, box);

        pd_container.align = prop_item_state_container_align_x_y_add(box, pd,
                              0, 100, 1, NULL,
                              _("hor:"), "%", _("ver:"), "%",
                              _("Change the position of the point of balance inside the container."),
                              _("Change the position of the point of balance inside the container."),
                              true);
        pd_container.padding = prop_item_state_container_padding_h_v_add(box, pd, 0.0, 999.0,
                                1.0, "%.0f", _("hor:"), _("px"), _("ver:"), _("px"),
                                _("Sets the horizontal space between cells in pixels."),
                                _("Sets the vertcal space between cells in pixels."),
                                false);
        pd_container.min = prop_item_state_container_min_h_v_add(box, pd,
                            _("This affects the minimum width calculation."),
                            _("This affects the minimum height calculation."));

        elm_box_pack_end(box, pd_container.align);
        elm_box_pack_end(box, pd_container.padding);
        elm_box_pack_end(box, pd_container.min);
        pd_container.frame = container_frame;
        elm_box_pack_after(prop_box, pd_container.frame, pd->part.frame);
     }
   else
     {
        prop_item_state_container_align_x_y_update(pd_container.align, pd, true);
        prop_item_state_container_padding_h_v_update(pd_container.padding, pd, false);
        prop_item_state_container_min_h_v_update(pd_container.min, pd);
        elm_box_pack_after(prop_box, pd_container.frame, pd->part.frame);
        evas_object_show(pd_container.frame);
     }
   return true;
}
static void
ui_property_state_container_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_container.frame);
   evas_object_hide(pd_container.frame);
}

#undef pd_container

#define pd_table pd->state_table

ITEM_1COMBOBOX_PART_STATE_CREATE(_("homogeneous"), state_table, homogeneous, unsigned char)

static Eina_Bool
ui_property_state_table_set(Evas_Object *property)
{
   Evas_Object *table_frame, *box, *prop_box;
   PROP_DATA_GET(EINA_FALSE)

   ui_property_state_table_unset(property);
   prop_box = elm_object_content_get(pd->visual);
   if (!pd_table.frame)
     {
        FRAME_PROPERTY_ADD(property, table_frame, true, _("Table"), pd->visual)
        BOX_ADD(table_frame, box, EINA_FALSE, EINA_FALSE)
        elm_box_align_set(box, 0.5, 0.0);
        elm_object_content_set(table_frame, box);

        pd_table.homogeneous = prop_item_state_table_homogeneous_add(box, pd,
                             _("Sets the homogeneous mode for the table."),
                             edje_homogeneous);

        elm_box_pack_end(box, pd_table.homogeneous);
        pd_table.frame = table_frame;
        elm_box_pack_after(prop_box, pd_table.frame, pd->state_container.frame);
     }
   else
     {
        prop_item_state_table_homogeneous_update(pd_table.homogeneous, pd);
        elm_box_pack_after(prop_box, pd_table.frame, pd->state_container.frame);
     }
   evas_object_show(pd_table.frame);
   return true;
}

static void
ui_property_state_table_unset(Evas_Object *property)
{
   Evas_Object *prop_box;
   PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd_table.frame);
   evas_object_hide(pd_table.frame);
}
#undef pd_table

#undef PROP_DATA
#undef PROP_DATA_GET
