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
#include "alloc.h"
#define FONT_DEFAULT "DEFAULT='align=middle font=Sans font_size=24 color=#000000 "
#define ITEM1 "item1"
#define ITEM2 "item2"

typedef struct _Style_Tag_Entries Style_Tag_Entries;
typedef struct _Style_entries Style_Entries;
typedef struct _Style_Editor Style_Editor;

struct _Style_Editor
{
   Project *pr;
   Evas_Object *mwin;
   Evas_Object *glist;
   Evas_Object *textblock_style;
   Evas_Object *entry_prev;
   Evas_Object *tabs;
   struct {
        const char *st_name;
        const char *st_tag;
        Eina_Stringshare *stvalue;
   } current_style;
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
#define CURRENT style_edit->current_style

static Elm_Genlist_Item_Class *_itc_style = NULL;
static Elm_Genlist_Item_Class *_itc_tags = NULL;
static const char *font_styles[] = { N_("Normal"),
                                     N_("Italic"),
                                     N_("Oblique"),
                                     NULL};

static const char *font_width_list[] = { N_("Normal"),
                                         N_("Ultracondensed"),
                                         N_("Extracondensed"),
                                         N_("Condensed"),
                                         N_("Extracondensed"),
                                         N_("Semicondensed"),
                                         N_("Semiexpanded"),
                                         N_("Expanded"),
                                         N_("Ultraexpanded"),
                                         NULL};

static const char *font_weight_list[] = { N_("Normal"),
                                          N_("Thin"),
                                          N_("Ultralight"),
                                          N_("Light"),
                                          N_("Book"),
                                          N_("Medium"),
                                          N_("Semibold"),
                                          N_("Bold"),
                                          N_("Ultrabold"),
                                          N_("Black"),
                                          N_("Extrablack"),
                                          NULL};

static const char *style_table[][2] = {{"font", NULL},
                                       {"font_fallback", NULL},
                                       {"font_size", NULL},
                                       {"font_source", NULL},
                                       {"font_weight", NULL},
                                       {"font_style", NULL},
                                       {"font_width", NULL},
                                       {"lang", NULL},
                                       {"color", NULL},
                                       {"underline_color", NULL},
                                       {"underline2_color", NULL},
                                       {"underline_dash_color", NULL},
                                       {"outline_color", NULL},
                                       {"shadow_color", NULL},
                                       {"glow_color", NULL},
                                       {"glow2_color", NULL},
                                       {"backing_color", NULL},
                                       {"strikethrough_color", NULL},
                                       {"align", NULL},
                                       {"valign", NULL},
                                       {"wrap", NULL},
                                       {"left_margin", NULL},
                                       {"right_margin", NULL},
                                       {"underline", NULL},
                                       {"strikethrough", NULL},
                                       {"backing", NULL},
                                       {"style", NULL},
                                       {"tabstops", NULL},
                                       {"linesize", NULL},
                                       {"linerelsize", NULL},
                                       {"linegap", NULL},
                                       {"linerelgap", NULL},
                                       {"item", NULL},
                                       {"linefill", NULL},
                                       {"ellipsis", NULL},
                                       {"password", NULL},
                                       {"underline_dash_width", NULL},
                                       {"underline_dash_gap", NULL},
                                       {NULL, NULL}};

static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = BANNED_SYMBOLS
};

static Elm_Entry_Filter_Accept_Set accept_value = {
   .accepted = NULL,
   .rejected = EDITORS_BANNED_SYMBOLS
};

static void
_text_tab_update(Style_Editor *style_edit, Evas_Object *tabs, Ewe_Tabs_Item *it, const char *value);

static void
_on_popup_bt_cancel(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *ei __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;

   evas_object_del(POPUP.dialog);
   POPUP.dialog = NULL;
}

static void
_style_edit_update(Style_Editor *style_edit)
{
   Evas_Textblock_Style *ts = NULL;
   Eina_Strbuf *style = eina_strbuf_new();

   eina_strbuf_append(style, FONT_DEFAULT);
   eina_strbuf_append(style, CURRENT.stvalue);
   eina_strbuf_append(style, "'");
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, eina_strbuf_string_get(style));
   evas_object_textblock_style_set(style_edit->textblock_style, ts);
   evas_object_size_hint_max_set(style_edit->textblock_style, EVAS_HINT_FILL,
                                 EVAS_HINT_FILL);
   eina_strbuf_free(style);
   evas_textblock_style_free(ts);
}

static void
_on_glit_selected(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   Eina_List *tags = NULL;
   Eina_List *l = NULL;
   Evas_Object *edje_edit_obj = NULL;

   const char *style_name = NULL;
   const char *tag, *value = NULL;
   Evas_Textblock_Style *ts = NULL;
   Ewe_Tabs_Item *it;

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
        eina_list_free(tags);
     }
   else
     {
        style_name = elm_object_item_data_get(glit_parent);
        tag = (char *)elm_object_item_data_get(glit);
        value = edje_edit_style_tag_value_get(edje_edit_obj, style_name, tag);
        eina_strbuf_append(style, value);
     }
   CURRENT.st_name = style_name;
   CURRENT.st_tag = tag;
   CURRENT.stvalue = eina_stringshare_add(value);
   it = ewe_tabs_active_item_get(style_edit->tabs);
   if (!strcmp(ewe_tabs_item_title_get(style_edit->tabs, it), _("Text")))
     _text_tab_update(style_edit, style_edit->tabs, it, eina_strbuf_string_get(style));

   elm_object_signal_emit(style_edit->entry_prev, "entry,show", "eflete");
   eina_strbuf_append(style, "'");
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, eina_strbuf_string_get(style));
   evas_object_textblock_style_set(style_edit->textblock_style, ts);
   evas_object_size_hint_max_set(style_edit->textblock_style, EVAS_HINT_FILL,
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

static void
_tag_parse(Style_Editor *style_edit, const char *value, const char *text)
{
   Evas_Object *edje_edit_obj = NULL;
   Eina_Strbuf *tag = eina_strbuf_new();
   char *token;
   int i = 0, k = 0, exist = 0;

   eina_strbuf_append(tag, CURRENT.stvalue);
   GET_OBJ(style_edit->pr, edje_edit_obj);
   token= strtok(eina_strbuf_string_steal(tag), " =+");
   while (token)
     {
        if ((i + 1) % 2 != 0)
          {
             for (k = 0; style_table[k][0] != NULL; k++)
               {
                  if(!strcmp(style_table[k][0], token)) exist = k;
               }
          }
        else style_table[exist][1] = eina_stringshare_add(token);
        token= strtok(0, " =+");
        i++;
     }
   for (k = 0; style_table[k][0] != NULL; k++)
     {
        if(!strcmp(style_table[k][0], text))
          {
             eina_stringshare_del(style_table[k][1]);
             style_table[k][1] = eina_stringshare_add(value);
          }
     }
   eina_strbuf_append(tag, "+ ");
   for (k = 0; style_table[k][0] != NULL; k++)
      {
         if (style_table[k][1] != NULL)
           {
              eina_strbuf_append(tag, style_table[k][0]);
              eina_strbuf_append(tag, "=");
              eina_strbuf_append(tag, style_table[k][1]);
              eina_strbuf_append(tag, " ");
              eina_stringshare_del(style_table[k][1]);
              style_table[k][1] = NULL;
           }
      }
   edje_edit_style_tag_value_set(edje_edit_obj, CURRENT.st_name,
                                 CURRENT.st_tag, eina_strbuf_string_get(tag));
   eina_stringshare_del(CURRENT.stvalue);
   CURRENT.stvalue = eina_stringshare_add(eina_strbuf_string_get(tag));
   eina_strbuf_free(tag);
}

#define ITEM_COLOR_CALLBACK(VALUE) \
static void \
_on_##VALUE##_change(void *data, \
                     Evas_Object *obj, \
                     void *ei __UNUSED__) \
{ \
   int r, g, b, a; \
   const char *value; \
   Style_Editor *style_edit = (Style_Editor *)data; \
   Evas_Object *color, *rect; \
   color = evas_object_data_get(obj, "color"); \
   colorselector_color_get(obj, &r, &g, &b, &a); \
   value = eina_stringshare_printf("#%02x%02x%02x%02x", r, g, b, a); \
   _tag_parse(style_edit, value, "color"); \
   rect = elm_object_part_content_get(color, "elm.swallow.content"); \
   evas_object_color_set(rect, r*a/255, g*a/255, b*a/255, a); \
   _style_edit_update(style_edit); \
   eina_stringshare_del(value); \
} \
static void \
_on_##VALUE##_dismissed(void *data, \
                        Evas_Object *obj, \
                        void *event_info __UNUSED__) \
{ \
   Style_Editor *style_edit = (Style_Editor *)data; \
   evas_object_smart_callback_del_full(obj, "color,changed", \
                                      _on_##VALUE##_change, style_edit); \
   evas_object_smart_callback_del_full(obj, "palette,item,selected", \
                                       _on_##VALUE##_change, style_edit); \
   evas_object_data_del(obj, "color"); \
   evas_object_hide(obj); \
} \
static void \
_on_##VALUE##_clicked(void *data, \
                      Evas *e __UNUSED__, \
                      Evas_Object *obj, \
                      void *event_info __UNUSED__) \
{ \
   int x, y; \
   int r, g, b, a; \
   Style_Editor *style_edit = (Style_Editor *)data; \
   Evas_Object *colorsel, *rect; \
   colorsel = colorselector_get(); \
   evas_object_data_set(colorsel, "color", obj); \
   rect = elm_object_part_content_get(obj, "elm.swallow.content"); \
   evas_object_color_get(rect, &r, &g, &b, &a); \
   colorselector_color_set(colorsel, r, g, b, a); \
   evas_object_smart_callback_add(colorsel, "color,changed", \
                                  _on_##VALUE##_change, style_edit); \
   evas_object_smart_callback_add(colorsel, "palette,item,selected", \
                                  _on_##VALUE##_change, style_edit); \
   evas_object_smart_callback_add(colorsel, "dismissed", \
                                  _on_##VALUE##_dismissed, style_edit); \
   evas_pointer_canvas_xy_get(evas_object_evas_get(obj), &x, &y); \
   evas_object_move(colorsel, x, y); \
   evas_object_show(colorsel); \
}

#define COMBOBOX_VALUE \
Ewe_Combobox_Item *item = ei; \
const char *value; \
value = eina_stringshare_add(item->title);

#define SEGMENT_VALUE \
Elm_Object_Item *item = ei; \
const char *value; \
value = eina_stringshare_add(elm_object_item_part_text_get(item, "elm.text"));

#define SPINNER_VALUE \
const char *value = NULL; \
if (!ei) \
  { \
     value = eina_stringshare_printf("%f", elm_spinner_value_get(obj)); \
  }

#define CHANGE_CALLBACK(VALUE, TEXT, WIDGET) \
static void \
_on_##VALUE##_change(void *data, \
                     Evas_Object *obj EINA_UNUSED, \
                     void *ei) \
{ \
   Style_Editor *style_edit = (Style_Editor *)data; \
   WIDGET##_VALUE \
   _tag_parse(style_edit, value, TEXT); \
   _style_edit_update(style_edit); \
   eina_stringshare_del(value); \
}

#define ITEM_CONTEINER_1LABEL_ADD(PARENT, ITEM, TEXT1, LAYOUT) \
   ITEM = elm_layout_add(PARENT); \
   evas_object_size_hint_weight_set(ITEM, 1.0, 0.0); \
   evas_object_size_hint_align_set(ITEM, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(ITEM, EFLETE_EDJ, "eflete/"LAYOUT"/container/1label"); \
   elm_object_part_text_set(ITEM, "eflete.text.start", TEXT1); \
   evas_object_show(ITEM);

#define ITEM_CONTEINER_2LABEL_ADD(PARENT, ITEM, TEXT1, TEXT2) \
   ITEM = elm_layout_add(PARENT); \
   evas_object_size_hint_weight_set(ITEM, 1.0, 0.0); \
   evas_object_size_hint_align_set(ITEM, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(ITEM, EFLETE_EDJ, "eflete/prop/container/2label"); \
   elm_object_part_text_set(ITEM, "eflete.text.start", TEXT1); \
   elm_object_part_text_set(ITEM, "eflete.text.end", TEXT2); \
   evas_object_show(ITEM);

//TODO: ITEM2_TEXT_ADD macros need refactoring
#define ITEM2_TEXT_ADD(TEXT, VALUE1, VALUE2, TEXT2) \
static Evas_Object * \
_style_item_##VALUE1##_##VALUE2##_add(Evas_Object *frame_box, Style_Editor *style_edit) \
{ \
   Evas_Object *item, *box, *layout1, *layout2, *combobox, *widget; \
   int i = 0; \
   ITEM_ADD(frame_box, item, TEXT":", "ui/style_viewer_window/item/default") \
   BOX_ADD(item, box, true, true) \
   elm_box_padding_set(box, 30.0, 0.0); \
   ITEM_CONTEINER_1LABEL_ADD(box, layout1, "", "prop"); \
   EWE_COMBOBOX_ADD(layout1, combobox); \
   elm_object_part_content_set(layout1, "eflete.content", combobox); \
   evas_object_smart_callback_add(combobox, "selected", _on_##VALUE1##_change, style_edit); \
   if (!strcmp(TEXT2, "spinner")) \
     { \
        ITEM_CONTEINER_2LABEL_ADD(box, layout2, "size", "pt"); \
        SPINNER_ADD(layout2, widget, 0, 9999, 1, true, DEFAULT_STYLE); \
        elm_object_part_content_set(layout2, "eflete.content", widget); \
        evas_object_smart_callback_add(widget, "changed", _on_font_size_change, style_edit); \
     } \
   if (!strcmp(TEXT2, "sc")) \
     { \
        ITEM_CONTEINER_2LABEL_ADD(box, layout2, "", ""); \
        SEGMENT_CONTROL_ADD(layout2, widget); \
        elm_object_part_content_set(layout2, "eflete.content", widget); \
        for (i = 0; font_styles[i] != NULL; i++) \
          elm_segment_control_item_add(widget, NULL, font_styles[i]); \
        evas_object_smart_callback_add(widget, "changed", _on_font_style_change, style_edit); \
     } \
   evas_object_data_set(item, ITEM1, combobox); \
   evas_object_data_set(item, ITEM2, widget); \
   elm_box_pack_end(box, layout1); \
   elm_box_pack_end(box, layout2); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_COLOR_ADD(TEXT, VALUE) \
static Evas_Object * \
_style_item_##VALUE##_add(Evas_Object *frame_box, Style_Editor *style_edit) \
{ \
   Evas_Object *item, *color, *rect; \
   ITEM_ADD(frame_box, item, TEXT, "ui/style_viewer_window/item/default") \
   color = elm_layout_add(item); \
   elm_layout_file_set(color, EFLETE_EDJ, "eflete/style/color"); \
   evas_object_size_hint_weight_set(color, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(color, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   rect = evas_object_rectangle_add(evas_object_evas_get(frame_box)); \
   evas_object_event_callback_add(color, EVAS_CALLBACK_MOUSE_DOWN, \
                                  _on_##VALUE##_clicked, style_edit); \
   evas_object_color_set(rect, 0, 0, 0, 255); \
   evas_object_data_set(item, ITEM1, rect); \
   elm_object_part_content_set(color, "elm.swallow.content", rect); \
   elm_object_part_content_set(item, "elm.swallow.content", color); \
   return item; \
}

/*#define _EMBED_ADD(VALUE) \
ITEM_CONTEINER_2LABEL_ADD(item, layout, "", "Embed font"); \
CHECK_ADD(layout, widget, DEFAULT_STYLE); \*/

#define WEIGHT_ADD(VALUE) \
ITEM_CONTEINER_1LABEL_ADD(item, layout, "", "style"); \
EWE_COMBOBOX_ADD(layout, widget); \
evas_object_smart_callback_add(widget, "selected", _on_##VALUE##_change, style_edit);

#define ITEM1_TEXT_ADD(TEXT, VALUE, WIDGET) \
static Evas_Object * \
_style_item_##VALUE##_add(Evas_Object *frame_box, Style_Editor *style_edit) \
{ \
   Evas_Object *item, *layout, *widget; \
   ITEM_ADD(frame_box, item, TEXT, "ui/style_viewer_window/item/default") \
\
   WIDGET##_ADD(VALUE) \
\
   elm_object_part_content_set(layout, "eflete.content", widget); \
   evas_object_data_set(item, ITEM1, widget); \
   elm_object_part_content_set(item, "elm.swallow.content", layout); \
   return item; \
}

static const char*
_tag_value_get(const char* text_style, char* a_tag)
{
   char *tag_list_copy = mem_malloc(strlen(text_style) + 1);
   const char *result = NULL;
   char *token;

   strcpy(tag_list_copy, text_style);
   token = strtok(tag_list_copy, " ");
   while (token)
     {
        char* equals_sign = strchr(token, '=');
        if (equals_sign)
          {
             *equals_sign = 0;
             if (!strcmp(token, a_tag))
               {
                  equals_sign++;
                  result = eina_stringshare_add(equals_sign);
               }
          }
        token = strtok(0, " ");
     }
   free(tag_list_copy);

   return result;
}

static void
_hex_to_rgb(const char *hex, int *r, int *g, int *b, int *a)
{
   unsigned long val;
   char *end;
   int length = (int)strlen(hex);
   switch (length)
     {
      case 7:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xff0000) >> 16;
           *g = (val & 0xff00) >> 8;
           *b = val & 0xff;
           *a = 255;
           break;
        }
      case 9:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xff000000) >> 24;
           *g = (val & 0xff0000) >> 16;
           *b = (val & 0xff00) >> 8;
           *a = val & 0xff;
           break;
        }
      case 5:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xf000) >> 12;
           *g = (val & 0xf00) >> 8;
           *b = (val & 0xf0) >> 4;
           *a = val & 0xf;
           *r = (*r << 4) | *r;
           *g = (*g << 4) | *g;
           *b = (*b << 4) | *b;
           *a = (*a << 4) | *a;
           break;
        }
      case 4:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xf00) >> 8;
           *g = (val & 0xf0) >> 4;
           *b = val & 0xf;
           *a = 255;
           *r = (*r << 4) | *r;
           *g = (*g << 4) | *g;
           *b = (*b << 4) | *b;
           break;
        }
      default:
        {
           *r = 0;
           *g = 0;
           *b = 0;
           *a = 255;
        }
     }
}

CHANGE_CALLBACK(fonts_list, "font", COMBOBOX)
CHANGE_CALLBACK(font, "font_width", COMBOBOX)
CHANGE_CALLBACK(weight_list, "font_weight", COMBOBOX)
CHANGE_CALLBACK(font_style, "font_style", SEGMENT)
CHANGE_CALLBACK(font_size, "font_size", SPINNER)
ITEM_COLOR_CALLBACK(text_color)

ITEM2_TEXT_ADD(N_("Font name"), fonts_list, font_size, "spinner")
ITEM2_TEXT_ADD(N_("Font style"), font, style, "sc")
ITEM1_TEXT_ADD(NULL, weight_list, WEIGHT)
//ITEM1_TEXT_ADD(NULL, font_check, EMBED)
ITEM_COLOR_ADD(N_("Color"), text_color)

static void
_text_tab_update(Style_Editor *style_edit, Evas_Object *tabs, Ewe_Tabs_Item *it, const char *value)
{
   Evas_Object *edje_edit_obj = NULL;
   Evas_Object *frame, *frame_box;
   Evas_Object *fonts_list, *combobox, *spinner;
   Evas_Object *font_style, *combobox2, *sc;
   Evas_Object *font_weight, *combobox3;
   Evas_Object *text_color, *rect;
   //Evas_Object *font_check;
   Elm_Object_Item *sc_item;
   int r, g, b, a;
   unsigned int i = 0;

   GET_OBJ(style_edit->pr, edje_edit_obj);

   FRAME_ADD(style_edit->mwin, frame, false, _("Text styles"))
   BOX_ADD(frame, frame_box, EINA_FALSE, EINA_FALSE)
   elm_box_align_set(frame_box, 0.5, 0.0);
   elm_object_content_set(frame, frame_box);
   ewe_tabs_item_content_set(tabs, it, frame);

   fonts_list = _style_item_fonts_list_font_size_add(frame_box, style_edit);
   font_style = _style_item_font_style_add(frame_box, style_edit);
   font_weight = _style_item_weight_list_add(frame_box, style_edit);
   text_color = _style_item_text_color_add(frame_box, style_edit);
   //font_check = _style_item_font_check_add(frame_box, style_edit);

   combobox = evas_object_data_get(fonts_list, ITEM1);
   spinner = evas_object_data_get(fonts_list, ITEM2);
   combobox2 = evas_object_data_get(font_style, ITEM1);
   sc = evas_object_data_get(font_style, ITEM2);
   combobox3 = evas_object_data_get(font_weight, ITEM1);
   rect = evas_object_data_get(text_color, ITEM1);
   if (value)
     {
        const char* font = _tag_value_get(value, "font");
        const char* spin_val = _tag_value_get(value, "font_size");
        const char* weight = _tag_value_get(value, "font_weight");
        if (!weight) weight = "Normal";
        const char* width = _tag_value_get(value, "font_width");
        if (!width) width = "Normal";
        const char* style = _tag_value_get(value, "font_style");
        if (!style) style = "Normal";
        const char* color = _tag_value_get(value, "color");

        ewe_combobox_text_set(combobox, font);
        elm_object_disabled_set(combobox, true);
        elm_spinner_value_set(spinner, atof(spin_val));
        ewe_combobox_text_set(combobox2, width);
        for (i = 0; font_width_list[i] != NULL; i++)
          ewe_combobox_item_add(combobox2, font_width_list[i]);
        ewe_combobox_text_set(combobox3, weight);
        for (i = 0; font_weight_list[i] != NULL; i++)
          ewe_combobox_item_add(combobox3, font_weight_list[i]);
        for (i = 0; font_styles[i] != NULL; i++)
          {
             if (!strcmp(elm_segment_control_item_label_get(sc, i), style))
               {
                  sc_item = elm_segment_control_item_get(sc, i);
                  elm_segment_control_item_selected_set(sc_item, true);
               }
          }
        _hex_to_rgb(color, &r, &g, &b, &a);
        evas_object_color_set(rect, r*a/255, g*a/255, b*a/255, a);

        eina_stringshare_del(font);
        eina_stringshare_del(spin_val);
        eina_stringshare_del(color);
     }

   elm_box_pack_end(frame_box, fonts_list);
   elm_box_pack_end(frame_box, font_style);
   elm_box_pack_end(frame_box, font_weight);
   elm_box_pack_end(frame_box, text_color);
   //elm_box_pack_end(frame_box, font_check);
}
#undef ITEM_CONTEINER_1LABEL_ADD
#undef ITEM_CONTEINER_2LABEL_ADD
#undef _COMBOBOX_VALUE
#undef _SEGMENT_VALUE
#undef _SPINNER_VALUE
#undef CHANGE_CALLBACK
#undef ITEM2_TEXT_ADD
#undef ITEM_COLOR_ADD
#undef ITEM1_TEXT_ADD
#undef WEIGHT_ADD
#undef EMBED_ADD

Evas_Object*
_form_right_side(Style_Editor *style_edit)
{
   Evas_Object *layout;
   Evas_Object *box_bg = NULL;
   Evas_Object *image_bg = NULL;
   Evas_Object *radio_group = NULL;
   Evas_Object *radio = NULL;

   layout = elm_layout_add(style_edit->mwin);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, EFLETE_EDJ, "ui/style_viewer_window/property");
   evas_object_show(layout);

   style_edit->tabs = ewe_tabs_add(style_edit->mwin);
   elm_object_part_content_set(layout, "swallow/tabs_entry", style_edit->tabs);
   ewe_tabs_item_append(style_edit->tabs, NULL, _("Text"), NULL);
   ewe_tabs_item_append(style_edit->tabs, NULL, _("Format"), NULL);
   ewe_tabs_orient_horizontal_set(style_edit->tabs, EINA_FALSE);
   evas_object_show(style_edit->tabs);

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

   eina_stringshare_del(CURRENT.stvalue);
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
   ap->modal_editor = false;
}

Evas_Object *
style_editor_window_add(Project *project)
{
   Evas_Object *panes, *panes_h;
   Evas_Object *window_layout, *button_box, *btn;
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
        ERR("Project isn't opened");
        return NULL;
     }

   style_edit = (Style_Editor *)mem_calloc(1, sizeof(Style_Editor));

   style_edit->pr = project;
   style_edit->mwin = mw_add(_on_viewer_exit, style_edit);
   mw_title_set(style_edit->mwin, _("Textblock style editor"));
   evas_object_event_callback_add(style_edit->mwin, EVAS_CALLBACK_FREE,
                                        _on_style_editor_close, style_edit);
   window_layout = elm_layout_add(style_edit->mwin);
   elm_layout_file_set(window_layout, EFLETE_EDJ, "eflete/editor/default");
   elm_win_inwin_content_set(style_edit->mwin, window_layout);

   panes = elm_panes_add(style_edit->mwin);
   elm_object_style_set(panes, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes, 0.2);
   elm_object_part_content_set(window_layout, "eflete.swallow.content", panes);
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

   style_edit->textblock_style = evas_object_textblock_add(canvas);
   elm_object_part_content_set(style_edit->entry_prev, "entry", style_edit->textblock_style);
   evas_object_textblock_valign_set(style_edit->textblock_style, 0.5);
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, style_buf);
   evas_object_textblock_style_set(style_edit->textblock_style, ts);
   evas_object_textblock_text_markup_set(style_edit->textblock_style,
                       _("The quick brown fox jumps over the lazy dog"));
   evas_object_show(style_edit->textblock_style);

   layout_right = _form_right_side(style_edit);
   elm_object_part_content_set(panes_h, "right", layout_right);
   evas_object_show(layout_right);

   BOX_ADD(window_layout, button_box, true, false)
   elm_box_align_set(button_box, 1.0, 0.5);

   BUTTON_ADD(style_edit->mwin, btn, _("Close viewer"));
   evas_object_smart_callback_add(btn, "clicked", _on_viewer_exit, style_edit);
   evas_object_size_hint_weight_set(btn, 0.0, 0.0);
   evas_object_size_hint_min_set(btn, 100, 30);
   evas_object_show(btn);
   elm_box_pack_end(button_box, btn);
   elm_object_part_content_set(window_layout, "eflete.swallow.button_box", button_box);

   ui_menu_locked_set(ap->menu_hash, true);
   evas_object_event_callback_add(style_edit->mwin, EVAS_CALLBACK_DEL, _on_mwin_del, ap);

   evas_object_show(style_edit->mwin);
   evas_textblock_style_free(ts);
   ap->modal_editor = true;
   return style_edit->mwin;
}

#undef FONT_DEFAULT
#undef POPUP
#undef CURRENT
