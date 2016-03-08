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
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "main_window.h"
#include "project_manager.h"
#include "validator.h"

TODO("Rename this file to textblock_style_manager")

#define FONT_DEFAULT "DEFAULT='align=middle font=Sans font_size=24 color=#000000 "
#define DIRECTION_NUM 39
#define DEFAULT_DIRECTION 2
#define WHITE_COLOR "#FFF"

#define TEST_TEXT \
"<\t>By the time London returned home, his stepfather had died." \
"It then became his responsibility to provide for his family all by himself." \
"Desperate for a job, he spent entire days walking around the Oakland business district." \
"Occasionally he found a temporary job. He pawned his watch, bicycle," \
"Mackintosh, and started writing again.<br/>" \
"<\t>He carefully studied different stories published in magazines," \
"trying to learn the secret of their popularity." \
"He referred to the works of famous writers." \
"By reading many of them he learned about the craft of constructing a plot," \
"about creating a particular style. Rudyard Kipling captured his attention." \
"He reached the conclusion that to write well he needed to write concisely" \
"and he needed to tell about strong human emotions and vivid characters.<br/>" \
"(c) Jack London. THE NORTHERN TALES. (NEW BATTLES. CHAPTER 5)"

typedef struct _Style_Tag_Entries Style_Tag_Entries;
typedef struct _Style_entries Style_Entries;
typedef struct _Style_Editor Style_Editor;
typedef struct _Search_Data Search_Data;

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Style_Editor
{
   Evas_Object *glist;
   Evas_Object *textblock_style;
   Evas_Object *entry_prev;
   Evas_Object *button_del;
   Evas_Object *menu;
   Elm_Object_Item *menu_tag;
   Search_Data style_search_data;
   struct {
      const char *st_name;
      const char *st_tag;
      Eina_Stringshare *stvalue;
   } current_style;
   struct {
      Evas_Object *name;
      Resource_Name_Validator *name_validator;
      Evas_Object *btn_add;
      Evas_Object *dialog;
      Eina_List *buf_resources;
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
static Eina_Bool is_expanded_glitem = EINA_FALSE;

static void
_on_popup_bt_cancel(void *data,
                    Evas_Object *obj,
                    void *ei __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;
   Resource *res;

   assert(style_edit != NULL);
   assert(POPUP.name_validator != NULL);

   if (obj && is_expanded_glitem)
     {
        elm_genlist_item_expanded_set(elm_genlist_selected_item_get(style_edit->glist), false);
        is_expanded_glitem = EINA_FALSE;
     }

   evas_object_del(POPUP.dialog);
   POPUP.dialog = NULL;

   resource_name_validator_free(POPUP.name_validator);
   POPUP.name_validator = NULL;

   EINA_LIST_FREE(POPUP.buf_resources, res)
     {
        eina_stringshare_del(res->name);
        free(res);
     }
   POPUP.buf_resources = NULL;
}

static void
_on_unselected_cb(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;
   assert(style_edit != NULL);

   elm_object_disabled_set(style_edit->button_del, true);
   evas_object_smart_callback_call(ap.win, SIGNAL_STYLE_SELECTED, NULL);
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

   Style_Editor *style_edit = (Style_Editor *)data;
   assert(style_edit != NULL);

   elm_object_disabled_set(style_edit->button_del, false);
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;

   Eina_Strbuf *style = eina_strbuf_new();
   eina_strbuf_append(style, FONT_DEFAULT);

   Elm_Object_Item *glit_parent = elm_genlist_item_parent_get(glit);

   Style_Data *current_style = (Style_Data *)mem_calloc(1, sizeof(Style_Data));

   edje_edit_obj = ap.project->global_object;

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
        if (!strcmp(tag, "DEFAULT"))
          elm_object_disabled_set(style_edit->button_del, true);
        value = edje_edit_style_tag_value_get(edje_edit_obj, style_name, tag);
        eina_strbuf_append(style, value);
     }

   current_style->st_name = style_name;
   current_style->st_tag = tag;
   current_style->stvalue = eina_stringshare_add(value);
   current_style->style = style;
   current_style->textblock_style = style_edit->textblock_style;
   if (!elm_genlist_item_parent_get(glit))
     {
        elm_object_signal_emit(style_edit->entry_prev, "entry,hide", "eflete");
        current_style->is_parent_item = EINA_TRUE;
     }
   else
     {
        elm_object_signal_emit(style_edit->entry_prev, "entry,show", "eflete");
        eina_strbuf_append(style, "'");
        ts = evas_textblock_style_new();
        evas_textblock_style_set(ts, eina_strbuf_string_get(style));
        evas_object_textblock_style_set(style_edit->textblock_style, ts);
        evas_object_size_hint_max_set(style_edit->textblock_style, EVAS_HINT_FILL,
                                      EVAS_HINT_FILL);
        evas_textblock_style_free(ts);
     }

   evas_object_smart_callback_call(ap.win, SIGNAL_STYLE_SELECTED, current_style);
}

static void
_on_st_add_bt_ok(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   Elm_Object_Item *glit_style = NULL;
   Evas_Object *edje_edit_obj = NULL;
   Style_Editor *style_edit = (Style_Editor *)data;
   Resource *res;

   assert(style_edit != NULL);

   const char *style_name = elm_entry_entry_get(POPUP.name);

   edje_edit_obj = ap.project->global_object;

   if (!edje_edit_style_add(edje_edit_obj, style_name))
     {
        WARN(_("Style name must be unique!"));
        return;
     }
   if (edje_edit_style_tag_add(edje_edit_obj, style_name, "DEFAULT"))
     {
        if (!edje_edit_style_tag_value_set(edje_edit_obj,
                                           style_name,
                                          "DEFAULT",
                                          "align=middle font=Sans font_size=24 color=#000000FF"))
          {
             WARN(_("Failed to add tag value. Tag will be deleted"));
             edje_edit_style_tag_del(edje_edit_obj, style_name, "DEFAULT");
             return;
          }
     }
   else
     {
        WARN(_("Failed to add tag."));
        return;
     }

   glit_style = elm_genlist_item_append(style_edit->glist, _itc_style,
                                        style_name, NULL, ELM_GENLIST_ITEM_TREE,
                                        _on_glit_selected, style_edit);
   elm_object_item_data_set(glit_style, (char *)style_name);

   elm_genlist_item_selected_set(glit_style, true);
   elm_genlist_item_bring_in(glit_style, ELM_GENLIST_ITEM_SCROLLTO_TOP);

   res = mem_calloc(1, sizeof(Resource));
   res->name = eina_stringshare_add(style_name);
   ap.project->styles = eina_list_sorted_insert(ap.project->styles,
                                                (Eina_Compare_Cb) resource_cmp,
                                                res);

   _on_popup_bt_cancel(style_edit, NULL, NULL);

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;
}

static void
_on_tag_add_bt_ok(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *ei __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;
   Elm_Object_Item *glit_tag = NULL;
   Evas_Object *edje_edit_obj = NULL;

   assert(style_edit != NULL);

   const char *style_name = elm_object_item_data_get(style_edit->tag);
   const char *tag_name = elm_entry_entry_get(POPUP.name);
   edje_edit_obj = ap.project->global_object;

   if (!edje_edit_style_tag_add(edje_edit_obj, style_name, tag_name))
     {
        WARN(_("Tag name must be unique!"));
        return;
     }
   else
     if (!edje_edit_style_tag_value_set(edje_edit_obj, style_name, tag_name, ""))
       {
          WARN(_("Failed to add tag value. Tag will be deleted"));
          edje_edit_style_tag_del(edje_edit_obj, style_name, tag_name);
          return;
       }
   glit_tag = elm_genlist_item_append(style_edit->glist, _itc_tags,
                                      tag_name, style_edit->tag,
                                      ELM_GENLIST_ITEM_NONE,
                                      _on_glit_selected, style_edit);
   elm_object_item_data_set(glit_tag,(char *)tag_name);
   elm_genlist_item_selected_set(glit_tag, true);
   elm_genlist_item_bring_in(glit_tag, ELM_GENLIST_ITEM_SCROLLTO_TOP);

   _on_popup_bt_cancel(style_edit, NULL, NULL);

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;
}

static void
_validate(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;

   if (ELM_REG_NOERROR != resource_name_validator_status_get(POPUP.name_validator))
     elm_object_disabled_set(POPUP.btn_add, true);
   else
     elm_object_disabled_set(POPUP.btn_add, false);
}

static void
_on_bt_style_add(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *box, *item, *button;

   Style_Editor *style_edit = (Style_Editor *)data;

   assert(style_edit != NULL);

   POPUP.dialog = elm_popup_add(ap.win);
   elm_popup_orient_set(POPUP.dialog, ELM_POPUP_ORIENT_CENTER);
   elm_object_part_text_set(POPUP.dialog, "title,text", _("Add textblock style"));

   BOX_ADD(POPUP.dialog, box, false, false);

   LAYOUT_PROP_ADD(box, _("Style name:"), "property", "1swallow")
   POPUP.name_validator = resource_name_validator_new(NAME_REGEX, NULL);
   resource_name_validator_list_set(POPUP.name_validator, &ap.project->styles, true);
   ENTRY_ADD(item, POPUP.name, true);
   eo_event_callback_add(POPUP.name, ELM_ENTRY_EVENT_VALIDATE, resource_name_validator_helper, POPUP.name_validator);
   evas_object_smart_callback_add(POPUP.name, "changed", _validate, style_edit);
   elm_object_part_text_set(POPUP.name, "guide", _("Type a new style name"));
   elm_object_part_content_set(item, "elm.swallow.content", POPUP.name);
   elm_box_pack_end(box, item);

   elm_object_content_set(POPUP.dialog, box);
   evas_object_show(box);

   BUTTON_ADD(POPUP.dialog, POPUP.btn_add, _("Ok"));
   evas_object_smart_callback_add(POPUP.btn_add, "clicked", _on_st_add_bt_ok,  style_edit);
   elm_object_part_content_set(POPUP.dialog, "button1", POPUP.btn_add);
   elm_object_disabled_set(POPUP.btn_add, true);

   BUTTON_ADD(POPUP.dialog, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_popup_bt_cancel, style_edit);
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

   assert(style_edit != NULL);
   assert(POPUP.name_validator == NULL);

   Elm_Object_Item *glit = elm_genlist_selected_item_get(style_edit->glist);
   Elm_Object_Item *glit_parent = elm_genlist_item_parent_get(glit);
   const char *style_name;
   Eina_Stringshare *buf;
   Eina_List *tags, *l_tg;
   char *tag;
   Resource *res;

   if (!glit)
     {
         WARN(_("Select a style!"));
         return;
     }

   if (!glit_parent)
     {
        if (!elm_genlist_item_expanded_get(glit))
          {
             elm_genlist_item_expanded_set(glit, true);
             is_expanded_glitem = EINA_TRUE;
          }

        style_name = elm_object_item_data_get(glit);
        style_edit->tag = glit;
     }
   else
     {
         style_name = elm_object_item_data_get(glit_parent);
         style_edit->tag = glit_parent;
     }

   tags = edje_edit_style_tags_list_get(ap.project->global_object, style_name);
   EINA_LIST_FOREACH(tags, l_tg, tag)
     {
        res = mem_calloc(1, sizeof(Resource));
        res->name = eina_stringshare_add(tag);
        POPUP.buf_resources = eina_list_sorted_insert(POPUP.buf_resources, (Eina_Compare_Cb) resource_cmp, res);
     }
   eina_list_free(tags);

   POPUP.dialog = elm_popup_add(ap.win);
   elm_popup_orient_set(POPUP.dialog, ELM_POPUP_ORIENT_CENTER);
   buf = eina_stringshare_printf(_("Add tag to style: %s"), style_name);
   elm_object_part_text_set(POPUP.dialog, "title,text", buf);

   BOX_ADD(POPUP.dialog, box, false, false);

   LAYOUT_PROP_ADD(box, "Tag name:", "property", "1swallow")
   POPUP.name_validator = resource_name_validator_new(NAME_REGEX, NULL);
   resource_name_validator_list_set(POPUP.name_validator, &POPUP.buf_resources, true);
   ENTRY_ADD(item, POPUP.name, true);
   eo_event_callback_add(POPUP.name, ELM_ENTRY_EVENT_VALIDATE, resource_name_validator_helper, POPUP.name_validator);
   evas_object_smart_callback_add(POPUP.name, "changed", _validate, style_edit);
   elm_object_part_text_set(POPUP.name, "guide", _("Type a new tag name."));
   elm_object_part_content_set(item, "elm.swallow.content", POPUP.name);
   elm_box_pack_end(box, item);

   elm_object_content_set(POPUP.dialog, box);
   evas_object_show(box);

   BUTTON_ADD(POPUP.dialog, POPUP.btn_add, _("Ok"));
   evas_object_smart_callback_add(POPUP.btn_add, "clicked", _on_tag_add_bt_ok, style_edit);
   elm_object_part_content_set(POPUP.dialog, "button1", POPUP.btn_add);
   elm_object_disabled_set(POPUP.btn_add, true);

   BUTTON_ADD(POPUP.dialog, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_popup_bt_cancel, style_edit);
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
   Resource *res;

   assert(style_edit != NULL);

   Elm_Object_Item *glit = elm_genlist_selected_item_get(style_edit->glist);
   if (!glit) return;
   Elm_Object_Item *glit_parent = elm_genlist_item_parent_get(glit);
   edje_edit_obj = ap.project->global_object;

   if (!glit_parent)
     {
        style_name = elm_object_item_part_text_get(glit, "elm.text");
        res = pm_resource_get(ap.project->styles, style_name);
        edje_edit_style_del(edje_edit_obj, style_name);
        ap.project->styles = pm_resource_del(ap.project->styles, res);
     }
   else
     {
        style_name = elm_object_item_part_text_get(glit_parent, "elm.text");
        tag = elm_object_item_part_text_get(glit, "elm.text");
        edje_edit_style_tag_del(edje_edit_obj, style_name, tag);
     }

   elm_object_item_del(glit);

   elm_object_disabled_set(style_edit->button_del, true);
   evas_object_smart_callback_call(ap.win, SIGNAL_STYLE_SELECTED, NULL);

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;
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

static Evas_Object *
_item_style_icon_get(void *data __UNUSED__,
                     Evas_Object *obj,
                     const char *part)
{
   Evas_Object *icon = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        ICON_STANDARD_ADD(obj, icon, true, "text_style");
     }

   return icon;
}

static Evas_Object *
_item_tags_icon_get(void *data __UNUSED__,
                    Evas_Object *obj,
                    const char *part)
{
   Evas_Object *icon = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        ICON_STANDARD_ADD(obj, icon, true, "text_style_tag");
     }

   return icon;
}

static inline Evas_Object *
_style_manager_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;

   assert(parent != NULL);

   ENTRY_ADD(parent, entry, true);
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}

static Eina_Bool
_search_tag_item_node(Evas_Object *obj,
                      Elm_Genlist_Item* item_start,
                      Search_Data *search_data,
                      Eina_Stringshare *str)
{
   Eina_List *tags, *l_tg;
   char *tag;
   Eina_Bool find_tag_item = EINA_FALSE;

   if ((!elm_genlist_item_parent_get(item_start)) && (!elm_genlist_item_expanded_get(item_start)))
     {
        const char *name = elm_object_item_data_get(item_start);

        tags = edje_edit_style_tags_list_get(ap.project->global_object, name);

        EINA_LIST_FOREACH(tags, l_tg, tag)
          {
             if (strstr(tag, str))
               {
                  elm_genlist_item_expanded_set(item_start, true);
                  search_data->last_item_found = elm_genlist_search_by_text_item_get(obj, item_start,
                                                                        "elm.text", tag, 0);
                  elm_genlist_item_selected_set(search_data->last_item_found, true);
                  elm_genlist_item_bring_in(search_data->last_item_found, ELM_GENLIST_ITEM_SCROLLTO_TOP);
                  elm_object_focus_set(search_data->search_entry, true);
                  find_tag_item = EINA_TRUE;
                  break;
                }
          }
        eina_list_free(tags);
     }

   return find_tag_item;
}

static Eina_Bool
_search_item_genlist_tree(Evas_Object *obj,
                          Elm_Genlist_Item* item,
                          Search_Data *search_data,
                          Eina_Stringshare *str)
{
   const char *text_item = elm_object_item_data_get(item);
   if (strstr(text_item, str))
     {
        elm_genlist_item_selected_set(item, true);
        elm_genlist_item_bring_in(item, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
        elm_object_focus_set(search_data->search_entry, true);

        search_data->last_item_found = item;
        return EINA_TRUE;
     }
   else
     {
        if (_search_tag_item_node(obj, item, search_data, str))
          return EINA_TRUE;
     }
   return EINA_FALSE;
}
static void
_genlist_item_search_first_search(Evas_Object *obj,
                                  Search_Data *search_data,
                                  Eina_Stringshare *str)
{
  Elm_Genlist_Item* item = elm_genlist_first_item_get(obj);
  if (!item) return;

  elm_genlist_item_bring_in(item, ELM_GENLIST_ITEM_SCROLLTO_TOP);

  search_data->last_item_found = NULL;

  for(; item; item = elm_genlist_item_next_get(item))
    {
        if (_search_item_genlist_tree(obj, item, search_data, str))
          return;
    }

  search_data->last_item_found = NULL;
}

static void
_genlist_item_search_next_search(Evas_Object *obj,
                                 Search_Data *search_data,
                                 Eina_Stringshare *str)
{
  Elm_Genlist_Item* item = NULL;

  if (_search_tag_item_node(obj, search_data->last_item_found, search_data, str))
    return;

  if (search_data->last_item_found == elm_genlist_last_item_get(obj))
    return;

  for (item = elm_genlist_item_next_get(search_data->last_item_found); item; item = elm_genlist_item_next_get(item))
    {
        if (_search_item_genlist_tree(obj, item, search_data, str))
          return;
    }

    search_data->last_item_found = NULL;
}

static void
_search_changed(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Style_Editor *style_edit = data;

   assert(style_edit != NULL);
   if (elm_entry_is_empty(style_edit->style_search_data.search_entry))
     {
       if (style_edit->style_search_data.last_item_found)
         {
            elm_genlist_item_selected_set(style_edit->style_search_data.last_item_found, false);
            style_edit->style_search_data.last_item_found = NULL;
         }
       elm_genlist_item_bring_in(elm_genlist_first_item_get(style_edit->glist),
                                                            ELM_GENLIST_ITEM_SCROLLTO_TOP);
       return;
     }

   Eina_Stringshare *str = eina_stringshare_printf("%s",
                                      elm_entry_entry_get(style_edit->style_search_data.search_entry));

   _genlist_item_search_first_search(style_edit->glist, &(style_edit->style_search_data), str);

   eina_stringshare_del(str);

}

static void
_search_nxt_gd_item(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Style_Editor *style_edit = data;

   assert(style_edit != NULL);
   if (style_edit->style_search_data.last_item_found)
     {
        Eina_Stringshare *str = eina_stringshare_printf("%s",
                                           elm_entry_entry_get(style_edit->style_search_data.search_entry));

        _genlist_item_search_next_search(style_edit->glist, &(style_edit->style_search_data), str);
        eina_stringshare_del(str);
     }

}

static void
_search_reset_cb(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Search_Data *search_data = data;

   assert(search_data != NULL);
   search_data->last_item_found = NULL;
}

static void
_expand_request_cb(void *data __UNUSED__,
                   Evas_Object *o __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_TRUE);
}

static void
_expanded_cb(void *data,
             Evas_Object *o __UNUSED__,
             void *event_info)
{
   Style_Editor *style_edit = (Style_Editor *)data;
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   const char *name = elm_object_item_data_get(glit);
   Eina_List *tags, *l_tg;
   Elm_Object_Item *glit_tag;
   char *tag;

   tags = edje_edit_style_tags_list_get(ap.project->global_object, name);
   EINA_LIST_FOREACH(tags, l_tg, tag)
     {
        glit_tag = elm_genlist_item_append(style_edit->glist, _itc_tags,
                                           tag, glit, ELM_GENLIST_ITEM_NONE,
                                           _on_glit_selected, style_edit);
        elm_object_item_data_set(glit_tag, tag);
     }
   eina_list_free(tags);
}

static void
_contract_request_cb(void *data __UNUSED__,
                     Evas_Object *o __UNUSED__,
                     void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_FALSE);
}

static void
_contracted_cb(void *data __UNUSED__,
               Evas_Object *o __UNUSED__,
               void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

static void
_on_clicked_double(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;

   if (elm_genlist_item_type_get(glit) == ELM_GENLIST_ITEM_TREE)
     {
        elm_genlist_item_expanded_set(glit, !elm_genlist_item_expanded_get(glit));
     }
}

static void
_on_bt_add(void *data,
           Evas_Object *obj __UNUSED__,
           void *event_info __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;
   Evas_Coord x, y, h;

   assert(style_edit != NULL);

   evas_object_geometry_get(obj, &x, &y, NULL, &h);
   elm_menu_move(style_edit->menu, x, y + h);
   evas_object_show(style_edit->menu);

   if (elm_genlist_selected_item_get(style_edit->glist))
     elm_object_item_disabled_set(style_edit->menu_tag, false);
   else
     elm_object_item_disabled_set(style_edit->menu_tag, true);
}

static void
_change_bg_cb(void *data,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;

   assert(style_edit != NULL);

   int state = elm_radio_state_value_get(obj);
   Evas_Object *bg = NULL;
   Evas *canvas = evas_object_evas_get(obj);
   switch (state)
     {
      case 0:
        {
           IMAGE_ADD_NEW(obj, bg, "bg", "tile");
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

static Evas_Object*
_add_box_bg(Style_Editor *style_edit)
{
   Evas_Object *box_bg, *image_bg, *radio_group, *radio;

   BOX_ADD(ap.win, box_bg, true, false);
   elm_box_padding_set(box_bg, 6, 6);
   elm_box_align_set(box_bg, 1.0, 0.5);

#define _RADIO_ADD(RADIO, VALUE, IMAGE) \
   RADIO = elm_radio_add(ap.win); \
   elm_object_style_set(RADIO, "style_editor"); \
   elm_radio_state_value_set(RADIO, VALUE); \
   evas_object_show(RADIO); \
   IMAGE_ADD_NEW(box_bg, image_bg, "preview", IMAGE); \
   elm_object_part_content_set(RADIO, "bg", image_bg); \
   evas_object_smart_callback_add(RADIO, "changed", _change_bg_cb, style_edit); \
   elm_box_pack_end(box_bg, RADIO);

   _RADIO_ADD(radio_group, 0, "bg-tile");
   _RADIO_ADD(radio, 1, "bg-black");
   elm_radio_group_add(radio, radio_group);
   _RADIO_ADD(radio, 2, "bg-white");
   elm_radio_group_add(radio, radio_group);
#undef _RADIO_ADD

   return box_bg;
}

static void
_on_style_manager_close(void *data,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;

   assert(style_edit != NULL);

   eina_stringshare_del(CURRENT.stvalue);
   free(style_edit);
}

Evas_Object *
style_manager_add()
{
   Evas_Object *main_layout;
   Evas_Object *bg, *box_bg;
   Evas *canvas;
   Elm_Object_Item *glit_style;
   Evas_Object *button_add, *search, *ic;
   Eina_List *styles, *l_st;
   Resource *res;
   Evas_Textblock_Style *ts;
   Style_Editor *style_edit;
   static const char *style_buf = FONT_DEFAULT"'";

   assert(ap.project != NULL);

   style_edit = (Style_Editor *)mem_calloc(1, sizeof(Style_Editor));

   main_layout = elm_layout_add(ap.win);
   elm_layout_theme_set(main_layout, "layout", "style_manager", "default");
   elm_object_part_text_set(main_layout, "elm.text", _("Preview"));
   elm_layout_text_set(main_layout, "elm.subtext", _("Font list"));

   style_edit->entry_prev = elm_layout_add(main_layout);
   elm_layout_theme_set(style_edit->entry_prev, "layout", "style_manager", "preview");
   evas_object_show(style_edit->entry_prev);
   elm_object_signal_emit(style_edit->entry_prev, "entry,hide", "eflete");

   canvas = evas_object_evas_get(ap.win);
   IMAGE_ADD_NEW(ap.win, bg, "bg", "tile");
   elm_object_part_content_set(style_edit->entry_prev, "background", bg);

   style_edit->textblock_style = evas_object_textblock_add(canvas);
   elm_object_part_content_set(style_edit->entry_prev, "entry", style_edit->textblock_style);
   evas_object_textblock_valign_set(style_edit->textblock_style, 0.5);
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, style_buf);
   evas_object_textblock_style_set(style_edit->textblock_style, ts);
   evas_object_textblock_text_markup_set(style_edit->textblock_style, TEST_TEXT);
   evas_object_show(style_edit->textblock_style);
   elm_object_part_content_set(main_layout, "elm.swallow.preview", style_edit->entry_prev);

   if (!_itc_style)
     {
        _itc_style = elm_genlist_item_class_new();
        _itc_style->item_style = "aligned";
        _itc_style->func.text_get = _item_style_label_get;
        _itc_style->func.content_get = _item_style_icon_get;
        _itc_style->func.state_get = NULL;
        _itc_style->func.del = NULL;
     }
   if (!_itc_tags)
     {
        _itc_tags= elm_genlist_item_class_new();
        _itc_tags->item_style = "aligned";
        _itc_tags->func.text_get = _item_tags_label_get;
        _itc_tags->func.content_get = _item_tags_icon_get;
        _itc_tags->func.state_get = NULL;
        _itc_tags->func.del = NULL;
     }

   search = _style_manager_search_field_create(main_layout);
   elm_object_part_content_set(main_layout, "elm.swallow.search", search);
   evas_object_smart_callback_add(search, "changed", _search_changed, style_edit);
   evas_object_smart_callback_add(search, "activated", _search_nxt_gd_item, style_edit);
   evas_object_smart_callback_add(style_edit->glist, "pressed", _search_reset_cb, &(style_edit->style_search_data));
   style_edit->style_search_data.search_entry = search;
   style_edit->style_search_data.last_item_found = NULL;

   style_edit->glist = elm_genlist_add(main_layout);
   elm_object_part_content_set(main_layout, "elm.swallow.list", style_edit->glist);
   evas_object_smart_callback_add(style_edit->glist, "clicked,double", _on_clicked_double, NULL);
   evas_object_smart_callback_add(style_edit->glist, "expand,request", _expand_request_cb, NULL);
   evas_object_smart_callback_add(style_edit->glist, "expanded", _expanded_cb, style_edit);
   evas_object_smart_callback_add(style_edit->glist, "contract,request", _contract_request_cb, NULL);
   evas_object_smart_callback_add(style_edit->glist, "contracted", _contracted_cb, NULL);
   evas_object_smart_callback_add(style_edit->glist, "unselected", _on_unselected_cb, style_edit);
   evas_object_show(style_edit->glist);
   /*elm_genlist_tree_effect_enabled_set(style_edit->glist, EINA_TRUE);*/

   styles = ap.project->styles;

   EINA_LIST_FOREACH(styles, l_st, res)
     {
        glit_style = elm_genlist_item_append(style_edit->glist, _itc_style,
                                             res->name, NULL, ELM_GENLIST_ITEM_TREE,
                                             _on_glit_selected, style_edit);
        elm_object_item_data_set(glit_style, (char *)res->name);
     }

   style_edit->menu = elm_menu_add(ap.win);
   elm_menu_item_add(style_edit->menu, NULL, "text_style", _("Style"), _on_bt_style_add, style_edit);
   style_edit->menu_tag = elm_menu_item_add(style_edit->menu, NULL, "text_style_tag", _("Tag"), _on_bt_tag_add, style_edit);

   button_add = elm_button_add(ap.win);
   elm_object_style_set(button_add, "anchor");
   evas_object_show(button_add);
   ic = elm_icon_add(button_add);
   elm_icon_standard_set(ic, "plus");
   elm_object_part_content_set(button_add, NULL, ic);
   evas_object_smart_callback_add(button_add, "clicked", _on_bt_add, style_edit);
   elm_object_part_content_set(main_layout, "elm.swallow.btn_add", button_add);

   style_edit->button_del = elm_button_add(ap.win);
   elm_object_style_set(style_edit->button_del, "anchor");
   evas_object_show(style_edit->button_del);
   ic = elm_icon_add(style_edit->button_del);
   elm_icon_standard_set(ic, "minus");
   elm_object_part_content_set(style_edit->button_del, NULL, ic);
   evas_object_smart_callback_add(style_edit->button_del, "clicked", _on_bt_del, style_edit);
   elm_object_part_content_set(main_layout, "elm.swallow.btn_del", style_edit->button_del);
   elm_object_disabled_set(style_edit->button_del, true);

   box_bg = _add_box_bg(style_edit);
   elm_object_part_content_set(main_layout, "elm.swallow.menu", box_bg);

   evas_textblock_style_free(ts);
   evas_object_event_callback_add(main_layout, EVAS_CALLBACK_DEL, _on_style_manager_close, style_edit);

   evas_object_smart_callback_call(ap.win, SIGNAL_STYLE_SELECTED, NULL);
   return main_layout;
}

#undef FONT_DEFAULT
#undef POPUP
#undef CURRENT
