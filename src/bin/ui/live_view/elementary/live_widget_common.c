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

#include "live_elementary_widgets.h"

Eina_Bool
standard_widget_name_parse(const char *full_group_name,
                           Eina_Stringshare **widget,
                           Eina_Stringshare **class,
                           Eina_Stringshare **style_name)
{
   char **c;
   unsigned int count;

   assert(full_group_name != NULL);

   c = eina_str_split_full(full_group_name, "/", 4, &count);

   if (count < 4)
     {
        if (widget) *widget = NULL;
        if (class) *class = NULL;
        if (style_name) *style_name = NULL;

        free(c[0]);
        free(c);
        return false;
     }

   if (widget) *widget = eina_stringshare_add(c[1]);
   if (class) *class = eina_stringshare_add(c[2]);
   if (style_name) *style_name = eina_stringshare_add(c[3]);

   free(c[0]);
   free(c);
   return true;
}

static char *
_gen_text_get(void        *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   return strdup(_("User Text"));
}
static Evas_Object *
_gen_content_get(void *data __UNUSED__,
                   Evas_Object *obj,
                   const char  *part __UNUSED__)
{

   Evas_Object *content = elm_button_add(obj);
   elm_object_text_set(content, _("User Text"));
   return content;
}
static void
_pop_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *nf = data;

   assert(nf != NULL);

   elm_naviframe_item_pop(nf);
}

static void
_next_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data, *bt;

   bt = elm_button_add(nf);
   elm_object_text_set(bt, _("Back"));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(bt, "clicked", _pop_page_cb, nf);
   evas_object_show(bt);

   elm_naviframe_item_push(nf, _("Page Next"), bt, NULL, NULL, NULL);
}

static void
_prev_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data, *bt;

   bt = elm_button_add(nf);
   elm_object_text_set(bt, _("Back"));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(bt, "clicked", _pop_page_cb, nf);
   evas_object_show(bt);

   elm_naviframe_item_push(nf, _("Page Prev"), NULL, bt, NULL, NULL);
}
Evas_Object *
object_generate(Demo_Part *part, Evas_Object *object)
{
   Evas_Object *content = NULL, *bt = NULL, *bt2 = NULL, *table = NULL;
   Elm_Genlist_Item_Class *ic = NULL;
   Elm_Gengrid_Item_Class *icg = NULL;
   Elm_Object_Item *item;
   Elm_Genlist_Item_Type type = 0;
   unsigned int i = 0, j = 0;

   int content_type = part->swallow_content;
   int widget_type = part->widget;

   if (content_type == CONTENT_RECTANGLE) /* create rect */
     {
        content = evas_object_rectangle_add(object);
     }
   else if ((content_type == CONTENT_IMAGE) && (part->image_path)) /* create picture */
     {
        content = elm_image_add(object);
        elm_image_file_set(content, part->image_path, NULL);
     }
   else if (content_type == CONTENT_WIDGET) /* create widget */
     {
        switch (widget_type)
          {
           case WIDGET_LAYOUT:
              content = elm_layout_add(object);
              elm_layout_file_set(content, ap.project->dev, part->content_style);
              break;
           case WIDGET_BUTTON:
              content = elm_button_add(object);
              elm_object_text_set(content, _("User Text"));
              break;
           case WIDGET_CHECK:
              content = elm_check_add(object);
              elm_object_text_set(content, _("User Text"));
              elm_check_state_set(content, true);
              break;
           case WIDGET_SLIDER:
              content = elm_slider_add(object);
              elm_slider_unit_format_set(content, "%1.2f units");
              elm_slider_indicator_format_set(content, "%1.2f");
              elm_slider_min_max_set(content, 0, 10);
              break;
           case WIDGET_ENTRY:
              content = elm_entry_add(object);
              elm_entry_single_line_set(content, false);
              break;
           case WIDGET_PROGRESSBAR:
              content = elm_progressbar_add(object);
              break;
           case WIDGET_RADIO:
              content = elm_radio_add(object);
              elm_object_text_set(content, _("User Text"));
              break;
           case WIDGET_SPINNER:
              content = elm_spinner_add(object);
              elm_spinner_min_max_set(content, -10, 10);
              elm_spinner_step_set(content, 1);
              break;
           case WIDGET_LABEL:
              content = elm_label_add(object);
              elm_object_text_set(content, _("User Text"));
              break;
           case WIDGET_MAP:
              content = elm_map_add(object);
              elm_map_overlay_circle_add(content, -45, -45, 10);
              break;
           case WIDGET_ICON:
              content = elm_icon_add(object);
              elm_icon_order_lookup_set(content,
                                        ELM_ICON_LOOKUP_THEME_FDO);
              elm_icon_standard_set(content, "home");
              break;
           case WIDGET_COLORSELECTOR:
              content = elm_colorselector_add(object);
              elm_colorselector_mode_set(content, ELM_COLORSELECTOR_ALL);
              break;
           case WIDGET_FILESELECTOR:
               content = elm_fileselector_add(object);
               elm_fileselector_expandable_set(content, false);
              break;
           case WIDGET_DAYSELECTOR:
              content = elm_dayselector_add(object);
              break;
           case WIDGET_DATETIME:
              content = elm_datetime_add(object);
              break;
           case WIDGET_FRAME:
              content = elm_frame_add(object);
              elm_frame_autocollapse_set(content, true);
              elm_object_text_set(content, _("User Text"));
              break;
           case WIDGET_PANEL:
              content = elm_panel_add(object);
              elm_panel_orient_set(content, ELM_PANEL_ORIENT_TOP);
              elm_panel_toggle(content);
              break;
           case WIDGET_SEGMENT_CONTROL:
              content = elm_segment_control_add(object);
              elm_segment_control_item_insert_at(content, NULL, _("User Text"), 1);
              elm_segment_control_item_insert_at(content, NULL, _("User Text"), 2);
              elm_segment_control_item_insert_at(content, NULL, _("User Text"), 3);
              break;
           case WIDGET_CLOCK:
              content = elm_clock_add(object);
              elm_clock_show_seconds_set(content, true);
              break;
           case WIDGET_PANES:
              content = elm_panes_add(object);
              bt = elm_button_add(content);
              elm_object_text_set(bt, _("Left"));
              evas_object_show(bt);
              elm_object_part_content_set(content, "left", bt);

              bt = elm_button_add(content);
              elm_object_text_set(bt, _("Right"));
              evas_object_show(bt);
              elm_object_part_content_set(content, "right", bt);
              break;
           case WIDGET_GENLIST:
              content = elm_genlist_add(object);
              evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
              evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
              elm_genlist_mode_set(content, ELM_LIST_SCROLL);
              if (!ic)
                {
                   ic = elm_genlist_item_class_new();
                   ic->func.text_get = _gen_text_get;
                   ic->func.content_get = _gen_content_get;
                   ic->func.state_get = NULL;
                   ic->func.del = NULL;
                }
              for (i = 0; i < ELEMENTS_BIG_COUNT; i++)
                elm_genlist_item_append(content, ic, NULL,
                                        NULL, type, NULL, NULL);
              elm_genlist_item_class_free(ic);
              break;
           case WIDGET_GENGRID:
              content = elm_gengrid_add(object);
              double scale = elm_config_scale_get();
              elm_gengrid_item_size_set(content, scale * 100, scale * 100);
              evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
              evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
              if (!icg)
                {
                   icg = elm_gengrid_item_class_new();
                   icg->func.text_get = _gen_text_get;
                   icg->func.content_get = _gen_content_get;
                   icg->func.state_get = NULL;
                   icg->func.del = NULL;
                }
              for (i = 0; i < ELEMENTS_BIG_COUNT; i++)
                elm_gengrid_item_append(content, icg, NULL, NULL, NULL);
              elm_gengrid_item_class_free(icg);
              break;
           case WIDGET_SCROLLER:
              content = elm_scroller_add(object);
              elm_scroller_policy_set(object, ELM_SCROLLER_POLICY_ON,
                                      ELM_SCROLLER_POLICY_ON);
              table = elm_table_add(content);
              for (j = 0; j < ELEMENTS_MID_COUNT; j++)
                {
                   for (i = 0; i < ELEMENTS_MID_COUNT; i++)
                     {
                        bt = elm_button_add(table);
                        elm_object_text_set(bt, _("User Text"));
                        elm_table_pack(table, bt, i, j, 1, 1);
                        evas_object_show(bt);
                     }
                }
              elm_object_content_set(content, table);
              evas_object_show(table);
              break;
           case WIDGET_TOOLBAR:
              content = elm_toolbar_add(object);

              elm_toolbar_shrink_mode_set(content, ELM_TOOLBAR_SHRINK_EXPAND);

              elm_toolbar_homogeneous_set(content, false);
              elm_toolbar_standard_priority_set(content, 50);

              bt = elm_button_add(content);
              elm_object_part_text_set(bt, NULL, _("Object"));
              item = elm_toolbar_item_append(content, NULL, NULL, NULL, NULL);
              elm_toolbar_item_priority_set(item, 20 * i);
              elm_object_item_part_content_set(item, NULL, bt);
              item = elm_toolbar_item_append(content, "folder-new", _("Enabled"), NULL, NULL);
              elm_toolbar_item_priority_set(item, 20 * i);
              item = elm_toolbar_item_append(content, "folder-new", _("Disabled"), NULL, NULL);
              elm_toolbar_item_priority_set(item, 20 * i);
              elm_object_item_disabled_set(item, true);
              item = elm_toolbar_item_append(content, NULL, NULL, NULL, NULL);
              elm_toolbar_item_priority_set(item, 20 * i);
              elm_toolbar_item_separator_set(item, true);

              break;
            case WIDGET_LIST:
              content = elm_list_add(object);
              for (i = 0; i < ELEMENTS_MID_COUNT; i++)
                {
                   bt = elm_button_add(content);
                   elm_object_text_set(bt, _("User Text"));
                   bt2 = elm_button_add(content);
                   elm_object_text_set(bt2, _("User Text"));
                   elm_list_item_append(content, _("No icons"), bt, bt2, NULL, NULL);
                }
              break;
            case WIDGET_GRID:
              content = elm_grid_add(object);
              elm_grid_size_set(content, 5, 5);
              for (j = 0; j < ELEMENTS_SMALL_COUNT; j++)
                {
                   for (i = 0; i < ELEMENTS_SMALL_COUNT; i++)
                     {
                        bt = elm_button_add(content);
                        elm_object_text_set(bt, _("User Text"));
                        elm_grid_pack(content, bt, i, j, 1, 1);
                        evas_object_show(bt);
                     }
                }
              break;
            case WIDGET_TABLE:
              content = elm_grid_add(object);
              elm_grid_size_set(content, 5, 5);
              for (j = 0; j < ELEMENTS_SMALL_COUNT; j++)
                {
                   for (i = 0; i < ELEMENTS_SMALL_COUNT; i++)
                     {
                        bt = elm_button_add(table);
                        elm_object_text_set(bt, _("User Text"));
                        elm_table_pack(table, bt, i, j, 1, 1);
                        evas_object_show(bt);
                     }
                }
              break;
            case WIDGET_BOX:
              content = elm_box_add(object);
              evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
              for (i = 0; i < ELEMENTS_SMALL_COUNT; i++)
                {
                   bt = elm_button_add(content);
                   elm_object_text_set(bt, _("User Text"));
                   elm_box_pack_end(content, bt);
                   evas_object_show(bt);
                }
              break;
            case WIDGET_NAVIFRAME:
              content = elm_naviframe_add(object);
              evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
              elm_naviframe_prev_btn_auto_pushed_set(content, false);
              item = elm_naviframe_item_push(content, _("Page 1"), NULL, NULL, NULL, NULL);
              elm_object_item_part_text_set(item, "subtitle", _("Subtitle 1"));
              elm_object_item_part_text_set(item, "title", _("Main Page"));
              evas_object_data_set(content, "main_page", item);

              bt = elm_button_add(content);
              elm_object_text_set(bt, _("Prev page"));
              elm_object_part_content_set(content, "elm.swallow.prev_btn", bt);
              evas_object_smart_callback_add(bt, "clicked", _prev_page_cb, content);

              bt2 = elm_button_add(content);
              elm_object_text_set(bt2, _("Next page"));
              elm_object_part_content_set(content, "elm.swallow.next_btn", bt2);
              evas_object_smart_callback_add(bt2, "clicked", _next_page_cb, content);
              break;
          }
        Elm_Theme *theme = elm_theme_new();
        elm_theme_set(theme, ap.project->dev);
        elm_object_theme_set(content, theme);
        elm_theme_free(theme);

        elm_object_style_set(content, part->content_style);
     }

   return content;
}

void
on_swallow_check(void *data __UNUSED__,
                 Evas_Object *obj,
                 void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *content;

   if (part->change)
     {
        if (part->object)
          {
             content = elm_object_part_content_unset(obj, part->name);
             evas_object_del(content);
             content = NULL;
             part->object = NULL;
          }

        part->object = object_generate(part, obj);
        part->change = false;
        elm_object_part_content_set(obj, part->name, part->object);
     }

   if (part->object)
     {
        if (part->widget < WIDGET_GENLIST)
        evas_object_color_set(part->object,
                              part->r,
                              part->g,
                              part->b,
                              part->a);

        evas_object_size_hint_min_set(part->object,
                                      part->min_w,
                                      part->min_h);
        evas_object_size_hint_max_set(part->object,
                                      part->max_w,
                                      part->max_h);
     }
}

void
on_text_check(void *data __UNUSED__,
              Evas_Object *obj,
              void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;

   elm_object_part_text_set(obj, part->name, part->text_content);
}

void
send_signal(void *data __UNUSED__,
             Evas_Object *obj,
             void *ei)
{
   Demo_Signal *sig = (Demo_Signal *)ei;

   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   elm_layout_signal_emit(obj, sig->sig_name, sig->source_name);
}
