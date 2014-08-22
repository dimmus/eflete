/**
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

#include "style_editor.h"
#include "main_window.h"
#define FONT_DEFAULT "DEFAULT='align=middle font=Sans font_size=24 color=#000 "

typedef struct _Style_Tag_Entries Style_Tag_Entries;
typedef struct _Style_entries Style_Entries;
typedef struct _Style_Editor Style_Editor;

struct _Style_Editor
{
   Project *pr;
   Evas_Object *mwin;
   Evas_Object *glist;
   Evas_Object *entry_style;
   Evas_Object *entry_tag;
   Evas_Object *entry_prop;
   Evas_Object *entry_prev;
   struct {
        Evas_Object *name;
        Evas_Object *value;
        Evas_Object *dialog;
   } popup;
   Elm_Object_Item *tag;
};


struct _Style_Entries
{
   Evas_Object *style_name;
   Evas_Object *default_tags;
};

struct _Style_Tag_Entries
{
   Elm_Object_Item *style_name;
   Evas_Object *tag_name;
   Evas_Object *tag_value;
};

#define POPUP style_edit->popup

static Elm_Genlist_Item_Class *_itc_style = NULL;
static Elm_Genlist_Item_Class *_itc_tags = NULL;

static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = BANNED_SYMBOLS
};

static Elm_Entry_Filter_Accept_Set accept_value = {
   .accepted = NULL,
   .rejected = EDITORS_BANNED_SYMBOLS
};

static void
_on_popup_bt_cancel(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *ei __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;

   evas_object_del(POPUP.dialog);
   POPUP.dialog = NULL;
}

/* For showing tags value and name in appropriate entry. */
static void
_on_glit_selected(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   Eina_List *tags = NULL;
   Eina_List *l = NULL;
   Evas_Object *edje_edit_obj = NULL;

   const char *style_name = NULL;
   const char *tag, *value;
   Evas_Textblock_Style *ts = NULL;

   Eina_Strbuf *style = eina_strbuf_new();
   eina_strbuf_append(style, FONT_DEFAULT);

   Style_Editor *style_edit = (Style_Editor *)data;
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Elm_Object_Item *glit_parent = elm_genlist_item_parent_get(glit);

   GET_OBJ(style_edit->pr, edje_edit_obj);

   if (!glit_parent)
     {
        style_name = (char *)elm_object_item_data_get(glit);
        tags = edje_edit_style_tags_list_get(edje_edit_obj, style_name);

        EINA_LIST_FOREACH(tags, l, tag)
          {
             value = edje_edit_style_tag_value_get(edje_edit_obj, style_name,
                                                   tag);
             eina_strbuf_append(style, value);
          }
        ewe_entry_entry_set(style_edit->entry_tag, "");
        ewe_entry_entry_set(style_edit->entry_prop, "");
        eina_list_free(tags);
     }
   else
     {
        style_name = elm_object_item_data_get(glit_parent);
        tag = (char *)elm_object_item_data_get(glit);
        value = edje_edit_style_tag_value_get(edje_edit_obj, style_name, tag);
        ewe_entry_entry_set(style_edit->entry_tag, tag);
        ewe_entry_entry_set(style_edit->entry_prop, value);
        eina_strbuf_append(style, value);
     }
   elm_object_signal_emit(style_edit->entry_prev, "entry,show", "eflete");
   eina_strbuf_append(style, "'");
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, eina_strbuf_string_get(style));
   evas_object_textblock_style_set(style_edit->entry_style, ts);
   evas_object_size_hint_max_set(style_edit->entry_style, EVAS_HINT_FILL,
                                 EVAS_HINT_FILL);
   eina_strbuf_free(style);
   evas_textblock_style_free(ts);
}

static void
_on_st_add_bt_ok(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   Elm_Object_Item *glit_style = NULL;
   Evas_Object *edje_edit_obj = NULL;
   Style_Editor *style_edit = (Style_Editor *)data;
   const char *style_name = elm_entry_entry_get(POPUP.name);
   const char *default_tags = elm_entry_entry_get(POPUP.value);

   GET_OBJ(style_edit->pr, edje_edit_obj);

   if ((!style_name) || (strcmp(style_name, "") == 0))
     {
        NOTIFY_WARNING(_("Style name can not be empty!"));
        return;
     }
   if (!((isalpha(default_tags[0])) || (default_tags[0] == '+')
         || (!strcmp(default_tags, ""))))
     {
        NOTIFY_WARNING(_("The default tag must begin from + or alphabetic symbol"));
        return;
     }
   if (!edje_edit_style_add(edje_edit_obj, style_name))
     {
        NOTIFY_WARNING(_("Style name must be unique!"));
        return;
     }
   if (edje_edit_style_tag_add(edje_edit_obj, style_name, "DEFAULT"))
     {
        if (!edje_edit_style_tag_value_set(edje_edit_obj, style_name, "DEFAULT",
                                        default_tags))
          {
             NOTIFY_WARNING(_("Failed to add tag value. Tag will be deleted"));
             edje_edit_style_tag_del(edje_edit_obj, style_name, "DEFAULT");
             return;
          }
     }
   else
     {
        NOTIFY_WARNING(_("Failed to add tag."));
        return;
     }

   glit_style = elm_genlist_item_append(style_edit->glist, _itc_style,
                                        style_name, NULL, ELM_GENLIST_ITEM_NONE,
                                        _on_glit_selected, style_edit);
   elm_object_item_data_set(glit_style, (char *)style_name);

   elm_genlist_item_append(style_edit->glist, _itc_tags,
                           "DEFAULT", glit_style, ELM_GENLIST_ITEM_NONE,
                           _on_glit_selected, style_edit);
   evas_object_del(POPUP.dialog);
   POPUP.dialog = NULL;
   elm_genlist_item_selected_set(glit_style, true);
   elm_genlist_item_bring_in(glit_style, ELM_GENLIST_ITEM_SCROLLTO_IN);
   elm_genlist_item_show(glit_style, ELM_GENLIST_ITEM_SCROLLTO_IN);
}

static void
_on_tag_add_bt_ok(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *ei __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;
   Elm_Object_Item *glit_tag = NULL;
   Evas_Object *edje_edit_obj = NULL;
   const char *style_name = elm_object_item_data_get(style_edit->tag);
   const char *tag_name = elm_entry_entry_get(POPUP.name);
   const char *tag_value = elm_entry_entry_get(POPUP.value);
   GET_OBJ(style_edit->pr, edje_edit_obj);

   if ((!tag_name) || (strcmp(tag_name, "") == 0))
     {
        NOTIFY_WARNING(_("Tag name can not be empty!"));
        return;
     }
   if ((!tag_value) || (strcmp(tag_value, "") == 0))
     {
        NOTIFY_WARNING(_("Tag value can not be empty!"));
        return;
     }
   if (!((isalpha(tag_value[0])) || (tag_value[0] == '+')))
     {
        NOTIFY_WARNING(_("Tag value must begin from + or alphabetic symbol"));
        return;
     }
   if (!edje_edit_style_tag_add(edje_edit_obj, style_name, tag_name))
     {
        NOTIFY_WARNING(_("Tag name must be unique!"));
        return;
     }
   else
     if (!edje_edit_style_tag_value_set(edje_edit_obj, style_name, tag_name,
                                        tag_value))
       {
          NOTIFY_WARNING(_("Failed to add tag value. Tag will be deleted"));
          edje_edit_style_tag_del(edje_edit_obj, style_name, tag_name);
          return;
       }
   glit_tag = elm_genlist_item_append(style_edit->glist, _itc_tags,
                                      tag_name, style_edit->tag,
                                      ELM_GENLIST_ITEM_NONE,
                                      _on_glit_selected, style_edit);
   elm_object_item_data_set(glit_tag,(char *)tag_name);
   evas_object_del(POPUP.dialog);
   POPUP.dialog = NULL;
   elm_genlist_item_selected_set(glit_tag, true);
   elm_genlist_item_show(style_edit->tag, ELM_GENLIST_ITEM_SCROLLTO_IN);
   elm_genlist_item_bring_in(glit_tag, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
}

static void
_on_bt_style_add(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *box, *item, *button;
   Style_Editor *style_edit = (Style_Editor *)data;

   POPUP.dialog = elm_popup_add(style_edit->mwin);
   elm_object_style_set(POPUP.dialog, "eflete");
   elm_object_part_text_set(POPUP.dialog, "title,text", _("Add textblock style"));

   BOX_ADD(POPUP.dialog, box, false, false);
   ITEM_ADD(box, item, "Style name:", "eflete/property/item/default")

   EWE_ENTRY_ADD(item, POPUP.name, true, DEFAULT_STYLE);
   elm_object_part_text_set(POPUP.name, "guide", _("Type a new style name."));
   elm_entry_markup_filter_append(POPUP.name, elm_entry_filter_accept_set,
                                  &accept_name);
   elm_object_part_content_set(item, "elm.swallow.content", POPUP.name);
   elm_box_pack_end(box, item);

   ITEM_ADD(box, item, "Default tags:", "eflete/property/item/default")
   EWE_ENTRY_ADD(box, POPUP.value, true, DEFAULT_STYLE);
   elm_object_part_text_set(POPUP.value, "guide",
                            _("Type tag which will be used as default."));
   elm_entry_markup_filter_append(POPUP.value, elm_entry_filter_accept_set,
                                  &accept_value);
   elm_object_part_content_set(item, "elm.swallow.content", POPUP.value);
   elm_box_pack_end(box, item);

   elm_object_content_set(POPUP.dialog, box);
   evas_object_show(box);

   BUTTON_ADD(POPUP.dialog, button, _("Ok"));
   evas_object_smart_callback_add(button, "clicked", _on_st_add_bt_ok,
                                  style_edit);
   elm_object_part_content_set(POPUP.dialog, "button1", button);

   BUTTON_ADD(POPUP.dialog, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_popup_bt_cancel,
                                  style_edit);
   elm_object_part_content_set(POPUP.dialog, "button2", button);

   evas_object_show(POPUP.dialog);
}

static void
_on_bt_tag_add(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *box, *item, *button;

   Style_Editor *style_edit = (Style_Editor *)data;
   Elm_Object_Item *glit = elm_genlist_selected_item_get(style_edit->glist);
   Elm_Object_Item *glit_parent = elm_genlist_item_parent_get(glit);
   const char *style_name;
   Eina_Stringshare *buf;

   if (!glit)
     {
         NOTIFY_WARNING(_("Select a style!"));
         return;
     }

   if (!glit_parent)
     {
         style_name = elm_object_item_data_get(glit);
         style_edit->tag = glit;
     }
   else
     {
         style_name = elm_object_item_data_get(glit_parent);
         style_edit->tag = glit_parent;
     }

   POPUP.dialog = elm_popup_add(style_edit->mwin);
   elm_object_style_set(POPUP.dialog, "eflete");
   buf = eina_stringshare_printf(_("Add tag to style: %s"), style_name);
   elm_object_part_text_set(POPUP.dialog, "title,text", buf);

   BOX_ADD(POPUP.dialog, box, false, false);
   ITEM_ADD(box, item, "Tag name:", "eflete/property/item/default")

   EWE_ENTRY_ADD(item, POPUP.name, true, DEFAULT_STYLE);
   elm_object_part_text_set(POPUP.name, "guide", _("Type a new tag name."));
   elm_entry_markup_filter_append(POPUP.name, elm_entry_filter_accept_set,
                                  &accept_name);
   elm_object_part_content_set(item, "elm.swallow.content", POPUP.name);
   elm_box_pack_end(box, item);

   ITEM_ADD(box, item, "Tag value:", "eflete/property/item/default")
   EWE_ENTRY_ADD(box, POPUP.value, true, DEFAULT_STYLE);
   elm_object_part_text_set(POPUP.value, "guide",
                            _("Type tag with be using as default."));
   elm_entry_markup_filter_append(POPUP.value, elm_entry_filter_accept_set,
                                  &accept_value);
   elm_object_part_content_set(item, "elm.swallow.content", POPUP.value);
   elm_box_pack_end(box, item);

   elm_object_content_set(POPUP.dialog, box);
   evas_object_show(box);

   BUTTON_ADD(POPUP.dialog, button, _("Ok"));
   evas_object_smart_callback_add(button, "clicked", _on_tag_add_bt_ok,
                                  style_edit);
   elm_object_part_content_set(POPUP.dialog, "button1", button);

   BUTTON_ADD(POPUP.dialog, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_popup_bt_cancel,
                                  style_edit);
   elm_object_part_content_set(POPUP.dialog, "button2", button);

   evas_object_show(POPUP.dialog);
   eina_stringshare_del(buf);
}

static void
_on_bt_del(void *data,
           Evas_Object *obj __UNUSED__,
           void *event_info __UNUSED__)
{
   Evas_Object *edje_edit_obj = NULL;
   const char *style_name, *tag;

   Style_Editor *style_edit = (Style_Editor *)data;
   Elm_Object_Item *glit = elm_genlist_selected_item_get(style_edit->glist);
   Elm_Object_Item *glit_parent = elm_genlist_item_parent_get(glit);
   GET_OBJ(style_edit->pr, edje_edit_obj);

   if (!glit_parent)
     {
        style_name = elm_object_item_part_text_get(glit, "elm.text");
        edje_edit_style_del(edje_edit_obj, style_name);
     }
   else
     {
        style_name = elm_object_item_part_text_get(glit_parent, "elm.text");
        tag = elm_object_item_part_text_get(glit, "elm.text");
        if (!strcmp(tag, "DEFAULT"))
          {
             NOTIFY_WARNING(_("DEFAULT tag cannot be deleted!"));
             return;
          }
        else
          edje_edit_style_tag_del(edje_edit_obj, style_name, tag);
     }
   elm_object_item_del(glit);
}

/* For GenList, getting the content for showing. Tag Names. */
static char *
_item_tags_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   char *tag_label = (char *)data;
   if (!tag_label)
     {
        ERR("It impossible, but it is occurred, tag's name is missing!");
        return NULL;
     }
   return strdup(tag_label);
}

/* For Style names. */
static char *
_item_style_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   char *style_label = (char *)data;
   /* char *formated_style_name; */
   if (!style_label)
     {
        ERR("It impossible, but it is occurred, style's name is missing!");
        return NULL;
     }
   /* 6 - <bold> */
   /*
   formated_style_name = mem_malloc(7 + strlen(style_label));
   sprintf(formated_style_name, "%s%s", "<bold>", style_label);
   return formated_style_name;
   */
   return strdup(style_label);
}

static void
_on_viewer_exit(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;

   pm_project_changed(app_data_get()->project);
   evas_object_del(style_edit->mwin);
}

/* Creating the view of the mwin!!! */
Evas_Object *
_form_left_side(Style_Editor *style_edit)
{
   Elm_Object_Item *glit_style, *glit_tag;
   Evas_Object *box, *btn;
   Eina_List *styles, *tags, *l_st, *l_tg;
   char *style, *tag;
   Evas_Object *edje_edit_obj = NULL;


   if (!_itc_style)
     {
        _itc_style = elm_genlist_item_class_new();
        _itc_style->item_style = "eflete/level1";
        _itc_style->func.text_get = _item_style_label_get;
        _itc_style->func.content_get = NULL;
        _itc_style->func.state_get = NULL;
        _itc_style->func.del = NULL;
     }
   if (!_itc_tags)
     {
        _itc_tags= elm_genlist_item_class_new();
        _itc_tags->item_style = "eflete/level3";
        _itc_tags->func.text_get = _item_tags_label_get;
        _itc_tags->func.content_get = NULL;
        _itc_tags->func.state_get = NULL;
        _itc_tags->func.del = NULL;
     }

   BOX_ADD(style_edit->mwin, box, false, false);

   style_edit->glist = elm_genlist_add(box);
   elm_object_style_set(style_edit->glist, DEFAULT_STYLE);
   elm_box_pack_end(box, style_edit->glist);
   evas_object_show(style_edit->glist);

   evas_object_size_hint_align_set(style_edit->glist, EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(style_edit->glist, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);
   GET_OBJ(style_edit->pr, edje_edit_obj);

   styles = edje_edit_styles_list_get(edje_edit_obj);

   EINA_LIST_FOREACH(styles, l_st, style)
     {
        glit_style = elm_genlist_item_append(style_edit->glist, _itc_style,
                                             style, NULL, ELM_GENLIST_ITEM_NONE,
                                             _on_glit_selected, style_edit);
        elm_object_item_data_set(glit_style, style);

        tags = edje_edit_style_tags_list_get(edje_edit_obj, style);
        EINA_LIST_FOREACH(tags, l_tg, tag)
          {
             glit_tag = elm_genlist_item_append(style_edit->glist, _itc_tags,
                                     tag, glit_style, ELM_GENLIST_ITEM_NONE,
                                    _on_glit_selected, style_edit);
             elm_object_item_data_set(glit_tag, tag);
          }
        eina_list_free(tags);
     }
   eina_list_free(styles);

   BUTTON_ADD(style_edit->mwin, btn, _("New style"));
   evas_object_size_hint_weight_set(btn, 0.0, 0.0);
   evas_object_smart_callback_add(btn, "clicked", _on_bt_style_add, style_edit);
   elm_box_pack_end(box, btn);

   BUTTON_ADD(style_edit->mwin, btn, _("New tag"));
   evas_object_size_hint_weight_set(btn, 0.0, 0.0);
   evas_object_smart_callback_add(btn, "clicked", _on_bt_tag_add, style_edit);
   elm_box_pack_end(box, btn);

   BUTTON_ADD(style_edit->mwin, btn, _("Delete"));
   evas_object_size_hint_weight_set(btn, 0.0, 0.0);
   evas_object_smart_callback_add(btn, "clicked", _on_bt_del, style_edit);
   elm_box_pack_end(box, btn);

   return box;
}

static void
_change_bg_cb(void *data,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;
   int state = elm_radio_state_value_get(obj);
   Evas_Object *bg = NULL;
   Evas *canvas = evas_object_evas_get(obj);
   switch (state)
     {
      case 0:
        {
           GET_IMAGE(bg, canvas, "bg_demo");
           evas_object_show(bg);
        }
      break;
      case 1:
        {
           bg = evas_object_rectangle_add(canvas);
           evas_object_color_set(bg, 0, 0, 0, 255);
           evas_object_show(bg);
        }
      break;
      case 2:
        {
           bg = evas_object_rectangle_add(canvas);
           evas_object_color_set(bg, 255, 255, 255, 255);
           evas_object_show(bg);
        }
      break;
      default:
      break;
     }
   elm_object_part_content_set(style_edit->entry_prev, "background", bg);
}

Evas_Object*
_form_right_side(Style_Editor *style_edit)
{
   Evas_Object *layout, *btn;
   Evas_Object *box_bg = NULL;
   Evas_Object *image_bg = NULL;
   Evas_Object *radio_group = NULL;
   Evas_Object *radio = NULL;

   layout = elm_layout_add(style_edit->mwin);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, EFLETE_EDJ, "ui/style_viewer_window/property");
   evas_object_show(layout);

   EWE_ENTRY_ADD(style_edit->mwin, style_edit->entry_tag, true, DEFAULT_STYLE);
   elm_object_part_content_set(layout, "swallow/tag_entry", style_edit->entry_tag);

   EWE_ENTRY_ADD(style_edit->mwin, style_edit->entry_prop, true, DEFAULT_STYLE);
   elm_object_part_content_set(layout, "swallow/prop_entry", style_edit->entry_prop);

   BUTTON_ADD(style_edit->mwin, btn, _("Close viewer"));
   evas_object_smart_callback_add(btn, "clicked", _on_viewer_exit, style_edit);
   elm_object_part_content_set(layout, "swallow/button_close", btn);
   evas_object_show(btn);

   BOX_ADD(style_edit->mwin, box_bg, true, false);
   elm_box_padding_set(box_bg, 10, 0);

#define _RADIO_ADD(RADIO, VALUE, IMAGE) \
   RADIO = elm_radio_add(style_edit->mwin); \
   elm_object_style_set(RADIO, "eflete/style_editor"); \
   elm_radio_state_value_set(RADIO, VALUE); \
   evas_object_show(RADIO); \
   IMAGE_ADD(box_bg, image_bg, IMAGE); \
   elm_object_part_content_set(RADIO, "bg", image_bg); \
   evas_object_smart_callback_add(RADIO, "changed", _change_bg_cb, style_edit); \
   elm_box_pack_end(box_bg, RADIO);

   _RADIO_ADD(radio_group, 0, "styles-preview-bg-transparent");
   _RADIO_ADD(radio, 1, "styles-preview-bg-black");
   elm_radio_group_add(radio, radio_group);
   _RADIO_ADD(radio, 2, "styles-preview-bg-white");
   elm_radio_group_add(radio, radio_group);
#undef _RADIO_ADD
   elm_object_part_content_set(layout, "menu_container", box_bg);
   return layout;
}

static void
_on_style_editor_close(void *data,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;

   free(style_edit);
}

static void
_on_mwin_del(void * data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_menu_locked_set(ap->menu_hash, false);
}

Evas_Object *
style_editor_window_add(Project *project)
{
   Evas_Object *panes, *panes_h;
   Evas_Object *layout_left, *layout_right;
   Evas_Object *bg = NULL;
   Evas *canvas = NULL;
   Style_Editor *style_edit = NULL;
   Evas_Textblock_Style *ts = NULL;
   static const char *style_buf = FONT_DEFAULT"'";

   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_data_get();

   if (!project)
     {
        ERR("Failed create style editor for non opened project");
        return NULL;
     }

   style_edit = (Style_Editor *)mem_calloc(1, sizeof(Style_Editor));

   style_edit->pr = project;
   style_edit->mwin = mw_add(_on_viewer_exit, style_edit);
   mw_title_set(style_edit->mwin, _("Textblock style editor"));
   evas_object_event_callback_add(style_edit->mwin, EVAS_CALLBACK_FREE,
                                        _on_style_editor_close, style_edit);

   panes = elm_panes_add(style_edit->mwin);
   elm_object_style_set(panes, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes, 0.2);
   elm_win_inwin_content_set(style_edit->mwin, panes);
   evas_object_show(panes);

   layout_left = _form_left_side(style_edit);
   elm_object_part_content_set(panes, "left", layout_left);
   evas_object_show(layout_left);

   panes_h = elm_panes_add(style_edit->mwin);
   elm_object_style_set(panes_h, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_h, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_h, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_horizontal_set(panes_h, true);
   elm_object_part_content_set(panes, "right", panes_h);
   evas_object_show(panes_h);

   style_edit->entry_prev = elm_layout_add(style_edit->mwin);
   evas_object_size_hint_weight_set(style_edit->entry_prev, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);
   elm_layout_file_set(style_edit->entry_prev, EFLETE_EDJ,
                       "ui/style_viewer_window/preview");
   evas_object_show(style_edit->entry_prev);
   elm_object_part_content_set(panes_h, "left", style_edit->entry_prev);
   elm_object_signal_emit(style_edit->entry_prev, "entry,hide", "eflete");

   canvas = evas_object_evas_get(style_edit->mwin);
   GET_IMAGE(bg, canvas, "bg_demo");
   elm_object_part_content_set(style_edit->entry_prev, "background", bg);
   evas_object_show(bg);

   style_edit->entry_style = evas_object_textblock_add(canvas);
   elm_object_part_content_set(style_edit->entry_prev, "entry", style_edit->entry_style);
   evas_object_textblock_valign_set(style_edit->entry_style, 0.5);
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, style_buf);
   evas_object_textblock_style_set(style_edit->entry_style, ts);
   evas_object_textblock_text_markup_set(style_edit->entry_style,
                       _("The quick brown fox jumps over the lazy dog"));
   evas_object_show(style_edit->entry_style);

   layout_right = _form_right_side(style_edit);
   elm_object_part_content_set(panes_h, "right", layout_right);
   evas_object_show(layout_right);

   ui_menu_locked_set(ap->menu_hash, true);
   evas_object_event_callback_add(style_edit->mwin, EVAS_CALLBACK_DEL, _on_mwin_del, ap);

   evas_object_show(style_edit->mwin);
   evas_textblock_style_free(ts);
   return style_edit->mwin;
}

#undef FONT_DEFAULT
#undef POPUP
