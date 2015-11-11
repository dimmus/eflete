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

#include "style_editor.h"
#include "main_window.h"
#include "alloc.h"
#include "editor.h"

TODO("Rename this file to textblock_style_manager")

#define FONT_DEFAULT "DEFAULT='align=middle font=Sans font_size=24 color=#000000 "
#define DIRECTION_NUM 39
#define DEFAULT_DIRECTION 2
#define WHITE_COLOR "#FFF"

typedef struct _Style_Tag_Entries Style_Tag_Entries;
typedef struct _Style_entries Style_Entries;
typedef struct _Style_Editor Style_Editor;
typedef struct _Search_Data Search_Data;

typedef enum {
   TAB_EDITOR_TEXT = 0,
   TAB_EDITOR_FORMAT,
   TAB_EDITOR_GLOW_SHADOW,
   TAB_EDITOR_LINES,
   TAB_LAST
} Tabs_Style_Editor;

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Style_Editor
{
   Evas_Object *mwin;
   Evas_Object *glist;
   Evas_Object *textblock_style;
   Evas_Object *entry_prev;
   struct {
      Evas_Object *layout;
      Evas_Object *content;
      Evas_Object *tabs;

      Evas_Object *content_text;
      Evas_Object *content_format;
      Evas_Object *content_glow_shadow;
      Evas_Object *content_lines;

      Tabs_Style_Editor current_tab;
   } prop;
   Search_Data style_search_data;
   struct {
      const char *st_name;
      const char *st_tag;
      Eina_Stringshare *stvalue;
   } current_style;
   struct {
      Evas_Object *name;
      Elm_Validator_Regexp *name_validator;
      Evas_Object *btn_add;
      Evas_Object *dialog;
   } popup;
   Elm_Object_Item *tag;
   Evas_Object *direction[8];
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
                                       {NULL, NULL},
                                       {"direction", NULL}};
/*
TODO("<number>, <number>% for align")
static const char *font_horizontal_align[] = { N_("auto"),
                                               N_("center"),
                                               N_("middle"),
                                               N_("left"),
                                               N_("right"),
                                               NULL};

TODO("<number>, <number>% for align")
static const char *font_horizontal_valign[] = { N_("top"),
                                                N_("bottom"),
                                                N_("middle"),
                                                N_("center"),
                                                N_("baseline"),
                                                N_("base"),
                                                NULL};
*/
/*
static const char *font_glow_list[] = { N_("none"),
                                        N_("plain"),
                                        N_("shadow"),
                                        N_("outline"),
                                        N_("soft_outline"),
                                        N_("outline_shadow"),
                                        N_("outline_soft_shadow"),
                                        N_("glow"),
                                        N_("shadow"),
                                        N_("far_shadow"),
                                        N_("soft_shadow"),
                                        N_("far_soft_shadow"),
                                        NULL};

static const char *direction_list[] = { N_("bottom_left"),
                                        N_("bottom"),
                                        N_("bottom_right"),
                                        N_("left"),
                                        N_("right"),
                                        N_("top_left"),
                                        N_("top"),
                                        N_("top_right"),
                                        NULL};
*/
static const char *underl_styles[] = { N_("single"),
                                       N_("double"),
                                       NULL};
static Eina_Tmpstr*
_tag_value_get(const char* text_style, char* a_tag);


static void
_text_tab_update(Style_Editor *style_edit, Evas_Object *tabs, const char *value);

/*
static void
_format_tab_update(Style_Editor *style_edit, Evas_Object *tabs, const char *value);
*/

/*
static void
_glow_tab_update(Style_Editor *style_edit, Evas_Object *tabs, const char *value);
*/
static void
_lines_tab_update(Style_Editor *style_edit, Evas_Object *tabs, const char *value);

static void
_on_popup_bt_cancel(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *ei __UNUSED__)
{
   Style_Editor *style_edit = (Style_Editor *)data;

   assert(style_edit != NULL);
   assert(POPUP.name_validator != NULL);
   elm_validator_regexp_free(POPUP.name_validator);
   POPUP.name_validator = NULL;

   evas_object_del(POPUP.dialog);
   POPUP.dialog = NULL;
}

static void
_style_edit_update(Style_Editor *style_edit)
{
   Evas_Textblock_Style *ts = NULL;
   Eina_Strbuf *style = eina_strbuf_new();

   assert(style_edit != NULL);

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

#define TAB_UPDATE(VALUE) \
static void \
_##VALUE##_update(Style_Editor *style_edit) \
{ \
   assert(style_edit != NULL); \
   _##VALUE##_tab_update(style_edit, style_edit->prop.tabs, CURRENT.stvalue); \
}
/*
TAB_UPDATE(glow)
*/
TAB_UPDATE(lines)

#undef TAB_UPDATE

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

   Eina_Strbuf *style = eina_strbuf_new();
   eina_strbuf_append(style, FONT_DEFAULT);

   Style_Editor *style_edit = (Style_Editor *)data;

   assert(style_edit != NULL);

   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Elm_Object_Item *glit_parent = elm_genlist_item_parent_get(glit);

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
        value = edje_edit_style_tag_value_get(edje_edit_obj, style_name, tag);
        eina_strbuf_append(style, value);
     }
   CURRENT.st_name = style_name;
   CURRENT.st_tag = tag;
   CURRENT.stvalue = eina_stringshare_add(value);

   evas_object_del(style_edit->prop.content_text);
   evas_object_del(style_edit->prop.content_format);
   evas_object_del(style_edit->prop.content_glow_shadow);
   evas_object_del(style_edit->prop.content_lines);

   if (!glit_parent)
     {
        elm_object_signal_emit(style_edit->entry_prev, "entry,hide", "eflete");
     }
   else
     {
        _text_tab_update(style_edit, NULL /*style_edit->prop.tabs*/, eina_strbuf_string_get(style));
        //_format_tab_update(style_edit, NULL /*style_edit->prop.tabs*/, eina_strbuf_string_get(style));
        //_glow_tab_update(style_edit, NULL /*style_edit->prop.tabs*/, eina_strbuf_string_get(style));
        //_lines_tab_update(style_edit, NULL /*style_edit->prop.tabs*/, eina_strbuf_string_get(style));

        elm_object_signal_emit(style_edit->entry_prev, "entry,show", "eflete");
        eina_strbuf_append(style, "'");
        ts = evas_textblock_style_new();
        evas_textblock_style_set(ts, eina_strbuf_string_get(style));
        evas_object_textblock_style_set(style_edit->textblock_style, ts);
        evas_object_size_hint_max_set(style_edit->textblock_style, EVAS_HINT_FILL,
                                      EVAS_HINT_FILL);
        evas_textblock_style_free(ts);
     }
   eina_strbuf_free(style);
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

   if ((!style_name) || (strcmp(style_name, "") == 0))
     {
        NOTIFY_WARNING(_("Style name can not be empty!"));
        return;
     }
   if (!edje_edit_style_add(edje_edit_obj, style_name))
     {
        NOTIFY_WARNING(_("Style name must be unique!"));
        return;
     }
   if (edje_edit_style_tag_add(edje_edit_obj, style_name, "DEFAULT"))
     {
        if (!edje_edit_style_tag_value_set(edje_edit_obj,
                                           style_name,
                                          "DEFAULT",
                                          "align=middle font=Sans font_size=24 color=#000000FF"))
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

   TODO("Need refactoring after callback logic for modal window implementation")

   elm_genlist_item_selected_set(glit_style, true);
   elm_genlist_item_bring_in(glit_style, ELM_GENLIST_ITEM_SCROLLTO_IN);
   elm_genlist_item_show(glit_style, ELM_GENLIST_ITEM_SCROLLTO_IN);

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

   if ((!tag_name) || (strcmp(tag_name, "") == 0))
     {
        NOTIFY_WARNING(_("Tag name can not be empty!"));
        return;
     }
   if (!edje_edit_style_tag_add(edje_edit_obj, style_name, tag_name))
     {
        NOTIFY_WARNING(_("Tag name must be unique!"));
        return;
     }
   else
     if (!edje_edit_style_tag_value_set(edje_edit_obj, style_name, tag_name, ""))
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
   elm_genlist_item_selected_set(glit_tag, true);
   elm_genlist_item_show(style_edit->tag, ELM_GENLIST_ITEM_SCROLLTO_IN);
   elm_genlist_item_bring_in(glit_tag, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);

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

   if (elm_validator_regexp_status_get(POPUP.name_validator) != ELM_REG_NOERROR)
     elm_object_disabled_set(POPUP.btn_add, true);
   else
     elm_object_disabled_set(POPUP.btn_add, false);
}

static void
_on_bt_style_add(Style_Editor *style_edit)
{
   Evas_Object *box, *item, *button;

   assert(style_edit != NULL);

   POPUP.dialog = elm_popup_add(ap.win);
   elm_object_part_text_set(POPUP.dialog, "title,text", _("Add textblock style"));

   BOX_ADD(POPUP.dialog, box, false, false);

   LAYOUT_PROP_ADD(box, _("Style name:"), "property", "1swallow")
   POPUP.name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);
   ENTRY_ADD(item, POPUP.name, true);
   eo_do(POPUP.name, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, POPUP.name_validator));
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
_on_bt_tag_add(Style_Editor *style_edit)
{
   Evas_Object *box, *item, *button;

   assert(style_edit != NULL);
   assert(POPUP.name_validator == NULL);

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

   POPUP.dialog = elm_popup_add(ap.win);
   buf = eina_stringshare_printf(_("Add tag to style: %s"), style_name);
   elm_object_part_text_set(POPUP.dialog, "title,text", buf);

   BOX_ADD(POPUP.dialog, box, false, false);

   LAYOUT_PROP_ADD(box, "Tag name:", "property", "1swallow")
   POPUP.name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);
   ENTRY_ADD(item, POPUP.name, true);
   eo_do(POPUP.name, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, POPUP.name_validator));
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
   State *state;
   char buf[BUFF_MAX];
   int symbs = 0;
   Eina_List *l;
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
        if (!res->used_in)
          {
             edje_edit_style_del(edje_edit_obj, style_name);
             ap.project->colorclasses = pm_resource_del(ap.project->colorclasses, res);
          }
        else
          {
             snprintf(buf, BUFF_MAX, _("Style is used in:"));
             symbs = strlen(buf);
             EINA_LIST_FOREACH(res->used_in, l, state)
               {
                  snprintf(buf + symbs, BUFF_MAX - symbs, _("<br>group: %s<br>part: %s<br>state: \"%s\" %2.1f"),
                           state->part->group->name, state->part->name, state->parsed_name, state->parsed_val);
                  symbs += strlen(res->name);
                  break; TODO("remove this break after warning style remake")
               }
             NOTIFY_WARNING("%s", buf);
             return;
          }
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

static inline Evas_Object *
_style_editor_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;

   assert(parent != NULL);

   ENTRY_ADD(parent, entry, true);
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}

static void
_on_bt_add(void *data,
           Evas_Object *obj __UNUSED__,
           void *event_info)
{
   Style_Editor *style_edit = (Style_Editor *)data;

   assert(style_edit != NULL);

   Ewe_Combobox_Item *selected_item = event_info;

   if (!selected_item->index) _on_bt_style_add(style_edit);
   else _on_bt_tag_add(style_edit);
}

ITEM_SEARCH_FUNC(genlist,ELM_GENLIST_ITEM_SCROLLTO_MIDDLE, "elm.text")

static void
_search_changed(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Style_Editor *style_edit = data;

   assert(style_edit != NULL);

   _genlist_item_search(style_edit->glist, &(style_edit->style_search_data),
                        style_edit->style_search_data.last_item_found);
}

static void
_search_nxt_gd_item(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Style_Editor *style_edit = data;
   Elm_Object_Item *start_from = NULL;

   assert(style_edit != NULL);

   if (style_edit->style_search_data.last_item_found)
     {
        start_from =
           elm_genlist_item_next_get(style_edit->style_search_data.last_item_found);
     }

   _genlist_item_search(style_edit->glist, &(style_edit->style_search_data), start_from);
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

/* Creating the view of the mwin!!! */
Evas_Object *
_form_right_side(Style_Editor *style_edit)
{
   Elm_Object_Item *glit_style;
   Evas_Object *layout, *btn, *combobox, *search, *ic;
   Eina_List *styles, *l_st;
   Resource *res;

   assert(style_edit != NULL);

   if (!_itc_style)
     {
        _itc_style = elm_genlist_item_class_new();
        _itc_style->item_style = "navigator";
        _itc_style->func.text_get = _item_style_label_get;
        _itc_style->func.content_get = NULL;
        _itc_style->func.state_get = NULL;
        _itc_style->func.del = NULL;
     }
   if (!_itc_tags)
     {
        _itc_tags= elm_genlist_item_class_new();
        _itc_tags->item_style = "default";
        _itc_tags->func.text_get = _item_tags_label_get;
        _itc_tags->func.content_get = NULL;
        _itc_tags->func.state_get = NULL;
        _itc_tags->func.del = NULL;
     }

   layout = elm_layout_add(ap.win);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout, "layout", "style_editor", "font_list");
   elm_object_part_text_set(layout, "label.font_list", _("Font list"));
   evas_object_show(layout);

   search = _style_editor_search_field_create(layout);
   elm_object_part_content_set(layout, "swallow.search", search);
   evas_object_smart_callback_add(search, "changed", _search_changed, style_edit);
   evas_object_smart_callback_add(search, "activated", _search_nxt_gd_item, style_edit);
   evas_object_smart_callback_add(style_edit->glist, "pressed", _search_reset_cb,
                                  &(style_edit->style_search_data));
   style_edit->style_search_data.search_entry = search;
   style_edit->style_search_data.last_item_found = NULL;

   style_edit->glist = elm_genlist_add(layout);
   elm_object_part_content_set(layout, "swallow.font_list", style_edit->glist);
   evas_object_smart_callback_add(style_edit->glist, "clicked,double", _on_clicked_double, NULL);
   evas_object_smart_callback_add(style_edit->glist, "expand,request", _expand_request_cb, NULL);
   evas_object_smart_callback_add(style_edit->glist, "expanded", _expanded_cb, style_edit);
   evas_object_smart_callback_add(style_edit->glist, "contract,request", _contract_request_cb, NULL);
   evas_object_smart_callback_add(style_edit->glist, "contracted", _contracted_cb, NULL);
   evas_object_show(style_edit->glist);

   evas_object_size_hint_align_set(style_edit->glist, EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(style_edit->glist, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   styles = ap.project->styles;

   EINA_LIST_FOREACH(styles, l_st, res)
     {
        glit_style = elm_genlist_item_append(style_edit->glist, _itc_style,
                                             res->name, NULL, ELM_GENLIST_ITEM_TREE,
                                             _on_glit_selected, style_edit);
        elm_object_item_data_set(glit_style, (char *)res->name);
     }

   EWE_COMBOBOX_ADD(layout, combobox);
   ewe_combobox_style_set(combobox, "small");
   ewe_combobox_item_add(combobox, _("New style"));
   ewe_combobox_item_add(combobox, _("New tag"));
   evas_object_smart_callback_add(combobox, "selected", _on_bt_add, style_edit);
   elm_object_part_content_set(layout, "swallow.button_add", combobox);

   btn = elm_button_add(ap.win);
   evas_object_show(btn);
   ic = elm_icon_add(btn);
   elm_icon_standard_set(ic, "minus");
   elm_object_part_content_set(btn, NULL, ic);

   evas_object_smart_callback_add(btn, "clicked", _on_bt_del, style_edit);
   elm_object_part_content_set(layout, "swallow.button_rm", btn);

   return layout;
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

static void
_entry_repch_update(Style_Editor *style_edit, Eina_Bool password)
{
   assert(style_edit != NULL);

   if (password)
     evas_object_textblock_replace_char_set(style_edit->textblock_style, "*");
   else
     evas_object_textblock_replace_char_set(style_edit->textblock_style, NULL);
}

static void
_tag_parse(Style_Editor *style_edit, const char *value, const char *text)
{
   Evas_Object *edje_edit_obj = NULL;
   Eina_Strbuf *tag = eina_strbuf_new();
   char *stolen_buf;
   char *token;
   int i = 0, k = 0, exist = 0, style_length = 0;

   assert(style_edit != NULL);
   assert(value != NULL);
   assert(text != NULL);

   eina_strbuf_append(tag, CURRENT.stvalue);
   edje_edit_obj = ap.project->global_object;
   stolen_buf = eina_strbuf_string_steal(tag);
   token = strtok(stolen_buf, " =+");
   while (token)
     {
        if ((i + 1) % 2 != 0)
          {
             for (k = 0; style_table[k][0] != NULL; k++)
               {
                  if (!strcmp(style_table[k][0], token)) exist = k;
               }
          }
        else if (strstr(token, "shadow"))
          {
             style_table[DIRECTION_NUM][1] = eina_stringshare_add(strchr(token, ','));
             style_length = (int)(strlen(token) - strlen(style_table[DIRECTION_NUM][1]));
             style_table[exist][1] = eina_stringshare_add_length(token, style_length);
          }
        else
          {
             style_table[exist][1] = eina_stringshare_add(token);
          }
        token= strtok(NULL, " =+");
        i++;
     }
   free(stolen_buf);
   if (!strcmp(text, "password"))
     _entry_repch_update(style_edit, !strcmp(value, "on"));

   if (!strcmp(text, "direction"))
     {
        if (style_table[DIRECTION_NUM][1]) eina_stringshare_del(style_table[DIRECTION_NUM][1]);
        style_table[DIRECTION_NUM][1] = eina_stringshare_printf(",%s", value);
     }
   else
     {
        for (k = 0; style_table[k][0] != NULL; k++)
          {
             if (!strcmp(style_table[k][0], text))
               {
                  eina_stringshare_del(style_table[k][1]);
                  style_table[k][1] = eina_stringshare_add(value);
               }
          }
     }
   if ((!strcmp(text, "style")) && (!style_table[DIRECTION_NUM][1]))
     style_table[DIRECTION_NUM][1] = eina_stringshare_add(",bottom_right");
   eina_strbuf_append(tag, "+ ");
   for (k = 0; style_table[k][0] != NULL; k++)
      {
         if ((style_table[k][1] != NULL) && (!strstr(style_table[k][1], "shadow")))
           {
              eina_strbuf_append(tag, style_table[k][0]);
              eina_strbuf_append(tag, "=");
              eina_strbuf_append(tag, style_table[k][1]);
              eina_strbuf_append(tag, " ");
              eina_stringshare_del(style_table[k][1]);
              style_table[k][1] = NULL;
           }
         else if ((style_table[k][1] != NULL) && (strstr(style_table[k][1], "shadow")))
           {
              eina_strbuf_append(tag, style_table[k][0]);
              eina_strbuf_append(tag, "=");
              eina_strbuf_append(tag, style_table[k][1]);
              eina_strbuf_append(tag, style_table[DIRECTION_NUM][1]);
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

static void
_lines_colors_update(Style_Editor *style_edit, const char *param)
{
   Eina_Tmpstr *val;

   assert(style_edit != NULL);
   assert(param != NULL);

   if (!strcmp(param, "underline"))
     {
        _lines_update(style_edit);
        val = _tag_value_get(style_edit->current_style.stvalue, "underline_color");
        if (!val) _tag_parse(style_edit, WHITE_COLOR, "underline_color");
        else eina_tmpstr_del(val);

        val = _tag_value_get(style_edit->current_style.stvalue, "underline2_color");
        if (!val) _tag_parse(style_edit, WHITE_COLOR, "underline2_color");
        else eina_tmpstr_del(val);
     }
   else if (!strcmp(param, "strikethrough"))
     {
        val = _tag_value_get(style_edit->current_style.stvalue, "strikethrough_color");
        if (!val) _tag_parse(style_edit, WHITE_COLOR, "strikethrough_color");
        else eina_tmpstr_del(val);
     }
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

#define PERCENT_SPINNER_VALUE \
const char *value = NULL; \
if (!ei) \
  { \
     value = eina_stringshare_printf("%f", elm_spinner_value_get(obj)/100); \
  }

#define CHECK_VALUE \
Evas_Object *check = obj; \
const char *value = NULL; \
if (elm_check_state_get(check)) value = eina_stringshare_add("on"); \
else value = eina_stringshare_add("off");

#define CHANGE_CALLBACK(VALUE, TEXT, WIDGET, FUNC) \
static void \
_on_##VALUE##_change(void *data, \
                     Evas_Object *obj EINA_UNUSED, \
                     void *ei EINA_UNUSED) \
{ \
   Style_Editor *style_edit = (Style_Editor *)data; \
   assert(style_edit != NULL); \
   WIDGET##_VALUE \
   _tag_parse(style_edit, value, TEXT); \
   FUNC; \
   _lines_colors_update(style_edit, TEXT); \
   _style_edit_update(style_edit); \
   eina_stringshare_del(value); \
   /*project_changed(false);*/ \
}

#define ITEM_COLOR_ADD(VALUE, TAG, TEXT) \
static void \
_on_##VALUE##_change(void *data, \
                     Evas_Object *obj, \
                     void *ei __UNUSED__) \
{ \
   int r, g, b, a; \
   const char *value; \
   Style_Editor *style_edit = (Style_Editor *)data; \
   assert(style_edit != NULL); \
   Evas_Object *color, *rect; \
   color = evas_object_data_get(obj, "color"); \
   colorselector_color_get(obj, &r, &g, &b, &a); \
   value = eina_stringshare_printf("#%02x%02x%02x%02x", r, g, b, a); \
   _tag_parse(style_edit, value, TAG); \
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
   assert(style_edit != NULL); \
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
   assert(style_edit != NULL); \
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
} \
static Evas_Object * \
_style_item_##VALUE##_add(Evas_Object *layout, Style_Editor *style_edit) \
{ \
   Evas_Object *color, *rect; \
   assert(layout != NULL); \
   assert(style_edit != NULL); \
   color = elm_layout_add(layout); \
   elm_layout_file_set(color, EFLETE_EDJ, "eflete/style/color"); \
   evas_object_size_hint_weight_set(color, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(color, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   rect = evas_object_rectangle_add(evas_object_evas_get(layout)); \
   evas_object_event_callback_add(color, EVAS_CALLBACK_MOUSE_DOWN, \
                                  _on_##VALUE##_clicked, style_edit); \
   evas_object_color_set(rect, 0, 0, 0, 255); \
   elm_object_part_content_set(color, "elm.swallow.content", rect); \
   elm_object_part_content_set(layout, "swallow."TEXT, color); \
   return rect; \
}

#define MIN_SP - 1.0
#define MARGIN_MIN_SP 0.0
#define MAX_SP 9999.0
#define MAX_PERCENT 100.0
#define STEP_SP 1.0

#define COMBO_ADD(VALUE) \
EWE_COMBOBOX_ADD(ap.win, widget); \
evas_object_smart_callback_add(widget, "selected", _on_##VALUE##_change, style_edit);

#define STYLE_ADD(VALUE) \
int i = 0; \
SEGMENT_CONTROL_ADD(layout, widget); \
elm_object_style_set(layout, "style_editor"); \
for (i = 0; font_styles[i] != NULL; i++) \
elm_segment_control_item_add(widget, NULL, font_styles[i]); \
evas_object_smart_callback_add(widget, "changed", _on_##VALUE##_change, style_edit); \
evas_object_smart_callback_del(widget, "changed", _on_##VALUE##_change);

#define UNDERLINE_ADD(VALUE) \
int i = 0; \
SEGMENT_CONTROL_ADD(layout, widget); \
elm_object_style_set(layout, "style_editor"); \
for (i = 0; underl_styles[i] != NULL; i++) \
elm_segment_control_item_add(widget, NULL, underl_styles[i]); \
evas_object_smart_callback_add(widget, "changed", _on_##VALUE##_change, style_edit); \
evas_object_smart_callback_del(widget, "changed", _on_##VALUE##_change);

#define SPIN_ADD(VALUE) \
SPINNER_ADD(layout, widget, MIN_SP, MAX_SP, STEP_SP, true); \
evas_object_smart_callback_add(widget, "changed", _on_##VALUE##_change, style_edit);

#define MARGIN_SPIN_ADD(VALUE) \
SPINNER_ADD(layout, widget, MARGIN_MIN_SP, MAX_SP, STEP_SP, true); \
evas_object_smart_callback_add(widget, "changed", _on_##VALUE##_change, style_edit);

#define CHK_ADD(VALUE) \
CHECK_ADD(layout, widget); \
evas_object_smart_callback_add(widget, "changed", _on_##VALUE##_change, style_edit);

#define ELLIPSIS_ADD(VALUE) \
SPINNER_ADD(layout, widget, MIN_SP, MAX_PERCENT, STEP_SP, true); \
evas_object_smart_callback_add(widget, "changed", _on_##VALUE##_change, style_edit);

#define ITEM_TEXT_ADD(SWALLOW, VALUE, WIDGET) \
static Evas_Object * \
_style_item_##VALUE##_add(Evas_Object *layout, Style_Editor *style_edit) \
{ \
   Evas_Object *widget; \
\
   assert(layout != NULL); \
   assert(style_edit != NULL); \
\
   WIDGET##_ADD(VALUE) \
\
   elm_object_part_content_set(layout, "swallow."SWALLOW, widget); \
   return widget; \
}

#define DIRECT_ADD(VALUE, TEXT, STYLE, VAL) \
static void \
_on_##VALUE##_select_direction(void *data, \
                              Evas_Object *obj __UNUSED__, \
                              void *event_info __UNUSED__) \
{ \
   Style_Editor *st = data; \
   assert(st != NULL); \
   style_table[DIRECTION_NUM][1] = eina_stringshare_add(TEXT); \
   _tag_parse(st, TEXT, "direction"); \
   _style_edit_update(st); \
} \
static Evas_Object * \
_direction_item_##VALUE##_add(Evas_Object *item, Style_Editor *style_edit) \
{ \
   Evas_Object *widget; \
\
   assert(style_edit != NULL); \
   RADIO_ADD(item, widget, VAL, ""); \
   elm_object_style_set(widget, STYLE); \
   elm_object_part_content_set(item, "swallow."TEXT, widget); \
   evas_object_smart_callback_add(widget, "focused", _on_##VALUE##_select_direction, style_edit); \
   return widget; \
}

/*
DIRECT_ADD(bl,"bottom_left", "bl", 0)
DIRECT_ADD(b, "bottom", "b", 1)
DIRECT_ADD(br, "bottom_right", "br", 2)
DIRECT_ADD(l, "left", "l", 3)
DIRECT_ADD(r, "right", "r", 4)
DIRECT_ADD(tl, "top_left", "tl", 5)
DIRECT_ADD(t, "top", "t", 6)
DIRECT_ADD(tr, "top_right", "tr", 7)
*/
static Eina_Tmpstr*
_tag_value_get(const char* text_style, char* a_tag)
{
   assert(text_style != NULL);
   assert(a_tag != NULL);

   char *tag_list_copy = mem_malloc(strlen(text_style) + 1);
   Eina_Tmpstr *result = NULL;
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
                  result = eina_tmpstr_add(equals_sign);
                  if (!strstr(FONT_DEFAULT, a_tag)) break;
               }
          }
        token = strtok(0, " ");
     }
   free(tag_list_copy);

   return result;
}

static Eina_Bool
_hex_to_rgb(const char *hex, int *r, int *g, int *b, int *a)
{
   assert(hex != NULL);
   assert(r != NULL);
   assert(g != NULL);
   assert(b != NULL);
   assert(a != NULL);

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
   return true;
}

CHANGE_CALLBACK(fonts_list, "font", COMBOBOX, NULL)
CHANGE_CALLBACK(font_size, "font_size", SPINNER, NULL)
CHANGE_CALLBACK(font_style, "font_style", SEGMENT, NULL)
CHANGE_CALLBACK(font_width, "font_width", COMBOBOX, NULL)
CHANGE_CALLBACK(font_weight, "font_weight", COMBOBOX, NULL)
/*
CHANGE_CALLBACK(font_align, "align", COMBOBOX, NULL)
CHANGE_CALLBACK(lmargin, "left_margin", SPINNER, NULL)
CHANGE_CALLBACK(font_valign, "valign", COMBOBOX, NULL)
CHANGE_CALLBACK(rmargin, "right_margin", SPINNER, NULL)
CHANGE_CALLBACK(text_tabstops, "tabstops", SPINNER, NULL)
CHANGE_CALLBACK(line_size, "linesize", SPINNER, NULL)
CHANGE_CALLBACK(rel_size, "linerelsize", SPINNER, NULL)
CHANGE_CALLBACK(font_password, "password", CHECK, NULL)
CHANGE_CALLBACK(font_background, "backing", CHECK, NULL)
CHANGE_CALLBACK(font_ellipsis, "ellipsis", PERCENT_SPINNER, NULL)
CHANGE_CALLBACK(style, "style", COMBOBOX, _glow_update(style_edit))
*/
CHANGE_CALLBACK(strikethrough, "strikethrough", CHECK, NULL)
CHANGE_CALLBACK(underline, "underline", CHECK, NULL)
CHANGE_CALLBACK(underline_style, "underline", SEGMENT, NULL)

ITEM_TEXT_ADD("font", fonts_list, COMBO)
ITEM_TEXT_ADD("size", font_size, SPIN)
ITEM_TEXT_ADD("width", font_width, COMBO)
ITEM_TEXT_ADD("style", font_style, STYLE)
ITEM_TEXT_ADD("weight", font_weight, COMBO)
ITEM_COLOR_ADD(text_color, "color", "color")
/*
ITEM_TEXT_ADD("align", font_align, COMBO)
ITEM_TEXT_ADD("lmargin", lmargin, MARGIN_SPIN)
ITEM_TEXT_ADD("valign", font_valign, COMBO)
ITEM_TEXT_ADD("rmargin", rmargin, MARGIN_SPIN)
ITEM_TEXT_ADD("tabstops", text_tabstops, SPIN)
ITEM_TEXT_ADD("line_size", line_size, SPIN)
ITEM_TEXT_ADD("rel_size", rel_size, SPIN)
ITEM_TEXT_ADD("password", font_password, CHK)
ITEM_TEXT_ADD("background", font_background, CHK)
*/
/*
ITEM_COLOR_ADD(font_backcolor, "backing_color", "backing_color")
ITEM_TEXT_ADD("ellipsis", font_ellipsis, ELLIPSIS)
ITEM_TEXT_ADD("style", style, COMBO)
ITEM_COLOR_ADD(inner_glow, "glow_color", "glow_color")
ITEM_COLOR_ADD(outer_glow, "glow2_color", "glow2_color")
ITEM_COLOR_ADD(shadow_color, "shadow_color", "glow_color")
*/
ITEM_TEXT_ADD("strikethru", strikethrough, CHK)
ITEM_TEXT_ADD("underl", underline, CHK)
ITEM_TEXT_ADD("underline", underline_style, UNDERLINE)
ITEM_COLOR_ADD(strikethrough_color, "strikethrough_color", "strikethrough")
ITEM_COLOR_ADD(underline_color, "underline_color", "underline_color")
ITEM_COLOR_ADD(underline2_color, "underline2_color", "underline2_color")

static void
_text_tab_update(Style_Editor *style_edit, Evas_Object *tabs __UNUSED__, const char *value)
{
   Evas_Object *layout;
   Evas_Object *fonts_list, *font_size;
   Evas_Object *font_weight;
   Evas_Object *font_width, *font_style;
   Evas_Object *text_color;
   Elm_Object_Item *sc_item;
   int r, g, b, a;
   unsigned int i = 0;
   Evas_Object *scr;
   Eina_Bool flag = false;

   assert(style_edit != NULL);
   //assert(tabs != NULL);

   SCROLLER_ADD(ap.win, scr);
   elm_scroller_policy_set(scr, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);

   layout = elm_layout_add(ap.win);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout, "layout", "style_editor", "text");
   evas_object_show(layout);
   elm_object_content_set(scr, layout);

   style_edit->prop.content_text = scr;

   elm_object_part_text_set(layout, "label.font", _("Font name:"));
   elm_object_part_text_set(layout, "label.style", _("Font style:"));
   elm_object_part_text_set(layout, "label.color", _("Color:"));
   elm_object_part_text_set(layout, "label.start", _("size"));
   elm_object_part_text_set(layout, "label.end", _("px"));
   fonts_list = _style_item_fonts_list_add(layout, style_edit);
   font_size = _style_item_font_size_add(layout, style_edit);
   font_style = _style_item_font_style_add(layout, style_edit);
   font_weight = _style_item_font_weight_add(layout, style_edit);
   font_width = _style_item_font_width_add(layout, style_edit);
   text_color = _style_item_text_color_add(layout, style_edit);

   if (value)
     {
        Eina_Tmpstr *font = _tag_value_get(value, "font");
        if (!font) font = eina_tmpstr_add("");
        Eina_Tmpstr *spin_val = _tag_value_get(value, "font_size");
        if (!spin_val) spin_val = eina_tmpstr_add("");
        Eina_Tmpstr *weight = _tag_value_get(value, "font_weight");
        if (!weight) weight = eina_tmpstr_add("Normal");
        Eina_Tmpstr *width = _tag_value_get(value, "font_width");
        if (!width) width = eina_tmpstr_add("Normal");
        Eina_Tmpstr *style = _tag_value_get(value, "font_style");
        if (!style) style = eina_tmpstr_add("Normal");
        Eina_Tmpstr *color = _tag_value_get(value, "color");

        ewe_combobox_text_set(fonts_list, font);
        elm_object_disabled_set(fonts_list, true);
        elm_spinner_value_set(font_size, atof(spin_val));
        ewe_combobox_text_set(font_width, width);
        for (i = 0; font_width_list[i] != NULL; i++)
          ewe_combobox_item_add(font_width, font_width_list[i]);
        ewe_combobox_text_set(font_weight, weight);
        for (i = 0; font_weight_list[i] != NULL; i++)
          ewe_combobox_item_add(font_weight, font_weight_list[i]);
        for (i = 0; font_styles[i] != NULL; i++)
          {
             if (!strcmp(elm_segment_control_item_label_get(font_style, i), style))
               {
                  sc_item = elm_segment_control_item_get(font_style, i);
                  elm_segment_control_item_selected_set(sc_item, true);
                  if (flag == true)
                    evas_object_smart_callback_add(font_style, "changed", _on_font_style_change, style_edit);
                  else flag = true;
               }
          }
        if (!_hex_to_rgb(color, &r, &g, &b, &a))
          {
             ERR("Can't convert color value");
             abort();
          }
        evas_object_color_set(text_color, r*a/255, g*a/255, b*a/255, a);

        eina_tmpstr_del(font);
        eina_tmpstr_del(spin_val);
        eina_tmpstr_del(color);
        eina_tmpstr_del(weight);
        eina_tmpstr_del(width);
        eina_tmpstr_del(style);
     }

   if (style_edit->prop.current_tab == TAB_EDITOR_TEXT)
     elm_layout_content_set(style_edit->prop.content, NULL, scr);
}

/*
static void
_format_tab_update(Style_Editor *style_edit, Evas_Object *tabs, const char *value)
{
   Evas_Object *box_frames, *frame1, *frame2, *layout1, *layout2;
   Evas_Object *font_align, *font_lmargin, *font_valign,  *font_rmargin;
   Evas_Object *text_tabstops, *line_size, *rel_size;
   Evas_Object *font_password, *font_background, *font_backcolor, *font_ellipsis;
   Eina_Bool pass, bg;
   int r, g, b, a;
   unsigned int i = 0;

   assert(style_edit != NULL);
   //assert(tabs != NULL);

   Evas_Object *scr;
   SCROLLER_ADD(ap.win, scr);
   elm_scroller_policy_set(scr, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
   BOX_ADD(scr, box_frames, EINA_FALSE, EINA_FALSE)
   elm_box_align_set(box_frames, 0.5, 0.0);
   elm_object_content_set(scr, box_frames);

   style_edit->prop.content_format = scr;

   FRAME_ADD(box_frames, frame1, false, _("Positioning option"))
   evas_object_size_hint_weight_set(frame1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(frame1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   layout1 = elm_layout_add(frame1);
   elm_layout_theme_set(layout1, "layout", "style_editor", "format1");
   evas_object_size_hint_weight_set(layout1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(layout1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(layout1);
   elm_object_content_set(frame1, layout1);

   elm_object_part_text_set(layout1, "label.align", _("Horizontal align:"));
   elm_object_part_text_set(layout1, "label.valign", _("Vertical align:"));
   elm_object_part_text_set(layout1, "label.after1", _("%"));
   elm_object_part_text_set(layout1, "label.after2", _("%"));
   elm_object_part_text_set(layout1, "label.start1", _("Margin:"));
   elm_object_part_text_set(layout1, "label.left1", _("left"));
   elm_object_part_text_set(layout1, "label.left2", _("right"));
   elm_object_part_text_set(layout1, "label.end1", _("px"));
   elm_object_part_text_set(layout1, "label.end2", _("px"));

   font_align = _style_item_font_align_add(layout1, style_edit);
   font_lmargin = _style_item_lmargin_add(layout1, style_edit);
   font_valign = _style_item_font_valign_add(layout1, style_edit);
   font_rmargin = _style_item_rmargin_add(layout1, style_edit);

   if (value)
     {
        Eina_Tmpstr *align = _tag_value_get(value, "align");
        if (!align) align = eina_tmpstr_add("auto");
        Eina_Tmpstr *lmargin = _tag_value_get(value, "left_margin");
        if (!lmargin) lmargin = eina_tmpstr_add("0");
        Eina_Tmpstr *valign = _tag_value_get(value, "valign");
        if (!valign) valign = eina_tmpstr_add("baseline");
        Eina_Tmpstr *rmargin = _tag_value_get(value, "right_margin");
        if (!rmargin) rmargin = eina_tmpstr_add("0");

        ewe_combobox_text_set(font_align, align);
        for (i = 0; font_horizontal_align[i] != NULL; i++)
          ewe_combobox_item_add(font_align, font_horizontal_align[i]);
        elm_spinner_value_set(font_lmargin, atof(lmargin));

        ewe_combobox_text_set(font_valign, valign);
        for (i = 0; font_horizontal_valign[i] != NULL; i++)
          ewe_combobox_item_add(font_valign, font_horizontal_valign[i]);
        elm_spinner_value_set(font_rmargin, atof(rmargin));
        eina_tmpstr_del(align);
        eina_tmpstr_del(lmargin);
        eina_tmpstr_del(valign);
        eina_tmpstr_del(rmargin);
     }

   //FRAME_ADD(tabs, frame2, false, _("Text format"))
   layout2 = elm_layout_add(frame2);
   evas_object_size_hint_weight_set(layout2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_weight_set(layout2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout2, "layout","style_editor", "format2");
   evas_object_show(layout2);
   elm_object_content_set(frame2, layout2);

   elm_object_part_text_set(layout2, "label.tabstops", _("Text tabstops:"));
   elm_object_part_text_set(layout2, "label.line_size", _("Line size:"));
   elm_object_part_text_set(layout2, "label.left1", _("Line relative size:"));
   elm_object_part_text_set(layout2, "label.end1", _("px"));
   elm_object_part_text_set(layout2, "label.right", _("px"));
   elm_object_part_text_set(layout2, "label.end2", _("px"));
   elm_object_part_text_set(layout2, "label.password", _("Password field"));
   elm_object_part_text_set(layout2, "label.background", _("Background color"));
   elm_object_part_text_set(layout2, "label.ellipsis", _("Replace overflow characters with ellipsis"));

   text_tabstops = _style_item_text_tabstops_add(layout2, style_edit);
   line_size = _style_item_line_size_add(layout2, style_edit);
   rel_size = _style_item_rel_size_add(layout2, style_edit);
   font_password = _style_item_font_password_add(layout2, style_edit);
   font_ellipsis = _style_item_font_ellipsis_add(layout2, style_edit);
   font_background = _style_item_font_background_add(layout2, style_edit);
   font_backcolor = _style_item_font_backcolor_add(layout2, style_edit);

   if (value)
     {
        Eina_Tmpstr *tabstops = _tag_value_get(value, "tabstops");
        if (!tabstops) tabstops = eina_tmpstr_add("0");
        Eina_Tmpstr *linesize = _tag_value_get(value, "linesize");
        if (!linesize) linesize = eina_tmpstr_add("0");
        Eina_Tmpstr *linerelsize = _tag_value_get(value, "linerelsize");
        if (!linerelsize) linerelsize = eina_tmpstr_add("0");
        Eina_Tmpstr *password = _tag_value_get(value, "password");
        if ((!password) || (!strcmp(password, "off"))) pass = EINA_FALSE;
        else pass = EINA_TRUE;
        Eina_Tmpstr *ellipsis = _tag_value_get(value, "ellipsis");
        if (!ellipsis) ellipsis = eina_tmpstr_add("0");
        Eina_Tmpstr *bground = _tag_value_get(value, "backing");
        if ((!bground) || (!strcmp(bground, "off"))) bg = EINA_FALSE;
        else bg = EINA_TRUE;
        Eina_Tmpstr *bcolor = _tag_value_get(value, "backing_color");
        if (!bcolor)
          {
             bcolor = eina_tmpstr_add(WHITE_COLOR);
             _tag_parse(style_edit, WHITE_COLOR, "backing_color");
          }

        elm_spinner_value_set(text_tabstops, atof(tabstops));
        elm_spinner_value_set(line_size, atof(linesize));
        elm_spinner_value_set(rel_size, atof(linerelsize));
        elm_check_state_set(font_password, pass);
        elm_check_state_set(font_background, bg);
        _entry_repch_update(style_edit, pass);
        if (!_hex_to_rgb(bcolor, &r, &g, &b, &a))
          ERR("This error should not happen in style editor... Contact devs please!");
        evas_object_color_set(font_backcolor, r*a/255, g*a/255, b*a/255, a);
        elm_spinner_value_set(font_ellipsis, atof(ellipsis));
        eina_tmpstr_del(tabstops);
        eina_tmpstr_del(linesize);
        eina_tmpstr_del(linerelsize);
        eina_tmpstr_del(password);
        eina_tmpstr_del(ellipsis);
        eina_tmpstr_del(bground);
        eina_tmpstr_del(bcolor);
     }

   elm_box_pack_end(box_frames, frame1);
   elm_box_pack_end(box_frames, frame2);

   if (style_edit->prop.current_tab == TAB_EDITOR_FORMAT)
     elm_layout_content_set(style_edit->prop.content, NULL, scr);
}
*/

/*
static void
_glow_tab_update(Style_Editor *style_edit, Evas_Object *tabs, const char *value)
{
   Evas_Object *layout, *item;
   Evas_Object *font_style, *shadow_color, *inner_glow, *outer_glow;
   int r, g, b, a;
   unsigned int i = 0;
   Evas_Object *scr;
   char *style_copy = NULL;
   char *token;
   int count = 0, direction = DEFAULT_DIRECTION;

   assert(style_edit != NULL);
   //assert(tabs != NULL);

   SCROLLER_ADD(ap.win, scr);

   layout = elm_layout_add(ap.win);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout, "layout", "style_editor", "glow");
   evas_object_show(layout);
   elm_object_content_set(scr, layout);

   style_edit->prop.content_glow_shadow = scr;

   elm_object_part_text_set(layout, "label.style", _("Style:"));
   font_style = _style_item_style_add(layout, style_edit);

   if (value)
     {
        Eina_Tmpstr *style = _tag_value_get(value, "style");
        if (!style) style = eina_tmpstr_add("none");
        Eina_Tmpstr *inner = _tag_value_get(value, "glow_color");
        if (!inner)
          {
             inner = eina_tmpstr_add(WHITE_COLOR);
             _tag_parse(style_edit, WHITE_COLOR, "glow_color");
          }
        Eina_Tmpstr *outer = _tag_value_get(value, "glow2_color");
        if (!outer)
          {
              outer = eina_tmpstr_add(WHITE_COLOR);
             _tag_parse(style_edit, WHITE_COLOR, "glow2_color");
          }
        Eina_Tmpstr *shadow = _tag_value_get(value, "shadow_color");
        if (!shadow)
          {
             shadow = eina_tmpstr_add(WHITE_COLOR);
             _tag_parse(style_edit, WHITE_COLOR, "shadow_color");
          }

        style_copy = mem_malloc(strlen(style) + 1);
        strcpy(style_copy, style);
        token = strtok(style_copy, ",");
        TODO("replace with eina_str_split_full")
        while (token)
          {
             if (count == 0)
               {
                  eina_tmpstr_del(style);
                  style = eina_tmpstr_add(token);
                  count++;
               }
             else
               {
                  for (i = 0; direction_list[i] != NULL; i++)
                    {
                       if (!strcmp(direction_list[i], token)) direction = i;
                    }
               }
             token = strtok(0, " ");
          }
        free(style_copy);

        if (strstr(style, "shadow"))
          {
             elm_object_part_text_set(layout, "label.glow_color", _("Color:"));
             elm_object_signal_emit(layout, "center", "label.glow2_color");
             elm_object_part_text_set(layout, "label.glow2_color", _("Direction:"));
             item = elm_layout_add(layout);
             elm_layout_theme_set(item, "layout", "style_editor", "direction");
             evas_object_show(item);

             if (elm_object_part_content_get(layout, "swallow.glow_color"))
               elm_object_part_content_unset(layout, "swallow.glow_color");
             shadow_color = _style_item_shadow_color_add(layout, style_edit);
             if (!_hex_to_rgb(shadow, &r, &g, &b, &a))
               ERR("This error should not happen in style editor... Contact devs please!");
             evas_object_color_set(shadow_color, r*a/255, g*a/255, b*a/255, a);

             style_edit->direction[0] = _direction_item_bl_add(item, style_edit);
             style_edit->direction[1] = _direction_item_b_add(item, style_edit);
             elm_radio_group_add(style_edit->direction[1], style_edit->direction[0]);
             style_edit->direction[2] = _direction_item_br_add(item, style_edit);
             elm_radio_group_add(style_edit->direction[2], style_edit->direction[0]);
             style_edit->direction[3] = _direction_item_l_add(item, style_edit);
             elm_radio_group_add(style_edit->direction[3], style_edit->direction[0]);
             style_edit->direction[4] = _direction_item_r_add(item, style_edit);
             elm_radio_group_add(style_edit->direction[4], style_edit->direction[0]);
             style_edit->direction[5] = _direction_item_tl_add(item, style_edit);
             elm_radio_group_add(style_edit->direction[5], style_edit->direction[0]);
             style_edit->direction[6] = _direction_item_t_add(item, style_edit);
             elm_radio_group_add(style_edit->direction[6], style_edit->direction[0]);
             style_edit->direction[7] = _direction_item_tr_add(item, style_edit);
             elm_radio_group_add(style_edit->direction[7], style_edit->direction[0]);

             if (elm_object_part_content_get(layout, "swallow.glow2_color"))
               elm_object_part_content_unset(layout, "swallow.glow2_color");
             elm_object_part_content_set(layout, "swallow.glow2_color", item);
             elm_object_signal_emit(style_edit->direction[direction], "mouse,up,1", "events");
          }
        else
          {
             elm_object_part_text_set(layout, "label.glow_color", _("Outer glow color:"));
             elm_object_signal_emit(layout, "top", "label.glow2_color");
             elm_object_part_text_set(layout, "label.glow2_color", _("Inner glow color:"));
             if (elm_object_part_content_get(layout, "swallow.glow_color"))
               elm_object_part_content_unset(layout, "swallow.glow_color");
             inner_glow = _style_item_inner_glow_add(layout, style_edit);
             if (elm_object_part_content_get(layout, "swallow.glow2_color"))
               elm_object_part_content_unset(layout, "swallow.glow2_color");
             outer_glow = _style_item_outer_glow_add(layout, style_edit);
             if (!_hex_to_rgb(inner, &r, &g, &b, &a))
               ERR("This error should not happen in style editor... Contact devs please!");
             evas_object_color_set(inner_glow, r*a/255, g*a/255, b*a/255, a);
             if (!_hex_to_rgb(outer, &r, &g, &b, &a))
               ERR("This error should not happen in style editor... Contact devs please!");
             evas_object_color_set(outer_glow, r*a/255, g*a/255, b*a/255, a);
          }

        ewe_combobox_text_set(font_style, style);
        for (i = 0; font_glow_list[i] != NULL; i++)
          ewe_combobox_item_add(font_style, font_glow_list[i]);

        eina_tmpstr_del(style);
        eina_tmpstr_del(inner);
        eina_tmpstr_del(outer);
        eina_tmpstr_del(shadow);
     }

   if (style_edit->prop.current_tab == TAB_EDITOR_GLOW_SHADOW)
     elm_layout_content_set(style_edit->prop.content, NULL, scr);
}*/

static void
_lines_tab_update(Style_Editor *style_edit, Evas_Object *tabs __UNUSED__, const char *value)
{
   Evas_Object *layout;
   Evas_Object *font_strikethrough, *strikethrough_color;
   Evas_Object *font_underline, *underline_style, *underline_color, *underline2_color;
   Elm_Object_Item *sc_item;
   Eina_Bool strikethr, underl;
   int r, g, b, a;
   unsigned int i = 0;

   assert(style_edit != NULL);
   //assert(tabs != NULL);

   Evas_Object *scr;
   SCROLLER_ADD(ap.win, scr);

   layout = elm_layout_add(ap.win);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout, "layout", "style_editor", "lines");
   evas_object_show(layout);
   elm_object_content_set(scr, layout);

   style_edit->prop.content_lines = scr;

   elm_object_part_text_set(layout, "label.strikethrough", _("Strikethrough color"));
   elm_object_part_text_set(layout, "label.underline", _("Underline"));
   elm_object_part_text_set(layout, "label.underline_color", _("Color"));

   font_strikethrough = _style_item_strikethrough_add(layout, style_edit);
   strikethrough_color = _style_item_strikethrough_color_add(layout, style_edit);
   font_underline = _style_item_underline_add(layout, style_edit);
   underline_style = _style_item_underline_style_add(layout, style_edit);
   underline_color = _style_item_underline_color_add(layout, style_edit);
   underline2_color = _style_item_underline2_color_add(layout, style_edit);

   if (value)
     {
        Eina_Tmpstr *strikethrough = _tag_value_get(value, "strikethrough");
        if ((!strikethrough) || (!strcmp(strikethrough, "off"))) strikethr = EINA_FALSE;
        else strikethr = EINA_TRUE;
        Eina_Tmpstr *strikethru_color = _tag_value_get(value, "strikethrough_color");
        if (!strikethru_color) strikethru_color = eina_tmpstr_add(WHITE_COLOR);
        Eina_Tmpstr *seg_item = NULL;
        Eina_Tmpstr *underline = _tag_value_get(value, "underline");
        if ((!underline) || (!strcmp(underline, "off"))) underl = EINA_FALSE;
        else
          {
             underl = EINA_TRUE;
             if (!strcmp(underline, "double")) seg_item = eina_tmpstr_add("double");
             else seg_item = eina_tmpstr_add("single");
             for (i = 0; underl_styles[i] != NULL; i++)
               {
                  if (!strcmp(elm_segment_control_item_label_get(underline_style, i), seg_item))
                    {
                       sc_item = elm_segment_control_item_get(underline_style, i);
                       elm_segment_control_item_selected_set(sc_item, true);
                    }
               }
          }
        Eina_Tmpstr *underl_color = _tag_value_get(value, "underline_color");
        if (!underl_color) underl_color = eina_tmpstr_add(WHITE_COLOR);
        Eina_Tmpstr *underl2_color = _tag_value_get(value, "underline2_color");
        if (!underl2_color) underl2_color = eina_tmpstr_add(WHITE_COLOR);

        elm_check_state_set(font_strikethrough, strikethr);
        elm_check_state_set(font_underline, underl);
        if (!_hex_to_rgb(strikethru_color, &r, &g, &b, &a))
          ERR("This error should not happen in style editor... Contact devs please!");
        evas_object_color_set(strikethrough_color, r*a/255, g*a/255, b*a/255, a);
        if (!_hex_to_rgb(underl_color, &r, &g, &b, &a))
          ERR("This error should not happen in style editor... Contact devs please!");
        evas_object_color_set(underline_color, r*a/255, g*a/255, b*a/255, a);
        if (!_hex_to_rgb(underl2_color, &r, &g, &b, &a))
          ERR("This error should not happen in style editor... Contact devs please!");
        evas_object_color_set(underline2_color, r*a/255, g*a/255, b*a/255, a);

        evas_object_smart_callback_add(underline_style, "changed", _on_underline_style_change, style_edit);

        eina_tmpstr_del(seg_item);
        eina_tmpstr_del(strikethrough);
        eina_tmpstr_del(strikethru_color);
        eina_tmpstr_del(underline);
        eina_tmpstr_del(underl_color);
        eina_tmpstr_del(underl2_color);
     }

   if (style_edit->prop.current_tab == TAB_EDITOR_LINES)
     elm_layout_content_set(style_edit->prop.content, NULL, scr);
}

#undef COMBOBOX_VALUE
#undef SEGMENT_VALUE
#undef SPINNER_VALUE
#undef PERCENT_SPINNER_VALUE
#undef CHECK_VALUE
#undef CHANGE_CALLBACK
#undef ITEM_COLOR_ADD
#undef COMBO_ADD
#undef STYLE_ADD
#undef ITEM1_TEXT_ADD
#undef SPIN_ADD
#undef MARGIN_SPIN_ADD
#undef ELLIPSIS_ADD
#undef CHK_ADD
#undef ITEM_TEXT_ADD
#undef MARGIN_MIN_SP
#undef MIN_SP
#undef MAX_SP
#undef MAX_PERCENT
#undef STEP_SP
#undef DIRECT_ADD
#undef WHITE_COLOR

#define _TAB_CHANGE_CALLBACK(PROPERTY, TAB_MARK) \
static void \
_##PROPERTY##_tab_change(void *data, \
                        Evas_Object *obj __UNUSED__, \
                        void *event_info __UNUSED__) \
{ \
   Style_Editor *style_edit = (Style_Editor *)data; \
   evas_object_hide(elm_layout_content_unset(style_edit->prop.content, NULL)); \
   elm_layout_content_set(style_edit->prop.content, NULL, style_edit->prop.PROPERTY); \
   style_edit->prop.current_tab = TAB_MARK; \
}

/*
_TAB_CHANGE_CALLBACK(content_text, TAB_EDITOR_TEXT);
_TAB_CHANGE_CALLBACK(content_format, TAB_EDITOR_FORMAT);
_TAB_CHANGE_CALLBACK(content_glow_shadow, TAB_EDITOR_GLOW_SHADOW);
_TAB_CHANGE_CALLBACK(content_lines, TAB_EDITOR_LINES);
*/

#undef _TAB_CHANGE_CALLBACK

Evas_Object*
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

/*
Evas_Object*
_form_right_side(Style_Editor *style_edit)
{
   Evas_Object *layout;
   Evas_Object *box_bg = NULL;
   Evas_Object *image_bg = NULL;
   Evas_Object *radio_group = NULL;
   Evas_Object *radio = NULL;

   assert(style_edit != NULL);

   layout = elm_layout_add(ap.win);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout, "layout", "style_editor", "property");
   evas_object_show(layout);

   style_edit->prop.content = elm_layout_add(layout);
   elm_layout_theme_set(style_edit->prop.content, "layout", "tab_home", "default");
   elm_object_part_content_set(layout, "swallow/tabs_entry", style_edit->prop.content);
   evas_object_show(style_edit->prop.content);

   style_edit->prop.tabs = elm_toolbar_add(style_edit->prop.content);
   elm_layout_content_set(style_edit->prop.content, "elm.swallow.toolbar", style_edit->prop.tabs);
   elm_toolbar_horizontal_set(style_edit->prop.tabs, false);
   elm_object_style_set(style_edit->prop.tabs,"tabs_vertical");
   elm_toolbar_shrink_mode_set(style_edit->prop.tabs, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(style_edit->prop.tabs, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_toolbar_align_set(style_edit->prop.tabs, 0.0);
   evas_object_show(style_edit->prop.tabs);

   elm_toolbar_item_append(style_edit->prop.tabs, NULL, _("Text"), _content_text_tab_change, style_edit);
   elm_toolbar_item_append(style_edit->prop.tabs, NULL, _("Format"), _content_format_tab_change, style_edit);
   elm_toolbar_item_append(style_edit->prop.tabs, NULL, _("Glow & Shadow"), _content_glow_shadow_tab_change, style_edit);
   elm_toolbar_item_append(style_edit->prop.tabs, NULL, _("Lines"), _content_lines_tab_change, style_edit);

   style_edit->prop.current_tab = TAB_EDITOR_TEXT;

   BOX_ADD(ap.win, box_bg, true, false);
   elm_box_padding_set(box_bg, 10, 0);

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
   elm_object_part_content_set(layout, "menu_container", box_bg);
   return layout;
}
*/

static void
_on_style_editor_close(void *data,
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
style_editor_window_add()
{
   Evas_Object *main_layout, *layout_right;
   Evas_Object *bg, *btn_apply, *box_bg;
   Evas *canvas;
   Evas_Textblock_Style *ts;
   Style_Editor *style_edit;
   static const char *style_buf = FONT_DEFAULT"'";

   assert(ap.project != NULL);

   style_edit = (Style_Editor *)mem_calloc(1, sizeof(Style_Editor));

   main_layout = elm_layout_add(ap.win);
   elm_layout_theme_set(main_layout, "layout", "tab_style", "style_editor");
   elm_object_part_text_set(main_layout, "label.preview", _("Preview"));

   style_edit->entry_prev = elm_layout_add(main_layout);
   evas_object_size_hint_weight_set(style_edit->entry_prev, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);
   elm_layout_theme_set(style_edit->entry_prev, "layout", "style_editor", "preview");
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
   evas_object_textblock_text_markup_set(style_edit->textblock_style,
                       _("The quick brown fox jumps over the lazy dog"));
   evas_object_show(style_edit->textblock_style);
   elm_object_part_content_set(main_layout, "swallow.preview", style_edit->entry_prev);

   layout_right = _form_right_side(style_edit);
   elm_object_part_content_set(main_layout, "swallow.font_list", layout_right);
   evas_object_show(layout_right);

   box_bg = _add_box_bg(style_edit);
   elm_object_part_content_set(main_layout, "menu_container", box_bg);

   BUTTON_ADD(main_layout, btn_apply, _("Apply"))
   elm_object_part_content_set(main_layout, "elm.swallow.btn_apply", btn_apply);

   evas_textblock_style_free(ts);
   evas_object_event_callback_add(main_layout, EVAS_CALLBACK_DEL,
                                  _on_style_editor_close, style_edit);

   return main_layout;
}

#undef FONT_DEFAULT
#undef POPUP
#undef CURRENT
